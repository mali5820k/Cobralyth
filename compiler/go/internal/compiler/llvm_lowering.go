package compiler

import (
	"fmt"
	"strconv"
	"strings"

	"github.com/mali5820k/clyth-bootstrap/internal/frontend/ast"
	"github.com/mali5820k/clyth-bootstrap/internal/frontend/parser"
)

// IRValue is the SSA value returned by the textual LLVM lowering layer.
// Aggregate Clyth values are deliberately represented by opaque LLVM pointers in
// the bootstrap backend. This keeps native ABI details isolated while the frontend
// and self-hosted compiler stabilize.
type IRValue struct {
	llvm_type string
	value     string
	type_info *SemanticType
	fresh     bool // unique aggregate temporary; value-copy destinations may consume it directly
}

type ir_local struct {
	address   string
	type_info *SemanticType
}

type value_cleanup struct {
	address   string
	type_info *SemanticType
}

type catch_context struct {
	error_value string
	end_label   string
	block_depth int
}

type llvm_lowerer struct {
	result                     *AnalysisResult
	program                    *ast.Program
	target                     string
	strings                    []string
	declarations               map[string]string
	defined                    map[string]bool
	struct_info                map[string]*StructInfo
	enum_values                map[string]map[string]int64
	register                   int
	label                      int
	lambda                     int
	function_defs              []string
	global_defs                []string
	dynamic_global_inits       []*ast.GlobalVarDecl
	globals                    map[string]ir_local
	current_lines              []string
	current_block              string
	locals                     map[string]ir_local
	break_labels               []string
	continue_labels            []string
	break_cleanup_depths       []int
	continue_cleanup_depths    []int
	return_type                *SemanticType
	receiver                   *StructInfo
	substitutions              map[string]*SemanticType
	external_specializations   []GenericSpecialization
	main_args_name             string
	main_argc_value            string
	lifetime_actions           map[ast.Node][]LifetimeAction
	cleanup_slots              map[int]string
	catch_stack                []catch_context
	block_stack                []*ast.BlockStmt
	mecc_root_scopes           [][]string
	value_cleanup_scopes       [][]value_cleanup
	pending_parameter_cleanups []value_cleanup
}

// lower_llvm_ir converts a semantically-resolved Clyth translation unit into
// textual LLVM IR. The bootstrap backend favors a small, deterministic ABI:
// primitive scalars retain their natural LLVM widths and Clyth aggregate values
// are passed as opaque pointers.
func lower_llvm_ir(program *ast.Program, result *AnalysisResult, target string) ([]byte, error) {
	return lower_llvm_ir_with_specializations(program, result, target, nil)
}

// lower_llvm_ir_with_specializations lowers a translation unit while honoring
// concrete generic instantiations requested by another unit. This is what lets
// a program instantiate List<int32> while the List template itself lives in a
// separately compiled runtime-module source file.
func lower_llvm_ir_with_specializations(program *ast.Program, result *AnalysisResult, target string, external_specializations []GenericSpecialization) ([]byte, error) {
	if target == "" {
		target = "x86_64-unknown-linux-musl"
	}
	lowerer := &llvm_lowerer{
		result: result, program: program, target: target,
		declarations: map[string]string{}, defined: map[string]bool{},
		struct_info: map[string]*StructInfo{}, enum_values: map[string]map[string]int64{}, globals: map[string]ir_local{},
		external_specializations: external_specializations, substitutions: map[string]*SemanticType{},
		lifetime_actions: map[ast.Node][]LifetimeAction{}, cleanup_slots: map[int]string{},
	}
	for _, action := range result.lifetime_actions {
		lowerer.lifetime_actions[action.node] = append(lowerer.lifetime_actions[action.node], action)
	}
	lowerer.collect_types_and_defined_symbols()
	lowerer.install_builtin_error_struct()
	lowerer.emit_top_level()
	lowerer.emit_dynamic_global_initializer()

	var output strings.Builder
	output.WriteString("; Clyth V1.0.0 bootstrap LLVM IR\n")
	output.WriteString("target triple = \"")
	output.WriteString(target)
	output.WriteString("\"\n\n")
	// Pointer-backed value strings use null as their zero representation.
	// Keep a canonical empty descriptor so zero/default strings behave as "".
	output.WriteString("@.clyth.empty.string = private unnamed_addr constant { ptr, [15 x i8], i8, i64 } zeroinitializer\n\n")

	// new and constructor lowering share the ordinary native allocator. The
	// final ownership pass is responsible for choosing matching destruction.
	if !lowerer.defined["malloc"] {
		lowerer.declarations["malloc"] = "declare ptr @malloc(i64)"
	}
	if !lowerer.defined["calloc"] {
		lowerer.declarations["calloc"] = "declare ptr @calloc(i64, i64)"
	}
	if !lowerer.defined["strlen"] {
		lowerer.declarations["strlen"] = "declare i64 @strlen(ptr)"
	}
	if !lowerer.defined["strcmp"] {
		lowerer.declarations["strcmp"] = "declare i32 @strcmp(ptr, ptr)"
	}
	if !lowerer.defined["memcpy"] {
		lowerer.declarations["memcpy"] = "declare ptr @memcpy(ptr, ptr, i64)"
	}
	if !lowerer.defined["snprintf"] {
		lowerer.declarations["snprintf"] = "declare i32 @snprintf(ptr, i64, ptr, ...)"
	}
	if !lowerer.defined["free"] {
		lowerer.declarations["free"] = "declare void @free(ptr)"
	}
	if !lowerer.defined["abort"] {
		lowerer.declarations["abort"] = "declare void @abort() noreturn"
	}
	if result.mecc_enabled {
		lowerer.declarations["clyth_mecc_adopt"] = "declare ptr @clyth_mecc_adopt(ptr, i1, ptr)"
		lowerer.declarations["clyth_mecc_store"] = "declare void @clyth_mecc_store(ptr, ptr, ptr)"
		lowerer.declarations["clyth_mecc_owner_release"] = "declare void @clyth_mecc_owner_release(ptr)"
		lowerer.declarations["clyth_mecc_collapse"] = "declare void @clyth_mecc_collapse()"
		lowerer.declarations["clyth_mecc_release"] = "declare void @clyth_mecc_release(ptr)"
	}

	for index, text := range lowerer.strings {
		bytes := []byte(text)
		if len(bytes) <= 15 {
			inline := make([]byte, 15)
			copy(inline, bytes)
			fmt.Fprintf(&output, "@.clyth.string.%d = private unnamed_addr constant { ptr, [15 x i8], i8, i64 } { ptr null, [15 x i8] c\"%s\", i8 0, i64 %d }\n", index, llvm_escape_bytes(inline), len(bytes))
		} else {
			raw := append(append([]byte{}, bytes...), 0)
			fmt.Fprintf(&output, "@.clyth.str.%d = private unnamed_addr constant [%d x i8] c\"%s\"\n", index, len(raw), llvm_escape_bytes(raw))
			fmt.Fprintf(&output, "@.clyth.string.%d = private unnamed_addr constant { ptr, [15 x i8], i8, i64 } { ptr getelementptr inbounds ([%d x i8], ptr @.clyth.str.%d, i64 0, i64 0), [15 x i8] zeroinitializer, i8 0, i64 %d }\n", index, len(raw), index, len(bytes))
		}
	}
	if len(lowerer.strings) > 0 {
		output.WriteString("\n")
	}
	for _, definition := range lowerer.global_defs {
		output.WriteString(definition)
		output.WriteByte('\n')
	}
	if len(lowerer.global_defs) > 0 {
		output.WriteByte('\n')
	}
	if len(lowerer.dynamic_global_inits) > 0 {
		output.WriteString("@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 65535, ptr @__clyth_init_globals, ptr null }]\n\n")
	}

	declaration_names := make([]string, 0, len(lowerer.declarations))
	for name := range lowerer.declarations {
		if !lowerer.defined[name] {
			declaration_names = append(declaration_names, name)
		}
	}
	sort_strings(declaration_names)
	for _, name := range declaration_names {
		output.WriteString(lowerer.declarations[name])
		output.WriteByte('\n')
	}
	if len(declaration_names) > 0 {
		output.WriteByte('\n')
	}
	for _, definition := range lowerer.function_defs {
		output.WriteString(definition)
		if !strings.HasSuffix(definition, "\n") {
			output.WriteByte('\n')
		}
		output.WriteByte('\n')
	}
	return []byte(output.String()), nil
}

func sort_strings(values []string) {
	for i := 1; i < len(values); i++ {
		for j := i; j > 0 && values[j] < values[j-1]; j-- {
			values[j], values[j-1] = values[j-1], values[j]
		}
	}
}

func llvm_escape_bytes(bytes []byte) string {
	var builder strings.Builder
	for _, value := range bytes {
		if value >= 32 && value <= 126 && value != '"' && value != '\\' {
			builder.WriteByte(value)
		} else {
			fmt.Fprintf(&builder, "\\%02X", value)
		}
	}
	return builder.String()
}

func (lowerer *llvm_lowerer) requested_specializations(base_name string) []GenericSpecialization {
	seen := map[string]bool{}
	result := []GenericSpecialization{}
	all := append(append([]GenericSpecialization{}, lowerer.result.specializations...), lowerer.external_specializations...)
	for _, specialization := range all {
		if specialization.base_name != base_name || !specialization_is_concrete(specialization) {
			continue
		}
		key := specialization_name(base_name, specialization.type_arguments)
		if seen[key] {
			continue
		}
		seen[key] = true
		result = append(result, specialization)
	}
	return result
}

func collect_concrete_generic_types(type_info *SemanticType, found map[string]GenericSpecialization) {
	if type_info == nil {
		return
	}
	if len(type_info.generic_args) > 0 {
		specialization := GenericSpecialization{base_name: type_info.name, concrete_name: specialization_name(type_info.name, type_info.generic_args), type_arguments: type_info.generic_args}
		if specialization_is_concrete(specialization) {
			found[specialization.concrete_name] = specialization
		}
	}
	for _, argument := range type_info.generic_args {
		collect_concrete_generic_types(argument, found)
	}
	if type_info.callable != nil {
		collect_concrete_generic_types(type_info.callable.return_type, found)
		for _, parameter := range type_info.callable.parameters {
			collect_concrete_generic_types(parameter, found)
		}
	}
}

func (lowerer *llvm_lowerer) expand_nested_specializations() {
	found := map[string]GenericSpecialization{}
	for _, specialization := range append(append([]GenericSpecialization{}, lowerer.result.specializations...), lowerer.external_specializations...) {
		if specialization_is_concrete(specialization) {
			found[specialization.concrete_name] = specialization
		}
	}
	changed := true
	for changed {
		changed = false
		for _, specialization := range found {
			template := lowerer.struct_info[specialization.base_name]
			if template == nil {
				continue
			}
			concrete := specialized_struct_info(template, specialization.type_arguments)
			for _, field := range concrete.fields {
				before := len(found)
				collect_concrete_generic_types(field.type_info, found)
				if len(found) > before {
					changed = true
				}
			}
			for _, method := range concrete.methods {
				before := len(found)
				collect_concrete_generic_types(method.type_info, found)
				if len(found) > before {
					changed = true
				}
			}
		}
	}
	for _, specialization := range found {
		present := false
		for _, existing := range lowerer.external_specializations {
			if existing.concrete_name == specialization.concrete_name {
				present = true
				break
			}
		}
		if !present {
			lowerer.external_specializations = append(lowerer.external_specializations, specialization)
		}
	}
}

func (lowerer *llvm_lowerer) collect_types_and_defined_symbols() {
	// Imported structs participate in field/method lowering even when their bodies
	// are linked from a separate source/module object.
	for _, namespace := range lowerer.result.namespaces {
		for name, info := range namespace.structs {
			if lowerer.struct_info[name] == nil {
				lowerer.struct_info[name] = info
			}
			for _, specialization := range lowerer.requested_specializations(name) {
				concrete := specialized_struct_info(info, specialization.type_arguments)
				lowerer.struct_info[concrete.name] = concrete
			}
		}
	}
	lowerer.expand_nested_specializations()
	for name, info := range lowerer.struct_info {
		if len(info.generic_parameters) == 0 {
			continue
		}
		for _, specialization := range lowerer.requested_specializations(name) {
			concrete := specialized_struct_info(info, specialization.type_arguments)
			lowerer.struct_info[concrete.name] = concrete
		}
	}
	for _, declaration := range lowerer.program.Declarations {
		switch item := declaration.(type) {
		case *ast.StructDecl:
			info := lowerer.result.root_namespace.structs[item.Name]
			if info != nil {
				lowerer.struct_info[item.Name] = info
			}
			if len(item.GenericParameters) == 0 {
				for _, member := range item.Members {
					switch member_item := member.(type) {
					case *ast.MethodDecl:
						lowerer.defined[llvm_method_name(item.Name, member_item.Name)] = true
					case *ast.ConstructorMember:
						lowerer.defined[llvm_constructor_name(item.Name)] = true
					case *ast.DestructorMember:
						lowerer.defined[llvm_destructor_name(item.Name)] = true
					}
				}
			} else if info != nil {
				for _, specialization := range lowerer.requested_specializations(item.Name) {
					concrete := specialized_struct_info(info, specialization.type_arguments)
					lowerer.struct_info[concrete.name] = concrete
					for _, member := range item.Members {
						switch member_item := member.(type) {
						case *ast.MethodDecl:
							lowerer.defined[llvm_method_name(concrete.name, member_item.Name)] = true
						case *ast.ConstructorMember:
							lowerer.defined[llvm_constructor_name(concrete.name)] = true
						}
					}
				}
			}
		case *ast.MethodBlockDecl:
			receiver_name := item.Receiver.Name
			if info := lowerer.result.root_namespace.structs[receiver_name]; info != nil {
				lowerer.struct_info[receiver_name] = info
			}
			for _, member := range item.Members {
				if method, ok := member.(*ast.MethodDecl); ok {
					lowerer.defined[llvm_method_name(receiver_name, method.Name)] = true
				}
			}
		case *ast.FunctionDecl:
			lowerer.defined[llvm_function_name(item)] = item.Body != nil && !item.Extern
			if item.Extern {
				lowerer.declarations[item.Name] = lowerer.function_declaration(item)
			}
		case *ast.GlobalVarDecl:
			sem := lowerer.resolve_ref(item.Type)
			if item.Type != nil && item.Type.Name == "auto" {
				if symbol := lowerer.result.root_namespace.symbols[item.Name]; symbol != nil && symbol.type_info != nil {
					sem = clone_type(symbol.type_info)
				}
			}
			name := sanitize_llvm_name(item.Name)
			lowerer.globals[item.Name] = ir_local{address: "@" + name, type_info: sem}
			initial, constant := lowerer.constant_initializer(item.Initializer, sem)
			if item.Initializer != nil && !constant {
				lowerer.dynamic_global_inits = append(lowerer.dynamic_global_inits, item)
			}
			lowerer.global_defs = append(lowerer.global_defs, fmt.Sprintf("@%s = global %s %s", name, lowerer.llvm_type(sem), initial))
		case *ast.EnumDecl:
			values := map[string]int64{}
			next := int64(0)
			for _, member := range item.Members {
				if literal, ok := member.Value.(*ast.LiteralExpr); ok && literal.Kind == ast.LiteralInteger {
					if parsed, err := strconv.ParseInt(strings.TrimSpace(literal.Raw), 0, 64); err == nil {
						next = parsed
					}
				}
				values[member.Name] = next
				next++
			}
			lowerer.enum_values[item.Name] = values
		}
	}
}

func (lowerer *llvm_lowerer) install_builtin_error_struct() {
	if lowerer.struct_info["Error"] != nil {
		return
	}
	info := &StructInfo{name: "Error", fields: map[string]*Symbol{}, methods: map[string]*Symbol{}, field_order: []string{"error", "error_type", "error_message"}}
	info.fields["error"] = &Symbol{name: "error", kind: symbol_field, type_info: &SemanticType{kind: type_bool, name: "bool"}}
	info.fields["error_type"] = &Symbol{name: "error_type", kind: symbol_field, type_info: &SemanticType{kind: type_error_type, name: "ErrorType"}}
	info.fields["error_message"] = &Symbol{name: "error_message", kind: symbol_field, type_info: &SemanticType{kind: type_string, name: "string"}}
	// A non-default synthetic signature prevents default_struct_value from
	// expecting an Error__ctor symbol. Error values are created by catch().
	info.constructors = []*SemanticCallable{{return_type: &SemanticType{kind: type_struct, name: "Error"}, parameters: []*SemanticType{{kind: type_bool, name: "bool"}}}}
	lowerer.struct_info["Error"] = info
}

func (lowerer *llvm_lowerer) emit_top_level() {
	for _, declaration := range lowerer.program.Declarations {
		switch item := declaration.(type) {
		case *ast.FunctionDecl:
			if item.Body != nil && !item.Extern {
				lowerer.emit_function(item, nil)
			}
		case *ast.StructDecl:
			template := lowerer.struct_info[item.Name]
			if len(item.GenericParameters) == 0 {
				lowerer.emit_struct_declaration(item, template, nil)
				continue
			}
			for _, specialization := range lowerer.requested_specializations(item.Name) {
				concrete := lowerer.struct_info[specialization_name(item.Name, specialization.type_arguments)]
				if concrete == nil {
					concrete = specialized_struct_info(template, specialization.type_arguments)
					lowerer.struct_info[concrete.name] = concrete
				}
				subs := build_substitutions(template.generic_parameters, specialization.type_arguments)
				lowerer.emit_struct_declaration(item, concrete, subs)
			}
		case *ast.MethodBlockDecl:
			info := lowerer.struct_info[item.Receiver.Name]
			for _, member := range item.Members {
				if method, ok := member.(*ast.MethodDecl); ok {
					lowerer.emit_function(&method.FunctionDecl, info)
				}
			}
		}
	}
}

// emit_dynamic_global_initializer lowers non-constant global initializers into
// one deterministic process-start constructor. Globals themselves are emitted
// with zero/null storage first, then initialized in source declaration order
// before main runs. This keeps the executable self-contained and gives global
// initializers the same expression semantics as ordinary Clyth code.
func (lowerer *llvm_lowerer) emit_dynamic_global_initializer() {
	if len(lowerer.dynamic_global_inits) == 0 {
		return
	}

	lowerer.register, lowerer.label = 0, 0
	lowerer.current_lines = nil
	lowerer.current_block = ""
	lowerer.locals = map[string]ir_local{}
	lowerer.break_labels, lowerer.continue_labels = nil, nil
	lowerer.break_cleanup_depths, lowerer.continue_cleanup_depths = nil, nil
	lowerer.return_type = &SemanticType{kind: type_void, name: "void"}
	lowerer.receiver = nil
	lowerer.substitutions = map[string]*SemanticType{}
	lowerer.cleanup_slots = map[int]string{}
	lowerer.catch_stack = nil
	lowerer.block_stack = nil
	lowerer.mecc_root_scopes = nil
	lowerer.value_cleanup_scopes = nil

	lowerer.raw_line("define internal void @__clyth_init_globals() {")
	lowerer.raw_line("entry:")
	for _, item := range lowerer.dynamic_global_inits {
		global, ok := lowerer.globals[item.Name]
		if !ok || global.type_info == nil || item.Initializer == nil {
			continue
		}
		value := lowerer.coerce(lowerer.emit_expr(item.Initializer), global.type_info)
		lowerer.line("store %s %s, ptr %s", lowerer.llvm_type(global.type_info), value.value, global.address)
		if lowerer.result.mecc_enabled && semantic_is_pointer(global.type_info) {
			lowerer.line("call void @clyth_mecc_store(ptr null, ptr %s, ptr %s)", global.address, value.value)
		}
	}
	lowerer.line("ret void")
	lowerer.raw_line("}")
	lowerer.function_defs = append(lowerer.function_defs, strings.Join(lowerer.current_lines, "\n"))
}

func (lowerer *llvm_lowerer) emit_struct_declaration(item *ast.StructDecl, info *StructInfo, substitutions map[string]*SemanticType) {
	if info == nil {
		return
	}
	saved := lowerer.substitutions
	if substitutions == nil {
		lowerer.substitutions = map[string]*SemanticType{}
	} else {
		lowerer.substitutions = substitutions
	}
	defer func() { lowerer.substitutions = saved }()
	has_constructor := false
	for _, member := range item.Members {
		switch member_item := member.(type) {
		case *ast.MethodDecl:
			lowerer.emit_function(&member_item.FunctionDecl, info)
		case *ast.ConstructorMember:
			has_constructor = true
			lowerer.emit_constructor(info.name, &member_item.ConstructorDecl, info)
		case *ast.DestructorMember:
			lowerer.emit_destructor(info.name, &member_item.DestructorDecl, info)
		}
	}
	if !has_constructor {
		lowerer.emit_default_constructor(info.name)
	}
}

func llvm_function_name(function *ast.FunctionDecl) string {
	if function.Receiver != "" {
		return llvm_method_name(function.Receiver, function.Name)
	}
	return sanitize_llvm_name(function.Name)
}

func llvm_method_name(receiver string, method string) string {
	return sanitize_llvm_name(receiver + "__" + method)
}

func llvm_constructor_name(name string) string { return sanitize_llvm_name(name + "__ctor") }
func llvm_destructor_name(name string) string  { return sanitize_llvm_name(name + "__dtor") }

func sanitize_llvm_name(name string) string {
	var builder strings.Builder
	for _, r := range name {
		if (r >= 'a' && r <= 'z') || (r >= 'A' && r <= 'Z') || (r >= '0' && r <= '9') || r == '_' || r == '.' {
			builder.WriteRune(r)
		} else {
			builder.WriteByte('_')
		}
	}
	if builder.Len() == 0 {
		return "anonymous"
	}
	return builder.String()
}

func (lowerer *llvm_lowerer) variadic_parameter_type(parameter *ast.Parameter) *SemanticType {
	if parameter == nil || parameter.Type == nil {
		return &SemanticType{kind: type_unknown}
	}
	if parameter.Type.Name == "auto" {
		return &SemanticType{kind: type_array, name: "auto_variadic", array_dimensions: []SemanticArrayDimension{{dynamic: true}}}
	}
	element := lowerer.resolve_ref(parameter.Type)
	array := clone_type(element)
	array.kind = type_array
	array.array_dimensions = []SemanticArrayDimension{{dynamic: true}}
	return array
}

func (lowerer *llvm_lowerer) function_declaration(function *ast.FunctionDecl) string {
	return_type := lowerer.llvm_type_from_ref(function.ReturnType)
	parameters := make([]string, 0, len(function.Parameters)+1)
	if function.Receiver != "" {
		parameters = append(parameters, "ptr")
	}
	variadic := false
	for _, parameter := range function.Parameters {
		if parameter.Variadic {
			if parameter.Type != nil {
				parameters = append(parameters, "ptr")
			} else {
				variadic = true
			}
			continue
		}
		parameters = append(parameters, lowerer.llvm_type_from_ref(parameter.Type))
	}
	if variadic {
		parameters = append(parameters, "...")
	}
	return fmt.Sprintf("declare %s @%s(%s)", return_type, llvm_function_name(function), strings.Join(parameters, ", "))
}

func (lowerer *llvm_lowerer) llvm_type_from_ref(ref *ast.TypeRef) string {
	return lowerer.llvm_type(lowerer.resolve_ref(ref))
}

func (lowerer *llvm_lowerer) llvm_type(type_info *SemanticType) string {
	if type_info == nil {
		return "ptr"
	}
	if type_info.pointer_depth > 0 || len(type_info.array_dimensions) > 0 || type_info.callable != nil {
		return "ptr"
	}
	switch type_info.kind {
	case type_void:
		return "void"
	case type_bool:
		return "i1"
	case type_string, type_struct, type_protocol, type_namespace, type_pointer, type_array, type_callable, type_null:
		return "ptr"
	case type_char:
		return "i8"
	case type_float:
		if type_info.name == "float32" {
			return "float"
		}
		return "double"
	case type_enum, type_error_type:
		return "i32"
	case type_integer:
		switch type_info.name {
		case "int8", "uint8":
			return "i8"
		case "int16", "uint16":
			return "i16"
		case "int32", "uint32":
			return "i32"
		default:
			return "i64"
		}
	}
	switch type_info.name {
	case "void":
		return "void"
	case "bool":
		return "i1"
	case "char":
		return "i8"
	case "int8", "uint8":
		return "i8"
	case "int16", "uint16":
		return "i16"
	case "int32", "uint32":
		return "i32"
	case "int64", "uint64", "uintptr":
		return "i64"
	case "float32":
		return "float"
	case "float64":
		return "double"
	}
	return "ptr"
}

func (lowerer *llvm_lowerer) next_register() string {
	lowerer.register++
	return fmt.Sprintf("%%v%d", lowerer.register)
}
func (lowerer *llvm_lowerer) next_label(prefix string) string {
	lowerer.label++
	return fmt.Sprintf("%s.%d", prefix, lowerer.label)
}
func (lowerer *llvm_lowerer) line(format string, args ...any) {
	lowerer.current_lines = append(lowerer.current_lines, "  "+fmt.Sprintf(format, args...))
}
func (lowerer *llvm_lowerer) raw_line(text string) {
	lowerer.current_lines = append(lowerer.current_lines, text)
	if strings.HasSuffix(text, ":") {
		lowerer.current_block = strings.TrimSuffix(text, ":")
	}
}

func (lowerer *llvm_lowerer) emit_function(function *ast.FunctionDecl, receiver *StructInfo) {
	lowerer.register, lowerer.label = 0, 0
	lowerer.current_lines = nil
	lowerer.locals = map[string]ir_local{}
	lowerer.break_labels, lowerer.continue_labels = nil, nil
	lowerer.break_cleanup_depths, lowerer.continue_cleanup_depths = nil, nil
	lowerer.receiver = receiver
	return_sem := lowerer.resolve_ref(function.ReturnType)
	lowerer.return_type = return_sem

	parameters := []string{}
	lowerer.main_args_name, lowerer.main_argc_value = "", ""
	main_argv := function.Name == "main" && function.Receiver == "" && len(function.Parameters) == 1 && function.Parameters[0].Type != nil && function.Parameters[0].Type.Name == "string" && len(function.Parameters[0].Type.Arrays) > 0
	if receiver != nil || function.Receiver != "" {
		parameters = append(parameters, "ptr %arg.this")
	}
	if main_argv {
		parameters = append(parameters, "i32 %clyth.argc", "ptr %clyth.argv")
		lowerer.main_args_name = function.Parameters[0].Name
		lowerer.main_argc_value = "%clyth.argc"
	} else {
		for index, parameter := range function.Parameters {
			if parameter.Variadic {
				if parameter.Type != nil {
					parameters = append(parameters, fmt.Sprintf("ptr %%arg.%d", index))
				}
				continue
			}
			parameters = append(parameters, fmt.Sprintf("%s %%arg.%d", lowerer.llvm_type_from_ref(parameter.Type), index))
		}
	}
	function_name := llvm_function_name(function)
	if receiver != nil {
		function_name = llvm_method_name(receiver.name, function.Name)
	}
	lowerer.raw_line(fmt.Sprintf("define %s @%s(%s) {", lowerer.llvm_type_from_ref(function.ReturnType), function_name, strings.Join(parameters, ", ")))
	lowerer.raw_line("entry:")
	lowerer.cleanup_slots = map[int]string{}
	lowerer.mecc_root_scopes = nil
	lowerer.value_cleanup_scopes = nil
	lowerer.pending_parameter_cleanups = nil
	lowerer.prepare_cleanup_slots(function.Body)
	if receiver != nil || function.Receiver != "" {
		lowerer.locals["this"] = ir_local{address: "%arg.this", type_info: &SemanticType{kind: type_struct, name: receiver_name(receiver, function)}}
	}
	for index, parameter := range function.Parameters {
		if parameter.Variadic && parameter.Type == nil {
			continue
		}
		sem := lowerer.resolve_ref(parameter.Type)
		if parameter.Variadic {
			sem = lowerer.variadic_parameter_type(parameter)
		}
		ty := lowerer.llvm_type(sem)
		address := lowerer.next_register()
		lowerer.line("%s = alloca %s", address, ty)
		if main_argv {
			// Native argv is char**. Clyth string[] stores pointers to Core string
			// descriptors, so adapt each process argument at the ABI boundary while
			// keeping the descriptors on main's stack for the process-entry lifetime.
			argc64 := lowerer.next_register()
			lowerer.line("%s = sext i32 %%clyth.argc to i64", argc64)
			data := lowerer.next_register()
			lowerer.line("%s = alloca ptr, i64 %s", data, argc64)
			descriptor_bytes := lowerer.next_register()
			lowerer.line("%s = mul i64 %s, 32", descriptor_bytes, argc64)
			descriptors := lowerer.next_register()
			lowerer.line("%s = alloca i8, i64 %s", descriptors, descriptor_bytes)
			index_slot := lowerer.next_register()
			lowerer.line("%s = alloca i64", index_slot)
			lowerer.line("store i64 0, ptr %s", index_slot)
			cond := lowerer.next_label("main.argv.cond")
			body := lowerer.next_label("main.argv.body")
			done := lowerer.next_label("main.argv.done")
			lowerer.line("br label %%%s", cond)
			lowerer.raw_line(cond + ":")
			index := lowerer.next_register()
			lowerer.line("%s = load i64, ptr %s", index, index_slot)
			more := lowerer.next_register()
			lowerer.line("%s = icmp ult i64 %s, %s", more, index, argc64)
			lowerer.line("br i1 %s, label %%%s, label %%%s", more, body, done)
			lowerer.raw_line(body + ":")
			raw_slot := lowerer.next_register()
			lowerer.line("%s = getelementptr ptr, ptr %%clyth.argv, i64 %s", raw_slot, index)
			raw := lowerer.next_register()
			lowerer.line("%s = load ptr, ptr %s", raw, raw_slot)
			descriptor_offset := lowerer.next_register()
			lowerer.line("%s = mul i64 %s, 32", descriptor_offset, index)
			descriptor := lowerer.next_register()
			lowerer.line("%s = getelementptr i8, ptr %s, i64 %s", descriptor, descriptors, descriptor_offset)
			heap_slot := lowerer.string_field_address(descriptor, "array_ptr")
			lowerer.line("store ptr %s, ptr %s", raw, heap_slot)
			raw_length := lowerer.next_register()
			lowerer.line("%s = call i64 @strlen(ptr %s)", raw_length, raw)
			string_length_slot := lowerer.string_field_address(descriptor, "length")
			lowerer.line("store i64 %s, ptr %s", raw_length, string_length_slot)
			data_element := lowerer.next_register()
			lowerer.line("%s = getelementptr ptr, ptr %s, i64 %s", data_element, data, index)
			lowerer.line("store ptr %s, ptr %s", descriptor, data_element)
			next := lowerer.next_register()
			lowerer.line("%s = add i64 %s, 1", next, index)
			lowerer.line("store i64 %s, ptr %s", next, index_slot)
			lowerer.line("br label %%%s", cond)
			lowerer.raw_line(done + ":")
			header := lowerer.emit_stack_array_header(data, 0)
			length_slot := lowerer.next_register()
			lowerer.line("%s = getelementptr i8, ptr %s, i64 8", length_slot, header)
			lowerer.line("store i64 %s, ptr %s", argc64, length_slot)
			lowerer.line("store ptr %s, ptr %s", header, address)
		} else if is_value_aggregate(sem) {
			// Aggregate arguments are caller-owned temporaries. Give the callee its
			// own value copy so reassignment/destruction of a parameter cannot free
			// the caller's temporary before the caller performs post-call cleanup.
			incoming := IRValue{llvm_type: ty, value: fmt.Sprintf("%%arg.%d", index), type_info: sem}
			owned := lowerer.clone_value_aggregate(incoming, sem)
			lowerer.line("store %s %s, ptr %s", ty, owned.value, address)
			lowerer.pending_parameter_cleanups = append(lowerer.pending_parameter_cleanups, value_cleanup{address: address, type_info: sem})
		} else {
			lowerer.line("store %s %%arg.%d, ptr %s", ty, index, address)
		}
		lowerer.locals[parameter.Name] = ir_local{address: address, type_info: sem}
	}
	terminated := lowerer.emit_block(function.Body)
	if !terminated {
		lowerer.emit_default_return(return_sem)
	}
	lowerer.raw_line("}")
	lowerer.function_defs = append(lowerer.function_defs, strings.Join(lowerer.current_lines, "\n"))
}

func receiver_name(receiver *StructInfo, function *ast.FunctionDecl) string {
	if receiver != nil {
		return receiver.name
	}
	return function.Receiver
}

func (lowerer *llvm_lowerer) emit_constructor(name string, constructor *ast.ConstructorDecl, info *StructInfo) {
	lowerer.register, lowerer.label = 0, 0
	lowerer.current_lines = nil
	lowerer.locals = map[string]ir_local{}
	lowerer.break_labels, lowerer.continue_labels = nil, nil
	lowerer.break_cleanup_depths, lowerer.continue_cleanup_depths = nil, nil
	lowerer.receiver = info
	lowerer.return_type = &SemanticType{kind: type_struct, name: name, pointer_depth: 1}
	parameters := []string{}
	for index, parameter := range constructor.Parameters {
		parameters = append(parameters, fmt.Sprintf("%s %%arg.%d", lowerer.llvm_type_from_ref(parameter.Type), index))
	}
	lowerer.raw_line(fmt.Sprintf("define ptr @%s(%s) {", llvm_constructor_name(name), strings.Join(parameters, ", ")))
	lowerer.raw_line("entry:")
	lowerer.cleanup_slots = map[int]string{}
	lowerer.mecc_root_scopes = nil
	lowerer.value_cleanup_scopes = nil
	lowerer.prepare_cleanup_slots(constructor.Body)
	object := lowerer.next_register()
	lowerer.line("%s = call ptr @calloc(i64 1, i64 %d)", object, lowerer.struct_storage_size(info))
	lowerer.initialize_struct_fields(object, info)
	lowerer.locals["this"] = ir_local{address: object, type_info: &SemanticType{kind: type_struct, name: name}}
	for index, parameter := range constructor.Parameters {
		sem := lowerer.resolve_ref(parameter.Type)
		ty := lowerer.llvm_type(sem)
		address := lowerer.next_register()
		lowerer.line("%s = alloca %s", address, ty)
		lowerer.line("store %s %%arg.%d, ptr %s", ty, index, address)
		lowerer.locals[parameter.Name] = ir_local{address: address, type_info: sem}
	}
	lowerer.emit_block(constructor.Body)
	lowerer.emit_mecc_clear_all_roots()
	lowerer.line("ret ptr %s", object)
	lowerer.raw_line("}")
	lowerer.function_defs = append(lowerer.function_defs, strings.Join(lowerer.current_lines, "\n"))
}

func (lowerer *llvm_lowerer) emit_destructor(name string, destructor *ast.DestructorDecl, info *StructInfo) {
	lowerer.register, lowerer.label = 0, 0
	lowerer.current_lines = nil
	lowerer.locals = map[string]ir_local{}
	lowerer.cleanup_slots = map[int]string{}
	lowerer.mecc_root_scopes = nil
	lowerer.value_cleanup_scopes = nil
	lowerer.receiver = info
	lowerer.return_type = &SemanticType{kind: type_void, name: "void"}
	lowerer.raw_line(fmt.Sprintf("define void @%s(ptr %%arg.this) {", llvm_destructor_name(name)))
	lowerer.raw_line("entry:")
	lowerer.prepare_cleanup_slots(destructor.Body)
	lowerer.locals["this"] = ir_local{address: "%arg.this", type_info: &SemanticType{kind: type_struct, name: name}}
	terminated := lowerer.emit_block(destructor.Body)
	if !terminated {
		lowerer.emit_mecc_function_frontier()
		lowerer.line("ret void")
	}
	lowerer.raw_line("}")
	lowerer.function_defs = append(lowerer.function_defs, strings.Join(lowerer.current_lines, "\n"))
}

func (lowerer *llvm_lowerer) emit_default_constructor(name string) {
	fn := llvm_constructor_name(name)
	if lowerer.defined[fn] {
		return
	}
	lowerer.defined[fn] = true
	info := lowerer.struct_info[name]
	lowerer.register, lowerer.label = 0, 0
	lowerer.current_lines = nil
	lowerer.raw_line(fmt.Sprintf("define ptr @%s() {", fn))
	lowerer.raw_line("entry:")
	object := lowerer.next_register()
	lowerer.line("%s = call ptr @calloc(i64 1, i64 %d)", object, lowerer.struct_storage_size(info))
	lowerer.initialize_struct_fields(object, info)
	lowerer.line("ret ptr %s", object)
	lowerer.raw_line("}")
	lowerer.function_defs = append(lowerer.function_defs, strings.Join(lowerer.current_lines, "\n"))
}

func (lowerer *llvm_lowerer) lookup_type_symbol(namespace *Namespace, name string, seen map[*Namespace]bool) *Symbol {
	if namespace == nil || seen[namespace] {
		return nil
	}
	seen[namespace] = true
	if symbol := namespace.symbols[name]; symbol != nil {
		return symbol
	}
	for _, imported := range namespace.open_imports {
		if symbol := lowerer.lookup_type_symbol(imported, name, seen); symbol != nil {
			return symbol
		}
	}
	return nil
}

func enum_member_value(symbol *Symbol, member_name string) (int64, bool) {
	if symbol == nil || symbol.type_info == nil || symbol.type_info.kind != type_enum {
		return 0, false
	}
	declaration, ok := symbol.declaration.(*ast.EnumDecl)
	if !ok || declaration == nil {
		return 0, false
	}
	next := int64(0)
	for _, member := range declaration.Members {
		if literal, ok := member.Value.(*ast.LiteralExpr); ok && literal.Kind == ast.LiteralInteger {
			if parsed, err := strconv.ParseInt(strings.TrimSpace(literal.Raw), 0, 64); err == nil {
				next = parsed
			}
		}
		if member.Name == member_name {
			return next, true
		}
		next++
	}
	return 0, false
}

func (lowerer *llvm_lowerer) resolve_ref(ref *ast.TypeRef) *SemanticType {
	if ref == nil {
		return &SemanticType{kind: type_unknown}
	}
	if replacement := lowerer.substitutions[ref.Name]; replacement != nil {
		result := clone_type(replacement)
		result.pointer_depth += ref.PointerDepth
		result.is_const = result.is_const || ref.Const
		for _, dimension := range ref.Arrays {
			result.array_dimensions = append(result.array_dimensions, SemanticArrayDimension{dynamic: dimension.Dynamic, size: array_dimension_size(dimension)})
		}
		if len(result.array_dimensions) > 0 {
			result.kind = type_array
		} else if result.pointer_depth > 0 {
			result.kind = type_pointer
		}
		return result
	}
	result := &SemanticType{name: ref.Name, pointer_depth: ref.PointerDepth, is_const: ref.Const}
	for _, argument := range ref.GenericArgs {
		result.generic_args = append(result.generic_args, lowerer.resolve_ref(argument))
	}
	switch ref.Name {
	case "void":
		result.kind = type_void
	case "bool":
		result.kind = type_bool
	case "char":
		result.kind = type_char
	case "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64", "uintptr":
		result.kind = type_integer
	case "float32", "float64":
		result.kind = type_float
	case "string":
		result.kind = type_string
	default:
		if _, ok := lowerer.enum_values[ref.Name]; ok {
			result.kind = type_enum
		} else if symbol := lowerer.lookup_type_symbol(lowerer.result.root_namespace, ref.Name, map[*Namespace]bool{}); symbol != nil && symbol.type_info != nil && symbol.type_info.kind == type_enum {
			// A function defined in this unit may use an enum declared by an open
			// import. Preserve the imported enum's integer ABI instead of treating
			// the unresolved local declaration as an opaque aggregate pointer.
			result.kind = type_enum
		} else {
			result.kind = type_struct
		}
	}
	for _, dimension := range ref.Arrays {
		result.array_dimensions = append(result.array_dimensions, SemanticArrayDimension{dynamic: dimension.Dynamic, size: array_dimension_size(dimension)})
	}
	if ref.PointerDepth > 0 {
		result.kind = type_pointer
	}
	if len(ref.Arrays) > 0 {
		result.kind = type_array
	}
	if ref.Callable != nil {
		result.kind = type_callable
		callable := &SemanticCallable{return_type: lowerer.resolve_ref(ref.Callable.Return)}
		for _, parameter := range ref.Callable.Parameters {
			callable.parameters = append(callable.parameters, lowerer.resolve_ref(parameter))
		}
		result.callable = callable
	}
	return result
}

func array_dimension_size(dimension ast.ArrayDimension) string {
	if dimension.Dynamic || dimension.Size == nil {
		return ""
	}
	if literal, ok := dimension.Size.(*ast.LiteralExpr); ok {
		return literal.Raw
	}
	return ""
}

func (lowerer *llvm_lowerer) specialize_type(type_info *SemanticType) *SemanticType {
	if type_info == nil {
		return nil
	}
	result := clone_type(type_info)
	if replacement := lowerer.substitutions[result.name]; replacement != nil {
		base := clone_type(replacement)
		base.pointer_depth += result.pointer_depth
		base.array_dimensions = append(base.array_dimensions, result.array_dimensions...)
		base.is_const = base.is_const || result.is_const
		if len(base.array_dimensions) > 0 {
			base.kind = type_array
		} else if base.pointer_depth > 0 {
			base.kind = type_pointer
		}
		return base
	}
	for index, argument := range result.generic_args {
		result.generic_args[index] = lowerer.specialize_type(argument)
	}
	if result.callable != nil {
		result.callable.return_type = lowerer.specialize_type(result.callable.return_type)
		for index, parameter := range result.callable.parameters {
			result.callable.parameters[index] = lowerer.specialize_type(parameter)
		}
	}
	return result
}

func specialized_struct_info(template *StructInfo, arguments []*SemanticType) *StructInfo {
	if template == nil {
		return nil
	}
	substitutions := build_substitutions(template.generic_parameters, arguments)
	info := &StructInfo{name: specialization_name(template.name, arguments), declaration: template.declaration, fields: map[string]*Symbol{}, field_order: append([]string(nil), template.field_order...), methods: map[string]*Symbol{}, constructors: template.constructors, destructor: template.destructor}
	for name, field := range template.fields {
		copy_symbol := *field
		copy_symbol.type_info = substitute_type(field.type_info, substitutions)
		info.fields[name] = &copy_symbol
	}
	for name, method := range template.methods {
		copy_symbol := *method
		copy_symbol.type_info = substitute_type(method.type_info, substitutions)
		info.methods[name] = &copy_symbol
	}
	return info
}

func (lowerer *llvm_lowerer) default_array_value(type_info *SemanticType) IRValue {
	count := 0
	if type_info != nil && len(type_info.array_dimensions) > 0 && !type_info.array_dimensions[0].dynamic {
		count, _ = strconv.Atoi(type_info.array_dimensions[0].size)
	}
	element := lowerer.array_element_type(type_info)
	element_size := lowerer.semantic_storage_size(element)
	header := lowerer.next_register()
	lowerer.line("%s = call ptr @calloc(i64 1, i64 16)", header)
	if count > 0 {
		data := lowerer.next_register()
		lowerer.line("%s = call ptr @calloc(i64 %d, i64 %d)", data, count, element_size)
		data_slot := lowerer.next_register()
		lowerer.line("%s = getelementptr i8, ptr %s, i64 0", data_slot, header)
		lowerer.line("store ptr %s, ptr %s", data, data_slot)
	}
	length_slot := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 8", length_slot, header)
	lowerer.line("store i64 %d, ptr %s", count, length_slot)
	return IRValue{llvm_type: "ptr", value: header, type_info: type_info, fresh: true}
}

func constructor_accepts_no_explicit_arguments(constructor *SemanticCallable) bool {
	if constructor == nil {
		return true
	}
	fixed := len(constructor.parameters)
	if constructor.variadic && !constructor.native_variadic && fixed > 0 {
		fixed--
	}
	for index := 0; index < fixed; index++ {
		if index >= len(constructor.default_values) || constructor.default_values[index] == nil {
			return false
		}
	}
	return true
}

func (lowerer *llvm_lowerer) default_struct_value(type_info *SemanticType) IRValue {
	name := base_type_name(type_info)
	info := lowerer.struct_info[name]
	if info == nil {
		return lowerer.zero_value(type_info)
	}
	can_call_default := len(info.constructors) == 0
	for _, constructor := range info.constructors {
		if constructor != nil && len(constructor.parameters) == 0 && !constructor.variadic {
			can_call_default = true
			break
		}
	}
	if can_call_default {
		fn := llvm_constructor_name(name)
		if !lowerer.defined[fn] && lowerer.declarations[fn] == "" {
			lowerer.declarations[fn] = fmt.Sprintf("declare ptr @%s()", fn)
		}
		value := lowerer.next_register()
		lowerer.line("%s = call ptr @%s()", value, fn)
		return IRValue{llvm_type: "ptr", value: value, type_info: type_info, fresh: true}
	}
	value := lowerer.next_register()
	lowerer.line("%s = call ptr @calloc(i64 1, i64 %d)", value, lowerer.struct_storage_size(info))
	lowerer.initialize_struct_fields(value, info)
	return IRValue{llvm_type: "ptr", value: value, type_info: type_info, fresh: true}
}

func (lowerer *llvm_lowerer) emit_block(block *ast.BlockStmt) bool {
	if block == nil {
		return false
	}
	// Lexical declarations may shadow outer names, but leaving the block must
	// restore the outer binding. The addresses themselves remain unchanged, so
	// assignments through an outer binding still have ordinary C-like effects.
	saved_locals := make(map[string]ir_local, len(lowerer.locals))
	for name, local := range lowerer.locals {
		saved_locals[name] = local
	}
	defer func() { lowerer.locals = saved_locals }()
	lowerer.block_stack = append(lowerer.block_stack, block)
	lowerer.mecc_root_scopes = append(lowerer.mecc_root_scopes, nil)
	parameter_cleanups := []value_cleanup(nil)
	if len(lowerer.value_cleanup_scopes) == 0 && len(lowerer.pending_parameter_cleanups) > 0 {
		parameter_cleanups = append(parameter_cleanups, lowerer.pending_parameter_cleanups...)
		lowerer.pending_parameter_cleanups = nil
	}
	lowerer.value_cleanup_scopes = append(lowerer.value_cleanup_scopes, parameter_cleanups)
	defer func() {
		lowerer.block_stack = lowerer.block_stack[:len(lowerer.block_stack)-1]
		lowerer.mecc_root_scopes = lowerer.mecc_root_scopes[:len(lowerer.mecc_root_scopes)-1]
		lowerer.value_cleanup_scopes = lowerer.value_cleanup_scopes[:len(lowerer.value_cleanup_scopes)-1]
	}()
	for _, statement := range block.Statements {
		if lowerer.emit_stmt(statement) {
			return true
		}
	}
	lowerer.emit_value_cleanup_scope(len(lowerer.value_cleanup_scopes) - 1)
	lowerer.emit_lifetime_cleanups(block)
	lowerer.emit_mecc_clear_root_scope(len(lowerer.mecc_root_scopes) - 1)
	return false
}

func (lowerer *llvm_lowerer) emit_stmt(statement ast.Stmt) bool {
	switch item := statement.(type) {
	case *ast.VarDeclStmt:
		sem := lowerer.resolve_ref(item.Type)
		if item.Type != nil && item.Type.Name == "auto" && item.Initializer != nil {
			sem = clone_type(lowerer.result.expression_types[item.Initializer])
		}
		ty := lowerer.llvm_type(sem)
		address := lowerer.next_register()
		lowerer.line("%s = alloca %s", address, ty)
		lowerer.locals[item.Name] = ir_local{address: address, type_info: sem}
		value := lowerer.zero_value(sem)
		if item.Initializer != nil {
			if array_expression, ok := item.Initializer.(*ast.ArrayExpr); ok && sem != nil && (sem.kind == type_array || len(sem.array_dimensions) > 0) {
				value = lowerer.emit_array_expr(array_expression, sem, lowerer.array_element_type(sem))
			} else {
				value = lowerer.emit_expr(item.Initializer)
			}
			value = lowerer.coerce(value, sem)
		} else if sem != nil && sem.kind == type_struct && sem.pointer_depth == 0 {
			value = lowerer.default_struct_value(sem)
		} else if sem != nil && (sem.kind == type_array || len(sem.array_dimensions) > 0) {
			value = lowerer.default_array_value(sem)
		}
		lowerer.line("store %s %s, ptr %s", ty, value.value, address)
		if is_value_aggregate(sem) && len(lowerer.value_cleanup_scopes) > 0 {
			index := len(lowerer.value_cleanup_scopes) - 1
			lowerer.value_cleanup_scopes[index] = append(lowerer.value_cleanup_scopes[index], value_cleanup{address: address, type_info: sem})
		}
		if lowerer.result.mecc_enabled && semantic_is_pointer(sem) {
			lowerer.line("call void @clyth_mecc_store(ptr null, ptr %s, ptr %s)", address, value.value)
			lowerer.register_mecc_root(address)
		}
	case *ast.ExprStmt:
		lowerer.emit_expr(item.Expression)
		if call, ok := item.Expression.(*ast.CallExpr); ok {
			if identifier, ok := call.Callee.(*ast.IdentifierExpr); ok && identifier.Name == "raise" && len(lowerer.catch_stack) > 0 {
				return true
			}
		}
	case *ast.ReturnStmt:
		if len(lowerer.catch_stack) > 0 {
			if item.Value != nil {
				lowerer.emit_expr(item.Value)
			}
			context := lowerer.catch_stack[len(lowerer.catch_stack)-1]
			lowerer.emit_value_cleanups_from(context.block_depth)
			lowerer.emit_lifetime_cleanups(item)
			lowerer.line("br label %%%s", lowerer.catch_stack[len(lowerer.catch_stack)-1].end_label)
			return true
		}
		value := lowerer.zero_value(lowerer.return_type)
		if item.Value != nil {
			value = lowerer.coerce(lowerer.emit_expr(item.Value), lowerer.return_type)
		}
		lowerer.emit_value_cleanups_from(0)
		lowerer.emit_lifetime_cleanups(item)
		lowerer.emit_mecc_function_frontier()
		if lowerer.llvm_type(lowerer.return_type) == "void" {
			lowerer.line("ret void")
		} else {
			lowerer.line("ret %s %s", lowerer.llvm_type(lowerer.return_type), value.value)
		}
		return true
	case *ast.BlockStmt:
		return lowerer.emit_block(item)
	case *ast.IfStmt:
		condition := lowerer.to_bool(lowerer.emit_expr(item.Condition))
		then_label := lowerer.next_label("if.then")
		else_label := lowerer.next_label("if.else")
		end_label := lowerer.next_label("if.end")
		lowerer.line("br i1 %s, label %%%s, label %%%s", condition.value, then_label, else_label)
		lowerer.raw_line(then_label + ":")
		then_term := lowerer.emit_stmt(item.Then)
		if !then_term {
			lowerer.line("br label %%%s", end_label)
		}
		lowerer.raw_line(else_label + ":")
		else_term := false
		if item.Else != nil {
			else_term = lowerer.emit_stmt(item.Else)
		}
		if !else_term {
			lowerer.line("br label %%%s", end_label)
		}
		if then_term && else_term {
			return true
		}
		lowerer.raw_line(end_label + ":")
	case *ast.WhileStmt:
		cond_label := lowerer.next_label("while.cond")
		body_label := lowerer.next_label("while.body")
		end_label := lowerer.next_label("while.end")
		lowerer.line("br label %%%s", cond_label)
		lowerer.raw_line(cond_label + ":")
		condition := lowerer.to_bool(lowerer.emit_expr(item.Condition))
		lowerer.line("br i1 %s, label %%%s, label %%%s", condition.value, body_label, end_label)
		lowerer.raw_line(body_label + ":")
		lowerer.break_labels = append(lowerer.break_labels, end_label)
		lowerer.continue_labels = append(lowerer.continue_labels, cond_label)
		lowerer.break_cleanup_depths = append(lowerer.break_cleanup_depths, len(lowerer.block_stack))
		lowerer.continue_cleanup_depths = append(lowerer.continue_cleanup_depths, len(lowerer.block_stack))
		term := lowerer.emit_stmt(item.Body)
		lowerer.break_labels = lowerer.break_labels[:len(lowerer.break_labels)-1]
		lowerer.continue_labels = lowerer.continue_labels[:len(lowerer.continue_labels)-1]
		lowerer.break_cleanup_depths = lowerer.break_cleanup_depths[:len(lowerer.break_cleanup_depths)-1]
		lowerer.continue_cleanup_depths = lowerer.continue_cleanup_depths[:len(lowerer.continue_cleanup_depths)-1]
		if !term {
			lowerer.line("br label %%%s", cond_label)
		}
		lowerer.raw_line(end_label + ":")
	case *ast.ForStmt:
		if item.Init != nil {
			lowerer.emit_stmt(item.Init)
		}
		cond_label := lowerer.next_label("for.cond")
		body_label := lowerer.next_label("for.body")
		post_label := lowerer.next_label("for.post")
		end_label := lowerer.next_label("for.end")
		lowerer.line("br label %%%s", cond_label)
		lowerer.raw_line(cond_label + ":")
		if item.Condition != nil {
			c := lowerer.to_bool(lowerer.emit_expr(item.Condition))
			lowerer.line("br i1 %s, label %%%s, label %%%s", c.value, body_label, end_label)
		} else {
			lowerer.line("br label %%%s", body_label)
		}
		lowerer.raw_line(body_label + ":")
		lowerer.break_labels = append(lowerer.break_labels, end_label)
		lowerer.continue_labels = append(lowerer.continue_labels, post_label)
		lowerer.break_cleanup_depths = append(lowerer.break_cleanup_depths, len(lowerer.block_stack))
		lowerer.continue_cleanup_depths = append(lowerer.continue_cleanup_depths, len(lowerer.block_stack))
		term := lowerer.emit_stmt(item.Body)
		lowerer.break_labels = lowerer.break_labels[:len(lowerer.break_labels)-1]
		lowerer.continue_labels = lowerer.continue_labels[:len(lowerer.continue_labels)-1]
		lowerer.break_cleanup_depths = lowerer.break_cleanup_depths[:len(lowerer.break_cleanup_depths)-1]
		lowerer.continue_cleanup_depths = lowerer.continue_cleanup_depths[:len(lowerer.continue_cleanup_depths)-1]
		if !term {
			lowerer.line("br label %%%s", post_label)
		}
		lowerer.raw_line(post_label + ":")
		if item.Post != nil {
			lowerer.emit_expr(item.Post)
		}
		lowerer.line("br label %%%s", cond_label)
		lowerer.raw_line(end_label + ":")
	case *ast.SwitchStmt:
		switch_value := lowerer.emit_expr(item.Value)
		end_label := lowerer.next_label("switch.end")
		case_labels := make([]string, len(item.Cases))
		default_label := end_label
		for i, c := range item.Cases {
			case_labels[i] = lowerer.next_label("switch.case")
			if c.Default {
				default_label = case_labels[i]
			}
		}
		// A chain of comparisons keeps switch lowering valid for enum/pointer-like cases too.
		current_test := lowerer.next_label("switch.test")
		lowerer.line("br label %%%s", current_test)
		nondefault := []int{}
		for i, c := range item.Cases {
			if !c.Default {
				nondefault = append(nondefault, i)
			}
		}
		for pos, index := range nondefault {
			lowerer.raw_line(current_test + ":")
			case_value := lowerer.coerce(lowerer.emit_expr(item.Cases[index].Value), switch_value.type_info)
			cmp := lowerer.next_register()
			predicate := "eq"
			ty := switch_value.llvm_type
			lowerer.line("%s = icmp %s %s %s, %s", cmp, predicate, ty, switch_value.value, case_value.value)
			next := default_label
			if pos+1 < len(nondefault) {
				next = lowerer.next_label("switch.test")
			}
			lowerer.line("br i1 %s, label %%%s, label %%%s", cmp, case_labels[index], next)
			current_test = next
		}
		if len(nondefault) == 0 {
			lowerer.raw_line(current_test + ":")
			lowerer.line("br label %%%s", default_label)
		}
		lowerer.break_labels = append(lowerer.break_labels, end_label)
		lowerer.break_cleanup_depths = append(lowerer.break_cleanup_depths, len(lowerer.block_stack))
		for i, c := range item.Cases {
			lowerer.raw_line(case_labels[i] + ":")
			terminated := false
			for _, statement := range c.Statements {
				if lowerer.emit_stmt(statement) {
					terminated = true
					break
				}
			}
			if !terminated {
				next := end_label
				if i+1 < len(item.Cases) {
					next = case_labels[i+1]
				}
				lowerer.line("br label %%%s", next)
			}
		}
		lowerer.break_labels = lowerer.break_labels[:len(lowerer.break_labels)-1]
		lowerer.break_cleanup_depths = lowerer.break_cleanup_depths[:len(lowerer.break_cleanup_depths)-1]
		lowerer.raw_line(end_label + ":")
	case *ast.BreakStmt:
		if len(lowerer.break_labels) > 0 {
			if len(lowerer.break_cleanup_depths) > 0 {
				lowerer.emit_control_scope_cleanups(lowerer.break_cleanup_depths[len(lowerer.break_cleanup_depths)-1])
			}
			lowerer.line("br label %%%s", lowerer.break_labels[len(lowerer.break_labels)-1])
			return true
		}
	case *ast.ContinueStmt:
		if len(lowerer.continue_labels) > 0 {
			if len(lowerer.continue_cleanup_depths) > 0 {
				lowerer.emit_control_scope_cleanups(lowerer.continue_cleanup_depths[len(lowerer.continue_cleanup_depths)-1])
			}
			lowerer.line("br label %%%s", lowerer.continue_labels[len(lowerer.continue_labels)-1])
			return true
		}
	}
	return false
}

func (lowerer *llvm_lowerer) indexable_method(object *SemanticType, name string) *Symbol {
	if object == nil {
		return nil
	}
	if info := lowerer.struct_info[base_type_name(object)]; info != nil {
		return info.methods[name]
	}
	return nil
}

func (lowerer *llvm_lowerer) declare_indexable_method(receiver string, name string, callable *SemanticCallable) string {
	functionName := llvm_method_name(receiver, name)
	if callable == nil || lowerer.defined[functionName] || lowerer.declarations[functionName] != "" {
		return functionName
	}
	params := []string{"ptr"}
	for _, parameter := range callable.parameters {
		params = append(params, lowerer.llvm_type(parameter))
	}
	lowerer.declarations[functionName] = fmt.Sprintf("declare %s @%s(%s)", lowerer.llvm_type(callable.return_type), functionName, strings.Join(params, ", "))
	return functionName
}

func (lowerer *llvm_lowerer) emit_indexable_read(item *ast.IndexExpr, objectSem *SemanticType) (IRValue, bool) {
	if objectSem != nil && len(objectSem.array_dimensions) == 0 && objectSem.pointer_depth == 0 && objectSem.kind != type_pointer && objectSem.name == "Map" && len(objectSem.generic_args) == 2 {
		object := lowerer.emit_expr(item.Object)
		key := lowerer.coerce(lowerer.emit_expr(item.Index), objectSem.generic_args[0])
		ret := objectSem.generic_args[1]
		functionName := llvm_method_name(base_type_name(objectSem), "get")
		if !lowerer.defined[functionName] && lowerer.declarations[functionName] == "" {
			lowerer.declarations[functionName] = fmt.Sprintf("declare %s @%s(ptr, %s)", lowerer.llvm_type(ret), functionName, lowerer.llvm_type(objectSem.generic_args[0]))
		}
		reg := lowerer.next_register()
		lowerer.line("%s = call %s @%s(ptr %s, %s %s)", reg, lowerer.llvm_type(ret), functionName, object.value, key.llvm_type, key.value)
		return IRValue{llvm_type: lowerer.llvm_type(ret), value: reg, type_info: ret}, true
	}
	method := lowerer.indexable_method(objectSem, "on_read_index")
	if method == nil || method.type_info == nil || method.type_info.callable == nil || len(method.type_info.callable.parameters) != 1 {
		return IRValue{}, false
	}
	object := lowerer.emit_expr(item.Object)
	index := lowerer.coerce(lowerer.emit_expr(item.Index), method.type_info.callable.parameters[0])
	ret := method.type_info.callable.return_type
	retTy := lowerer.llvm_type(ret)
	if retTy == "void" {
		return IRValue{}, false
	}
	reg := lowerer.next_register()
	functionName := lowerer.declare_indexable_method(base_type_name(objectSem), "on_read_index", method.type_info.callable)
	lowerer.line("%s = call %s @%s(ptr %s, %s %s)", reg, retTy, functionName, object.value, index.llvm_type, index.value)
	return IRValue{llvm_type: retTy, value: reg, type_info: ret}, true
}

func (lowerer *llvm_lowerer) emit_indexable_write(item *ast.AssignExpr, indexed *ast.IndexExpr, objectSem *SemanticType, value IRValue) (IRValue, bool) {
	if objectSem != nil && len(objectSem.array_dimensions) == 0 && objectSem.pointer_depth == 0 && objectSem.kind != type_pointer && objectSem.name == "Map" && len(objectSem.generic_args) == 2 {
		object := lowerer.emit_expr(indexed.Object)
		key := lowerer.coerce(lowerer.emit_expr(indexed.Index), objectSem.generic_args[0])
		value = lowerer.coerce(value, objectSem.generic_args[1])
		if item.Operator != "=" {
			read, ok := lowerer.emit_indexable_read(indexed, objectSem)
			if !ok {
				return IRValue{}, false
			}
			op := map[string]string{"+=": "add", "-=": "sub", "*=": "mul", "/=": "sdiv", "%=": "srem"}[item.Operator]
			if read.llvm_type == "float" || read.llvm_type == "double" {
				op = map[string]string{"+=": "fadd", "-=": "fsub", "*=": "fmul", "/=": "fdiv", "%=": "frem"}[item.Operator]
			} else if is_unsigned_type(read.type_info) {
				if item.Operator == "/=" {
					op = "udiv"
				}
				if item.Operator == "%=" {
					op = "urem"
				}
			}
			if item.Operator == "+=" && read.type_info != nil && read.type_info.name == "string" {
				value = lowerer.emit_string_concat(read, value)
			} else {
				combined := lowerer.next_register()
				lowerer.line("%s = %s %s %s, %s", combined, op, read.llvm_type, read.value, value.value)
				value = IRValue{llvm_type: read.llvm_type, value: combined, type_info: read.type_info}
			}
		}
		functionName := llvm_method_name(base_type_name(objectSem), "put")
		if !lowerer.defined[functionName] && lowerer.declarations[functionName] == "" {
			lowerer.declarations[functionName] = fmt.Sprintf("declare void @%s(ptr, %s, %s)", functionName, lowerer.llvm_type(objectSem.generic_args[0]), lowerer.llvm_type(objectSem.generic_args[1]))
		}
		lowerer.line("call void @%s(ptr %s, %s %s, %s %s)", functionName, object.value, key.llvm_type, key.value, value.llvm_type, value.value)
		return value, true
	}
	write := lowerer.indexable_method(objectSem, "on_write_index")
	if write == nil || write.type_info == nil || write.type_info.callable == nil || len(write.type_info.callable.parameters) != 2 {
		return IRValue{}, false
	}
	object := lowerer.emit_expr(indexed.Object)
	index := lowerer.coerce(lowerer.emit_expr(indexed.Index), write.type_info.callable.parameters[0])
	value = lowerer.coerce(value, write.type_info.callable.parameters[1])
	if item.Operator != "=" {
		read, ok := lowerer.emit_indexable_read(indexed, objectSem)
		if !ok {
			return IRValue{}, false
		}
		op := map[string]string{"+=": "add", "-=": "sub", "*=": "mul", "/=": "sdiv", "%=": "srem"}[item.Operator]
		if read.llvm_type == "float" || read.llvm_type == "double" {
			op = map[string]string{"+=": "fadd", "-=": "fsub", "*=": "fmul", "/=": "fdiv", "%=": "frem"}[item.Operator]
		} else if is_unsigned_type(read.type_info) {
			if item.Operator == "/=" {
				op = "udiv"
			}
			if item.Operator == "%=" {
				op = "urem"
			}
		}
		if item.Operator == "+=" && read.type_info != nil && read.type_info.name == "string" {
			value = lowerer.emit_string_concat(read, value)
		} else {
			combined := lowerer.next_register()
			lowerer.line("%s = %s %s %s, %s", combined, op, read.llvm_type, read.value, value.value)
			value = IRValue{llvm_type: read.llvm_type, value: combined, type_info: read.type_info}
		}
	}
	functionName := lowerer.declare_indexable_method(base_type_name(objectSem), "on_write_index", write.type_info.callable)
	lowerer.line("call void @%s(ptr %s, %s %s, %s %s)", functionName, object.value, index.llvm_type, index.value, value.llvm_type, value.value)
	return value, true
}

func (lowerer *llvm_lowerer) emit_expr(expression ast.Expr) IRValue {
	sem := lowerer.specialize_type(lowerer.result.expression_types[expression])
	if sem == nil || sem.kind == type_unknown {
		sem = lowerer.specialize_type(lowerer.infer_expression_type(expression))
	}
	if sem == nil {
		sem = &SemanticType{kind: type_unknown}
	}
	switch item := expression.(type) {
	case *ast.LiteralExpr:
		natural := lowerer.literal_type(item)
		switch item.Kind {
		case ast.LiteralInteger:
			raw := strings.ReplaceAll(item.Raw, "_", "")
			if raw == "" {
				raw = "0"
			}
			return IRValue{llvm_type: lowerer.llvm_type(natural), value: raw, type_info: natural}
		case ast.LiteralFloat:
			return IRValue{llvm_type: lowerer.llvm_type(natural), value: item.Raw, type_info: natural}
		case ast.LiteralBool:
			if item.Raw == "true" {
				return IRValue{llvm_type: "i1", value: "1", type_info: natural}
			}
			return IRValue{llvm_type: "i1", value: "0", type_info: natural}
		case ast.LiteralNull:
			return IRValue{llvm_type: "ptr", value: "null", type_info: natural}
		case ast.LiteralCharacter:
			value := int64(0)
			unquoted, err := strconv.Unquote(item.Raw)
			if err == nil && len(unquoted) > 0 {
				value = int64([]rune(unquoted)[0])
			}
			return IRValue{llvm_type: "i8", value: strconv.FormatInt(value, 10), type_info: natural}
		case ast.LiteralString:
			text := item.Raw
			if unquoted, err := strconv.Unquote(item.Raw); err == nil {
				text = unquoted
			}
			return lowerer.static_string_value(text, natural)
		case ast.LiteralInterpolatedString:
			return lowerer.emit_interpolated_string(item, natural)
		}
	case *ast.IdentifierExpr:
		if local, ok := lowerer.locals[item.Name]; ok {
			// Receiver "this" is already a value pointer rather than an alloca slot.
			if item.Name == "this" {
				return IRValue{llvm_type: "ptr", value: local.address, type_info: local.type_info}
			}
			ty := lowerer.llvm_type(local.type_info)
			reg := lowerer.next_register()
			lowerer.line("%s = load %s, ptr %s", reg, ty, local.address)
			return IRValue{llvm_type: ty, value: reg, type_info: local.type_info}
		}
		if global, ok := lowerer.globals[item.Name]; ok {
			ty := lowerer.llvm_type(global.type_info)
			reg := lowerer.next_register()
			lowerer.line("%s = load %s, ptr %s", reg, ty, global.address)
			return IRValue{llvm_type: ty, value: reg, type_info: global.type_info}
		}
		if lowerer.receiver != nil {
			if field := lowerer.receiver.fields[item.Name]; field != nil {
				return lowerer.load_receiver_field(item.Name, field.type_info)
			}
		}
		if symbol := lowerer.lookup_error_type_symbol(lowerer.result.root_namespace, item.Name, map[*Namespace]bool{}); symbol != nil {
			return IRValue{llvm_type: "i32", value: strconv.Itoa(symbol.error_type_id), type_info: &SemanticType{kind: type_error_type, name: "ErrorType"}}
		}
		if id, ok := lowerer.result.error_type_ids["builtin::"+item.Name]; ok {
			return IRValue{llvm_type: "i32", value: strconv.Itoa(id), type_info: &SemanticType{kind: type_error_type, name: "ErrorType"}}
		}
		if symbol := lowerer.result.root_namespace.symbols[item.Name]; symbol != nil && symbol.kind == symbol_function {
			return IRValue{llvm_type: "ptr", value: "@" + sanitize_llvm_name(item.Name), type_info: symbol.type_info}
		}
		// Enum members may be referenced through namespace member expressions; a bare unknown identifier is conservatively null.
		return lowerer.zero_value(sem)
	case *ast.UnaryExpr:
		if item.Operator == "++" || item.Operator == "--" {
			address, target_sem, _ := lowerer.assignment_address_with_owner(item.Operand)
			if address == "" {
				return lowerer.zero_value(sem)
			}
			ty := lowerer.llvm_type(target_sem)
			old := lowerer.next_register()
			lowerer.line("%s = load %s, ptr %s", old, ty, address)
			updated := lowerer.next_register()
			if ty == "float" || ty == "double" {
				op := "fadd"
				if item.Operator == "--" {
					op = "fsub"
				}
				lowerer.line("%s = %s %s %s, 1.0", updated, op, ty, old)
			} else {
				op := "add"
				if item.Operator == "--" {
					op = "sub"
				}
				lowerer.line("%s = %s %s %s, 1", updated, op, ty, old)
			}
			lowerer.line("store %s %s, ptr %s", ty, updated, address)
			result := updated
			if !item.Prefix {
				result = old
			}
			return IRValue{llvm_type: ty, value: result, type_info: target_sem}
		}
		operand := lowerer.emit_expr(item.Operand)
		switch item.Operator {
		case "!":
			b := lowerer.to_bool(operand)
			reg := lowerer.next_register()
			lowerer.line("%s = xor i1 %s, true", reg, b.value)
			return IRValue{llvm_type: "i1", value: reg, type_info: sem}
		case "-":
			reg := lowerer.next_register()
			if operand.llvm_type == "float" || operand.llvm_type == "double" {
				lowerer.line("%s = fsub %s 0.0, %s", reg, operand.llvm_type, operand.value)
			} else {
				lowerer.line("%s = sub %s 0, %s", reg, operand.llvm_type, operand.value)
			}
			return IRValue{llvm_type: operand.llvm_type, value: reg, type_info: sem}
		case "&":
			return IRValue{llvm_type: "ptr", value: lowerer.address_of(item.Operand), type_info: sem}
		case "*":
			if operand.llvm_type == "ptr" {
				non_null := lowerer.next_register()
				lowerer.line("%s = icmp ne ptr %s, null", non_null, operand.value)
				lowerer.emit_checked_failure(non_null, "null_reference", "null pointer dereference", "nullref")
				ty := lowerer.llvm_type(sem)
				reg := lowerer.next_register()
				lowerer.line("%s = load %s, ptr %s", reg, ty, operand.value)
				return IRValue{llvm_type: ty, value: reg, type_info: sem}
			}
		}
		return operand
	case *ast.BinaryExpr:
		left := lowerer.emit_expr(item.Left)
		if item.Operator == "&&" || item.Operator == "||" {
			// Clyth guarantees true short-circuit semantics. Keep the RHS in its
			// own CFG block so observable side effects occur only when required.
			l := lowerer.to_bool(left)
			rhs_label := lowerer.next_label("logic.rhs")
			short_label := lowerer.next_label("logic.short")
			end_label := lowerer.next_label("logic.end")
			if item.Operator == "&&" {
				lowerer.line("br i1 %s, label %%%s, label %%%s", l.value, rhs_label, short_label)
			} else {
				lowerer.line("br i1 %s, label %%%s, label %%%s", l.value, short_label, rhs_label)
			}
			lowerer.raw_line(rhs_label + ":")
			right := lowerer.to_bool(lowerer.emit_expr(item.Right))
			rhs_value := right.value
			rhs_predecessor := lowerer.current_block
			lowerer.line("br label %%%s", end_label)
			lowerer.raw_line(short_label + ":")
			short_value := "0"
			if item.Operator == "||" {
				short_value = "1"
			}
			lowerer.line("br label %%%s", end_label)
			lowerer.raw_line(end_label + ":")
			reg := lowerer.next_register()
			lowerer.line("%s = phi i1 [ %s, %%%s ], [ %s, %%%s ]", reg, rhs_value, rhs_predecessor, short_value, short_label)
			return IRValue{llvm_type: "i1", value: reg, type_info: sem}
		}
		right := lowerer.coerce(lowerer.emit_expr(item.Right), left.type_info)
		if item.Operator == "+" && left.type_info != nil && left.type_info.name == "string" {
			return lowerer.emit_string_concat(left, right)
		}
		if item.Operator == "==" || item.Operator == "!=" || item.Operator == "<" || item.Operator == "<=" || item.Operator == ">" || item.Operator == ">=" {
			if left.llvm_type == "ptr" && right.value == "0" {
				right.llvm_type = "ptr"
				right.value = "null"
			}
			if right.llvm_type == "ptr" && left.value == "0" {
				left.llvm_type = "ptr"
				left.value = "null"
			}
			reg := lowerer.next_register()
			if left.type_info != nil && left.type_info.name == "string" && (item.Operator == "==" || item.Operator == "!=") {
				left_c := lowerer.string_to_c_pointer(left.value)
				right_c := lowerer.string_to_c_pointer(right.value)
				cmp := lowerer.next_register()
				lowerer.line("%s = call i32 @strcmp(ptr %s, ptr %s)", cmp, left_c, right_c)
				pred := "eq"
				if item.Operator == "!=" {
					pred = "ne"
				}
				lowerer.line("%s = icmp %s i32 %s, 0", reg, pred, cmp)
				return IRValue{llvm_type: "i1", value: reg, type_info: sem}
			}
			pred := map[string]string{"==": "eq", "!=": "ne", "<": "slt", "<=": "sle", ">": "sgt", ">=": "sge"}[item.Operator]
			if is_unsigned_type(left.type_info) {
				pred = map[string]string{"==": "eq", "!=": "ne", "<": "ult", "<=": "ule", ">": "ugt", ">=": "uge"}[item.Operator]
			}
			if left.llvm_type == "float" || left.llvm_type == "double" {
				fp := map[string]string{"eq": "oeq", "ne": "one", "slt": "olt", "sle": "ole", "sgt": "ogt", "sge": "oge", "ult": "olt", "ule": "ole", "ugt": "ogt", "uge": "oge"}[pred]
				lowerer.line("%s = fcmp %s %s %s, %s", reg, fp, left.llvm_type, left.value, right.value)
			} else {
				lowerer.line("%s = icmp %s %s %s, %s", reg, pred, left.llvm_type, left.value, right.value)
			}
			return IRValue{llvm_type: "i1", value: reg, type_info: sem}
		}
		reg := lowerer.next_register()
		op := map[string]string{"+": "add", "-": "sub", "*": "mul", "/": "sdiv", "%": "srem"}[item.Operator]
		if is_unsigned_type(left.type_info) {
			if item.Operator == "/" {
				op = "udiv"
			}
			if item.Operator == "%" {
				op = "urem"
			}
		}
		if left.llvm_type == "float" || left.llvm_type == "double" {
			op = map[string]string{"+": "fadd", "-": "fsub", "*": "fmul", "/": "fdiv", "%": "frem"}[item.Operator]
		}
		if op == "" {
			return left
		}
		lowerer.line("%s = %s %s %s, %s", reg, op, left.llvm_type, left.value, right.value)
		return IRValue{llvm_type: left.llvm_type, value: reg, type_info: sem}
	case *ast.AssignExpr:
		value := lowerer.emit_expr(item.Value)
		if indexed, ok := item.Target.(*ast.IndexExpr); ok {
			objectSem := lowerer.specialize_type(lowerer.result.expression_types[indexed.Object])
			if objectSem != nil && !(objectSem.pointer_depth > 0 || objectSem.kind == type_pointer || objectSem.kind == type_array || len(objectSem.array_dimensions) > 0) {
				if written, handled := lowerer.emit_indexable_write(item, indexed, objectSem, value); handled {
					return written
				}
			}
		}
		lowerer.emit_lifetime_cleanups(item)
		address, target_sem, mecc_owner := lowerer.assignment_address_with_owner(item.Target)
		if address != "" {
			value = lowerer.coerce(value, target_sem)
			if item.Operator == "=" && is_value_aggregate(target_sem) {
				old := lowerer.next_register()
				lowerer.line("%s = load ptr, ptr %s", old, address)
				lowerer.emit_destroy_value(old, target_sem)
			}
			if item.Operator != "=" {
				old := lowerer.next_register()
				ty := lowerer.llvm_type(target_sem)
				lowerer.line("%s = load %s, ptr %s", old, ty, address)
				if item.Operator == "+=" && target_sem != nil && target_sem.name == "string" {
					value = lowerer.emit_string_concat(IRValue{llvm_type: ty, value: old, type_info: target_sem}, value)
					lowerer.emit_destroy_value(old, target_sem)
					lowerer.line("store %s %s, ptr %s", value.llvm_type, value.value, address)
					return value
				}
				op := map[string]string{"+=": "add", "-=": "sub", "*=": "mul", "/=": "sdiv", "%=": "srem"}[item.Operator]
				if ty == "float" || ty == "double" {
					op = map[string]string{"+=": "fadd", "-=": "fsub", "*=": "fmul", "/=": "fdiv", "%=": "frem"}[item.Operator]
				} else if is_unsigned_type(target_sem) {
					if item.Operator == "/=" {
						op = "udiv"
					}
					if item.Operator == "%=" {
						op = "urem"
					}
				}
				combined := lowerer.next_register()
				lowerer.line("%s = %s %s %s, %s", combined, op, ty, old, value.value)
				value = IRValue{llvm_type: ty, value: combined, type_info: target_sem}
			}
			lowerer.line("store %s %s, ptr %s", value.llvm_type, value.value, address)
			if lowerer.result.mecc_enabled && semantic_is_pointer(target_sem) && value.llvm_type == "ptr" {
				lowerer.line("call void @clyth_mecc_store(ptr %s, ptr %s, ptr %s)", mecc_owner, address, value.value)
			}
		}
		return value
	case *ast.CallExpr:
		return lowerer.emit_call(item, sem)
	case *ast.MemberExpr:
		if identifier, ok := item.Object.(*ast.IdentifierExpr); ok {
			if symbol := lowerer.lookup_type_symbol(lowerer.result.root_namespace, identifier.Name, map[*Namespace]bool{}); symbol != nil {
				if value, exists := enum_member_value(symbol, item.Name); exists {
					enum_type := clone_type(symbol.type_info)
					return IRValue{llvm_type: "i32", value: strconv.FormatInt(value, 10), type_info: enum_type}
				}
			}
		}
		object_sem := lowerer.specialize_type(lowerer.result.expression_types[item.Object])
		if object_sem == nil || object_sem.kind == type_unknown {
			object_sem = lowerer.specialize_type(lowerer.infer_expression_type(item.Object))
		}
		if object_sem != nil && object_sem.kind == type_namespace {
			if enum_map, ok := lowerer.enum_values[object_sem.namespace_name]; ok {
				if value, exists := enum_map[item.Name]; exists {
					return IRValue{llvm_type: "i32", value: strconv.FormatInt(value, 10), type_info: sem}
				}
			}
			if symbol := lowerer.lookup_type_symbol(lowerer.result.root_namespace, object_sem.namespace_name, map[*Namespace]bool{}); symbol != nil {
				if value, exists := enum_member_value(symbol, item.Name); exists {
					return IRValue{llvm_type: "i32", value: strconv.FormatInt(value, 10), type_info: sem}
				}
			}
			return lowerer.zero_value(sem)
		}
		if identifier, ok := item.Object.(*ast.IdentifierExpr); ok && identifier.Name == lowerer.main_args_name && item.Name == "length" && lowerer.main_argc_value != "" {
			reg := lowerer.next_register()
			lowerer.line("%s = sext i32 %s to i64", reg, lowerer.main_argc_value)
			return IRValue{llvm_type: "i64", value: reg, type_info: &SemanticType{kind: type_integer, name: "int64"}}
		}
		if object_sem != nil && object_sem.name == "string" && len(object_sem.array_dimensions) == 0 && item.Name == "length" {
			object := lowerer.emit_expr(item.Object)
			length := lowerer.string_length(object.value)
			return IRValue{llvm_type: "i64", value: length, type_info: &SemanticType{kind: type_integer, name: "uint64"}}
		}
		if object_sem != nil && (object_sem.kind == type_array || len(object_sem.array_dimensions) > 0) && item.Name == "length" {
			object := lowerer.emit_expr(item.Object)
			length_slot := lowerer.next_register()
			lowerer.line("%s = getelementptr i8, ptr %s, i64 8", length_slot, object.value)
			length := lowerer.next_register()
			lowerer.line("%s = load i64, ptr %s", length, length_slot)
			return IRValue{llvm_type: "i64", value: length, type_info: &SemanticType{kind: type_integer, name: "int64"}}
		}
		object := lowerer.emit_expr(item.Object)
		if object_sem != nil && (object_sem.pointer_depth > 0 || object_sem.kind == type_pointer) {
			non_null := lowerer.next_register()
			lowerer.line("%s = icmp ne ptr %s, null", non_null, object.value)
			lowerer.emit_checked_failure(non_null, "null_reference", "null pointer member access", "nullref")
		}
		if info := lowerer.struct_info[base_type_name(object_sem)]; info != nil {
			if field := info.fields[item.Name]; field != nil {
				address := lowerer.field_address(object.value, info, item.Name)
				if count, _, inline := lowerer.intrinsic_inline_fixed_array(info, field.type_info); inline {
					header := lowerer.emit_inline_array_header(address, count)
					return IRValue{llvm_type: "ptr", value: header, type_info: field.type_info}
				}
				ty := lowerer.llvm_type(field.type_info)
				reg := lowerer.next_register()
				lowerer.line("%s = load %s, ptr %s", reg, ty, address)
				return IRValue{llvm_type: ty, value: reg, type_info: field.type_info}
			}
		}
		return lowerer.zero_value(sem)
	case *ast.IndexExpr:
		object_sem := lowerer.specialize_type(lowerer.result.expression_types[item.Object])
		if object_sem == nil || object_sem.kind == type_unknown {
			object_sem = lowerer.specialize_type(lowerer.infer_expression_type(item.Object))
		}
		if object_sem != nil && !(object_sem.pointer_depth > 0 || object_sem.kind == type_pointer || object_sem.kind == type_array || len(object_sem.array_dimensions) > 0) {
			if value, handled := lowerer.emit_indexable_read(item, object_sem); handled {
				return value
			}
		}
		object := lowerer.emit_expr(item.Object)
		index := lowerer.coerce(lowerer.emit_expr(item.Index), &SemanticType{kind: type_integer, name: "int64"})
		if object_sem != nil && (object_sem.pointer_depth > 0 || object_sem.kind == type_pointer || object_sem.kind == type_array || len(object_sem.array_dimensions) > 0) {
			sem = lowerer.array_element_type(object_sem)
		}
		elem_ty := lowerer.llvm_type(sem)
		if object_sem != nil && object_sem.name == "string" && len(object_sem.array_dimensions) == 0 {
			sem = &SemanticType{kind: type_char, name: "char"}
			elem_ty = "i8"
		}
		data := object.value
		if object_sem != nil && object_sem.kind == type_string && len(object_sem.array_dimensions) == 0 {
			data = lowerer.string_data_pointer(object.value)
		} else if object_sem != nil && (object_sem.pointer_depth > 0 || object_sem.kind == type_pointer) && object_sem.kind != type_array && len(object_sem.array_dimensions) == 0 {
			// Raw pointers already are element-storage addresses. A T*[] also has
			// pointer_depth > 0 because the element is a pointer, but the outer
			// value is still an array descriptor and must load its data pointer.
			data = object.value
		} else if object_sem == nil || object_sem.kind != type_string || len(object_sem.array_dimensions) != 0 {
			data = lowerer.array_data_pointer(object.value)
		}
		lowerer.emit_index_bounds_check(item, object, index, object_sem)
		address := lowerer.next_register()
		lowerer.line("%s = getelementptr %s, ptr %s, i64 %s", address, elem_ty, data, index.value)
		value := lowerer.next_register()
		lowerer.line("%s = load %s, ptr %s", value, elem_ty, address)
		return IRValue{llvm_type: elem_ty, value: value, type_info: sem}
	case *ast.ArrayExpr:
		return lowerer.emit_array_expr(item, sem, nil)
	case *ast.KeyValueExpr:
		return lowerer.emit_expr(item.Value)
	case *ast.GenericSpecializationExpr:
		return lowerer.emit_expr(item.Target)
	case *ast.LambdaExpr:
		return lowerer.emit_lambda(item, sem)
	case *ast.NewExpr:
		var created IRValue
		// Struct constructor calls already materialize native storage.
		if call, ok := item.Value.(*ast.CallExpr); ok {
			if ident, ok2 := call.Callee.(*ast.IdentifierExpr); ok2 {
				if _, exists := lowerer.struct_info[ident.Name]; exists {
					created = lowerer.emit_expr(item.Value)
				}
			}
		}
		if created.value == "" {
			value := lowerer.emit_expr(item.Value)
			size := lowerer.semantic_storage_size(value.type_info)
			if size < 1 {
				size = 1
			}
			allocation := lowerer.next_register()
			lowerer.line("%s = call ptr @calloc(i64 1, i64 %d)", allocation, size)
			if value.llvm_type != "void" && value.llvm_type != "ptr" {
				lowerer.line("store %s %s, ptr %s", value.llvm_type, value.value, allocation)
			}
			created = IRValue{llvm_type: "ptr", value: allocation, type_info: clone_type(sem)}
		}
		id := lowerer.result.allocation_by_new[item]
		if info := lowerer.result.allocations[id]; info != nil && info.mecc {
			managed := lowerer.next_register()
			flag := 0
			if info.policy.compacting {
				flag = 1
			}
			destructor := "null"
			if info.type_info != nil {
				owned_type := clone_type(info.type_info)
				if owned_type.pointer_depth > 0 {
					owned_type.pointer_depth--
				}
				if struct_info := lowerer.struct_info[base_type_name(owned_type)]; struct_info != nil && struct_info.destructor {
					destructor = "@" + llvm_destructor_name(struct_info.name)
				}
			}
			lowerer.line("%s = call ptr @clyth_mecc_adopt(ptr %s, i1 %d, ptr %s)", managed, created.value, flag, destructor)
			created.value = managed
		}
		if slot := lowerer.cleanup_slots[id]; slot != "" {
			lowerer.line("store ptr %s, ptr %s", created.value, slot)
		}
		return created
	case *ast.CastExpr:
		value := lowerer.emit_expr(item.Value)
		target := lowerer.resolve_ref(item.Type)
		return lowerer.coerce(value, target)
	}
	return lowerer.zero_value(sem)
}

func (lowerer *llvm_lowerer) array_element_type(array_type *SemanticType) *SemanticType {
	if array_type == nil {
		return &SemanticType{kind: type_unknown}
	}
	result := clone_type(array_type)
	had_array_dimension := len(result.array_dimensions) > 0
	if had_array_dimension {
		result.array_dimensions = result.array_dimensions[1:]
	}
	if len(result.array_dimensions) > 0 {
		result.kind = type_array
		return result
	}
	// Removing an array dimension reveals the stored element type directly.
	// Pointer depth belongs to that element (T*[] -> T*), and must not be
	// peeled as though the operation were raw-pointer dereference.
	if !had_array_dimension && result.pointer_depth > 0 {
		result.pointer_depth--
		if result.pointer_depth > 0 {
			result.kind = type_pointer
			return result
		}
	}
	switch result.name {
	case "bool":
		result.kind = type_bool
	case "char":
		result.kind = type_char
	case "string":
		result.kind = type_string
	case "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64", "uintptr":
		result.kind = type_integer
	case "float32", "float64":
		result.kind = type_float
	default:
		result.kind = type_struct
	}
	return result
}

func (lowerer *llvm_lowerer) emit_array_expr(item *ast.ArrayExpr, array_sem *SemanticType, element_override *SemanticType) IRValue {
	elem_sem := element_override
	if elem_sem == nil {
		elem_sem = &SemanticType{kind: type_integer, name: "int64"}
		if len(item.Elements) > 0 {
			if inferred := lowerer.specialize_type(lowerer.result.expression_types[item.Elements[0]]); inferred != nil {
				elem_sem = inferred
			} else if literal, ok := item.Elements[0].(*ast.LiteralExpr); ok {
				elem_sem = lowerer.literal_type(literal)
			}
		}
	}
	elem_ty := lowerer.llvm_type(elem_sem)
	count := len(item.Elements)
	bytes := count * lowerer.llvm_storage_size(elem_ty)
	if bytes == 0 {
		bytes = 1
	}
	data := lowerer.next_register()
	lowerer.line("%s = call ptr @malloc(i64 %d)", data, bytes)
	for i, expression := range item.Elements {
		var value IRValue
		if pair, ok := expression.(*ast.KeyValueExpr); ok && elem_sem.kind == type_struct && elem_sem.name == "MapEntry" && len(elem_sem.generic_args) == 2 {
			key := lowerer.coerce(lowerer.emit_expr(pair.Key), elem_sem.generic_args[0])
			mapped := lowerer.coerce(lowerer.emit_expr(pair.Value), elem_sem.generic_args[1])
			constructor_name := llvm_constructor_name(base_type_name(elem_sem))
			if !lowerer.defined[constructor_name] && lowerer.declarations[constructor_name] == "" {
				lowerer.declarations[constructor_name] = fmt.Sprintf("declare ptr @%s(%s, %s)", constructor_name, lowerer.llvm_type(elem_sem.generic_args[0]), lowerer.llvm_type(elem_sem.generic_args[1]))
			}
			register := lowerer.next_register()
			lowerer.line("%s = call ptr @%s(%s %s, %s %s)", register, constructor_name, key.llvm_type, key.value, mapped.llvm_type, mapped.value)
			value = IRValue{llvm_type: "ptr", value: register, type_info: elem_sem}
		} else {
			value = lowerer.coerce(lowerer.emit_expr(expression), elem_sem)
		}
		address := lowerer.next_register()
		lowerer.line("%s = getelementptr %s, ptr %s, i64 %d", address, elem_ty, data, i)
		lowerer.line("store %s %s, ptr %s", elem_ty, value.value, address)
	}
	header := lowerer.next_register()
	lowerer.line("%s = call ptr @malloc(i64 16)", header)
	data_slot := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 0", data_slot, header)
	lowerer.line("store ptr %s, ptr %s", data, data_slot)
	length_slot := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 8", length_slot, header)
	lowerer.line("store i64 %d, ptr %s", count, length_slot)
	return IRValue{llvm_type: "ptr", value: header, type_info: array_sem, fresh: true}
}

func (lowerer *llvm_lowerer) emit_variadic_pack(expressions []ast.Expr, element_type *SemanticType, heterogeneous bool) (IRValue, []IRValue) {
	count := len(expressions)
	temporaries := []IRValue{}
	storage_count := count
	if storage_count == 0 {
		storage_count = 1
	}
	if heterogeneous {
		data := lowerer.next_register()
		lowerer.line("%s = alloca [%d x i64]", data, storage_count*2)
		for index, expression := range expressions {
			value := lowerer.emit_expr(expression)
			if value.fresh && (is_value_aggregate(value.type_info) || (value.type_info != nil && value.type_info.kind == type_string)) {
				temporaries = append(temporaries, value)
			}
			type_slot := lowerer.next_register()
			lowerer.line("%s = getelementptr [%d x i64], ptr %s, i64 0, i64 %d", type_slot, storage_count*2, data, index*2)
			lowerer.line("store i64 %d, ptr %s", stable_variadic_type_id(type_name(value.type_info)), type_slot)
			payload_slot := lowerer.next_register()
			lowerer.line("%s = getelementptr [%d x i64], ptr %s, i64 0, i64 %d", payload_slot, storage_count*2, data, index*2+1)
			payload := lowerer.variadic_payload_i64(value)
			lowerer.line("store i64 %s, ptr %s", payload, payload_slot)
		}
		header := lowerer.emit_stack_array_header(data, count)
		return IRValue{llvm_type: "ptr", value: header, type_info: &SemanticType{kind: type_array, name: "auto_variadic", array_dimensions: []SemanticArrayDimension{{dynamic: true}}}}, temporaries
	}

	target := lowerer.specialize_type(element_type)
	elem_ty := lowerer.llvm_type(target)
	data := lowerer.next_register()
	lowerer.line("%s = alloca [%d x %s]", data, storage_count, elem_ty)
	for index, expression := range expressions {
		value := lowerer.emit_expr(expression)
		if target != nil && target.kind == type_string && (value.type_info == nil || value.type_info.kind != type_string) {
			value = lowerer.emit_to_string(value)
		} else {
			value = lowerer.coerce(value, target)
		}
		if is_value_aggregate(target) || (target != nil && target.kind == type_string && value.fresh) {
			temporaries = append(temporaries, value)
		}
		slot := lowerer.next_register()
		lowerer.line("%s = getelementptr [%d x %s], ptr %s, i64 0, i64 %d", slot, storage_count, elem_ty, data, index)
		lowerer.line("store %s %s, ptr %s", elem_ty, value.value, slot)
	}
	header := lowerer.emit_stack_array_header(data, count)
	array_type := clone_type(target)
	if array_type == nil {
		array_type = &SemanticType{kind: type_unknown}
	}
	array_type.kind = type_array
	array_type.array_dimensions = []SemanticArrayDimension{{dynamic: true}}
	return IRValue{llvm_type: "ptr", value: header, type_info: array_type}, temporaries
}

func (lowerer *llvm_lowerer) emit_stack_array_header(data string, count int) string {
	header := lowerer.next_register()
	lowerer.line("%s = alloca [16 x i8]", header)
	data_slot := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 0", data_slot, header)
	lowerer.line("store ptr %s, ptr %s", data, data_slot)
	length_slot := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 8", length_slot, header)
	lowerer.line("store i64 %d, ptr %s", count, length_slot)
	return header
}

func stable_variadic_type_id(name string) uint64 {
	// Small deterministic compiler-owned type-id mixer. This is metadata identity,
	// not a cryptographic hash and does not depend on a runtime registry.
	var hash uint64 = 1469598103934665603
	for index := 0; index < len(name); index++ {
		hash ^= uint64(name[index])
		hash *= 1099511628211
	}
	return hash
}

func (lowerer *llvm_lowerer) variadic_payload_i64(value IRValue) string {
	if value.value == "" {
		return "0"
	}
	if value.llvm_type == "ptr" {
		converted := lowerer.next_register()
		lowerer.line("%s = ptrtoint ptr %s to i64", converted, value.value)
		return converted
	}
	if value.llvm_type == "double" {
		converted := lowerer.next_register()
		lowerer.line("%s = bitcast double %s to i64", converted, value.value)
		return converted
	}
	if value.llvm_type == "float" {
		bits := lowerer.next_register()
		lowerer.line("%s = bitcast float %s to i32", bits, value.value)
		converted := lowerer.next_register()
		lowerer.line("%s = zext i32 %s to i64", converted, bits)
		return converted
	}
	if strings.HasPrefix(value.llvm_type, "i") {
		bits, _ := strconv.Atoi(strings.TrimPrefix(value.llvm_type, "i"))
		if bits == 64 {
			return value.value
		}
		converted := lowerer.next_register()
		op := "sext"
		if is_unsigned_type(value.type_info) || bits == 1 {
			op = "zext"
		}
		lowerer.line("%s = %s %s %s to i64", converted, op, value.llvm_type, value.value)
		return converted
	}
	return "0"
}

func (lowerer *llvm_lowerer) emit_to_string(value IRValue) IRValue {
	string_type := &SemanticType{kind: type_string, name: "string"}
	if value.type_info != nil && value.type_info.kind == type_string {
		value.type_info = string_type
		return value
	}
	if value.type_info != nil && value.type_info.kind == type_bool && value.llvm_type == "i1" {
		true_value := lowerer.static_string_value("true", string_type)
		false_value := lowerer.static_string_value("false", string_type)
		selected := lowerer.next_register()
		lowerer.line("%s = select i1 %s, ptr %s, ptr %s", selected, value.value, true_value.value, false_value.value)
		return IRValue{llvm_type: "ptr", value: selected, type_info: string_type}
	}
	if value.type_info != nil && value.type_info.kind == type_struct {
		if info := lowerer.struct_info[base_type_name(value.type_info)]; info != nil {
			if method := info.methods["to_string"]; method != nil && method.type_info != nil && method.type_info.callable != nil && len(method.type_info.callable.parameters) == 0 {
				result := lowerer.next_register()
				lowerer.line("%s = call ptr @%s(ptr %s)", result, llvm_method_name(info.name, "to_string"), value.value)
				return IRValue{llvm_type: "ptr", value: result, type_info: string_type}
			}
		}
	}

	format := "%lld"
	argument := value
	if value.type_info != nil && is_unsigned_type(value.type_info) {
		format = "%llu"
	}
	if value.llvm_type == "float" || value.llvm_type == "double" {
		format = "%g"
		if value.llvm_type == "float" {
			promoted := lowerer.next_register()
			lowerer.line("%s = fpext float %s to double", promoted, value.value)
			argument = IRValue{llvm_type: "double", value: promoted, type_info: &SemanticType{kind: type_float, name: "float64"}}
		}
	} else if value.llvm_type == "ptr" {
		format = "%p"
	} else if value.type_info != nil && value.type_info.kind == type_char {
		format = "%c"
	}
	format_value := lowerer.static_string_value(format, string_type)
	required32 := lowerer.next_register()
	format_c := lowerer.string_to_c_pointer(format_value.value)
	lowerer.line("%s = call i32 (ptr, i64, ptr, ...) @snprintf(ptr null, i64 0, ptr %s, %s %s)", required32, format_c, argument.llvm_type, argument.value)
	required64 := lowerer.next_register()
	lowerer.line("%s = sext i32 %s to i64", required64, required32)
	capacity := lowerer.next_register()
	lowerer.line("%s = add i64 %s, 1", capacity, required64)
	buffer := lowerer.next_register()
	lowerer.line("%s = call ptr @malloc(i64 %s)", buffer, capacity)
	lowerer.line("call i32 (ptr, i64, ptr, ...) @snprintf(ptr %s, i64 %s, ptr %s, %s %s)", buffer, capacity, format_c, argument.llvm_type, argument.value)
	result := lowerer.emit_string_from_data(buffer, required64)
	lowerer.line("call void @free(ptr %s)", buffer)
	return result
}

func (lowerer *llvm_lowerer) emit_string_concat(left, right IRValue) IRValue {
	left_len := lowerer.string_length(left.value)
	right_len := lowerer.string_length(right.value)
	total := lowerer.next_register()
	lowerer.line("%s = add i64 %s, %s", total, left_len, right_len)
	temporary := lowerer.next_register()
	lowerer.line("%s = alloca i8, i64 %s", temporary, total)
	left_data := lowerer.string_data_pointer(left.value)
	right_data := lowerer.string_data_pointer(right.value)
	lowerer.line("call ptr @memcpy(ptr %s, ptr %s, i64 %s)", temporary, left_data, left_len)
	right_dest := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 %s", right_dest, temporary, left_len)
	lowerer.line("call ptr @memcpy(ptr %s, ptr %s, i64 %s)", right_dest, right_data, right_len)
	return lowerer.emit_string_from_data(temporary, total)
}

func (lowerer *llvm_lowerer) constant_initializer(expression ast.Expr, target *SemanticType) (string, bool) {
	if expression == nil {
		return lowerer.zero_value(target).value, true
	}
	if literal, ok := expression.(*ast.LiteralExpr); ok {
		switch literal.Kind {
		case ast.LiteralInteger:
			return strings.ReplaceAll(literal.Raw, "_", ""), true
		case ast.LiteralBool:
			if literal.Raw == "true" {
				return "1", true
			}
			return "0", true
		case ast.LiteralFloat:
			return literal.Raw, true
		case ast.LiteralNull:
			return "null", true
		}
	}
	return lowerer.zero_value(target).value, false
}

func (lowerer *llvm_lowerer) static_string_value(text string, type_info *SemanticType) IRValue {
	index := len(lowerer.strings)
	lowerer.strings = append(lowerer.strings, text)
	return IRValue{llvm_type: "ptr", value: fmt.Sprintf("@.clyth.string.%d", index), type_info: type_info}
}

func (lowerer *llvm_lowerer) string_info() *StructInfo {
	return lowerer.struct_info["string"]
}

func (lowerer *llvm_lowerer) string_field_address(value string, field string) string {
	info := lowerer.string_info()
	if info == nil {
		// Core is implicitly loaded for ordinary compilation. Keep these offsets
		// only as a defensive fallback for isolated lowerer tests.
		offset := map[string]int{"array_ptr": 0, "local_stack_cache": 8, "length": 24}[field]
		address := lowerer.next_register()
		lowerer.line("%s = getelementptr i8, ptr %s, i64 %d", address, value, offset)
		return address
	}
	return lowerer.field_address(value, info, field)
}

func (lowerer *llvm_lowerer) safe_string_descriptor(value string) string {
	if value == "null" {
		return "@.clyth.empty.string"
	}
	is_null := lowerer.next_register()
	lowerer.line("%s = icmp eq ptr %s, null", is_null, value)
	safe := lowerer.next_register()
	lowerer.line("%s = select i1 %s, ptr @.clyth.empty.string, ptr %s", safe, is_null, value)
	return safe
}

func (lowerer *llvm_lowerer) string_length(value string) string {
	value = lowerer.safe_string_descriptor(value)
	address := lowerer.string_field_address(value, "length")
	length := lowerer.next_register()
	lowerer.line("%s = load i64, ptr %s", length, address)
	return length
}

func (lowerer *llvm_lowerer) string_data_pointer(value string) string {
	value = lowerer.safe_string_descriptor(value)
	heap_slot := lowerer.string_field_address(value, "array_ptr")
	heap := lowerer.next_register()
	lowerer.line("%s = load ptr, ptr %s", heap, heap_slot)
	inline := lowerer.string_field_address(value, "local_stack_cache")
	is_heap := lowerer.next_register()
	lowerer.line("%s = icmp ne ptr %s, null", is_heap, heap)
	data := lowerer.next_register()
	lowerer.line("%s = select i1 %s, ptr %s, ptr %s", data, is_heap, heap, inline)
	return data
}

// string_to_c_pointer materializes a NUL-terminated view for C ABI calls.
// Internal Clyth strings remain length-carrying descriptors and do not depend
// on C-string termination. The temporary exists for the duration of the call.
func (lowerer *llvm_lowerer) string_to_c_pointer(value string) string {
	length := lowerer.string_length(value)
	capacity := lowerer.next_register()
	lowerer.line("%s = add i64 %s, 1", capacity, length)
	temporary := lowerer.next_register()
	lowerer.line("%s = alloca i8, i64 %s", temporary, capacity)
	data := lowerer.string_data_pointer(value)
	lowerer.line("call ptr @memcpy(ptr %s, ptr %s, i64 %s)", temporary, data, length)
	nul := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 %s", nul, temporary, length)
	lowerer.line("store i8 0, ptr %s", nul)
	return temporary
}

// emit_string_from_data constructs an owned Clyth string descriptor from an
// existing byte range. Values up to 15 bytes use the intrinsic inline cache;
// larger values allocate heap storage. The returned descriptor is owned/fresh.
func (lowerer *llvm_lowerer) emit_string_from_data(data string, length string) IRValue {
	string_type := &SemanticType{kind: type_string, name: "string"}
	info := lowerer.string_info()
	size := 32
	if info != nil {
		size = lowerer.struct_storage_size(info)
	}
	object := lowerer.next_register()
	lowerer.line("%s = call ptr @calloc(i64 1, i64 %d)", object, size)
	length_slot := lowerer.string_field_address(object, "length")
	lowerer.line("store i64 %s, ptr %s", length, length_slot)
	inline_test := lowerer.next_register()
	lowerer.line("%s = icmp ule i64 %s, 15", inline_test, length)
	inline_label := lowerer.next_label("string.inline")
	heap_label := lowerer.next_label("string.heap")
	done_label := lowerer.next_label("string.ready")
	lowerer.line("br i1 %s, label %%%s, label %%%s", inline_test, inline_label, heap_label)
	lowerer.raw_line(inline_label + ":")
	inline := lowerer.string_field_address(object, "local_stack_cache")
	lowerer.line("call ptr @memcpy(ptr %s, ptr %s, i64 %s)", inline, data, length)
	lowerer.line("br label %%%s", done_label)
	lowerer.raw_line(heap_label + ":")
	capacity := lowerer.next_register()
	lowerer.line("%s = add i64 %s, 1", capacity, length)
	heap := lowerer.next_register()
	lowerer.line("%s = call ptr @malloc(i64 %s)", heap, capacity)
	lowerer.line("call ptr @memcpy(ptr %s, ptr %s, i64 %s)", heap, data, length)
	nul := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 %s", nul, heap, length)
	lowerer.line("store i8 0, ptr %s", nul)
	heap_slot := lowerer.string_field_address(object, "array_ptr")
	lowerer.line("store ptr %s, ptr %s", heap, heap_slot)
	lowerer.line("br label %%%s", done_label)
	lowerer.raw_line(done_label + ":")
	return IRValue{llvm_type: "ptr", value: object, type_info: string_type, fresh: true}
}

type interpolation_piece struct {
	literal    string
	expression string
}

func split_interpolated_string(raw string) []interpolation_piece {
	text := strings.TrimSuffix(strings.TrimPrefix(raw, "`"), "`")
	pieces := []interpolation_piece{}
	start := 0
	for i := 0; i+1 < len(text); i++ {
		if text[i] != '$' || text[i+1] != '{' {
			continue
		}
		if i > start {
			pieces = append(pieces, interpolation_piece{literal: text[start:i]})
		}
		depth := 1
		j := i + 2
		quote := byte(0)
		for ; j < len(text); j++ {
			c := text[j]
			if quote != 0 {
				if c == '\\' {
					j++
					continue
				}
				if c == quote {
					quote = 0
				}
				continue
			}
			if c == '\'' || c == '"' {
				quote = c
				continue
			}
			if c == '{' {
				depth++
			}
			if c == '}' {
				depth--
				if depth == 0 {
					break
				}
			}
		}
		if j >= len(text) {
			pieces = append(pieces, interpolation_piece{literal: text[i:]})
			return pieces
		}
		pieces = append(pieces, interpolation_piece{expression: text[i+2 : j]})
		i = j
		start = j + 1
	}
	if start < len(text) {
		pieces = append(pieces, interpolation_piece{literal: text[start:]})
	}
	return pieces
}

func (lowerer *llvm_lowerer) interpolation_argument(value IRValue) (string, IRValue) {
	if value.type_info != nil && value.type_info.name == "string" {
		cValue := lowerer.string_to_c_pointer(value.value)
		return "%s", IRValue{llvm_type: "ptr", value: cValue, type_info: value.type_info}
	}
	if value.llvm_type == "ptr" {
		return "%p", value
	}
	if value.llvm_type == "float" {
		reg := lowerer.next_register()
		lowerer.line("%s = fpext float %s to double", reg, value.value)
		return "%g", IRValue{llvm_type: "double", value: reg, type_info: &SemanticType{kind: type_float, name: "float64"}}
	}
	if value.llvm_type == "double" {
		return "%g", value
	}
	if value.llvm_type == "i1" {
		return "%d", lowerer.coerce(value, &SemanticType{kind: type_integer, name: "int32"})
	}
	if strings.HasPrefix(value.llvm_type, "i") {
		if is_unsigned_type(value.type_info) {
			return "%llu", lowerer.coerce(value, &SemanticType{kind: type_integer, name: "uint64"})
		}
		return "%lld", lowerer.coerce(value, &SemanticType{kind: type_integer, name: "int64"})
	}
	return "%p", value
}

func (lowerer *llvm_lowerer) infer_expression_type(expression ast.Expr) *SemanticType {
	if expression == nil {
		return &SemanticType{kind: type_unknown}
	}
	if existing := lowerer.result.expression_types[expression]; existing != nil {
		return lowerer.specialize_type(existing)
	}
	switch item := expression.(type) {
	case *ast.LiteralExpr:
		return lowerer.literal_type(item)
	case *ast.IdentifierExpr:
		if local := lowerer.locals[item.Name]; local.type_info != nil {
			return local.type_info
		}
		if global := lowerer.globals[item.Name]; global.type_info != nil {
			return global.type_info
		}
		if lowerer.receiver != nil {
			if field := lowerer.receiver.fields[item.Name]; field != nil {
				return field.type_info
			}
		}
		if symbol := lowerer.result.root_namespace.symbols[item.Name]; symbol != nil {
			return symbol.type_info
		}
	case *ast.IndexExpr:
		object := lowerer.infer_expression_type(item.Object)
		if object != nil && object.name == "string" {
			return &SemanticType{kind: type_char, name: "char"}
		}
		return lowerer.array_element_type(object)
	case *ast.MemberExpr:
		if identifier, ok := item.Object.(*ast.IdentifierExpr); ok {
			if symbol := lowerer.lookup_type_symbol(lowerer.result.root_namespace, identifier.Name, map[*Namespace]bool{}); symbol != nil {
				if _, exists := enum_member_value(symbol, item.Name); exists {
					return clone_type(symbol.type_info)
				}
			}
		}
		object := lowerer.infer_expression_type(item.Object)
		if object != nil && (object.kind == type_array || len(object.array_dimensions) > 0) && item.Name == "length" {
			return &SemanticType{kind: type_integer, name: "int64"}
		}
		if object != nil && object.name == "string" && item.Name == "length" {
			return &SemanticType{kind: type_integer, name: "uint64"}
		}
		if info := lowerer.struct_info[base_type_name(object)]; info != nil {
			if field := info.fields[item.Name]; field != nil {
				return field.type_info
			}
		}
	case *ast.UnaryExpr:
		operand := lowerer.infer_expression_type(item.Operand)
		if item.Operator == "!" {
			return &SemanticType{kind: type_bool, name: "bool"}
		}
		return operand
	case *ast.BinaryExpr:
		if item.Operator == "==" || item.Operator == "!=" || item.Operator == "<" || item.Operator == "<=" || item.Operator == ">" || item.Operator == ">=" || item.Operator == "&&" || item.Operator == "||" {
			return &SemanticType{kind: type_bool, name: "bool"}
		}
		return lowerer.infer_expression_type(item.Left)
	case *ast.CallExpr:
		callee := lowerer.infer_expression_type(item.Callee)
		if callee != nil && callee.callable != nil {
			return callee.callable.return_type
		}
	case *ast.CastExpr:
		return lowerer.resolve_ref(item.Type)
	}
	return &SemanticType{kind: type_unknown}
}

func (lowerer *llvm_lowerer) annotate_expression_types(expression ast.Expr) {
	if expression == nil {
		return
	}
	switch item := expression.(type) {
	case *ast.IndexExpr:
		lowerer.annotate_expression_types(item.Object)
		lowerer.annotate_expression_types(item.Index)
	case *ast.MemberExpr:
		lowerer.annotate_expression_types(item.Object)
	case *ast.UnaryExpr:
		lowerer.annotate_expression_types(item.Operand)
	case *ast.BinaryExpr:
		lowerer.annotate_expression_types(item.Left)
		lowerer.annotate_expression_types(item.Right)
	case *ast.CallExpr:
		lowerer.annotate_expression_types(item.Callee)
		for _, arg := range item.Arguments {
			lowerer.annotate_expression_types(arg)
		}
	case *ast.CastExpr:
		lowerer.annotate_expression_types(item.Value)
	}
	lowerer.result.expression_types[expression] = lowerer.infer_expression_type(expression)
}

func (lowerer *llvm_lowerer) emit_interpolated_string(literal *ast.LiteralExpr, type_info *SemanticType) IRValue {
	pieces := split_interpolated_string(literal.Raw)
	var format strings.Builder
	arguments := []IRValue{}
	for _, piece := range pieces {
		if piece.literal != "" {
			format.WriteString(strings.ReplaceAll(piece.literal, "%", "%%"))
			continue
		}
		expression, parse_errors, lex_errors := parser.ParseExpression(piece.expression)
		if expression == nil || len(parse_errors) != 0 || len(lex_errors) != 0 {
			format.WriteString("<invalid-interpolation>")
			continue
		}
		lowerer.annotate_expression_types(expression)
		value := lowerer.emit_expr(expression)
		placeholder, promoted := lowerer.interpolation_argument(value)
		format.WriteString(placeholder)
		arguments = append(arguments, promoted)
	}
	format_value := lowerer.static_string_value(format.String(), &SemanticType{kind: type_string, name: "string"})
	// Static Clyth strings are descriptors, not raw C strings. snprintf must
	// receive a temporary NUL-terminated data view of the descriptor.
	format_c := lowerer.string_to_c_pointer(format_value.value)
	arg_text := []string{}
	for _, argument := range arguments {
		arg_text = append(arg_text, argument.llvm_type+" "+argument.value)
	}
	suffix := ""
	if len(arg_text) > 0 {
		suffix = ", " + strings.Join(arg_text, ", ")
	}
	required32 := lowerer.next_register()
	lowerer.line("%s = call i32 (ptr, i64, ptr, ...) @snprintf(ptr null, i64 0, ptr %s%s)", required32, format_c, suffix)
	required64 := lowerer.next_register()
	lowerer.line("%s = sext i32 %s to i64", required64, required32)
	capacity := lowerer.next_register()
	lowerer.line("%s = add i64 %s, 1", capacity, required64)
	buffer := lowerer.next_register()
	lowerer.line("%s = call ptr @malloc(i64 %s)", buffer, capacity)
	lowerer.line("call i32 (ptr, i64, ptr, ...) @snprintf(ptr %s, i64 %s, ptr %s%s)", buffer, capacity, format_c, suffix)
	// Formatted literals are ordinary owned Clyth strings. Convert the
	// temporary C buffer into the intrinsic descriptor representation before
	// returning it; returning the malloc buffer directly aliases a char* as a
	// string descriptor and corrupts every subsequent string operation.
	result := lowerer.emit_string_from_data(buffer, required64)
	lowerer.line("call void @free(ptr %s)", buffer)
	result.type_info = type_info
	return result
}

func (lowerer *llvm_lowerer) literal_type(literal *ast.LiteralExpr) *SemanticType {
	switch literal.Kind {
	case ast.LiteralInteger:
		return &SemanticType{kind: type_integer, name: "int64", untyped_literal: true}
	case ast.LiteralFloat:
		return &SemanticType{kind: type_float, name: "float64", untyped_literal: true}
	case ast.LiteralBool:
		return &SemanticType{kind: type_bool, name: "bool"}
	case ast.LiteralCharacter:
		return &SemanticType{kind: type_char, name: "char"}
	case ast.LiteralString, ast.LiteralInterpolatedString:
		return &SemanticType{kind: type_string, name: "string"}
	case ast.LiteralNull:
		return &SemanticType{kind: type_null, name: "null"}
	}
	return &SemanticType{kind: type_unknown}
}

func base_type_name(type_info *SemanticType) string {
	if type_info == nil {
		return ""
	}
	if len(type_info.generic_args) > 0 {
		return specialization_name(type_info.name, type_info.generic_args)
	}
	return type_info.name
}

func is_value_aggregate(type_info *SemanticType) bool {
	if type_info == nil {
		return false
	}
	// Arrays are value aggregates even when their base element type is a pointer.
	// pointer_depth belongs to the element type in representations such as T*[],
	// so rejecting semantic_is_pointer(type_info) first misclassifies pointer
	// arrays as scalar pointer values and skips clone/destruction ownership.
	if type_info.kind == type_array || len(type_info.array_dimensions) > 0 {
		return true
	}
	if semantic_is_pointer(type_info) {
		return false
	}
	return type_info.kind == type_struct || type_info.kind == type_string
}

func (lowerer *llvm_lowerer) active_catch() (catch_context, bool) {
	if len(lowerer.catch_stack) == 0 {
		return catch_context{}, false
	}
	return lowerer.catch_stack[len(lowerer.catch_stack)-1], true
}

func (lowerer *llvm_lowerer) emit_checked_failure(condition string, error_name string, message string, prefix string) {
	context, ok := lowerer.active_catch()
	fail_label := lowerer.next_label(prefix + ".fail")
	ok_label := lowerer.next_label(prefix + ".ok")
	lowerer.line("br i1 %s, label %%%s, label %%%s", condition, ok_label, fail_label)
	lowerer.raw_line(fail_label + ":")
	if !ok {
		lowerer.line("call void @abort()")
		lowerer.line("unreachable")
		lowerer.raw_line(ok_label + ":")
		return
	}
	id := lowerer.result.error_type_ids["builtin::"+error_name]
	message_value := lowerer.static_string_value(message, &SemanticType{kind: type_string, name: "string"})
	lowerer.store_error_value(context.error_value, "1", strconv.Itoa(id), message_value.value)
	lowerer.emit_catch_scope_cleanups(context)
	lowerer.line("br label %%%s", context.end_label)
	lowerer.raw_line(ok_label + ":")
}

func (lowerer *llvm_lowerer) emit_catch_scope_cleanups(context catch_context) {
	start := context.block_depth
	if start < 0 {
		start = 0
	}
	if start > len(lowerer.block_stack) {
		start = len(lowerer.block_stack)
	}
	for index := len(lowerer.block_stack) - 1; index >= start; index-- {
		lowerer.emit_value_cleanup_scope(index)
		lowerer.emit_lifetime_cleanups(lowerer.block_stack[index])
		lowerer.emit_mecc_clear_root_scope(index)
	}
}

func (lowerer *llvm_lowerer) emit_control_scope_cleanups(start int) {
	if start < 0 {
		start = 0
	}
	for index := len(lowerer.block_stack) - 1; index >= start; index-- {
		lowerer.emit_value_cleanup_scope(index)
		lowerer.emit_lifetime_cleanups(lowerer.block_stack[index])
		lowerer.emit_mecc_clear_root_scope(index)
	}
}

func (lowerer *llvm_lowerer) emit_value_cleanup_scope(index int) {
	if index < 0 || index >= len(lowerer.value_cleanup_scopes) {
		return
	}
	cleanups := lowerer.value_cleanup_scopes[index]
	for i := len(cleanups) - 1; i >= 0; i-- {
		cleanup := cleanups[i]
		value := lowerer.next_register()
		lowerer.line("%s = load ptr, ptr %s", value, cleanup.address)
		lowerer.emit_destroy_value(value, cleanup.type_info)
		lowerer.line("store ptr null, ptr %s", cleanup.address)
	}
}

func (lowerer *llvm_lowerer) emit_value_cleanups_from(start int) {
	if start < 0 {
		start = 0
	}
	for index := len(lowerer.value_cleanup_scopes) - 1; index >= start; index-- {
		lowerer.emit_value_cleanup_scope(index)
	}
}

func (lowerer *llvm_lowerer) emit_destroy_value(value string, type_info *SemanticType) {
	type_info = lowerer.specialize_type(type_info)
	if type_info == nil || value == "" || (semantic_is_pointer(type_info) && type_info.kind != type_array && len(type_info.array_dimensions) == 0) {
		return
	}
	non_null := lowerer.next_register()
	destroy_label := lowerer.next_label("value.destroy")
	done_label := lowerer.next_label("value.destroy.done")
	lowerer.line("%s = icmp ne ptr %s, null", non_null, value)
	lowerer.line("br i1 %s, label %%%s, label %%%s", non_null, destroy_label, done_label)
	lowerer.raw_line(destroy_label + ":")
	lowerer.emit_destroy_value_nonnull(value, type_info)
	lowerer.line("br label %%%s", done_label)
	lowerer.raw_line(done_label + ":")
}

func (lowerer *llvm_lowerer) emit_destroy_call_temporary(value IRValue) {
	if is_value_aggregate(value.type_info) {
		lowerer.emit_destroy_value(value.value, value.type_info)
	}
}

func (lowerer *llvm_lowerer) emit_destroy_string(value string) {
	heap_slot := lowerer.string_field_address(value, "array_ptr")
	heap := lowerer.next_register()
	lowerer.line("%s = load ptr, ptr %s", heap, heap_slot)
	has_heap := lowerer.next_register()
	lowerer.line("%s = icmp ne ptr %s, null", has_heap, heap)
	free_heap := lowerer.next_label("string.free.heap")
	free_object := lowerer.next_label("string.free.object")
	lowerer.line("br i1 %s, label %%%s, label %%%s", has_heap, free_heap, free_object)
	lowerer.raw_line(free_heap + ":")
	lowerer.line("call void @free(ptr %s)", heap)
	lowerer.line("br label %%%s", free_object)
	lowerer.raw_line(free_object + ":")
	lowerer.line("call void @free(ptr %s)", value)
}

func (lowerer *llvm_lowerer) emit_destroy_value_nonnull(value string, type_info *SemanticType) {
	if lowerer.result.mecc_enabled {
		// Aggregate backing storage can itself own MECC reference slots even when
		// the aggregate is an ordinary value (and therefore not a managed node).
		// Retire those roots/edges before the backing address becomes stale.
		lowerer.line("call void @clyth_mecc_owner_release(ptr %s)", value)
	}
	if type_info.kind == type_string {
		heap_slot := lowerer.string_field_address(value, "array_ptr")
		heap := lowerer.next_register()
		lowerer.line("%s = load ptr, ptr %s", heap, heap_slot)
		has_heap := lowerer.next_register()
		lowerer.line("%s = icmp ne ptr %s, null", has_heap, heap)
		free_heap := lowerer.next_label("string.value.free.heap")
		done_heap := lowerer.next_label("string.value.free.done")
		lowerer.line("br i1 %s, label %%%s, label %%%s", has_heap, free_heap, done_heap)
		lowerer.raw_line(free_heap + ":")
		lowerer.line("call void @free(ptr %s)", heap)
		lowerer.line("br label %%%s", done_heap)
		lowerer.raw_line(done_heap + ":")
		lowerer.line("call void @free(ptr %s)", value)
		return
	}
	if type_info.kind == type_array || len(type_info.array_dimensions) > 0 {
		lowerer.emit_destroy_array_value(value, type_info)
		return
	}
	if type_info.kind != type_struct {
		return
	}
	info := lowerer.struct_info[base_type_name(type_info)]
	if info == nil {
		lowerer.line("call void @free(ptr %s)", value)
		return
	}
	if info.destructor {
		lowerer.line("call void @%s(ptr %s)", llvm_destructor_name(info.name), value)
	}
	for _, name := range info.field_order {
		field := info.fields[name]
		if field == nil {
			continue
		}
		field_type := lowerer.specialize_type(field.type_info)
		if !is_value_aggregate(field_type) {
			continue
		}
		address := lowerer.field_address(value, info, name)
		nested := lowerer.next_register()
		lowerer.line("%s = load ptr, ptr %s", nested, address)
		lowerer.emit_destroy_value(nested, field_type)
	}
	lowerer.line("call void @free(ptr %s)", value)
}

func (lowerer *llvm_lowerer) emit_destroy_array_value(header string, type_info *SemanticType) {
	data := lowerer.array_data_pointer(header)
	element := lowerer.array_element_type(type_info)
	if is_value_aggregate(element) {
		length_slot := lowerer.next_register()
		lowerer.line("%s = getelementptr i8, ptr %s, i64 8", length_slot, header)
		length := lowerer.next_register()
		lowerer.line("%s = load i64, ptr %s", length, length_slot)
		index_slot := lowerer.next_register()
		lowerer.line("%s = alloca i64", index_slot)
		lowerer.line("store i64 0, ptr %s", index_slot)
		cond_label := lowerer.next_label("value.destroy.cond")
		body_label := lowerer.next_label("value.destroy.body")
		end_label := lowerer.next_label("value.destroy.end")
		lowerer.line("br label %%%s", cond_label)
		lowerer.raw_line(cond_label + ":")
		index := lowerer.next_register()
		lowerer.line("%s = load i64, ptr %s", index, index_slot)
		more := lowerer.next_register()
		lowerer.line("%s = icmp ult i64 %s, %s", more, index, length)
		lowerer.line("br i1 %s, label %%%s, label %%%s", more, body_label, end_label)
		lowerer.raw_line(body_label + ":")
		address := lowerer.next_register()
		lowerer.line("%s = getelementptr ptr, ptr %s, i64 %s", address, data, index)
		nested := lowerer.next_register()
		lowerer.line("%s = load ptr, ptr %s", nested, address)
		lowerer.emit_destroy_value(nested, element)
		next := lowerer.next_register()
		lowerer.line("%s = add i64 %s, 1", next, index)
		lowerer.line("store i64 %s, ptr %s", next, index_slot)
		lowerer.line("br label %%%s", cond_label)
		lowerer.raw_line(end_label + ":")
	}
	lowerer.line("call void @free(ptr %s)", data)
	lowerer.line("call void @free(ptr %s)", header)
}

func (lowerer *llvm_lowerer) lookup_error_type_symbol(namespace *Namespace, name string, seen map[*Namespace]bool) *Symbol {
	if namespace == nil || seen[namespace] {
		return nil
	}
	seen[namespace] = true
	if symbol := namespace.symbols[name]; symbol != nil && symbol.kind == symbol_error_type {
		return symbol
	}
	for _, imported := range namespace.open_imports {
		if symbol := lowerer.lookup_error_type_symbol(imported, name, seen); symbol != nil {
			return symbol
		}
	}
	return nil
}

func (lowerer *llvm_lowerer) store_error_value(object string, flag string, error_type string, message string) {
	info := lowerer.struct_info["Error"]
	if info == nil {
		return
	}
	flag_address := lowerer.field_address(object, info, "error")
	lowerer.line("store i1 %s, ptr %s", flag, flag_address)
	type_address := lowerer.field_address(object, info, "error_type")
	lowerer.line("store i32 %s, ptr %s", error_type, type_address)
	message_address := lowerer.field_address(object, info, "error_message")
	lowerer.line("store ptr %s, ptr %s", message, message_address)
}

func (lowerer *llvm_lowerer) emit_catch(call *ast.CallExpr, result_sem *SemanticType) IRValue {
	info := lowerer.struct_info["Error"]
	object := lowerer.next_register()
	lowerer.line("%s = call ptr @calloc(i64 1, i64 %d)", object, lowerer.struct_storage_size(info))
	empty := lowerer.static_string_value("", &SemanticType{kind: type_string, name: "string"})
	lowerer.store_error_value(object, "0", "0", empty.value)
	end_label := lowerer.next_label("catch.end")
	context := catch_context{error_value: object, end_label: end_label, block_depth: len(lowerer.block_stack)}
	lowerer.catch_stack = append(lowerer.catch_stack, context)
	if len(call.Arguments) == 1 {
		if lambda, ok := call.Arguments[0].(*ast.LambdaExpr); ok {
			terminated := lowerer.emit_block(lambda.Body)
			if !terminated {
				lowerer.line("br label %%%s", end_label)
			}
		} else {
			// Non-lambda void() callables remain supported; call them normally.
			callback := lowerer.emit_expr(call.Arguments[0])
			lowerer.line("call void %s()", callback.value)
			lowerer.line("br label %%%s", end_label)
		}
	} else {
		lowerer.line("br label %%%s", end_label)
	}
	lowerer.catch_stack = lowerer.catch_stack[:len(lowerer.catch_stack)-1]
	lowerer.raw_line(end_label + ":")
	return IRValue{llvm_type: "ptr", value: object, type_info: &SemanticType{kind: type_struct, name: "Error"}, fresh: true}
}

func (lowerer *llvm_lowerer) lookup_function_decl(namespace *Namespace, name string, seen map[*Namespace]bool) *ast.FunctionDecl {
	if namespace == nil || seen[namespace] {
		return nil
	}
	seen[namespace] = true
	if symbol := namespace.symbols[name]; symbol != nil && symbol.kind == symbol_function {
		if declaration, ok := symbol.declaration.(*ast.FunctionDecl); ok {
			return declaration
		}
	}
	for _, imported := range namespace.open_imports {
		if declaration := lowerer.lookup_function_decl(imported, name, seen); declaration != nil {
			return declaration
		}
	}
	return nil
}

func (lowerer *llvm_lowerer) is_extern_c_function(name string) bool {
	declaration := lowerer.lookup_function_decl(lowerer.result.root_namespace, name, map[*Namespace]bool{})
	return declaration != nil && declaration.Extern && strings.EqualFold(declaration.ABI, "C")
}

func (lowerer *llvm_lowerer) emit_raw_array_constructor(call *ast.CallExpr, result_sem *SemanticType) IRValue {
	array_sem := lowerer.specialize_type(result_sem)
	if array_sem == nil {
		array_sem = result_sem
	}
	if len(call.Arguments) != 1 && len(call.Arguments) != 2 {
		return lowerer.zero_value(array_sem)
	}
	lengthIndex := 0
	var source IRValue
	if len(call.Arguments) == 2 {
		source = lowerer.emit_expr(call.Arguments[0])
		lengthIndex = 1
	}
	length := lowerer.coerce(lowerer.emit_expr(call.Arguments[lengthIndex]), &SemanticType{kind: type_integer, name: "uint64"})
	element := lowerer.array_element_type(array_sem)
	element_size := lowerer.semantic_storage_size(element)
	if element_size < 1 {
		element_size = 1
	}
	header := lowerer.next_register()
	lowerer.line("%s = call ptr @calloc(i64 1, i64 16)", header)
	data := lowerer.next_register()
	lowerer.line("%s = call ptr @calloc(i64 %s, i64 %d)", data, length.value, element_size)
	lowerer.line("store ptr %s, ptr %s", data, header)
	length_slot := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 8", length_slot, header)
	lowerer.line("store i64 %s, ptr %s", length.value, length_slot)
	// Value-aggregate elements use pointer-backed storage in arrays. A freshly
	// allocated array<Struct>(n) therefore cannot leave its slots as null: field
	// access such as dimensions[0].dynamic must observe a default-constructed
	// value. Resizing must also deep-copy existing struct elements rather than
	// memcpy their backing pointers.
	if element != nil && element.kind == type_struct && element.pointer_depth == 0 {
		var sourceData, copyCount string
		if len(call.Arguments) == 2 && source.value != "" && source.value != "null" {
			sourceData = lowerer.next_register()
			lowerer.line("%s = load ptr, ptr %s", sourceData, source.value)
			sourceLengthSlot := lowerer.next_register()
			lowerer.line("%s = getelementptr i8, ptr %s, i64 8", sourceLengthSlot, source.value)
			sourceLength := lowerer.next_register()
			lowerer.line("%s = load i64, ptr %s", sourceLength, sourceLengthSlot)
			useSource := lowerer.next_register()
			lowerer.line("%s = icmp ult i64 %s, %s", useSource, sourceLength, length.value)
			copyCount = lowerer.next_register()
			lowerer.line("%s = select i1 %s, i64 %s, i64 %s", copyCount, useSource, sourceLength, length.value)
		}
		indexSlot := lowerer.next_register()
		lowerer.line("%s = alloca i64", indexSlot)
		lowerer.line("store i64 0, ptr %s", indexSlot)
		condLabel := lowerer.next_label("array.construct.cond")
		bodyLabel := lowerer.next_label("array.construct.body")
		endLabel := lowerer.next_label("array.construct.end")
		lowerer.line("br label %%%s", condLabel)
		lowerer.raw_line(condLabel + ":")
		index := lowerer.next_register()
		lowerer.line("%s = load i64, ptr %s", index, indexSlot)
		more := lowerer.next_register()
		lowerer.line("%s = icmp ult i64 %s, %s", more, index, length.value)
		lowerer.line("br i1 %s, label %%%s, label %%%s", more, bodyLabel, endLabel)
		lowerer.raw_line(bodyLabel + ":")
		storedSlot := lowerer.next_register()
		lowerer.line("%s = alloca ptr", storedSlot)
		if sourceData != "" {
			hasSource := lowerer.next_register()
			lowerer.line("%s = icmp ult i64 %s, %s", hasSource, index, copyCount)
			copyLabel := lowerer.next_label("array.construct.copy")
			defaultLabel := lowerer.next_label("array.construct.default")
			storeLabel := lowerer.next_label("array.construct.store")
			lowerer.line("br i1 %s, label %%%s, label %%%s", hasSource, copyLabel, defaultLabel)
			lowerer.raw_line(copyLabel + ":")
			sourceAddress := lowerer.next_register()
			lowerer.line("%s = getelementptr ptr, ptr %s, i64 %s", sourceAddress, sourceData, index)
			sourceElement := lowerer.next_register()
			lowerer.line("%s = load ptr, ptr %s", sourceElement, sourceAddress)
			isNull := lowerer.next_register()
			lowerer.line("%s = icmp eq ptr %s, null", isNull, sourceElement)
			nullLabel := lowerer.next_label("array.construct.null")
			cloneLabel := lowerer.next_label("array.construct.clone")
			lowerer.line("br i1 %s, label %%%s, label %%%s", isNull, nullLabel, cloneLabel)
			lowerer.raw_line(nullLabel + ":")
			defaultValue := lowerer.default_struct_value(element)
			lowerer.line("store ptr %s, ptr %s", defaultValue.value, storedSlot)
			lowerer.line("br label %%%s", storeLabel)
			lowerer.raw_line(cloneLabel + ":")
			cloned := lowerer.clone_struct_value(IRValue{llvm_type: "ptr", value: sourceElement, type_info: element}, element)
			lowerer.line("store ptr %s, ptr %s", cloned.value, storedSlot)
			lowerer.line("br label %%%s", storeLabel)
			lowerer.raw_line(defaultLabel + ":")
			defaultValue = lowerer.default_struct_value(element)
			lowerer.line("store ptr %s, ptr %s", defaultValue.value, storedSlot)
			lowerer.line("br label %%%s", storeLabel)
			lowerer.raw_line(storeLabel + ":")
		} else {
			defaultValue := lowerer.default_struct_value(element)
			lowerer.line("store ptr %s, ptr %s", defaultValue.value, storedSlot)
		}
		storedElement := lowerer.next_register()
		lowerer.line("%s = load ptr, ptr %s", storedElement, storedSlot)
		destinationAddress := lowerer.next_register()
		lowerer.line("%s = getelementptr ptr, ptr %s, i64 %s", destinationAddress, data, index)
		lowerer.line("store ptr %s, ptr %s", storedElement, destinationAddress)
		if lowerer.result.mecc_enabled {
			lowerer.line("call void @clyth_mecc_store(ptr %s, ptr %s, ptr %s)", header, destinationAddress, storedElement)
		}
		nextIndex := lowerer.next_register()
		lowerer.line("%s = add i64 %s, 1", nextIndex, index)
		lowerer.line("store i64 %s, ptr %s", nextIndex, indexSlot)
		lowerer.line("br label %%%s", condLabel)
		lowerer.raw_line(endLabel + ":")
	} else if len(call.Arguments) == 2 && source.value != "" && source.value != "null" {
		sourceData := lowerer.next_register()
		lowerer.line("%s = load ptr, ptr %s", sourceData, source.value)
		sourceLengthSlot := lowerer.next_register()
		lowerer.line("%s = getelementptr i8, ptr %s, i64 8", sourceLengthSlot, source.value)
		sourceLength := lowerer.next_register()
		lowerer.line("%s = load i64, ptr %s", sourceLength, sourceLengthSlot)
		useSource := lowerer.next_register()
		lowerer.line("%s = icmp ult i64 %s, %s", useSource, sourceLength, length.value)
		copyCount := lowerer.next_register()
		lowerer.line("%s = select i1 %s, i64 %s, i64 %s", copyCount, useSource, sourceLength, length.value)
		copyBytes := lowerer.next_register()
		lowerer.line("%s = mul i64 %s, %d", copyBytes, copyCount, element_size)
		lowerer.line("call void @llvm.memcpy.p0.p0.i64(ptr %s, ptr %s, i64 %s, i1 false)", data, sourceData, copyBytes)
		lowerer.declarations["llvm.memcpy.p0.p0.i64"] = "declare void @llvm.memcpy.p0.p0.i64(ptr, ptr, i64, i1)"
	}
	return IRValue{llvm_type: "ptr", value: header, type_info: array_sem}
}

func (lowerer *llvm_lowerer) emit_call(call *ast.CallExpr, result_sem *SemanticType) IRValue {
	if specialization, ok := call.Callee.(*ast.GenericSpecializationExpr); ok {
		if identifier, ok := specialization.Target.(*ast.IdentifierExpr); ok && identifier.Name == "array" {
			return lowerer.emit_raw_array_constructor(call, result_sem)
		}
	}
	if identifier, ok := call.Callee.(*ast.IdentifierExpr); ok && identifier.Name == "catch" {
		return lowerer.emit_catch(call, result_sem)
	}
	if identifier, ok := call.Callee.(*ast.IdentifierExpr); ok && identifier.Name == "raise" {
		if len(call.Arguments) == 2 && len(lowerer.catch_stack) > 0 {
			error_type := lowerer.emit_expr(call.Arguments[0])
			message := lowerer.emit_expr(call.Arguments[1])
			context := lowerer.catch_stack[len(lowerer.catch_stack)-1]
			lowerer.store_error_value(context.error_value, "1", error_type.value, message.value)
			lowerer.emit_catch_scope_cleanups(context)
			lowerer.line("br label %%%s", context.end_label)
			return IRValue{llvm_type: "void", type_info: result_sem}
		}
		return IRValue{llvm_type: "void", type_info: result_sem}
	}
	if identifier, ok := call.Callee.(*ast.IdentifierExpr); ok && identifier.Name == "free" {
		if len(call.Arguments) == 1 {
			argument := lowerer.emit_expr(call.Arguments[0])
			if argument.value != "null" {
				if lowerer.result.mecc_enabled {
					lowerer.line("call void @clyth_mecc_owner_release(ptr %s)", argument.value)
				}
				id := lowerer.result.expression_allocations[call.Arguments[0]]
				if info := lowerer.result.allocations[id]; info != nil && info.mecc {
					lowerer.line("call void @clyth_mecc_release(ptr %s)", argument.value)
				} else {
					lowerer.emit_destructor_for_value(argument.value, info)
					lowerer.line("call void @free(ptr %s)", argument.value)
				}
				if slot := lowerer.cleanup_slots[id]; slot != "" {
					lowerer.line("store ptr null, ptr %s", slot)
				}
			}
		}
		return IRValue{llvm_type: "void", type_info: result_sem}
	}
	callee_sem := lowerer.result.expression_types[call.Callee]
	if callee_sem == nil {
		callee_sem = lowerer.infer_expression_type(call.Callee)
	}
	expected := []*SemanticType{}
	default_values := []ast.Expr{}
	variadic := false
	native_variadic := false
	heterogeneous_variadic := false
	if callee_sem != nil && callee_sem.callable != nil {
		expected = callee_sem.callable.parameters
		default_values = callee_sem.callable.default_values
		variadic = callee_sem.callable.variadic
		native_variadic = callee_sem.callable.native_variadic
		heterogeneous_variadic = callee_sem.callable.heterogeneous_variadic
	}
	arguments := []IRValue{}
	function_name := ""
	indirect := ""
	switch callee := call.Callee.(type) {
	case *ast.IdentifierExpr:
		if lowerer.receiver != nil && lowerer.receiver.methods[callee.Name] != nil {
			function_name = llvm_method_name(lowerer.receiver.name, callee.Name)
			arguments = append(arguments, IRValue{llvm_type: "ptr", value: lowerer.locals["this"].address, type_info: &SemanticType{kind: type_struct, name: lowerer.receiver.name}})
		} else if template, exists := lowerer.struct_info[callee.Name]; exists {
			constructor_type := lowerer.specialize_type(result_sem)
			if len(call.Arguments) == 0 && len(template.constructors) > 0 && !constructor_accepts_no_explicit_arguments(template.constructors[0]) {
				return lowerer.default_struct_value(constructor_type)
			}
			function_name = llvm_constructor_name(base_type_name(constructor_type))
			if len(template.constructors) > 0 {
				constructor := template.constructors[0]
				substitutions := build_substitutions(template.generic_parameters, constructor_type.generic_args)
				expected = expected[:0]
				for _, parameter := range constructor.parameters {
					expected = append(expected, substitute_type(parameter, substitutions))
				}
				default_values = constructor.default_values
				variadic = constructor.variadic
				native_variadic = constructor.native_variadic
				heterogeneous_variadic = constructor.heterogeneous_variadic
			}
		} else if local, ok := lowerer.locals[callee.Name]; ok && local.type_info != nil && local.type_info.callable != nil {
			loaded := lowerer.emit_expr(callee)
			indirect = loaded.value
		} else {
			function_name = sanitize_llvm_name(callee.Name)
		}
	case *ast.MemberExpr:
		object_sem := lowerer.specialize_type(lowerer.result.expression_types[callee.Object])
		if object_sem != nil && object_sem.kind == type_namespace {
			function_name = sanitize_llvm_name(callee.Name)
		} else {
			object := lowerer.emit_expr(callee.Object)
			arguments = append(arguments, object)
			function_name = llvm_method_name(base_type_name(object_sem), callee.Name)
		}
	case *ast.GenericSpecializationExpr:
		if id, ok := callee.Target.(*ast.IdentifierExpr); ok {
			constructor_type := lowerer.specialize_type(result_sem)
			if constructor_type == nil || constructor_type.name == "" || constructor_type.kind == type_unknown {
				constructor_type = &SemanticType{kind: type_struct, name: id.Name}
				for _, argument := range callee.TypeArguments {
					constructor_type.generic_args = append(constructor_type.generic_args, lowerer.resolve_ref(argument))
				}
			}
			if constructor_type.name == id.Name && len(constructor_type.generic_args) == 0 {
				for _, argument := range callee.TypeArguments {
					constructor_type.generic_args = append(constructor_type.generic_args, lowerer.resolve_ref(argument))
				}
			}
			constructor_type = lowerer.specialize_type(constructor_type)
			if info := lowerer.struct_info[base_type_name(constructor_type)]; info != nil {
				if len(call.Arguments) == 0 && len(info.constructors) > 0 && !constructor_accepts_no_explicit_arguments(info.constructors[0]) {
					return lowerer.default_struct_value(constructor_type)
				}
				function_name = llvm_constructor_name(info.name)
				if len(info.constructors) > 0 {
					constructor := info.constructors[0]
					expected = append(expected[:0], constructor.parameters...)
					default_values = constructor.default_values
					variadic = constructor.variadic
					native_variadic = constructor.native_variadic
					heterogeneous_variadic = constructor.heterogeneous_variadic
				}
			} else {
				function_name = sanitize_llvm_name(id.Name)
			}
		} else {
			v := lowerer.emit_expr(callee)
			indirect = v.value
		}
	default:
		v := lowerer.emit_expr(call.Callee)
		indirect = v.value
	}
	effective_args := append([]ast.Expr(nil), call.Arguments...)
	fixed_count := len(expected)
	if variadic && !native_variadic && fixed_count > 0 {
		fixed_count--
	}
	for len(effective_args) < fixed_count {
		index := len(effective_args)
		if index >= len(default_values) || default_values[index] == nil {
			break
		}
		effective_args = append(effective_args, default_values[index])
	}

	regular_count := len(effective_args)
	if variadic && !native_variadic {
		if regular_count > fixed_count {
			regular_count = fixed_count
		}
	}
	extern_c := function_name != "" && lowerer.is_extern_c_function(function_name)
	value_argument_temporaries := []IRValue{}
	for index := 0; index < regular_count; index++ {
		arg := effective_args[index]
		var value IRValue
		if index < len(expected) {
			target := lowerer.specialize_type(expected[index])
			if array_expr, ok := arg.(*ast.ArrayExpr); ok && target != nil && (target.kind == type_array || len(target.array_dimensions) > 0) {
				value = lowerer.emit_array_expr(array_expr, target, lowerer.array_element_type(target))
			} else {
				value = lowerer.coerce(lowerer.emit_expr(arg), target)
			}
			if is_value_aggregate(target) || (target != nil && target.kind == type_string && value.fresh) {
				value_argument_temporaries = append(value_argument_temporaries, value)
			}
		} else {
			value = lowerer.emit_expr(arg)
		}
		if extern_c && value.type_info != nil && value.type_info.kind == type_string {
			value = IRValue{llvm_type: "ptr", value: lowerer.string_to_c_pointer(value.value), type_info: value.type_info}
		}
		arguments = append(arguments, value)
	}
	if variadic && !native_variadic && len(expected) > 0 {
		variadic_type := lowerer.specialize_type(expected[len(expected)-1])
		pack, variadic_temporaries := lowerer.emit_variadic_pack(effective_args[regular_count:], variadic_type, heterogeneous_variadic)
		arguments = append(arguments, pack)
		value_argument_temporaries = append(value_argument_temporaries, variadic_temporaries...)
	} else {
		for index := regular_count; index < len(effective_args); index++ {
			value := lowerer.emit_expr(effective_args[index])
			if extern_c && value.type_info != nil && value.type_info.kind == type_string {
				value = IRValue{llvm_type: "ptr", value: lowerer.string_to_c_pointer(value.value), type_info: value.type_info}
			}
			arguments = append(arguments, value)
		}
	}
	return_ty := lowerer.llvm_type(result_sem)
	arg_text := make([]string, 0, len(arguments))
	for _, arg := range arguments {
		arg_text = append(arg_text, arg.llvm_type+" "+arg.value)
	}
	callee_text := "@" + function_name
	if indirect != "" {
		callee_text = indirect
	}
	if function_name != "" && !lowerer.defined[function_name] && lowerer.declarations[function_name] == "" {
		declaration_args := []string{}
		if len(arguments) > 0 {
			for _, arg := range arguments {
				declaration_args = append(declaration_args, arg.llvm_type)
			}
		} else {
			for _, arg := range expected {
				declaration_args = append(declaration_args, lowerer.llvm_type(arg))
			}
		}
		if variadic {
			declaration_args = append(declaration_args, "...")
		}
		lowerer.declarations[function_name] = fmt.Sprintf("declare %s @%s(%s)", return_ty, function_name, strings.Join(declaration_args, ", "))
	}
	if return_ty == "void" {
		lowerer.line("call void %s(%s)", callee_text, strings.Join(arg_text, ", "))
		for index := len(value_argument_temporaries) - 1; index >= 0; index-- {
			temporary := value_argument_temporaries[index]
			lowerer.emit_destroy_call_temporary(temporary)
		}
		return IRValue{llvm_type: "void", value: "", type_info: result_sem}
	}
	reg := lowerer.next_register()
	lowerer.line("%s = call %s %s(%s)", reg, return_ty, callee_text, strings.Join(arg_text, ", "))
	if extern_c && result_sem != nil && result_sem.kind == type_string {
		length := lowerer.next_register()
		lowerer.line("%s = call i64 @strlen(ptr %s)", length, reg)
		wrapped := lowerer.emit_string_from_data(reg, length)
		reg = wrapped.value
	}
	for index := len(value_argument_temporaries) - 1; index >= 0; index-- {
		temporary := value_argument_temporaries[index]
		lowerer.emit_destroy_call_temporary(temporary)
	}
	fresh := result_sem != nil && result_sem.kind == type_struct && result_sem.pointer_depth == 0 && strings.Contains(function_name, "__ctor")
	return IRValue{llvm_type: return_ty, value: reg, type_info: result_sem, fresh: fresh}
}

func (lowerer *llvm_lowerer) emit_lambda(lambda *ast.LambdaExpr, sem *SemanticType) IRValue {
	lowerer.lambda++
	name := fmt.Sprintf("clyth.lambda.%d", lowerer.lambda)
	// Save the surrounding function builder and emit the non-capturing lambda separately.
	saved_lines, saved_locals, saved_return, saved_receiver := lowerer.current_lines, lowerer.locals, lowerer.return_type, lowerer.receiver
	saved_reg, saved_label := lowerer.register, lowerer.label
	lowerer.register, lowerer.label = 0, 0
	lowerer.current_lines = nil
	lowerer.locals = map[string]ir_local{}
	lowerer.receiver = nil
	callable := sem.callable
	if callable == nil {
		callable = &SemanticCallable{return_type: &SemanticType{kind: type_void, name: "void"}}
	}
	lowerer.return_type = callable.return_type
	params := []string{}
	for i, p := range lambda.Parameters {
		pt := &SemanticType{kind: type_unknown}
		if i < len(callable.parameters) {
			pt = callable.parameters[i]
		}
		ty := lowerer.llvm_type(pt)
		params = append(params, fmt.Sprintf("%s %%arg.%d", ty, i))
		addr := fmt.Sprintf("%%lambda.arg.addr.%d", i)
		lowerer.locals[p.Name] = ir_local{address: addr, type_info: pt}
	}
	lowerer.raw_line(fmt.Sprintf("define internal %s @%s(%s) {", lowerer.llvm_type(callable.return_type), name, strings.Join(params, ", ")))
	lowerer.raw_line("entry:")
	for i, p := range lambda.Parameters {
		local := lowerer.locals[p.Name]
		ty := lowerer.llvm_type(local.type_info)
		lowerer.line("%s = alloca %s", local.address, ty)
		lowerer.line("store %s %%arg.%d, ptr %s", ty, i, local.address)
	}
	term := lowerer.emit_block(lambda.Body)
	if !term {
		lowerer.emit_default_return(callable.return_type)
	}
	lowerer.raw_line("}")
	lambda_def := strings.Join(lowerer.current_lines, "\n")
	lowerer.current_lines, saved_lines = saved_lines, lowerer.current_lines
	_ = saved_lines
	lowerer.locals = saved_locals
	lowerer.return_type = saved_return
	lowerer.receiver = saved_receiver
	lowerer.register = saved_reg
	lowerer.label = saved_label
	lowerer.function_defs = append(lowerer.function_defs, lambda_def)
	return IRValue{llvm_type: "ptr", value: "@" + name, type_info: sem}
}

func span_contains(outer, inner ast.Node) bool {
	if outer == nil || inner == nil {
		return false
	}
	return inner.Span().Start.Offset >= outer.Span().Start.Offset && inner.Span().End.Offset <= outer.Span().End.Offset
}

func (lowerer *llvm_lowerer) prepare_cleanup_slots(body ast.Node) {
	if body == nil {
		return
	}
	// Cleanup actions are path-sensitive; AllocationInfo flags such as
	// explicitly_freed/escaped are aggregate summaries across all analyzed paths.
	// Allocate a slot whenever at least one concrete path requires compiler
	// cleanup. Explicit free nulls the slot on paths that destroy early, which
	// gives us exactly-once destruction without suppressing cleanup elsewhere.
	needed := map[int]bool{}
	for _, action := range lowerer.result.lifetime_actions {
		if action.kind != lifetime_cleanup || action.allocation <= 0 {
			continue
		}
		info := lowerer.result.allocations[action.allocation]
		if info == nil || info.new_expr == nil || info.mecc || !span_contains(body, info.new_expr) {
			continue
		}
		needed[action.allocation] = true
	}
	ids := make([]int, 0, len(needed))
	for id := range needed {
		ids = append(ids, id)
	}
	for i := 1; i < len(ids); i++ {
		for j := i; j > 0 && ids[j] < ids[j-1]; j-- {
			ids[j], ids[j-1] = ids[j-1], ids[j]
		}
	}
	for _, id := range ids {
		slot := lowerer.next_register()
		lowerer.line("%s = alloca ptr", slot)
		lowerer.line("store ptr null, ptr %s", slot)
		lowerer.cleanup_slots[id] = slot
	}
}

func (lowerer *llvm_lowerer) emit_destructor_for_value(value string, info *AllocationInfo) {
	if info == nil || info.type_info == nil {
		return
	}
	type_info := clone_type(info.type_info)
	if type_info.pointer_depth > 0 {
		type_info.pointer_depth--
	}
	name := base_type_name(type_info)
	struct_info := lowerer.struct_info[name]
	if struct_info == nil || !struct_info.destructor {
		return
	}
	fn := llvm_destructor_name(name)
	if !lowerer.defined[fn] && lowerer.declarations[fn] == "" {
		lowerer.declarations[fn] = fmt.Sprintf("declare void @%s(ptr)", fn)
	}
	lowerer.line("call void @%s(ptr %s)", fn, value)
}

func (lowerer *llvm_lowerer) emit_lifetime_cleanups(node ast.Node) {
	for _, action := range lowerer.lifetime_actions[node] {
		if action.kind != lifetime_cleanup {
			continue
		}
		slot := lowerer.cleanup_slots[action.allocation]
		if slot == "" {
			continue
		}
		value := lowerer.next_register()
		lowerer.line("%s = load ptr, ptr %s", value, slot)
		occupied := lowerer.next_register()
		lowerer.line("%s = icmp ne ptr %s, null", occupied, value)
		free_label := lowerer.next_label("cleanup.free")
		done_label := lowerer.next_label("cleanup.done")
		lowerer.line("br i1 %s, label %%%s, label %%%s", occupied, free_label, done_label)
		lowerer.raw_line(free_label + ":")
		info := lowerer.result.allocations[action.allocation]
		lowerer.emit_destructor_for_value(value, info)
		lowerer.line("call void @free(ptr %s)", value)
		lowerer.line("store ptr null, ptr %s", slot)
		lowerer.line("br label %%%s", done_label)
		lowerer.raw_line(done_label + ":")
	}
}

func (lowerer *llvm_lowerer) register_mecc_root(slot string) {
	if !lowerer.result.mecc_enabled || slot == "" || len(lowerer.mecc_root_scopes) == 0 {
		return
	}
	index := len(lowerer.mecc_root_scopes) - 1
	lowerer.mecc_root_scopes[index] = append(lowerer.mecc_root_scopes[index], slot)
}

func (lowerer *llvm_lowerer) emit_mecc_clear_root_scope(index int) {
	if !lowerer.result.mecc_enabled || index < 0 || index >= len(lowerer.mecc_root_scopes) {
		return
	}
	for _, slot := range lowerer.mecc_root_scopes[index] {
		lowerer.line("call void @clyth_mecc_store(ptr null, ptr %s, ptr null)", slot)
	}
}

func (lowerer *llvm_lowerer) emit_mecc_clear_all_roots() {
	if !lowerer.result.mecc_enabled {
		return
	}
	for index := len(lowerer.mecc_root_scopes) - 1; index >= 0; index-- {
		lowerer.emit_mecc_clear_root_scope(index)
	}
}

func (lowerer *llvm_lowerer) emit_mecc_function_frontier() {
	if !lowerer.result.mecc_enabled {
		return
	}
	lowerer.emit_mecc_clear_all_roots()
	// Any pointer-backed return transfers storage/references to the caller before
	// it can establish its own roots. This includes value aggregates (structs,
	// arrays, and strings), not only semantic pointer types. Collapsing here can
	// reclaim managed pointees reachable from the returned aggregate before the
	// caller receives it. Defer collapse until the next non-pointer-backed frontier.
	if lowerer.llvm_type(lowerer.return_type) != "ptr" {
		lowerer.line("call void @clyth_mecc_collapse()")
	}
}

func (lowerer *llvm_lowerer) emit_default_return(type_info *SemanticType) {
	lowerer.emit_mecc_function_frontier()
	v := lowerer.zero_value(type_info)
	if v.llvm_type == "void" {
		lowerer.line("ret void")
	} else {
		lowerer.line("ret %s %s", v.llvm_type, v.value)
	}
}
func (lowerer *llvm_lowerer) zero_value(type_info *SemanticType) IRValue {
	ty := lowerer.llvm_type(type_info)
	value := "0"
	if ty == "ptr" {
		value = "null"
	} else if ty == "float" || ty == "double" {
		value = "0.0"
	} else if ty == "void" {
		value = ""
	}
	return IRValue{llvm_type: ty, value: value, type_info: type_info}
}

func (lowerer *llvm_lowerer) clone_struct_value(value IRValue, target *SemanticType) IRValue {
	info := lowerer.struct_info[base_type_name(target)]
	if info == nil || value.value == "null" {
		return IRValue{llvm_type: "ptr", value: value.value, type_info: target}
	}
	copy_value := lowerer.next_register()
	lowerer.line("%s = call ptr @calloc(i64 1, i64 %d)", copy_value, lowerer.struct_storage_size(info))
	for _, name := range info.field_order {
		field := info.fields[name]
		if field == nil {
			continue
		}
		field_type := lowerer.specialize_type(field.type_info)
		source_address := lowerer.field_address(value.value, info, name)
		destination_address := lowerer.field_address(copy_value, info, name)
		ty := lowerer.llvm_type(field_type)
		loaded := lowerer.next_register()
		lowerer.line("%s = load %s, ptr %s", loaded, ty, source_address)
		field_value := IRValue{llvm_type: ty, value: loaded, type_info: field_type}
		if is_value_aggregate(field_type) {
			field_value = lowerer.clone_value_aggregate(field_value, field_type)
		}
		lowerer.line("store %s %s, ptr %s", field_value.llvm_type, field_value.value, destination_address)
		if lowerer.result.mecc_enabled && semantic_is_pointer(field_type) && field_value.llvm_type == "ptr" {
			// Aggregate cloning must recreate MECC ownership edges for shallow pointer
			// fields. Otherwise destroying the source aggregate can retire the only
			// registered edge and leave the cloned aggregate with a dangling pointer.
			lowerer.line("call void @clyth_mecc_store(ptr %s, ptr %s, ptr %s)", copy_value, destination_address, field_value.value)
		}
	}
	return IRValue{llvm_type: "ptr", value: copy_value, type_info: target, fresh: true}
}

func (lowerer *llvm_lowerer) clone_array_value(value IRValue, target *SemanticType) IRValue {
	if value.value == "null" {
		return IRValue{llvm_type: "ptr", value: "null", type_info: target}
	}
	length_slot := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 8", length_slot, value.value)
	length := lowerer.next_register()
	lowerer.line("%s = load i64, ptr %s", length, length_slot)
	source_data := lowerer.array_data_pointer(value.value)
	element := lowerer.array_element_type(target)
	element_size := lowerer.semantic_storage_size(element)
	new_header := lowerer.next_register()
	lowerer.line("%s = call ptr @calloc(i64 1, i64 16)", new_header)
	bytes := lowerer.next_register()
	lowerer.line("%s = mul i64 %s, %d", bytes, length, element_size)
	new_data := lowerer.next_register()
	lowerer.line("%s = call ptr @calloc(i64 %s, i64 %d)", new_data, length, element_size)
	if is_value_aggregate(element) {
		index_slot := lowerer.next_register()
		lowerer.line("%s = alloca i64", index_slot)
		lowerer.line("store i64 0, ptr %s", index_slot)
		cond_label := lowerer.next_label("array.copy.cond")
		body_label := lowerer.next_label("array.copy.body")
		end_label := lowerer.next_label("array.copy.end")
		lowerer.line("br label %%%s", cond_label)
		lowerer.raw_line(cond_label + ":")
		index := lowerer.next_register()
		lowerer.line("%s = load i64, ptr %s", index, index_slot)
		more := lowerer.next_register()
		lowerer.line("%s = icmp ult i64 %s, %s", more, index, length)
		lowerer.line("br i1 %s, label %%%s, label %%%s", more, body_label, end_label)
		lowerer.raw_line(body_label + ":")
		source_element_address := lowerer.next_register()
		lowerer.line("%s = getelementptr ptr, ptr %s, i64 %s", source_element_address, source_data, index)
		source_element := lowerer.next_register()
		lowerer.line("%s = load ptr, ptr %s", source_element, source_element_address)
		stored_slot := lowerer.next_register()
		lowerer.line("%s = alloca ptr", stored_slot)
		is_null := lowerer.next_register()
		lowerer.line("%s = icmp eq ptr %s, null", is_null, source_element)
		null_label := lowerer.next_label("array.copy.null")
		clone_label := lowerer.next_label("array.copy.clone")
		store_label := lowerer.next_label("array.copy.store")
		lowerer.line("br i1 %s, label %%%s, label %%%s", is_null, null_label, clone_label)
		lowerer.raw_line(null_label + ":")
		lowerer.line("store ptr null, ptr %s", stored_slot)
		lowerer.line("br label %%%s", store_label)
		lowerer.raw_line(clone_label + ":")
		cloned := lowerer.clone_value_aggregate(IRValue{llvm_type: "ptr", value: source_element, type_info: element}, element)
		lowerer.line("store ptr %s, ptr %s", cloned.value, stored_slot)
		lowerer.line("br label %%%s", store_label)
		lowerer.raw_line(store_label + ":")
		stored_element := lowerer.next_register()
		lowerer.line("%s = load ptr, ptr %s", stored_element, stored_slot)
		destination_element_address := lowerer.next_register()
		lowerer.line("%s = getelementptr ptr, ptr %s, i64 %s", destination_element_address, new_data, index)
		lowerer.line("store ptr %s, ptr %s", stored_element, destination_element_address)
		if lowerer.result.mecc_enabled {
			// The cloned array owns each cloned value-aggregate element. Recreate
			// that edge explicitly so a collapse cannot retire the element while
			// it remains reachable through the destination array.
			lowerer.line("call void @clyth_mecc_store(ptr %s, ptr %s, ptr %s)", new_header, destination_element_address, stored_element)
		}
		next_index := lowerer.next_register()
		lowerer.line("%s = add i64 %s, 1", next_index, index)
		lowerer.line("store i64 %s, ptr %s", next_index, index_slot)
		lowerer.line("br label %%%s", cond_label)
		lowerer.raw_line(end_label + ":")
	} else {
		lowerer.line("call ptr @memcpy(ptr %s, ptr %s, i64 %s)", new_data, source_data, bytes)
		if lowerer.result.mecc_enabled && semantic_is_pointer(element) {
			// memcpy duplicates pointer bits but not MECC estate edges. Walk the
			// cloned pointer array and register each destination slot as owned by
			// the new array header.
			index_slot := lowerer.next_register()
			lowerer.line("%s = alloca i64", index_slot)
			lowerer.line("store i64 0, ptr %s", index_slot)
			cond_label := lowerer.next_label("array.edge.cond")
			body_label := lowerer.next_label("array.edge.body")
			end_label := lowerer.next_label("array.edge.end")
			lowerer.line("br label %%%s", cond_label)
			lowerer.raw_line(cond_label + ":")
			index := lowerer.next_register()
			lowerer.line("%s = load i64, ptr %s", index, index_slot)
			more := lowerer.next_register()
			lowerer.line("%s = icmp ult i64 %s, %s", more, index, length)
			lowerer.line("br i1 %s, label %%%s, label %%%s", more, body_label, end_label)
			lowerer.raw_line(body_label + ":")
			destination_element_address := lowerer.next_register()
			lowerer.line("%s = getelementptr ptr, ptr %s, i64 %s", destination_element_address, new_data, index)
			target := lowerer.next_register()
			lowerer.line("%s = load ptr, ptr %s", target, destination_element_address)
			lowerer.line("call void @clyth_mecc_store(ptr %s, ptr %s, ptr %s)", new_header, destination_element_address, target)
			next_index := lowerer.next_register()
			lowerer.line("%s = add i64 %s, 1", next_index, index)
			lowerer.line("store i64 %s, ptr %s", next_index, index_slot)
			lowerer.line("br label %%%s", cond_label)
			lowerer.raw_line(end_label + ":")
		}
	}
	data_slot := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 0", data_slot, new_header)
	lowerer.line("store ptr %s, ptr %s", new_data, data_slot)
	new_length_slot := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 8", new_length_slot, new_header)
	lowerer.line("store i64 %s, ptr %s", length, new_length_slot)
	return IRValue{llvm_type: "ptr", value: new_header, type_info: target, fresh: true}
}

func (lowerer *llvm_lowerer) clone_string_value(value IRValue) IRValue {
	if value.value == "" || value.value == "null" {
		return value
	}
	data := lowerer.string_data_pointer(value.value)
	length := lowerer.string_length(value.value)
	return lowerer.emit_string_from_data(data, length)
}

func (lowerer *llvm_lowerer) clone_value_aggregate(value IRValue, target *SemanticType) IRValue {
	target = lowerer.specialize_type(target)
	if target == nil || value.value == "" || value.value == "null" {
		return IRValue{llvm_type: value.llvm_type, value: value.value, type_info: target, fresh: true}
	}
	if semantic_is_pointer(target) && target.kind != type_array && len(target.array_dimensions) == 0 {
		return IRValue{llvm_type: value.llvm_type, value: value.value, type_info: target, fresh: true}
	}
	if !is_value_aggregate(target) {
		value.type_info = target
		return value
	}

	// Value aggregates are pointer-backed. A default aggregate is represented by
	// a runtime null pointer, which may arrive in an SSA register rather than as
	// the literal LLVM operand "null". Guard before dereferencing/cloning it.
	result_slot := lowerer.next_register()
	lowerer.line("%s = alloca ptr", result_slot)
	is_null := lowerer.next_register()
	lowerer.line("%s = icmp eq ptr %s, null", is_null, value.value)
	null_label := lowerer.next_label("aggregate.clone.null")
	clone_label := lowerer.next_label("aggregate.clone.value")
	done_label := lowerer.next_label("aggregate.clone.done")
	lowerer.line("br i1 %s, label %%%s, label %%%s", is_null, null_label, clone_label)
	lowerer.raw_line(null_label + ":")
	lowerer.line("store ptr null, ptr %s", result_slot)
	lowerer.line("br label %%%s", done_label)
	lowerer.raw_line(clone_label + ":")
	cloned := value
	if target.kind == type_string {
		cloned = lowerer.clone_string_value(value)
	} else if target.kind == type_struct {
		cloned = lowerer.clone_struct_value(value, target)
	} else if target.kind == type_array || len(target.array_dimensions) > 0 {
		cloned = lowerer.clone_array_value(value, target)
	}
	lowerer.line("store ptr %s, ptr %s", cloned.value, result_slot)
	lowerer.line("br label %%%s", done_label)
	lowerer.raw_line(done_label + ":")
	result := lowerer.next_register()
	lowerer.line("%s = load ptr, ptr %s", result, result_slot)
	return IRValue{llvm_type: "ptr", value: result, type_info: target, fresh: true}
}

func (lowerer *llvm_lowerer) coerce(value IRValue, target *SemanticType) IRValue {
	if target == nil || is_unknown_type(target) {
		return value
	}
	target_ty := lowerer.llvm_type(target)
	if target.kind == type_string && value.llvm_type == "ptr" && value.type_info != nil && value.type_info.kind == type_string {
		if value.fresh {
			value.type_info = target
			value.fresh = false
			return value
		}
		return lowerer.clone_value_aggregate(value, target)
	}
	if target.kind == type_struct && target.pointer_depth == 0 && value.llvm_type == "ptr" && value.type_info != nil && value.type_info.kind == type_struct && value.type_info.pointer_depth == 0 {
		if value.fresh {
			value.type_info = target
			value.fresh = false
			return value
		}
		return lowerer.clone_value_aggregate(value, target)
	}
	if (target.kind == type_array || len(target.array_dimensions) > 0) && value.llvm_type == "ptr" && value.type_info != nil && (value.type_info.kind == type_array || len(value.type_info.array_dimensions) > 0) {
		if value.fresh {
			value.type_info = target
			value.fresh = false
			return value
		}
		return lowerer.clone_value_aggregate(value, target)
	}
	if value.llvm_type == target_ty {
		value.type_info = target
		return value
	}
	if target_ty == "void" {
		return IRValue{llvm_type: "void", type_info: target}
	}
	if value.llvm_type == "ptr" && target_ty == "ptr" {
		return IRValue{llvm_type: "ptr", value: value.value, type_info: target}
	}
	reg := lowerer.next_register()
	if strings.HasPrefix(value.llvm_type, "i") && strings.HasPrefix(target_ty, "i") {
		from_bits, _ := strconv.Atoi(strings.TrimPrefix(value.llvm_type, "i"))
		to_bits, _ := strconv.Atoi(strings.TrimPrefix(target_ty, "i"))
		if from_bits < to_bits {
			op := "sext"
			if is_unsigned_type(value.type_info) {
				op = "zext"
			}
			lowerer.line("%s = %s %s %s to %s", reg, op, value.llvm_type, value.value, target_ty)
		} else {
			lowerer.line("%s = trunc %s %s to %s", reg, value.llvm_type, value.value, target_ty)
		}
		return IRValue{llvm_type: target_ty, value: reg, type_info: target}
	}
	if value.llvm_type == "ptr" && strings.HasPrefix(target_ty, "i") {
		lowerer.line("%s = ptrtoint ptr %s to %s", reg, value.value, target_ty)
		return IRValue{llvm_type: target_ty, value: reg, type_info: target}
	}
	if strings.HasPrefix(value.llvm_type, "i") && target_ty == "ptr" {
		lowerer.line("%s = inttoptr %s %s to ptr", reg, value.llvm_type, value.value)
		return IRValue{llvm_type: "ptr", value: reg, type_info: target}
	}
	if strings.HasPrefix(value.llvm_type, "i") && (target_ty == "float" || target_ty == "double") {
		op := "sitofp"
		if is_unsigned_type(value.type_info) {
			op = "uitofp"
		}
		lowerer.line("%s = %s %s %s to %s", reg, op, value.llvm_type, value.value, target_ty)
		return IRValue{llvm_type: target_ty, value: reg, type_info: target}
	}
	if (value.llvm_type == "float" || value.llvm_type == "double") && strings.HasPrefix(target_ty, "i") {
		op := "fptosi"
		if is_unsigned_type(target) {
			op = "fptoui"
		}
		lowerer.line("%s = %s %s %s to %s", reg, op, value.llvm_type, value.value, target_ty)
		return IRValue{llvm_type: target_ty, value: reg, type_info: target}
	}
	return lowerer.zero_value(target)
}

func is_unsigned_type(type_info *SemanticType) bool {
	if type_info == nil {
		return false
	}
	return strings.HasPrefix(type_info.name, "uint") || type_info.name == "uintptr" || type_info.name == "char" || type_info.kind == type_bool || type_info.name == "bool"
}

func (lowerer *llvm_lowerer) to_bool(value IRValue) IRValue {
	if value.llvm_type == "i1" {
		return value
	}
	reg := lowerer.next_register()
	if value.llvm_type == "ptr" {
		lowerer.line("%s = icmp ne ptr %s, null", reg, value.value)
	} else if value.llvm_type == "float" || value.llvm_type == "double" {
		lowerer.line("%s = fcmp one %s %s, 0.0", reg, value.llvm_type, value.value)
	} else {
		lowerer.line("%s = icmp ne %s %s, 0", reg, value.llvm_type, value.value)
	}
	return IRValue{llvm_type: "i1", value: reg, type_info: &SemanticType{kind: type_bool, name: "bool"}}
}

func (lowerer *llvm_lowerer) address_of(expression ast.Expr) string {
	address, _ := lowerer.assignment_address(expression)
	if address == "" {
		return "null"
	}
	return address
}
func (lowerer *llvm_lowerer) assignment_address(expression ast.Expr) (string, *SemanticType) {
	address, semantic, _ := lowerer.assignment_address_with_owner(expression)
	return address, semantic
}

func (lowerer *llvm_lowerer) assignment_address_with_owner(expression ast.Expr) (string, *SemanticType, string) {
	switch item := expression.(type) {
	case *ast.IdentifierExpr:
		if local, ok := lowerer.locals[item.Name]; ok {
			return local.address, local.type_info, "null"
		}
		if global, ok := lowerer.globals[item.Name]; ok {
			return global.address, global.type_info, "null"
		}
		if lowerer.receiver != nil {
			if field := lowerer.receiver.fields[item.Name]; field != nil {
				owner := lowerer.locals["this"].address
				return lowerer.field_address(owner, lowerer.receiver, item.Name), field.type_info, owner
			}
		}
	case *ast.MemberExpr:
		object := lowerer.emit_expr(item.Object)
		object_sem := lowerer.specialize_type(lowerer.result.expression_types[item.Object])
		if object_sem == nil || object_sem.kind == type_unknown {
			object_sem = lowerer.specialize_type(lowerer.infer_expression_type(item.Object))
		}
		// Aggregate array elements and aggregate-valued fields are represented as
		// pointer slots.  For an lvalue chain such as values[i].field or
		// values[i].nested.field, emit_expr clones/loads the aggregate value; using
		// that temporary as the assignment base can mutate a copy and then destroy
		// it.  Resolve the underlying slot instead and load the aggregate pointer.
		switch item.Object.(type) {
		case *ast.IndexExpr, *ast.MemberExpr:
			if base_address, base_sem, _ := lowerer.assignment_address_with_owner(item.Object); base_address != "" && base_sem != nil && base_type_name(base_sem) != "" {
				if lowerer.struct_info[base_type_name(base_sem)] != nil {
					loaded := lowerer.next_register()
					lowerer.line("%s = load ptr, ptr %s", loaded, base_address)
					object = IRValue{llvm_type: "ptr", value: loaded, type_info: base_sem}
				}
			}
		}
		if object_sem != nil && (object_sem.pointer_depth > 0 || object_sem.kind == type_pointer) {
			non_null := lowerer.next_register()
			lowerer.line("%s = icmp ne ptr %s, null", non_null, object.value)
			lowerer.emit_checked_failure(non_null, "null_reference", "null pointer member access", "nullref")
		}
		if info := lowerer.struct_info[base_type_name(object_sem)]; info != nil {
			if field := info.fields[item.Name]; field != nil {
				return lowerer.field_address(object.value, info, item.Name), field.type_info, object.value
			}
		}
	case *ast.IndexExpr:
		object := lowerer.emit_expr(item.Object)
		index := lowerer.coerce(lowerer.emit_expr(item.Index), &SemanticType{kind: type_integer, name: "int64"})
		object_sem := lowerer.specialize_type(lowerer.result.expression_types[item.Object])
		if object_sem == nil || object_sem.kind == type_unknown {
			object_sem = lowerer.specialize_type(lowerer.infer_expression_type(item.Object))
		}
		lowerer.emit_index_bounds_check(item, object, index, object_sem)
		sem := lowerer.specialize_type(lowerer.result.expression_types[item])
		if object_sem != nil && (object_sem.pointer_depth > 0 || object_sem.kind == type_pointer || object_sem.kind == type_array || len(object_sem.array_dimensions) > 0) {
			sem = lowerer.array_element_type(object_sem)
		}
		ty := lowerer.llvm_type(sem)
		data := object.value
		if object_sem != nil && object_sem.kind == type_string && len(object_sem.array_dimensions) == 0 {
			data = lowerer.string_data_pointer(object.value)
		} else if object_sem != nil && (object_sem.pointer_depth > 0 || object_sem.kind == type_pointer) && object_sem.kind != type_array && len(object_sem.array_dimensions) == 0 {
			// Raw pointers already are element-storage addresses. A T*[] also has
			// pointer_depth > 0 because the element is a pointer, but the outer
			// value is still an array descriptor and must load its data pointer.
			data = object.value
		} else if object_sem == nil || object_sem.kind != type_string || len(object_sem.array_dimensions) != 0 {
			data = lowerer.array_data_pointer(object.value)
		}
		address := lowerer.next_register()
		lowerer.line("%s = getelementptr %s, ptr %s, i64 %s", address, ty, data, index.value)
		return address, sem, object.value
	case *ast.UnaryExpr:
		if item.Operator == "*" {
			pointer := lowerer.emit_expr(item.Operand)
			if pointer.llvm_type != "ptr" {
				return "", lowerer.result.expression_types[expression], "null"
			}
			non_null := lowerer.next_register()
			lowerer.line("%s = icmp ne ptr %s, null", non_null, pointer.value)
			lowerer.emit_checked_failure(non_null, "null_reference", "null pointer dereference", "nullref")
			return pointer.value, lowerer.specialize_type(lowerer.result.expression_types[item]), "null"
		}
	}
	return "", lowerer.result.expression_types[expression], "null"
}

func (lowerer *llvm_lowerer) emit_index_bounds_check(item *ast.IndexExpr, object IRValue, index IRValue, object_sem *SemanticType) {
	// Raw Clyth indexing is deliberately unchecked at runtime. Static semantic
	// analysis may still reject provably-invalid fixed-array accesses, but an
	// unresolved index lowers directly to pointer arithmetic/load/store. Safety
	// wrappers belong in user/runtime collection abstractions, not T[] itself.
}

func (lowerer *llvm_lowerer) array_data_pointer(header string) string {
	data_slot := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 0", data_slot, header)
	data := lowerer.next_register()
	lowerer.line("%s = load ptr, ptr %s", data, data_slot)
	return data
}

func (lowerer *llvm_lowerer) llvm_storage_size(llvm_type string) int {
	switch llvm_type {
	case "i1", "i8":
		return 1
	case "i16":
		return 2
	case "i32", "float":
		return 4
	case "i64", "double", "ptr":
		return 8
	default:
		return 8
	}
}

func align_up(value int, alignment int) int {
	if alignment <= 1 {
		return value
	}
	return (value + alignment - 1) / alignment * alignment
}

func (lowerer *llvm_lowerer) semantic_storage_size(type_info *SemanticType) int {
	return lowerer.llvm_storage_size(lowerer.llvm_type(type_info))
}

func fixed_array_count(type_info *SemanticType) (int, bool) {
	if type_info == nil || len(type_info.array_dimensions) != 1 || type_info.array_dimensions[0].dynamic {
		return 0, false
	}
	count, err := strconv.Atoi(type_info.array_dimensions[0].size)
	if err != nil || count < 0 {
		return 0, false
	}
	return count, true
}

// intrinsic_field_storage_size is the physical ABI size of a field in a
// compiler-owned intrinsic_struct. Fixed arrays of scalar values are embedded
// inline so definitions such as string.local_stack_cache: char[15] really are
// part of the containing object rather than pointers to separately allocated
// array descriptors. Ordinary Clyth struct fields retain the normal value ABI.
func (lowerer *llvm_lowerer) intrinsic_field_storage_size(info *StructInfo, type_info *SemanticType) int {
	if info != nil && info.intrinsic {
		if count, ok := fixed_array_count(type_info); ok {
			element := lowerer.array_element_type(type_info)
			if element != nil && element.pointer_depth == 0 && element.kind != type_struct && element.kind != type_array {
				return count * lowerer.semantic_storage_size(element)
			}
		}
	}
	return lowerer.semantic_storage_size(type_info)
}

func (lowerer *llvm_lowerer) semantic_alignment(type_info *SemanticType) int {
	size := lowerer.semantic_storage_size(type_info)
	if size > 8 {
		return 8
	}
	if size < 1 {
		return 1
	}
	return size
}

func (lowerer *llvm_lowerer) intrinsic_field_alignment(info *StructInfo, type_info *SemanticType) int {
	if info != nil && info.intrinsic {
		if _, ok := fixed_array_count(type_info); ok {
			element := lowerer.array_element_type(type_info)
			if element != nil && element.pointer_depth == 0 && element.kind != type_struct && element.kind != type_array {
				return lowerer.semantic_alignment(element)
			}
		}
	}
	return lowerer.semantic_alignment(type_info)
}

func (lowerer *llvm_lowerer) intrinsic_inline_fixed_array(info *StructInfo, type_info *SemanticType) (int, *SemanticType, bool) {
	if info == nil || !info.intrinsic {
		return 0, nil, false
	}
	count, ok := fixed_array_count(type_info)
	if !ok {
		return 0, nil, false
	}
	element := lowerer.array_element_type(type_info)
	if element == nil || element.pointer_depth > 0 || element.kind == type_struct || element.kind == type_array {
		return 0, nil, false
	}
	return count, element, true
}

func (lowerer *llvm_lowerer) emit_inline_array_header(data string, count int) string {
	header := lowerer.next_register()
	lowerer.line("%s = alloca [16 x i8]", header)
	data_slot := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 0", data_slot, header)
	lowerer.line("store ptr %s, ptr %s", data, data_slot)
	length_slot := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 8", length_slot, header)
	lowerer.line("store i64 %d, ptr %s", count, length_slot)
	return header
}

func (lowerer *llvm_lowerer) struct_field_offset(info *StructInfo, field_name string) int {
	if info == nil {
		return 0
	}
	offset := 0
	names := info.field_order
	if len(names) == 0 {
		names = make([]string, 0, len(info.fields))
		for name := range info.fields {
			names = append(names, name)
		}
		sort_strings(names)
	}
	for _, name := range names {
		field := info.fields[name]
		if field == nil {
			continue
		}
		alignment := lowerer.intrinsic_field_alignment(info, field.type_info)
		offset = align_up(offset, alignment)
		if name == field_name {
			return offset
		}
		offset += lowerer.intrinsic_field_storage_size(info, field.type_info)
	}
	return offset
}

func (lowerer *llvm_lowerer) struct_storage_size(info *StructInfo) int {
	if info == nil || len(info.fields) == 0 {
		return 1
	}
	offset, max_alignment := 0, 1
	names := info.field_order
	if len(names) == 0 {
		names = make([]string, 0, len(info.fields))
		for name := range info.fields {
			names = append(names, name)
		}
		sort_strings(names)
	}
	for _, name := range names {
		field := info.fields[name]
		if field == nil {
			continue
		}
		alignment := lowerer.intrinsic_field_alignment(info, field.type_info)
		if alignment > max_alignment {
			max_alignment = alignment
		}
		offset = align_up(offset, alignment)
		offset += lowerer.intrinsic_field_storage_size(info, field.type_info)
	}
	return align_up(offset, max_alignment)
}

func (lowerer *llvm_lowerer) initialize_struct_fields(object string, info *StructInfo) {
	if info == nil {
		return
	}
	for _, name := range info.field_order {
		field := info.fields[name]
		if field == nil {
			continue
		}
		field_type := lowerer.specialize_type(field.type_info)
		address := lowerer.field_address(object, info, name)
		if _, _, inline := lowerer.intrinsic_inline_fixed_array(info, field_type); inline {
			// calloc already zero-initialized the inline bytes. No descriptor or
			// backing allocation exists for an intrinsic inline fixed-array field.
			continue
		}
		if len(field_type.array_dimensions) > 0 || field_type.kind == type_array {
			count := 0
			if len(field_type.array_dimensions) > 0 && !field_type.array_dimensions[0].dynamic {
				count, _ = strconv.Atoi(field_type.array_dimensions[0].size)
			}
			element := lowerer.array_element_type(field_type)
			element_size := lowerer.semantic_storage_size(element)
			header := lowerer.next_register()
			lowerer.line("%s = call ptr @calloc(i64 1, i64 16)", header)
			if count > 0 {
				data := lowerer.next_register()
				lowerer.line("%s = call ptr @calloc(i64 %d, i64 %d)", data, count, element_size)
				data_slot := lowerer.next_register()
				lowerer.line("%s = getelementptr i8, ptr %s, i64 0", data_slot, header)
				lowerer.line("store ptr %s, ptr %s", data, data_slot)
			}
			length_slot := lowerer.next_register()
			lowerer.line("%s = getelementptr i8, ptr %s, i64 8", length_slot, header)
			lowerer.line("store i64 %d, ptr %s", count, length_slot)
			lowerer.line("store ptr %s, ptr %s", header, address)
		} else if field_type.kind == type_struct && field_type.pointer_depth == 0 {
			nested_info := lowerer.struct_info[base_type_name(field_type)]
			if nested_info != nil {
				nested := lowerer.next_register()
				lowerer.line("%s = call ptr @calloc(i64 1, i64 %d)", nested, lowerer.struct_storage_size(nested_info))
				lowerer.initialize_struct_fields(nested, nested_info)
				lowerer.line("store ptr %s, ptr %s", nested, address)
			}
		}
	}
}

func (lowerer *llvm_lowerer) field_address(object string, info *StructInfo, field_name string) string {
	byte_offset := lowerer.struct_field_offset(info, field_name)
	address := lowerer.next_register()
	lowerer.line("%s = getelementptr i8, ptr %s, i64 %d", address, object, byte_offset)
	return address
}
func (lowerer *llvm_lowerer) load_receiver_field(name string, sem *SemanticType) IRValue {
	address := lowerer.field_address(lowerer.locals["this"].address, lowerer.receiver, name)
	if count, _, inline := lowerer.intrinsic_inline_fixed_array(lowerer.receiver, sem); inline {
		header := lowerer.emit_inline_array_header(address, count)
		return IRValue{llvm_type: "ptr", value: header, type_info: sem}
	}
	ty := lowerer.llvm_type(sem)
	reg := lowerer.next_register()
	lowerer.line("%s = load %s, ptr %s", reg, ty, address)
	return IRValue{llvm_type: ty, value: reg, type_info: sem}
}
