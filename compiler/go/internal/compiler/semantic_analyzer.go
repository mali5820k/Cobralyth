package compiler

import (
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strconv"
	"strings"

	"github.com/mali5820k/clyth-bootstrap/internal/frontend/ast"
	"github.com/mali5820k/clyth-bootstrap/internal/frontend/parser"
	"github.com/mali5820k/clyth-bootstrap/internal/frontend/token"
)

// SemanticAnalyzer owns one complete semantic-analysis session, including all
// recursively loaded local files and read-only runtime module namespaces.
type SemanticAnalyzer struct {
	root_path           string
	repo_root           string
	namespaces          map[string]*Namespace
	path_namespaces     map[string]*Namespace
	programs            map[string]*ast.Program
	scopes              []*Scope
	semantic_errors     []SemanticError
	specializations     []GenericSpecialization
	specialization_keys map[string]bool
	expression_types    map[ast.Expr]*SemanticType
	breakable_depth     int
	loop_depth          int
	current_receiver    string
	catch_depth         int
	error_type_ids      map[string]int
	integer_ranges      []map[string]integer_range
	integer_constants   map[int]map[string]int64
	index_safety        map[*ast.IndexExpr]IndexSafety
}

type integer_range struct {
	minimum int64
	maximum int64
}

type SemanticOptions struct {
	mecc_enabled bool
}

func analyze_file(path string, program *ast.Program) (*AnalysisResult, []SemanticError, error) {
	return analyze_file_with_options(path, program, SemanticOptions{})
}

func analyze_file_with_options(path string, program *ast.Program, options SemanticOptions) (*AnalysisResult, []SemanticError, error) {
	absolute_path, err := filepath.Abs(path)
	if err != nil {
		return nil, nil, err
	}
	analyzer := &SemanticAnalyzer{
		root_path:  absolute_path,
		repo_root:  find_repo_root(filepath.Dir(absolute_path)),
		namespaces: map[string]*Namespace{}, path_namespaces: map[string]*Namespace{}, programs: map[string]*ast.Program{},
		specialization_keys: map[string]bool{}, expression_types: map[ast.Expr]*SemanticType{}, error_type_ids: map[string]int{}, integer_constants: map[int]map[string]int64{}, index_safety: map[*ast.IndexExpr]IndexSafety{},
	}
	root_namespace := analyzer.load_source_namespace(absolute_path, "root", ast.ImportFile, program)
	if root_namespace == nil {
		return nil, analyzer.semantic_errors, nil
	}
	analyzer.resolve_all_imports()
	// Clyth Core is a protected compiler-owned module that is implicitly visible
	// to every translation unit. User imports cannot shadow its intrinsic types
	// or protocols.
	if core := analyzer.load_module_namespace("clyth-core"); core != nil {
		root_namespace.open_imports = append([]*Namespace{core}, root_namespace.open_imports...)
	}
	analyzer.register_error_types()
	analyzer.collect_all_declarations()
	analyzer.bind_all_methods()
	analyzer.validate_all_protocol_conformance()
	analyzer.analyze_namespace_bodies(root_namespace)
	result := &AnalysisResult{
		root_path: absolute_path, root_namespace: root_namespace, scopes: analyzer.scopes,
		namespaces: analyzer.namespaces, specializations: analyzer.specializations,
		semantic_errors: analyzer.semantic_errors, expression_types: analyzer.expression_types,
		mecc_enabled: options.mecc_enabled, mecc_policy_by_node: map[ast.Node]MeccPolicy{},
		allocations: map[int]*AllocationInfo{}, allocation_by_new: map[*ast.NewExpr]int{}, expression_allocations: map[ast.Expr]int{},
		error_type_ids: analyzer.error_type_ids, index_safety: analyzer.index_safety,
	}
	analyze_lifetimes(analyzer, result, options)
	result.semantic_errors = analyzer.semantic_errors
	return result, analyzer.semantic_errors, nil
}

func find_repo_root(start string) string {
	current := start
	for {
		if info, err := os.Stat(filepath.Join(current, "module-src")); err == nil && info.IsDir() {
			return current
		}
		parent := filepath.Dir(current)
		if parent == current {
			return start
		}
		current = parent
	}
}

func (analyzer *SemanticAnalyzer) add_error(path string, node ast.Node, message string) {
	analyzer.semantic_errors = append(analyzer.semantic_errors, SemanticError{source_path: path, source_span: node.Span(), message: message})
}

func (analyzer *SemanticAnalyzer) load_source_namespace(path string, name string, kind ast.ImportKind, supplied *ast.Program) *Namespace {
	clean_path, _ := filepath.Abs(path)
	if existing := analyzer.path_namespaces[clean_path]; existing != nil {
		return existing
	}
	program := supplied
	if program == nil {
		data, err := os.ReadFile(clean_path)
		if err != nil {
			analyzer.semantic_errors = append(analyzer.semantic_errors, SemanticError{source_path: clean_path, message: err.Error()})
			return nil
		}
		parsed, parse_errors, lex_errors := parser.Parse(string(data))
		for _, item := range lex_errors {
			analyzer.semantic_errors = append(analyzer.semantic_errors, SemanticError{source_path: clean_path, source_span: item.Span, message: item.Message})
		}
		for _, item := range parse_errors {
			analyzer.semantic_errors = append(analyzer.semantic_errors, SemanticError{source_path: clean_path, source_span: item.Span, message: item.Message})
		}
		program = parsed
	}
	namespace := &Namespace{name: name, source_path: clean_path, import_kind: kind, symbols: map[string]*Symbol{}, structs: map[string]*StructInfo{}, protocols: map[string]*ProtocolInfo{}, alias_imports: map[string]*Namespace{}}
	analyzer.path_namespaces[clean_path] = namespace
	analyzer.namespaces[name+"@"+clean_path] = namespace
	analyzer.programs[clean_path] = program
	return namespace
}

func (analyzer *SemanticAnalyzer) resolve_all_imports() {
	for changed := true; changed; {
		changed = false
		paths := make([]string, 0, len(analyzer.path_namespaces))
		for path := range analyzer.path_namespaces {
			paths = append(paths, path)
		}
		for _, path := range paths {
			namespace := analyzer.path_namespaces[path]
			before := len(analyzer.path_namespaces)
			analyzer.resolve_imports(namespace, analyzer.programs[path])
			if len(analyzer.path_namespaces) > before {
				changed = true
			}
		}
	}
}

func (analyzer *SemanticAnalyzer) resolve_imports(namespace *Namespace, program *ast.Program) {
	if namespace == nil || program == nil || len(namespace.open_imports)+len(namespace.alias_imports) > 0 {
		return
	}
	for _, declaration := range program.Declarations {
		import_decl, ok := declaration.(*ast.ImportDecl)
		if !ok {
			continue
		}
		var imported *Namespace
		if import_decl.Kind == ast.ImportFile {
			imported_path := filepath.Join(filepath.Dir(namespace.source_path), filepath.FromSlash(import_decl.Target))
			imported = analyzer.load_source_namespace(imported_path, import_decl.Target, ast.ImportFile, nil)
		} else {
			imported = analyzer.load_module_namespace(import_decl.Target)
		}
		if imported == nil {
			analyzer.add_error(namespace.source_path, import_decl, "unable to resolve import "+import_decl.Target)
			continue
		}
		if import_decl.Alias != "" {
			namespace.alias_imports[import_decl.Alias] = imported
		} else {
			namespace.open_imports = append(namespace.open_imports, imported)
		}
	}
}

func (analyzer *SemanticAnalyzer) load_module_namespace(module_name string) *Namespace {
	for _, namespace := range analyzer.namespaces {
		if namespace.name == module_name && namespace.import_kind == ast.ImportModule {
			return namespace
		}
	}
	patterns := []string{filepath.Join(analyzer.repo_root, "module-src", "runtime", module_name), filepath.Join(analyzer.repo_root, "module-src", "core", module_name)}
	var module_dir string
	for _, candidate := range patterns {
		if info, err := os.Stat(candidate); err == nil && info.IsDir() {
			module_dir = candidate
			break
		}
	}
	if module_dir == "" {
		return nil
	}
	namespace := &Namespace{name: module_name, source_path: module_dir, import_kind: ast.ImportModule, symbols: map[string]*Symbol{}, structs: map[string]*StructInfo{}, protocols: map[string]*ProtocolInfo{}, alias_imports: map[string]*Namespace{}}
	analyzer.namespaces[module_name+"@"+module_dir] = namespace
	source_files, _ := filepath.Glob(filepath.Join(module_dir, "src", "*.clyth"))
	for _, source_file := range source_files {
		data, err := os.ReadFile(source_file)
		if err != nil {
			continue
		}
		program, parse_errors, lex_errors := parser.Parse(string(data))
		if len(parse_errors)+len(lex_errors) > 0 {
			continue
		}
		analyzer.programs[source_file] = program
		analyzer.path_namespaces[source_file] = namespace
	}
	return namespace
}

func (analyzer *SemanticAnalyzer) register_error_types() {
	// Built-in identities occupy a stable low range; user/module identities are
	// assigned deterministically from their fully-qualified declaration names.
	builtins := []string{"none", "bounds_error", "null_reference", "invalid_memory_access", "arithmetic_error", "io_error", "device_timeout"}
	for index, name := range builtins {
		analyzer.error_type_ids["builtin::"+name] = index
	}
	type declaration struct {
		qualified string
		name      string
		path      string
		node      *ast.ErrorTypeDirectiveDecl
	}
	items := []declaration{}
	for path, program := range analyzer.programs {
		namespace := analyzer.path_namespaces[path]
		if namespace == nil {
			continue
		}
		for _, raw := range program.Declarations {
			item, ok := raw.(*ast.ErrorTypeDirectiveDecl)
			if !ok {
				continue
			}
			items = append(items, declaration{qualified: namespace.name + "::" + item.Name, name: item.Name, path: path, node: item})
		}
	}
	sort.Slice(items, func(i, j int) bool { return items[i].qualified < items[j].qualified })
	for _, item := range items {
		if _, exists := analyzer.error_type_ids[item.qualified]; exists {
			analyzer.add_error(item.path, item.node, "duplicate #ErrorType declaration '"+item.name+"'")
			continue
		}
		id := 1024 + len(analyzer.error_type_ids)
		analyzer.error_type_ids[item.qualified] = id
		namespace := analyzer.path_namespaces[item.path]
		analyzer.declare_symbol(namespace, item.path, item.name, &Symbol{
			name: item.name, kind: symbol_error_type,
			type_info:   &SemanticType{kind: type_error_type, name: "ErrorType"},
			declaration: item.node, namespace_name: namespace.name, error_type_id: id,
		})
	}
}

func (analyzer *SemanticAnalyzer) collect_all_declarations() {
	// Hoist named types across every loaded namespace before resolving any
	// function, field, or global signatures. This makes module dependency order
	// irrelevant and mirrors Clyth's declaration-before-body semantics.
	for path, program := range analyzer.programs {
		namespace := analyzer.path_namespaces[path]
		for _, declaration := range program.Declarations {
			switch item := declaration.(type) {
			case *ast.StructDecl:
				generic_names := make([]string, 0, len(item.GenericParameters))
				for _, parameter := range item.GenericParameters {
					generic_names = append(generic_names, parameter.Name)
				}
				if namespace.structs[item.Name] == nil {
					namespace.structs[item.Name] = &StructInfo{name: item.Name, declaration: item, intrinsic: item.Intrinsic, generic_parameters: generic_names, fields: map[string]*Symbol{}, methods: map[string]*Symbol{}}
				}
				analyzer.declare_symbol(namespace, path, item.Name, &Symbol{name: item.Name, kind: symbol_struct, type_info: &SemanticType{kind: type_struct, name: item.Name}, declaration: item, namespace_name: namespace.name, generic_parameters: generic_names})
			case *ast.EnumDecl:
				analyzer.declare_symbol(namespace, path, item.Name, &Symbol{name: item.Name, kind: symbol_struct, type_info: &SemanticType{kind: type_enum, name: item.Name}, declaration: item, namespace_name: namespace.name})
			case *ast.ProtocolDecl:
				generic_names := make([]string, 0, len(item.GenericParameters))
				for _, parameter := range item.GenericParameters {
					generic_names = append(generic_names, parameter.Name)
				}
				if namespace.protocols[item.Name] == nil {
					namespace.protocols[item.Name] = &ProtocolInfo{name: item.Name, declaration: item, generic_parameters: generic_names, methods: map[string]*Symbol{}}
				}
				analyzer.declare_symbol(namespace, path, item.Name, &Symbol{name: item.Name, kind: symbol_protocol, type_info: &SemanticType{kind: type_protocol, name: item.Name}, declaration: item, namespace_name: namespace.name, generic_parameters: generic_names})
			}
		}
	}
	for path, program := range analyzer.programs {
		analyzer.collect_declarations(analyzer.path_namespaces[path], path, program)
	}
}

func (analyzer *SemanticAnalyzer) core_namespace() *Namespace {
	for _, namespace := range analyzer.namespaces {
		if namespace != nil && namespace.name == "clyth-core" && namespace.import_kind == ast.ImportModule {
			return namespace
		}
	}
	return nil
}

func (analyzer *SemanticAnalyzer) reject_core_shadow(namespace *Namespace, path string, node ast.Node, name string) {
	core := analyzer.core_namespace()
	if core == nil || namespace == nil || namespace == core {
		return
	}
	if core.symbols[name] != nil {
		analyzer.add_error(path, node, "declaration '"+name+"' shadows protected Clyth Core identity")
	}
}

func (analyzer *SemanticAnalyzer) collect_declarations(namespace *Namespace, path string, program *ast.Program) {
	for _, declaration := range program.Declarations {
		switch item := declaration.(type) {
		case *ast.StructDecl:
			analyzer.reject_core_shadow(namespace, path, item, item.Name)
			if item.Intrinsic && !analyzer.is_core_intrinsic_path(path) {
				analyzer.add_error(path, item, "intrinsic_struct declarations are reserved for the protected Clyth Core module")
			}
			generic_names := make([]string, 0, len(item.GenericParameters))
			for _, parameter := range item.GenericParameters {
				generic_names = append(generic_names, parameter.Name)
			}
			info := namespace.structs[item.Name]
			if info == nil {
				info = &StructInfo{name: item.Name, declaration: item, intrinsic: item.Intrinsic, generic_parameters: generic_names, fields: map[string]*Symbol{}, methods: map[string]*Symbol{}}
				namespace.structs[item.Name] = info
			}
			analyzer.declare_symbol(namespace, path, item.Name, &Symbol{name: item.Name, kind: symbol_struct, type_info: &SemanticType{kind: type_struct, name: item.Name}, declaration: item, namespace_name: namespace.name, generic_parameters: generic_names})
		case *ast.EnumDecl:
			analyzer.reject_core_shadow(namespace, path, item, item.Name)
			analyzer.declare_symbol(namespace, path, item.Name, &Symbol{name: item.Name, kind: symbol_struct, type_info: &SemanticType{kind: type_enum, name: item.Name}, declaration: item, namespace_name: namespace.name})
		case *ast.ProtocolDecl:
			analyzer.reject_core_shadow(namespace, path, item, item.Name)
			generic_names := make([]string, 0, len(item.GenericParameters))
			for _, parameter := range item.GenericParameters {
				generic_names = append(generic_names, parameter.Name)
			}
			namespace.protocols[item.Name] = &ProtocolInfo{name: item.Name, declaration: item, generic_parameters: generic_names, methods: map[string]*Symbol{}}
			analyzer.declare_symbol(namespace, path, item.Name, &Symbol{name: item.Name, kind: symbol_protocol, type_info: &SemanticType{kind: type_protocol, name: item.Name}, declaration: item, namespace_name: namespace.name, generic_parameters: generic_names})
		case *ast.FunctionDecl:
			analyzer.reject_core_shadow(namespace, path, item, item.Name)
			function_type := analyzer.resolve_callable_type(namespace, item.ReturnType, item.Parameters, nil)
			analyzer.declare_symbol(namespace, path, item.Name, &Symbol{name: item.Name, kind: symbol_function, type_info: function_type, declaration: item, namespace_name: namespace.name, receiver_name: item.Receiver})
		case *ast.GlobalVarDecl:
			analyzer.reject_core_shadow(namespace, path, item, item.Name)
			analyzer.declare_symbol(namespace, path, item.Name, &Symbol{name: item.Name, kind: symbol_global, type_info: analyzer.resolve_type_ref(namespace, item.Type, nil, path), declaration: item, namespace_name: namespace.name})
		}
	}
	for _, declaration := range program.Declarations {
		if item, ok := declaration.(*ast.StructDecl); ok {
			analyzer.collect_struct_members(namespace, path, namespace.structs[item.Name], item.Members, nil)
		}
		if item, ok := declaration.(*ast.ProtocolDecl); ok {
			analyzer.collect_protocol_members(namespace, path, item)
		}
	}
}

func (analyzer *SemanticAnalyzer) declare_symbol(namespace *Namespace, path string, name string, symbol *Symbol) {
	if existing := namespace.symbols[name]; existing != nil && existing.declaration != symbol.declaration {
		analyzer.add_error(path, symbol.declaration, "duplicate declaration '"+name+"'")
		return
	}
	namespace.symbols[name] = symbol
}

func (analyzer *SemanticAnalyzer) collect_struct_members(namespace *Namespace, path string, info *StructInfo, members []ast.StructMember, substitutions map[string]*SemanticType) {
	if info == nil {
		return
	}
	generic_set := map[string]*SemanticType{}
	for _, name := range info.generic_parameters {
		generic_set[name] = &SemanticType{kind: type_unknown, name: name}
	}
	for key, value := range substitutions {
		generic_set[key] = value
	}
	for _, member := range members {
		switch item := member.(type) {
		case *ast.FieldDecl:
			if info.fields[item.Name] == nil {
				info.field_order = append(info.field_order, item.Name)
			}
			info.fields[item.Name] = &Symbol{name: item.Name, kind: symbol_field, type_info: analyzer.resolve_type_ref(namespace, item.Type, generic_set, path), declaration: item, namespace_name: namespace.name, receiver_name: info.name}
		case *ast.MethodDecl:
			function_type := analyzer.resolve_callable_type(namespace, item.ReturnType, item.Parameters, generic_set)
			info.methods[item.Name] = &Symbol{name: item.Name, kind: symbol_method, type_info: function_type, declaration: item, namespace_name: namespace.name, receiver_name: info.name}
		case *ast.ConstructorMember:
			info.constructors = append(info.constructors, analyzer.resolve_callable_type(namespace, &ast.TypeRef{Name: info.name}, item.Parameters, generic_set).callable)
		case *ast.DestructorMember:
			info.destructor = true
		}
	}
}

func (analyzer *SemanticAnalyzer) collect_protocol_members(namespace *Namespace, path string, declaration *ast.ProtocolDecl) {
	info := namespace.protocols[declaration.Name]
	if info == nil {
		return
	}
	generic_set := map[string]*SemanticType{}
	for _, name := range info.generic_parameters {
		generic_set[name] = &SemanticType{kind: type_unknown, name: name}
	}
	for _, member := range declaration.Members {
		info.methods[member.Name] = &Symbol{name: member.Name, kind: symbol_method, type_info: analyzer.resolve_callable_type(namespace, member.ReturnType, member.Parameters, generic_set), declaration: member, namespace_name: namespace.name}
	}
}

func (analyzer *SemanticAnalyzer) bind_all_methods() {
	for path, program := range analyzer.programs {
		namespace := analyzer.path_namespaces[path]
		for _, declaration := range program.Declarations {
			switch item := declaration.(type) {
			case *ast.MethodBlockDecl:
				info := analyzer.lookup_struct(namespace, item.Receiver.Name)
				if info != nil {
					analyzer.collect_struct_members(namespace, path, info, item.Members, nil)
				}
			case *ast.FunctionDecl:
				if item.Receiver != "" {
					if info := analyzer.lookup_struct(namespace, item.Receiver); info != nil {
						info.methods[item.Name] = &Symbol{name: item.Name, kind: symbol_method, type_info: analyzer.resolve_callable_type(namespace, item.ReturnType, item.Parameters, nil), declaration: item, namespace_name: namespace.name, receiver_name: item.Receiver}
					}
				}
			}
		}
	}
}

func (analyzer *SemanticAnalyzer) is_core_intrinsic_path(path string) bool {
	clean, _ := filepath.Abs(path)
	core := filepath.Join(analyzer.repo_root, "module-src", "core", "clyth-core")
	core, _ = filepath.Abs(core)
	rel, err := filepath.Rel(core, clean)
	return err == nil && rel != ".." && !strings.HasPrefix(rel, ".."+string(filepath.Separator))
}

func protocol_signature_matches(requirement, implementation *SemanticCallable) bool {
	if requirement == nil || implementation == nil || requirement.variadic != implementation.variadic || len(requirement.parameters) != len(implementation.parameters) {
		return false
	}
	wildcard := func(t *SemanticType) bool { return t == nil || t.kind == type_unknown || t.name == "auto" }
	if !wildcard(requirement.return_type) && !types_compatible(requirement.return_type, implementation.return_type) {
		return false
	}
	for i := range requirement.parameters {
		if !wildcard(requirement.parameters[i]) && !types_compatible(requirement.parameters[i], implementation.parameters[i]) {
			return false
		}
	}
	return true
}

func (analyzer *SemanticAnalyzer) validate_indexable_protocol(path string, declaration *ast.StructDecl, info *StructInfo) {
	read := info.methods["on_read_index"]
	write := info.methods["on_write_index"]
	if read == nil || read.type_info == nil || read.type_info.callable == nil || len(read.type_info.callable.parameters) != 1 {
		analyzer.add_error(path, declaration, "Indexable requires on_read_index(index) with exactly one index parameter")
		return
	}
	if write == nil || write.type_info == nil || write.type_info.callable == nil || len(write.type_info.callable.parameters) != 2 {
		analyzer.add_error(path, declaration, "Indexable requires on_write_index(index, value) with exactly two parameters")
		return
	}
	rc := read.type_info.callable
	wc := write.type_info.callable
	if type_name(rc.parameters[0]) != type_name(wc.parameters[0]) {
		analyzer.add_error(path, declaration, "Indexable read/write index parameter types must match")
	}
	if type_name(rc.return_type) != type_name(wc.parameters[1]) {
		analyzer.add_error(path, declaration, "Indexable read return type must match write value type")
	}
	if wc.return_type == nil || wc.return_type.kind != type_void {
		analyzer.add_error(path, declaration, "Indexable on_write_index must return void")
	}
}

func (analyzer *SemanticAnalyzer) validate_all_protocol_conformance() {
	for path, program := range analyzer.programs {
		namespace := analyzer.path_namespaces[path]
		if namespace == nil || program == nil {
			continue
		}
		for _, raw := range program.Declarations {
			declaration, ok := raw.(*ast.StructDecl)
			if !ok || len(declaration.Bases) == 0 {
				continue
			}
			info := namespace.structs[declaration.Name]
			if info == nil {
				continue
			}
			for _, base := range declaration.Bases {
				protocol := analyzer.lookup_protocol(namespace, base.Name)
				if protocol == nil {
					continue
				}
				for name, required := range protocol.methods {
					implemented := info.methods[name]
					if implemented == nil || implemented.type_info == nil || !protocol_signature_matches(required.type_info.callable, implemented.type_info.callable) {
						analyzer.add_error(path, declaration, "struct '"+declaration.Name+"' does not satisfy protocol '"+protocol.name+"': missing or incompatible method '"+name+"'")
					}
				}
				if protocol.name == "Indexable" {
					analyzer.validate_indexable_protocol(path, declaration, info)
				}
			}
		}
	}
}

func (analyzer *SemanticAnalyzer) lookup_protocol(namespace *Namespace, name string) *ProtocolInfo {
	if namespace == nil {
		return nil
	}
	if p := namespace.protocols[name]; p != nil {
		return p
	}
	for _, imported := range namespace.open_imports {
		if p := imported.protocols[name]; p != nil {
			return p
		}
	}
	return nil
}

func (analyzer *SemanticAnalyzer) resolve_callable_type(namespace *Namespace, return_ref *ast.TypeRef, parameters []*ast.Parameter, substitutions map[string]*SemanticType) *SemanticType {
	callable := &SemanticCallable{return_type: analyzer.resolve_type_ref(namespace, return_ref, substitutions, namespace.source_path)}
	seen_default := false
	for _, parameter := range parameters {
		parameter_type := analyzer.resolve_type_ref(namespace, parameter.Type, substitutions, namespace.source_path)
		callable.parameters = append(callable.parameters, parameter_type)
		callable.default_values = append(callable.default_values, parameter.DefaultValue)
		if parameter.DefaultValue != nil {
			seen_default = true
		} else if !parameter.Variadic && !seen_default {
			callable.required_parameters++
		}
		if parameter.Variadic {
			callable.variadic = true
			callable.native_variadic = parameter.Type == nil
			callable.heterogeneous_variadic = parameter.Type != nil && parameter.Type.Name == "auto"
		}
	}
	return &SemanticType{kind: type_callable, callable: callable}
}

func (analyzer *SemanticAnalyzer) resolve_type_ref(namespace *Namespace, ref *ast.TypeRef, substitutions map[string]*SemanticType, path string) *SemanticType {
	if ref == nil {
		return &SemanticType{kind: type_unknown}
	}
	if substitutions != nil {
		if replacement := substitutions[ref.Name]; replacement != nil {
			cloned := clone_type(replacement)
			cloned.pointer_depth += ref.PointerDepth
			cloned.is_const = cloned.is_const || ref.Const
			for _, dimension := range ref.Arrays {
				size := ""
				if literal, ok := dimension.Size.(*ast.LiteralExpr); ok {
					size = literal.Raw
				}
				cloned.array_dimensions = append(cloned.array_dimensions, SemanticArrayDimension{dynamic: dimension.Dynamic, size: size})
			}
			if len(cloned.array_dimensions) > 0 {
				cloned.kind = type_array
			} else if cloned.pointer_depth > 0 {
				cloned.kind = type_pointer
			}
			return cloned
		}
	}
	if ref.Callable != nil {
		callable := &SemanticCallable{return_type: analyzer.resolve_type_ref(namespace, ref.Callable.Return, substitutions, path), variadic: ref.Callable.Variadic}
		for index, parameter := range ref.Callable.Parameters {
			callable.parameters = append(callable.parameters, analyzer.resolve_type_ref(namespace, parameter, substitutions, path))
			if !(ref.Callable.Variadic && index == len(ref.Callable.Parameters)-1) {
				callable.required_parameters++
			}
		}
		if ref.Callable.Variadic && len(ref.Callable.Parameters) > 0 {
			last := ref.Callable.Parameters[len(ref.Callable.Parameters)-1]
			callable.heterogeneous_variadic = last != nil && last.Name == "auto"
		}
		return &SemanticType{kind: type_callable, callable: callable, is_const: ref.Const}
	}
	kind := type_unknown
	switch ref.Name {
	case "void":
		kind = type_void
	case "bool":
		kind = type_bool
	case "string":
		kind = type_string
	case "Error":
		kind = type_struct
	case "ErrorType":
		kind = type_error_type
	case "char":
		kind = type_char
	case "float32", "float64":
		kind = type_float
	case "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64":
		kind = type_integer
	}
	if symbol := analyzer.lookup_namespace_symbol(namespace, ref.Name); symbol != nil {
		// Preserve the semantic identity of named imported types. Intrinsic Core
		// structs may describe a compiler-known structural primitive (currently
		// string) without erasing that primitive semantic kind. Their StructInfo
		// remains available separately for protocol/layout metadata.
		if symbol.type_info != nil && symbol.type_info.kind == type_enum {
			kind = type_enum
		} else if symbol.kind == symbol_struct {
			info := analyzer.lookup_struct(namespace, ref.Name)
			if kind == type_unknown || info == nil || !info.intrinsic {
				kind = type_struct
			}
		} else if symbol.kind == symbol_protocol {
			kind = type_protocol
		}
	}
	result := &SemanticType{kind: kind, name: ref.Name, pointer_depth: ref.PointerDepth, is_const: ref.Const}
	for _, argument := range ref.GenericArgs {
		result.generic_args = append(result.generic_args, analyzer.resolve_type_ref(namespace, argument, substitutions, path))
	}
	for _, dimension := range ref.Arrays {
		size := ""
		if literal, ok := dimension.Size.(*ast.LiteralExpr); ok {
			size = literal.Raw
		}
		result.array_dimensions = append(result.array_dimensions, SemanticArrayDimension{dynamic: dimension.Dynamic, size: size})
	}
	if len(result.array_dimensions) > 0 {
		result.kind = type_array
	}
	if result.pointer_depth > 0 {
		result.kind = type_pointer
	}
	if len(result.generic_args) > 0 {
		analyzer.record_specialization(ref.Name, result.generic_args, ref.Span(), path)
		analyzer.validate_hashable_generic(namespace, result, ref, path)
	}
	return result
}

// validate_hashable_generic enforces the V1.0.0 Map/Set key contract.
// Generic templates whose key remains unresolved are checked at specialization.
func (analyzer *SemanticAnalyzer) validate_hashable_generic(namespace *Namespace, type_info *SemanticType, node ast.Node, path string) {
	if type_info == nil || len(type_info.generic_args) == 0 {
		return
	}
	var key *SemanticType
	switch type_info.name {
	case "Map":
		if len(type_info.generic_args) >= 1 {
			key = type_info.generic_args[0]
		}
	case "Set":
		key = type_info.generic_args[0]
	default:
		return
	}
	if key == nil || key.kind == type_unknown {
		return
	}
	if !analyzer.type_is_hashable(namespace, key, map[string]bool{}) {
		analyzer.add_error(path, node, type_info.name+" key type '"+type_name(key)+"' is not hashable; provide a deterministic uint64 hash() implementation or use a hashable key type")
	}
}

func (analyzer *SemanticAnalyzer) type_is_hashable(namespace *Namespace, type_info *SemanticType, visiting map[string]bool) bool {
	if type_info == nil || type_info.kind == type_unknown {
		return false
	}
	if type_info.pointer_depth > 0 || type_info.kind == type_pointer {
		// A pointer used as the key itself has identity semantics.
		return true
	}
	if len(type_info.array_dimensions) > 0 || type_info.kind == type_array || type_info.kind == type_callable || type_info.kind == type_namespace || type_info.kind == type_protocol || type_info.kind == type_void {
		return false
	}
	switch type_info.kind {
	case type_bool, type_integer, type_float, type_string, type_char, type_enum:
		return true
	}
	if type_info.kind != type_struct {
		return false
	}
	key := type_name(type_info)
	if visiting[key] {
		// Recursive value/deep-pointer graphs require an explicit custom hash to
		// define cycle/identity behavior deterministically.
		return false
	}
	info := analyzer.lookup_struct(namespace, type_info.name)
	if info == nil {
		return false
	}
	substitutions := build_substitutions(info.generic_parameters, type_info.generic_args)
	if method := info.methods["hash"]; method != nil && method.type_info != nil && method.type_info.callable != nil {
		callable := substitute_type(method.type_info, substitutions).callable
		if callable != nil && len(callable.parameters) == 0 && callable.return_type != nil && callable.return_type.name == "uint64" {
			return true
		}
	}
	visiting[key] = true
	defer delete(visiting, key)
	for _, field := range info.fields {
		field_type := substitute_type(field.type_info, substitutions)
		if field_type == nil {
			return false
		}
		if field_type.pointer_depth > 0 || field_type.kind == type_pointer {
			// Nested pointer members use deep value hashing by default, not address
			// identity. Remove one pointer layer and validate the pointee.
			pointee := clone_type(field_type)
			if pointee.pointer_depth > 0 {
				pointee.pointer_depth--
			}
			if pointee.pointer_depth == 0 {
				if analyzer.lookup_struct(namespace, pointee.name) != nil {
					pointee.kind = type_struct
				}
			}
			if !analyzer.type_is_hashable(namespace, pointee, visiting) {
				return false
			}
			continue
		}
		if !analyzer.type_is_hashable(namespace, field_type, visiting) {
			return false
		}
	}
	return true
}

func (analyzer *SemanticAnalyzer) lookup_namespace_symbol(namespace *Namespace, name string) *Symbol {
	return analyzer.lookup_namespace_symbol_seen(namespace, name, map[*Namespace]bool{})
}

func (analyzer *SemanticAnalyzer) lookup_namespace_symbol_seen(namespace *Namespace, name string, seen map[*Namespace]bool) *Symbol {
	if namespace == nil || seen[namespace] {
		return nil
	}
	seen[namespace] = true
	if symbol := namespace.symbols[name]; symbol != nil {
		return symbol
	}
	var found *Symbol
	for _, imported := range namespace.open_imports {
		symbol := analyzer.lookup_namespace_symbol_seen(imported, name, seen)
		if symbol != nil {
			if found != nil && found != symbol {
				// Runtime modules may intentionally re-export the same native API.
				// Compatible duplicate declarations are one logical imported symbol;
				// incompatible duplicates remain ambiguous.
				if found.kind != symbol.kind || !types_compatible(found.type_info, symbol.type_info) {
					return nil
				}
			}
			if found == nil {
				found = symbol
			}
		}
	}
	return found
}

func (analyzer *SemanticAnalyzer) lookup_struct(namespace *Namespace, name string) *StructInfo {
	return analyzer.lookup_struct_seen(namespace, name, map[*Namespace]bool{})
}

func (analyzer *SemanticAnalyzer) lookup_struct_seen(namespace *Namespace, name string, seen map[*Namespace]bool) *StructInfo {
	if namespace == nil || seen[namespace] {
		return nil
	}
	seen[namespace] = true
	if info := namespace.structs[name]; info != nil {
		return info
	}
	for _, imported := range namespace.open_imports {
		if info := analyzer.lookup_struct_seen(imported, name, seen); info != nil {
			return info
		}
	}
	return nil
}

func (analyzer *SemanticAnalyzer) new_scope(parent_id int, kind string) *Scope {
	scope := &Scope{id: len(analyzer.scopes), parent_id: parent_id, kind: kind, symbols: map[string]*Symbol{}}
	analyzer.scopes = append(analyzer.scopes, scope)
	analyzer.integer_constants[scope.id] = map[string]int64{}
	return scope
}

func (analyzer *SemanticAnalyzer) scope_lookup(scope *Scope, name string) *Symbol {
	current := scope
	for current != nil {
		if symbol := current.symbols[name]; symbol != nil {
			return symbol
		}
		if current.parent_id < 0 {
			return nil
		}
		current = analyzer.scopes[current.parent_id]
	}
	return nil
}

func (analyzer *SemanticAnalyzer) declare_local(path string, scope *Scope, node ast.Node, name string, kind SymbolKind, type_info *SemanticType) {
	if existing := scope.symbols[name]; existing != nil {
		// Receiver fields/methods are seeded into method/constructor scope for
		// implicit member access. Ordinary parameters/locals may shadow that
		// convenience binding; explicit this.field remains available.
		if existing.kind != symbol_field && existing.kind != symbol_method {
			analyzer.add_error(path, node, "duplicate declaration '"+name+"' in the same scope")
			return
		}
	}
	scope.symbols[name] = &Symbol{name: name, kind: kind, type_info: type_info, declaration: node, scope_id: scope.id}
}

func (analyzer *SemanticAnalyzer) analyze_namespace_bodies(namespace *Namespace) {
	if namespace == nil || namespace.analyzed_bodies {
		return
	}
	namespace.analyzed_bodies = true
	for path, program := range analyzer.programs {
		if analyzer.path_namespaces[path] != namespace {
			continue
		}
		analyzer.analyze_program_bodies(namespace, path, program)
	}
	for _, imported := range namespace.alias_imports {
		if imported.import_kind == ast.ImportFile {
			analyzer.analyze_namespace_bodies(imported)
		}
	}
	for _, imported := range namespace.open_imports {
		if imported.import_kind == ast.ImportFile {
			analyzer.analyze_namespace_bodies(imported)
		}
	}
}

func (analyzer *SemanticAnalyzer) analyze_program_bodies(namespace *Namespace, path string, program *ast.Program) {
	global_scope := analyzer.new_scope(-1, "namespace")
	for name, symbol := range namespace.symbols {
		global_scope.symbols[name] = symbol
	}
	for alias, imported := range namespace.alias_imports {
		global_scope.symbols[alias] = &Symbol{name: alias, kind: symbol_namespace, type_info: &SemanticType{kind: type_namespace, namespace_name: imported.name, name: imported.name}, namespace_name: imported.name}
	}
	for _, declaration := range program.Declarations {
		switch item := declaration.(type) {
		case *ast.GlobalVarDecl:
			if item.Initializer != nil {
				is_auto := item.Type != nil && item.Type.Name == "auto" && item.Type.Callable == nil
				target := analyzer.resolve_type_ref(namespace, item.Type, nil, path)
				expected_type := target
				if is_auto {
					expected_type = nil
				}
				value := analyzer.infer_expr(namespace, path, global_scope, item.Initializer, expected_type)
				if is_auto {
					target = clone_type(value)
					if symbol := namespace.symbols[item.Name]; symbol != nil {
						symbol.type_info = target
					}
				} else if !types_compatible(target, value) {
					analyzer.add_error(path, item, "cannot initialize '"+item.Name+"' of type "+type_name(target)+" with "+type_name(value))
				}
			} else if item.Type != nil && item.Type.Name == "auto" {
				analyzer.add_error(path, item, "auto declaration '"+item.Name+"' requires an initializer")
			}
		case *ast.FunctionDecl:
			if item.Body != nil {
				analyzer.analyze_function(namespace, path, global_scope, item, item.Receiver)
			}
		case *ast.StructDecl:
			for _, member := range item.Members {
				analyzer.analyze_struct_member(namespace, path, global_scope, item.Name, member)
			}
		case *ast.MethodBlockDecl:
			for _, member := range item.Members {
				analyzer.analyze_struct_member(namespace, path, global_scope, item.Receiver.Name, member)
			}
		}
	}
}

func (analyzer *SemanticAnalyzer) parameter_type(namespace *Namespace, path string, parameter *ast.Parameter) *SemanticType {
	resolved := analyzer.resolve_type_ref(namespace, parameter.Type, nil, path)
	if parameter != nil && parameter.Variadic && parameter.Type != nil {
		if parameter.Type.Name == "auto" {
			return &SemanticType{kind: type_array, name: "auto_variadic", array_dimensions: []SemanticArrayDimension{{dynamic: true}}}
		}
		element := clone_type(resolved)
		array := clone_type(element)
		array.kind = type_array
		array.name = element.name
		array.array_dimensions = []SemanticArrayDimension{{dynamic: true}}
		return array
	}
	return resolved
}

func (analyzer *SemanticAnalyzer) analyze_struct_member(namespace *Namespace, path string, parent *Scope, receiver string, member ast.StructMember) {
	previous_receiver := analyzer.current_receiver
	analyzer.current_receiver = receiver
	defer func() { analyzer.current_receiver = previous_receiver }()
	switch item := member.(type) {
	case *ast.MethodDecl:
		if item.Body != nil {
			analyzer.analyze_function(namespace, path, parent, &item.FunctionDecl, receiver)
		}
	case *ast.ConstructorMember:
		scope := analyzer.new_scope(parent.id, "constructor")
		analyzer.seed_receiver_scope(namespace, scope, receiver)
		for _, parameter := range item.Parameters {
			analyzer.declare_local(path, scope, parameter, parameter.Name, symbol_parameter, analyzer.parameter_type(namespace, path, parameter))
		}
		analyzer.analyze_block(namespace, path, scope, item.Body, &SemanticType{kind: type_void, name: "void"})
	case *ast.DestructorMember:
		scope := analyzer.new_scope(parent.id, "destructor")
		analyzer.seed_receiver_scope(namespace, scope, receiver)
		analyzer.analyze_block(namespace, path, scope, item.Body, &SemanticType{kind: type_void, name: "void"})
	}
}

func (analyzer *SemanticAnalyzer) seed_receiver_scope(namespace *Namespace, scope *Scope, receiver string) {
	receiver_type := &SemanticType{kind: type_struct, name: receiver}
	scope.symbols["this"] = &Symbol{name: "this", kind: symbol_parameter, type_info: receiver_type, scope_id: scope.id}
	if info := analyzer.lookup_struct(namespace, receiver); info != nil {
		for field_name, field := range info.fields {
			scope.symbols[field_name] = &Symbol{name: field_name, kind: symbol_field, type_info: field.type_info, declaration: field.declaration, scope_id: scope.id}
		}
		for method_name, method := range info.methods {
			scope.symbols[method_name] = &Symbol{name: method_name, kind: symbol_method, type_info: method.type_info, declaration: method.declaration, scope_id: scope.id}
		}
	}
}

func (analyzer *SemanticAnalyzer) analyze_function(namespace *Namespace, path string, parent *Scope, function *ast.FunctionDecl, receiver string) {
	previous_receiver := analyzer.current_receiver
	analyzer.current_receiver = receiver
	defer func() { analyzer.current_receiver = previous_receiver }()
	scope := analyzer.new_scope(parent.id, "function")
	if receiver != "" {
		analyzer.seed_receiver_scope(namespace, scope, receiver)
	}
	for _, parameter := range function.Parameters {
		analyzer.declare_local(path, scope, parameter, parameter.Name, symbol_parameter, analyzer.parameter_type(namespace, path, parameter))
	}
	return_type := analyzer.resolve_type_ref(namespace, function.ReturnType, nil, path)
	analyzer.analyze_block(namespace, path, scope, function.Body, return_type)
}

func (analyzer *SemanticAnalyzer) analyze_block(namespace *Namespace, path string, parent *Scope, block *ast.BlockStmt, return_type *SemanticType) {
	if block == nil {
		return
	}
	scope := analyzer.new_scope(parent.id, "block")
	for _, statement := range block.Statements {
		analyzer.analyze_stmt(namespace, path, scope, statement, return_type)
	}
}

func (analyzer *SemanticAnalyzer) analyze_stmt(namespace *Namespace, path string, scope *Scope, statement ast.Stmt, return_type *SemanticType) {
	switch item := statement.(type) {
	case *ast.VarDeclStmt:
		target := analyzer.resolve_type_ref(namespace, item.Type, nil, path)
		is_auto := item.Type != nil && item.Type.Name == "auto" && item.Type.Callable == nil
		if item.Initializer != nil {
			expected_type := target
			if is_auto {
				expected_type = nil
			}
			value := analyzer.infer_expr(namespace, path, scope, item.Initializer, expected_type)
			if is_auto {
				target = clone_type(value)
			} else if !types_compatible(target, value) {
				analyzer.add_error(path, item, "cannot initialize '"+item.Name+"' of type "+type_name(target)+" with "+type_name(value))
			}
		} else if is_auto {
			analyzer.add_error(path, item, "auto declaration '"+item.Name+"' requires an initializer")
		}
		analyzer.declare_local(path, scope, item, item.Name, symbol_local, target)
		if value, ok := static_integer_value(item.Initializer); ok {
			analyzer.integer_constants[scope.id][item.Name] = value
		} else {
			delete(analyzer.integer_constants[scope.id], item.Name)
		}
	case *ast.ReturnStmt:
		value := &SemanticType{kind: type_void, name: "void"}
		if item.Value != nil {
			value = analyzer.infer_expr(namespace, path, scope, item.Value, return_type)
		}
		if !types_compatible(return_type, value) {
			analyzer.add_error(path, item, "return type mismatch: expected "+type_name(return_type)+", got "+type_name(value))
		}
	case *ast.ExprStmt:
		analyzer.infer_expr(namespace, path, scope, item.Expression, nil)
	case *ast.IfStmt:
		analyzer.infer_expr(namespace, path, scope, item.Condition, &SemanticType{kind: type_bool, name: "bool"})
		analyzer.analyze_nested_stmt(namespace, path, scope, item.Then, return_type)
		if item.Else != nil {
			analyzer.analyze_nested_stmt(namespace, path, scope, item.Else, return_type)
		}
	case *ast.SwitchStmt:
		analyzer.breakable_depth++
		defer func() { analyzer.breakable_depth-- }()
		switch_type := analyzer.infer_expr(namespace, path, scope, item.Value, nil)
		for _, switch_case := range item.Cases {
			if switch_case.Value != nil {
				case_type := analyzer.infer_expr(namespace, path, scope, switch_case.Value, switch_type)
				if !types_compatible(switch_type, case_type) {
					analyzer.add_error(path, switch_case, "switch case type does not match switch expression")
				}
			}
			case_scope := analyzer.new_scope(scope.id, "switch_case")
			for _, case_statement := range switch_case.Statements {
				analyzer.analyze_stmt(namespace, path, case_scope, case_statement, return_type)
			}
		}
	case *ast.WhileStmt:
		analyzer.infer_expr(namespace, path, scope, item.Condition, &SemanticType{kind: type_bool, name: "bool"})
		analyzer.breakable_depth++
		analyzer.loop_depth++
		if name, bounds, ok := analyzer.derive_while_integer_range(scope, item); ok {
			analyzer.integer_ranges = append(analyzer.integer_ranges, map[string]integer_range{name: bounds})
			analyzer.analyze_nested_stmt(namespace, path, scope, item.Body, return_type)
			analyzer.integer_ranges = analyzer.integer_ranges[:len(analyzer.integer_ranges)-1]
			analyzer.invalidate_integer_constant(scope, name)
		} else {
			analyzer.analyze_nested_stmt(namespace, path, scope, item.Body, return_type)
		}
		analyzer.loop_depth--
		analyzer.breakable_depth--
	case *ast.ForStmt:
		analyzer.breakable_depth++
		analyzer.loop_depth++
		defer func() { analyzer.loop_depth--; analyzer.breakable_depth-- }()
		loop := analyzer.new_scope(scope.id, "for")
		if item.Init != nil {
			analyzer.analyze_stmt(namespace, path, loop, item.Init, return_type)
		}
		if item.Condition != nil {
			analyzer.infer_expr(namespace, path, loop, item.Condition, nil)
		}
		name, bounds, has_bounds := analyzer.derive_for_integer_range(loop, item)
		if item.Post != nil {
			analyzer.infer_expr(namespace, path, loop, item.Post, nil)
		}
		if has_bounds {
			analyzer.integer_ranges = append(analyzer.integer_ranges, map[string]integer_range{name: bounds})
			analyzer.analyze_nested_stmt(namespace, path, loop, item.Body, return_type)
			analyzer.integer_ranges = analyzer.integer_ranges[:len(analyzer.integer_ranges)-1]
		} else {
			analyzer.analyze_nested_stmt(namespace, path, loop, item.Body, return_type)
		}
	case *ast.BreakStmt:
		if analyzer.breakable_depth == 0 {
			analyzer.add_error(path, item, "break is only valid inside a loop or switch")
		}
	case *ast.ContinueStmt:
		if analyzer.loop_depth == 0 {
			analyzer.add_error(path, item, "continue is only valid inside a loop")
		}
	case *ast.BlockStmt:
		analyzer.analyze_block(namespace, path, scope, item, return_type)
	}
}

func (analyzer *SemanticAnalyzer) analyze_nested_stmt(namespace *Namespace, path string, scope *Scope, statement ast.Stmt, return_type *SemanticType) {
	if block, ok := statement.(*ast.BlockStmt); ok {
		analyzer.analyze_block(namespace, path, scope, block, return_type)
	} else {
		analyzer.analyze_stmt(namespace, path, scope, statement, return_type)
	}
}

func (analyzer *SemanticAnalyzer) indexable_methods(namespace *Namespace, object *SemanticType) (*Symbol, *Symbol) {
	if object == nil {
		return nil, nil
	}
	info := analyzer.lookup_struct(namespace, object.name)
	if info == nil {
		return nil, nil
	}
	return info.methods["on_read_index"], info.methods["on_write_index"]
}

func (analyzer *SemanticAnalyzer) infer_expr(namespace *Namespace, path string, scope *Scope, expression ast.Expr, expected *SemanticType) *SemanticType {
	if expression == nil {
		return &SemanticType{kind: type_unknown}
	}
	result := &SemanticType{kind: type_unknown}
	switch item := expression.(type) {
	case *ast.IdentifierExpr:
		if symbol := analyzer.scope_lookup(scope, item.Name); symbol != nil {
			result = symbol.type_info
		} else if symbol := analyzer.lookup_namespace_symbol(namespace, item.Name); symbol != nil {
			result = symbol.type_info
		} else if _, ok := analyzer.error_type_ids["builtin::"+item.Name]; ok {
			result = &SemanticType{kind: type_error_type, name: "ErrorType"}
		} else {
			analyzer.add_error(path, item, "unknown identifier '"+item.Name+"'")
		}
	case *ast.LiteralExpr:
		switch item.Kind {
		case ast.LiteralInteger:
			result = &SemanticType{kind: type_integer, name: "int64", untyped_literal: true}
		case ast.LiteralFloat:
			result = &SemanticType{kind: type_float, name: "float64", untyped_literal: true}
		case ast.LiteralString, ast.LiteralInterpolatedString:
			result = &SemanticType{kind: type_string, name: "string"}
		case ast.LiteralCharacter:
			result = &SemanticType{kind: type_char, name: "char"}
		case ast.LiteralBool:
			result = &SemanticType{kind: type_bool, name: "bool"}
		case ast.LiteralNull:
			result = &SemanticType{kind: type_null, name: "null"}
		}
	case *ast.UnaryExpr:
		operand := analyzer.infer_expr(namespace, path, scope, item.Operand, expected)
		if item.Operator == "++" || item.Operator == "--" {
			if !is_assignable_expression(item.Operand) {
				analyzer.add_error(path, item, item.Operator+" requires an assignable target")
			}
			if expression_is_const_target(analyzer, item.Operand) {
				analyzer.add_error(path, item, "cannot modify a const value")
			}
			if !is_numeric_type(operand) {
				analyzer.add_error(path, item, item.Operator+" requires a numeric target")
			}
			if identifier, ok := item.Operand.(*ast.IdentifierExpr); ok {
				analyzer.invalidate_integer_constant(scope, identifier.Name)
			}
		}
		result = clone_type(operand)
		if result == nil {
			result = &SemanticType{kind: type_unknown}
		}
		switch item.Operator {
		case "&":
			result.pointer_depth++
			result.kind = type_pointer
		case "*":
			if result.pointer_depth > 0 {
				result.pointer_depth--
			}
			if result.pointer_depth == 0 {
				switch result.name {
				case "bool":
					result.kind = type_bool
				case "char":
					result.kind = type_char
				case "string":
					result.kind = type_string
				case "float32", "float64":
					result.kind = type_float
				case "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64", "uintptr":
					result.kind = type_integer
				default:
					result.kind = type_struct
				}
			}
		}
	case *ast.BinaryExpr:
		left := analyzer.infer_expr(namespace, path, scope, item.Left, nil)
		right := analyzer.infer_expr(namespace, path, scope, item.Right, left)
		switch item.Operator {
		case "==", "!=", "<", "<=", ">", ">=", "&&", "||":
			result = &SemanticType{kind: type_bool, name: "bool"}
		default:
			if is_numeric_type(left) && is_numeric_type(right) {
				if left.untyped_literal {
					result = right
				} else {
					result = left
				}
			} else if item.Operator == "+" && left.name == "string" && right.name == "string" {
				result = left
			} else {
				result = left
			}
		}
	case *ast.AssignExpr:
		target := analyzer.infer_expr(namespace, path, scope, item.Target, nil)
		value := analyzer.infer_expr(namespace, path, scope, item.Value, target)
		if indexed, ok := item.Target.(*ast.IndexExpr); ok {
			object := analyzer.expression_types[indexed.Object]
			if object != nil && len(object.array_dimensions) == 0 && object.pointer_depth == 0 && object.kind != type_pointer && object.name == "Map" && len(object.generic_args) == 2 {
				indexType := analyzer.expression_types[indexed.Index]
				if !types_compatible(object.generic_args[0], indexType) {
					analyzer.add_error(path, indexed.Index, "map index type mismatch")
				}
				if !types_compatible(object.generic_args[1], value) {
					analyzer.add_error(path, item.Value, "map write value type mismatch")
				}
			}
			// Raw arrays and raw pointers use their storage element type directly.
			// Do not reinterpret T[] writes as protocol writes for T itself
			// (notably string[] -> string's char Indexable contract).
			if object != nil && len(object.array_dimensions) == 0 && object.pointer_depth == 0 && object.kind != type_pointer {
				if _, write := analyzer.indexable_methods(namespace, object); write != nil && write.type_info != nil && write.type_info.callable != nil && len(write.type_info.callable.parameters) == 2 {
					indexType := analyzer.expression_types[indexed.Index]
					if !types_compatible(write.type_info.callable.parameters[0], indexType) {
						analyzer.add_error(path, indexed.Index, "index type mismatch for Indexable write")
					}
					if !types_compatible(write.type_info.callable.parameters[1], value) {
						analyzer.add_error(path, item.Value, "Indexable write value type mismatch")
					}
				}
			}
		}
		if !is_assignable_expression(item.Target) {
			analyzer.add_error(path, item.Target, "assignment target is not assignable")
		}
		if expression_is_const_target(analyzer, item.Target) {
			analyzer.add_error(path, item.Target, "cannot assign through a const value")
		}
		if identifier, ok := item.Target.(*ast.IdentifierExpr); ok {
			analyzer.invalidate_integer_constant(scope, identifier.Name)
		}
		if !types_compatible(target, value) {
			analyzer.add_error(path, item, "assignment type mismatch: "+type_name(target)+" <- "+type_name(value))
		}
		result = target
	case *ast.MemberExpr:
		object := analyzer.infer_expr(namespace, path, scope, item.Object, nil)
		if object.kind == type_namespace {
			for _, candidate := range analyzer.namespaces {
				if candidate.name == object.namespace_name {
					if symbol := candidate.symbols[item.Name]; symbol != nil {
						result = symbol.type_info
					}
					break
				}
			}
		} else if object.name == "Error" {
			switch item.Name {
			case "error":
				result = &SemanticType{kind: type_bool, name: "bool"}
			case "error_type":
				result = &SemanticType{kind: type_error_type, name: "ErrorType"}
			case "error_message":
				result = &SemanticType{kind: type_string, name: "string"}
			default:
				analyzer.add_error(path, item, "Error has no member '"+item.Name+"'")
			}
		} else if object.name == "string" && item.Name == "length" {
			result = &SemanticType{kind: type_integer, name: "int64"}
		} else if (object.kind == type_array || len(object.array_dimensions) > 0) && item.Name == "length" {
			result = &SemanticType{kind: type_integer, name: "int64"}
		} else if object.kind == type_array || len(object.array_dimensions) > 0 {
			analyzer.add_error(path, item, "array has no member '"+item.Name+"'; the only built-in array member is 'length'")
		} else if info := analyzer.lookup_struct(namespace, object.name); info != nil {
			substitutions := build_substitutions(info.generic_parameters, object.generic_args)
			if field := info.fields[item.Name]; field != nil {
				if analyzer.symbol_is_private(field) && analyzer.current_receiver != info.name {
					analyzer.add_error(path, item, "private field '"+item.Name+"' is only accessible from struct '"+info.name+"'")
				}
				result = substitute_type(field.type_info, substitutions)
			} else if method := info.methods[item.Name]; method != nil {
				if analyzer.symbol_is_private(method) && analyzer.current_receiver != info.name {
					analyzer.add_error(path, item, "private method '"+item.Name+"' is only accessible from struct '"+info.name+"'")
				}
				result = substitute_type(method.type_info, substitutions)
			}
		}
	case *ast.CallExpr:
		// array<T>(length) is the explicit construction form for dynamic raw-array
		// storage. It allocates a fresh descriptor/backing store; it never mutates
		// or resizes an existing T[] value.
		if specialization, ok := item.Callee.(*ast.GenericSpecializationExpr); ok {
			if identifier, ok := specialization.Target.(*ast.IdentifierExpr); ok && identifier.Name == "array" {
				if len(specialization.TypeArguments) != 1 {
					analyzer.add_error(path, item, fmt.Sprintf("array<T> construction requires exactly one element type, got %d", len(specialization.TypeArguments)))
					result = &SemanticType{kind: type_unknown}
					break
				}
				element := analyzer.resolve_type_ref(namespace, specialization.TypeArguments[0], nil, path)
				if len(item.Arguments) != 1 && len(item.Arguments) != 2 {
					analyzer.add_error(path, item, fmt.Sprintf("array<T> construction expects (length) or (source, length), got %d arguments", len(item.Arguments)))
				} else {
					lengthIndex := 0
					if len(item.Arguments) == 2 {
						sourceExpected := clone_type(element)
						sourceExpected.array_dimensions = append([]SemanticArrayDimension{{dynamic: true}}, sourceExpected.array_dimensions...)
						sourceExpected.kind = type_array
						analyzer.infer_expr(namespace, path, scope, item.Arguments[0], sourceExpected)
						lengthIndex = 1
					}
					length_type := analyzer.infer_expr(namespace, path, scope, item.Arguments[lengthIndex], &SemanticType{kind: type_integer, name: "uint64"})
					if length_type.kind != type_integer {
						message := "array<T>(source, length) requires an integer length"
						if len(item.Arguments) == 1 {
							message = "array<T>(length) requires an integer length"
						}
						analyzer.add_error(path, item.Arguments[lengthIndex], message)
					}
				}
				result = clone_type(element)
				result.array_dimensions = append([]SemanticArrayDimension{{dynamic: true}}, result.array_dimensions...)
				result.kind = type_array
				break
			}
		}
		if identifier, ok := item.Callee.(*ast.IdentifierExpr); ok && identifier.Name == "catch" {
			if len(item.Arguments) != 1 {
				analyzer.add_error(path, item, fmt.Sprintf("catch expects exactly one callable argument, got %d", len(item.Arguments)))
			} else {
				expected_callback := &SemanticType{kind: type_callable, callable: &SemanticCallable{return_type: &SemanticType{kind: type_void, name: "void"}}}
				analyzer.catch_depth++
				actual := analyzer.infer_expr(namespace, path, scope, item.Arguments[0], expected_callback)
				analyzer.catch_depth--
				if actual.callable == nil || len(actual.callable.parameters) != 0 || actual.callable.return_type.kind != type_void {
					analyzer.add_error(path, item.Arguments[0], "catch requires a void() callable")
				}
			}
			result = &SemanticType{kind: type_struct, name: "Error"}
			break
		}
		if identifier, ok := item.Callee.(*ast.IdentifierExpr); ok && identifier.Name == "raise" {
			if len(item.Arguments) != 2 {
				analyzer.add_error(path, item, fmt.Sprintf("raise expects ErrorType and string arguments, got %d", len(item.Arguments)))
			} else {
				error_type := analyzer.infer_expr(namespace, path, scope, item.Arguments[0], &SemanticType{kind: type_error_type, name: "ErrorType"})
				message_type := analyzer.infer_expr(namespace, path, scope, item.Arguments[1], &SemanticType{kind: type_string, name: "string"})
				if error_type.kind != type_error_type {
					analyzer.add_error(path, item.Arguments[0], "raise first argument must be a declared ErrorType")
				}
				if message_type.kind != type_string {
					analyzer.add_error(path, item.Arguments[1], "raise second argument must be a string")
				}
			}
			result = &SemanticType{kind: type_void, name: "void"}
			break
		}
		if identifier, ok := item.Callee.(*ast.IdentifierExpr); ok && identifier.Name == "free" {
			if len(item.Arguments) != 1 {
				analyzer.add_error(path, item, fmt.Sprintf("free expects exactly one argument, got %d", len(item.Arguments)))
			} else {
				argument := analyzer.infer_expr(namespace, path, scope, item.Arguments[0], nil)
				if argument.kind != type_null && !semantic_is_pointer(argument) {
					analyzer.add_error(path, item.Arguments[0], "free requires a raw pointer or null")
				}
			}
			result = &SemanticType{kind: type_void, name: "void"}
			break
		}
		callee := analyzer.infer_expr(namespace, path, scope, item.Callee, nil)
		if callee.callable != nil {
			callable := callee.callable
			fixed_count := len(callable.parameters)
			if callable.variadic && fixed_count > 0 {
				fixed_count--
			}
			for index, argument := range item.Arguments {
				expected_arg := (*SemanticType)(nil)
				heterogeneous := false
				if index < fixed_count {
					expected_arg = callable.parameters[index]
				} else if callable.variadic && len(callable.parameters) > 0 {
					expected_arg = callable.parameters[len(callable.parameters)-1]
					heterogeneous = callable.heterogeneous_variadic
				} else if index < len(callable.parameters) {
					expected_arg = callable.parameters[index]
				}
				actual := analyzer.infer_expr(namespace, path, scope, argument, expected_arg)
				if expected_arg != nil && !heterogeneous && !types_compatible(expected_arg, actual) {
					if expected_arg.kind != type_string || !analyzer.can_convert_to_string(namespace, actual) {
						analyzer.add_error(path, argument, "argument type mismatch: expected "+type_name(expected_arg)+", got "+type_name(actual))
					}
				}
			}
			if len(item.Arguments) < callable.required_parameters {
				analyzer.add_error(path, item, fmt.Sprintf("call expects at least %d argument(s), got %d", callable.required_parameters, len(item.Arguments)))
			} else if !callable.variadic && len(item.Arguments) > len(callable.parameters) {
				analyzer.add_error(path, item, fmt.Sprintf("call expects at most %d argument(s), got %d", len(callable.parameters), len(item.Arguments)))
			}
			result = callable.return_type
		} else if identifier, ok := item.Callee.(*ast.IdentifierExpr); ok {
			if info := analyzer.lookup_struct(namespace, identifier.Name); info != nil {
				if expected != nil && expected.name == info.name {
					result = clone_type(expected)
				} else {
					result = &SemanticType{kind: type_struct, name: info.name}
				}
			}
		}
	case *ast.GenericSpecializationExpr:
		target := analyzer.infer_expr(namespace, path, scope, item.Target, nil)
		args := make([]*SemanticType, 0, len(item.TypeArguments))
		for _, arg := range item.TypeArguments {
			args = append(args, analyzer.resolve_type_ref(namespace, arg, nil, path))
		}
		result = clone_type(target)
		result.generic_args = args
		base := type_name(target)
		if identifier, ok := item.Target.(*ast.IdentifierExpr); ok {
			base = identifier.Name
		}
		analyzer.record_specialization(base, args, item.Span(), path)
	case *ast.IndexExpr:
		object := analyzer.infer_expr(namespace, path, scope, item.Object, nil)
		indexType := analyzer.infer_expr(namespace, path, scope, item.Index, nil)
		if len(object.array_dimensions) > 0 {
			dimension := object.array_dimensions[0]
			analyzer_index_safety := index_safety_runtime
			if !dimension.dynamic && dimension.size != "" {
				if length, err := strconv.ParseInt(strings.ReplaceAll(dimension.size, "_", ""), 0, 64); err == nil {
					if bounds, ok := analyzer.static_index_range(scope, item.Index); ok {
						if bounds.minimum < 0 || bounds.maximum >= length {
							if identifier, is_identifier := item.Index.(*ast.IdentifierExpr); is_identifier {
								analyzer.add_error(path, item, fmt.Sprintf("iterating index value '%s' exceeds the length of known array; max array-length: %d, max-iterator invocation: %d, first invalid index: %d", identifier.Name, length, bounds.maximum, length))
							} else if bounds.minimum == bounds.maximum {
								analyzer.add_error(path, item, fmt.Sprintf("fixed array index %d is out of bounds; array length is %d and valid indices are 0..%d", bounds.maximum, length, length-1))
							} else {
								analyzer.add_error(path, item, fmt.Sprintf("index expression can exceed fixed array bounds; array length is %d, computed index range is %d..%d", length, bounds.minimum, bounds.maximum))
							}
						} else {
							analyzer_index_safety = index_safety_proven
						}
					}
				}
			}
			// AnalysisResult is constructed after body analysis, so stash the
			// classification alongside expression types until finalization.
			if analyzer.index_safety == nil {
				analyzer.index_safety = map[*ast.IndexExpr]IndexSafety{}
			}
			analyzer.index_safety[item] = analyzer_index_safety
			result = clone_type(object)
			result.array_dimensions = result.array_dimensions[1:]
			if len(result.array_dimensions) == 0 {
				switch result.name {
				case "bool":
					result.kind = type_bool
				case "char":
					result.kind = type_char
				case "float32", "float64":
					result.kind = type_float
				case "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64":
					result.kind = type_integer
				case "string":
					result.kind = type_string
				default:
					result.kind = type_struct
				}
			}
		} else if object.pointer_depth > 0 || object.kind == type_pointer {
			// Raw pointer indexing is C-like and deliberately unchecked. T*[i]
			// resolves to T while preserving any remaining pointer depth.
			result = clone_type(object)
			if result.pointer_depth > 0 {
				result.pointer_depth--
			}
			if result.pointer_depth > 0 {
				result.kind = type_pointer
			} else {
				switch result.name {
				case "bool":
					result.kind = type_bool
				case "char":
					result.kind = type_char
				case "float32", "float64":
					result.kind = type_float
				case "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64", "uintptr":
					result.kind = type_integer
				case "string":
					result.kind = type_string
				default:
					result.kind = type_struct
				}
			}
		} else if len(object.generic_args) > 0 && (object.name == "List" || object.name == "Set") {
			result = object.generic_args[0]
		} else if object.name == "Map" && len(object.generic_args) == 2 {
			if !types_compatible(object.generic_args[0], indexType) {
				analyzer.add_error(path, item.Index, "map index type mismatch: expected "+type_name(object.generic_args[0])+", got "+type_name(indexType))
			}
			result = clone_type(object.generic_args[1])
		} else if read, _ := analyzer.indexable_methods(namespace, object); read != nil && read.type_info != nil && read.type_info.callable != nil {
			callable := read.type_info.callable
			if len(callable.parameters) == 1 && !types_compatible(callable.parameters[0], indexType) {
				analyzer.add_error(path, item.Index, "index type mismatch: expected "+type_name(callable.parameters[0])+", got "+type_name(indexType))
			}
			result = clone_type(callable.return_type)
		} else {
			result = &SemanticType{kind: type_unknown}
		}
	case *ast.ArrayExpr:
		if len(item.Elements) == 0 {
			result = &SemanticType{kind: type_array, name: "unknown", array_dimensions: []SemanticArrayDimension{{dynamic: true}}}
		} else {
			element := analyzer.infer_expr(namespace, path, scope, item.Elements[0], nil)
			for _, value := range item.Elements[1:] {
				analyzer.infer_expr(namespace, path, scope, value, element)
			}
			result = clone_type(element)
			result.array_dimensions = append([]SemanticArrayDimension{{dynamic: true}}, result.array_dimensions...)
			result.kind = type_array
		}
	case *ast.KeyValueExpr:
		analyzer.infer_expr(namespace, path, scope, item.Key, nil)
		analyzer.infer_expr(namespace, path, scope, item.Value, nil)
	case *ast.LambdaExpr:
		lambda_scope := analyzer.new_scope(scope.id, "lambda")
		expected_callable := (*SemanticCallable)(nil)
		if expected != nil {
			expected_callable = expected.callable
		}
		if expected_callable != nil && len(expected_callable.parameters) != len(item.Parameters) {
			analyzer.add_error(path, item, "lambda parameter count does not match receiving callable type")
		}
		parameters := make([]*SemanticType, 0, len(item.Parameters))
		for parameter_index, parameter := range item.Parameters {
			var parameter_type *SemanticType
			if parameter.Type != nil {
				parameter_type = analyzer.resolve_type_ref(namespace, parameter.Type, nil, path)
			} else if expected_callable != nil && parameter_index < len(expected_callable.parameters) {
				parameter_type = clone_type(expected_callable.parameters[parameter_index])
			} else {
				parameter_type = &SemanticType{kind: type_unknown}
				analyzer.add_error(path, parameter, "lambda parameter '"+parameter.Name+"' requires a type or a receiving callable signature")
			}
			parameters = append(parameters, parameter_type)
			analyzer.declare_local(path, lambda_scope, parameter, parameter.Name, symbol_parameter, parameter_type)
		}

		return_type := (*SemanticType)(nil)
		if item.ReturnType != nil {
			return_type = analyzer.resolve_type_ref(namespace, item.ReturnType, nil, path)
		} else if expected_callable != nil {
			return_type = clone_type(expected_callable.return_type)
		} else {
			return_type = analyzer.infer_lambda_return_type(namespace, path, lambda_scope, item.Body)
		}
		if return_type == nil {
			return_type = &SemanticType{kind: type_void, name: "void"}
		}
		analyzer.analyze_block(namespace, path, lambda_scope, item.Body, return_type)
		result = &SemanticType{kind: type_callable, callable: &SemanticCallable{return_type: return_type, parameters: parameters}}
		if expected_callable != nil && !callables_compatible(expected_callable, result.callable) {
			analyzer.add_error(path, item, "lambda type "+type_name(result)+" does not match receiving callable type "+type_name(expected))
		}
	case *ast.NewExpr:
		value_type := analyzer.infer_expr(namespace, path, scope, item.Value, nil)
		result = clone_type(value_type)
		if result == nil {
			result = &SemanticType{kind: type_unknown}
		}
		// new always yields a raw pointer to the allocated object. Pointer
		// mutability/constness and allocation ownership are tracked separately.
		result.pointer_depth++
		result.kind = type_pointer
	case *ast.CastExpr:
		analyzer.infer_expr(namespace, path, scope, item.Value, nil)
		result = analyzer.resolve_type_ref(namespace, item.Type, nil, path)
	}
	analyzer.expression_types[expression] = result
	return result
}

func (analyzer *SemanticAnalyzer) can_convert_to_string(namespace *Namespace, actual *SemanticType) bool {
	if actual == nil {
		return false
	}
	switch actual.kind {
	case type_string, type_char, type_bool, type_integer, type_float, type_enum, type_error_type, type_pointer, type_null:
		return true
	case type_struct:
		if info := analyzer.lookup_struct(namespace, actual.name); info != nil {
			if method := info.methods["to_string"]; method != nil && method.type_info != nil && method.type_info.callable != nil {
				callable := method.type_info.callable
				return len(callable.parameters) == 0 && callable.return_type != nil && callable.return_type.kind == type_string
			}
		}
	}
	return false
}

func (analyzer *SemanticAnalyzer) symbol_is_private(symbol *Symbol) bool {
	if symbol == nil || symbol.declaration == nil {
		return false
	}
	switch declaration := symbol.declaration.(type) {
	case *ast.FieldDecl:
		return declaration.Visibility == ast.VisibilityPrivate
	case *ast.MethodDecl:
		return declaration.Visibility == ast.VisibilityPrivate
	case *ast.FunctionDecl:
		return declaration.Visibility == ast.VisibilityPrivate
	}
	return false
}

// infer_lambda_return_type determines the callable return type when no receiving
// signature or explicit lambda return type is available. All reachable value-return
// expressions must agree; a lambda with no value returns resolves to void.
func (analyzer *SemanticAnalyzer) infer_lambda_return_type(namespace *Namespace, path string, scope *Scope, block *ast.BlockStmt) *SemanticType {
	var inferred *SemanticType
	var visit_statement func(ast.Stmt)
	visit_statement = func(statement ast.Stmt) {
		switch item := statement.(type) {
		case *ast.ReturnStmt:
			candidate := &SemanticType{kind: type_void, name: "void"}
			if item.Value != nil {
				candidate = analyzer.infer_expr(namespace, path, scope, item.Value, nil)
			}
			if inferred == nil {
				inferred = clone_type(candidate)
			} else if !types_compatible(inferred, candidate) || !types_compatible(candidate, inferred) {
				analyzer.add_error(path, item, "lambda return expressions do not resolve to a compatible type")
			}
		case *ast.BlockStmt:
			for _, nested := range item.Statements {
				visit_statement(nested)
			}
		case *ast.IfStmt:
			visit_statement(item.Then)
			if item.Else != nil {
				visit_statement(item.Else)
			}
		case *ast.SwitchStmt:
			for _, switch_case := range item.Cases {
				for _, nested := range switch_case.Statements {
					visit_statement(nested)
				}
			}
		}
	}
	if block != nil {
		for _, statement := range block.Statements {
			visit_statement(statement)
		}
	}
	if inferred == nil {
		return &SemanticType{kind: type_void, name: "void"}
	}
	return inferred
}

func (analyzer *SemanticAnalyzer) lookup_integer_constant(scope *Scope, name string) (int64, bool) {
	current := scope
	for current != nil {
		if values := analyzer.integer_constants[current.id]; values != nil {
			if value, ok := values[name]; ok {
				return value, true
			}
		}
		if current.parent_id < 0 {
			break
		}
		current = analyzer.scopes[current.parent_id]
	}
	return 0, false
}

func (analyzer *SemanticAnalyzer) invalidate_integer_constant(scope *Scope, name string) {
	current := scope
	for current != nil {
		if _, ok := analyzer.integer_constants[current.id][name]; ok {
			delete(analyzer.integer_constants[current.id], name)
			return
		}
		if current.parent_id < 0 {
			return
		}
		current = analyzer.scopes[current.parent_id]
	}
}

func loop_step_for_identifier(statement ast.Stmt, name string) (int64, bool) {
	block, ok := statement.(*ast.BlockStmt)
	if !ok {
		return 0, false
	}
	for _, nested := range block.Statements {
		expression_statement, ok := nested.(*ast.ExprStmt)
		if !ok {
			continue
		}
		switch expression := expression_statement.Expression.(type) {
		case *ast.UnaryExpr:
			identifier, ok := expression.Operand.(*ast.IdentifierExpr)
			if !ok || identifier.Name != name {
				continue
			}
			if expression.Operator == "++" {
				return 1, true
			}
			if expression.Operator == "--" {
				return -1, true
			}
		case *ast.AssignExpr:
			identifier, ok := expression.Target.(*ast.IdentifierExpr)
			if !ok || identifier.Name != name {
				continue
			}
			value, ok := static_integer_value(expression.Value)
			if !ok {
				continue
			}
			if expression.Operator == "+=" {
				return value, true
			}
			if expression.Operator == "-=" {
				return -value, true
			}
		}
	}
	return 0, false
}

func (analyzer *SemanticAnalyzer) derive_while_integer_range(scope *Scope, loop *ast.WhileStmt) (string, integer_range, bool) {
	if loop == nil || loop.Condition == nil || loop.Body == nil {
		return "", integer_range{}, false
	}
	condition, ok := loop.Condition.(*ast.BinaryExpr)
	if !ok {
		return "", integer_range{}, false
	}
	identifier, ok := condition.Left.(*ast.IdentifierExpr)
	if !ok {
		return "", integer_range{}, false
	}
	start, ok := analyzer.lookup_integer_constant(scope, identifier.Name)
	if !ok {
		return "", integer_range{}, false
	}
	limit, ok := analyzer.static_integer_value(scope, condition.Right)
	if !ok {
		return "", integer_range{}, false
	}
	step, ok := loop_step_for_identifier(loop.Body, identifier.Name)
	if !ok || step == 0 {
		return "", integer_range{}, false
	}
	if step > 0 {
		switch condition.Operator {
		case "<":
			if start >= limit {
				return "", integer_range{}, false
			}
			return identifier.Name, integer_range{minimum: start, maximum: limit - 1}, true
		case "<=":
			if start > limit {
				return "", integer_range{}, false
			}
			return identifier.Name, integer_range{minimum: start, maximum: limit}, true
		}
	} else {
		switch condition.Operator {
		case ">":
			if start <= limit {
				return "", integer_range{}, false
			}
			return identifier.Name, integer_range{minimum: limit + 1, maximum: start}, true
		case ">=":
			if start < limit {
				return "", integer_range{}, false
			}
			return identifier.Name, integer_range{minimum: limit, maximum: start}, true
		}
	}
	return "", integer_range{}, false
}

func (analyzer *SemanticAnalyzer) derive_for_integer_range(scope *Scope, loop *ast.ForStmt) (string, integer_range, bool) {
	if loop == nil || loop.Init == nil || loop.Condition == nil || loop.Post == nil {
		return "", integer_range{}, false
	}
	init, ok := loop.Init.(*ast.VarDeclStmt)
	if !ok || init.Initializer == nil {
		return "", integer_range{}, false
	}
	start, ok := analyzer.static_integer_value(scope, init.Initializer)
	if !ok {
		return "", integer_range{}, false
	}
	condition, ok := loop.Condition.(*ast.BinaryExpr)
	if !ok {
		return "", integer_range{}, false
	}
	identifier, ok := condition.Left.(*ast.IdentifierExpr)
	if !ok || identifier.Name != init.Name {
		return "", integer_range{}, false
	}
	limit, ok := analyzer.static_integer_value(scope, condition.Right)
	if !ok {
		return "", integer_range{}, false
	}
	post, ok := loop.Post.(*ast.UnaryExpr)
	if !ok || post.Prefix {
		return "", integer_range{}, false
	}
	post_identifier, ok := post.Operand.(*ast.IdentifierExpr)
	if !ok || post_identifier.Name != init.Name {
		return "", integer_range{}, false
	}
	switch {
	case post.Operator == "++" && condition.Operator == "<":
		if start >= limit {
			return init.Name, integer_range{}, false
		}
		return init.Name, integer_range{minimum: start, maximum: limit - 1}, true
	case post.Operator == "++" && condition.Operator == "<=":
		if start > limit {
			return init.Name, integer_range{}, false
		}
		return init.Name, integer_range{minimum: start, maximum: limit}, true
	case post.Operator == "--" && condition.Operator == ">":
		if start <= limit {
			return init.Name, integer_range{}, false
		}
		return init.Name, integer_range{minimum: limit + 1, maximum: start}, true
	case post.Operator == "--" && condition.Operator == ">=":
		if start < limit {
			return init.Name, integer_range{}, false
		}
		return init.Name, integer_range{minimum: limit, maximum: start}, true
	}
	return "", integer_range{}, false
}

func (analyzer *SemanticAnalyzer) static_index_range(scope *Scope, expression ast.Expr) (integer_range, bool) {
	switch item := expression.(type) {
	case *ast.IdentifierExpr:
		for index := len(analyzer.integer_ranges) - 1; index >= 0; index-- {
			if value, ok := analyzer.integer_ranges[index][item.Name]; ok {
				return value, true
			}
		}
		if value, ok := analyzer.lookup_integer_constant(scope, item.Name); ok {
			return integer_range{minimum: value, maximum: value}, true
		}
	case *ast.BinaryExpr:
		left, left_ok := analyzer.static_index_range(scope, item.Left)
		right, right_ok := analyzer.static_index_range(scope, item.Right)
		if !left_ok || !right_ok {
			return integer_range{}, false
		}
		switch item.Operator {
		case "+":
			return integer_range{minimum: left.minimum + right.minimum, maximum: left.maximum + right.maximum}, true
		case "-":
			return integer_range{minimum: left.minimum - right.maximum, maximum: left.maximum - right.minimum}, true
		case "*":
			values := []int64{left.minimum * right.minimum, left.minimum * right.maximum, left.maximum * right.minimum, left.maximum * right.maximum}
			minimum, maximum := values[0], values[0]
			for _, value := range values[1:] {
				if value < minimum {
					minimum = value
				}
				if value > maximum {
					maximum = value
				}
			}
			return integer_range{minimum: minimum, maximum: maximum}, true
		}
	default:
		if value, ok := analyzer.static_integer_value(scope, expression); ok {
			return integer_range{minimum: value, maximum: value}, true
		}
	}
	return integer_range{}, false
}

func (analyzer *SemanticAnalyzer) static_integer_value(scope *Scope, expression ast.Expr) (int64, bool) {
	if value, ok := static_integer_value(expression); ok {
		return value, true
	}
	if identifier, ok := expression.(*ast.IdentifierExpr); ok {
		return analyzer.lookup_integer_constant(scope, identifier.Name)
	}
	return 0, false
}

func static_integer_value(expression ast.Expr) (int64, bool) {
	switch item := expression.(type) {
	case *ast.LiteralExpr:
		if item.Kind != ast.LiteralInteger {
			return 0, false
		}
		value, err := strconv.ParseInt(strings.ReplaceAll(item.Raw, "_", ""), 0, 64)
		return value, err == nil
	case *ast.UnaryExpr:
		if item.Operator == "-" {
			if value, ok := static_integer_value(item.Operand); ok {
				return -value, true
			}
		}
	}
	return 0, false
}

func is_assignable_expr(expression ast.Expr) bool {
	switch expression.(type) {
	case *ast.IdentifierExpr, *ast.MemberExpr, *ast.IndexExpr:
		return true
	default:
		return false
	}
}

func build_substitutions(names []string, args []*SemanticType) map[string]*SemanticType {
	result := map[string]*SemanticType{}
	for index, name := range names {
		if index < len(args) {
			result[name] = args[index]
		}
	}
	return result
}
func substitute_type(input *SemanticType, substitutions map[string]*SemanticType) *SemanticType {
	if input == nil {
		return nil
	}
	if replacement := substitutions[input.name]; replacement != nil {
		result := clone_type(replacement)
		result.pointer_depth += input.pointer_depth
		result.array_dimensions = append(result.array_dimensions, input.array_dimensions...)
		result.is_const = result.is_const || input.is_const
		if len(result.array_dimensions) > 0 {
			result.kind = type_array
		} else if result.pointer_depth > 0 {
			result.kind = type_pointer
		}
		return result
	}
	result := clone_type(input)
	for index, arg := range result.generic_args {
		result.generic_args[index] = substitute_type(arg, substitutions)
	}
	if result.callable != nil {
		result.callable.return_type = substitute_type(result.callable.return_type, substitutions)
		for index, param := range result.callable.parameters {
			result.callable.parameters[index] = substitute_type(param, substitutions)
		}
	}
	return result
}

// record_specialization stores deterministic metadata for a resolved generic type.
func (analyzer *SemanticAnalyzer) record_specialization(base_name string, args []*SemanticType, source_span token.Span, path string) {
	key := path + ":" + strconv.Itoa(source_span.Start.Offset) + ":" + base_name + ":" + specialization_name(base_name, args)
	if analyzer.specialization_keys[key] {
		return
	}
	analyzer.specialization_keys[key] = true
	analyzer.specializations = append(analyzer.specializations, GenericSpecialization{base_name: base_name, concrete_name: specialization_name(base_name, args), type_arguments: args, source_span: source_span, source_path: path})
}

func is_assignable_expression(expression ast.Expr) bool {
	switch item := expression.(type) {
	case *ast.IdentifierExpr, *ast.MemberExpr, *ast.IndexExpr:
		return true
	case *ast.UnaryExpr:
		return item.Operator == "*"
	default:
		return false
	}
}

func expression_is_const_target(analyzer *SemanticAnalyzer, expression ast.Expr) bool {
	if expression == nil {
		return false
	}
	if resolved := analyzer.expression_types[expression]; resolved != nil && resolved.is_const {
		return true
	}
	switch item := expression.(type) {
	case *ast.MemberExpr:
		object := analyzer.expression_types[item.Object]
		return object != nil && object.is_const
	case *ast.IndexExpr:
		object := analyzer.expression_types[item.Object]
		return object != nil && object.is_const
	case *ast.UnaryExpr:
		if item.Operator == "*" {
			pointer := analyzer.expression_types[item.Operand]
			return pointer != nil && pointer.is_const
		}
	}
	return false
}
