package compiler

import (
	"encoding/json"
	"fmt"
	"sort"
	"strings"

	"github.com/mali5820k/clyth-bootstrap/internal/frontend/ast"
	"github.com/mali5820k/clyth-bootstrap/internal/frontend/token"
)

// TypeKind identifies the semantic category assigned to an expression or declaration.
// The semantic layer deliberately keeps "unknown" as a first-class type so one
// unresolved imported/native surface does not produce a cascade of unrelated errors.
type TypeKind string

const (
	type_unknown    TypeKind = "unknown"
	type_void       TypeKind = "void"
	type_bool       TypeKind = "bool"
	type_integer    TypeKind = "integer"
	type_float      TypeKind = "float"
	type_string     TypeKind = "string"
	type_char       TypeKind = "char"
	type_struct     TypeKind = "struct"
	type_protocol   TypeKind = "protocol"
	type_enum       TypeKind = "enum"
	type_array      TypeKind = "array"
	type_pointer    TypeKind = "pointer"
	type_callable   TypeKind = "callable"
	type_namespace  TypeKind = "namespace"
	type_error_type TypeKind = "error_type"
	type_null       TypeKind = "null"
)

// SemanticType is the canonical resolved representation of a Clyth type.
// Struct names remain Go-style exported type names, while implementation state
// uses snake_case identifiers as required by the bootstrap compiler style.
type SemanticType struct {
	kind             TypeKind
	name             string
	generic_args     []*SemanticType
	array_dimensions []SemanticArrayDimension
	pointer_depth    int
	is_const         bool
	callable         *SemanticCallable
	namespace_name   string
	untyped_literal  bool
}

// SemanticArrayDimension records one fixed or dynamic array dimension.
type SemanticArrayDimension struct {
	dynamic bool
	size    string
}

// SemanticCallable describes the resolved signature of a function or callable value.
type SemanticCallable struct {
	return_type            *SemanticType
	parameters             []*SemanticType
	default_values         []ast.Expr
	required_parameters    int
	variadic               bool
	heterogeneous_variadic bool
	native_variadic        bool
}

// SymbolKind identifies the declaration category stored in a scope or namespace.
type SymbolKind string

const (
	symbol_function   SymbolKind = "function"
	symbol_struct     SymbolKind = "struct"
	symbol_protocol   SymbolKind = "protocol"
	symbol_global     SymbolKind = "global"
	symbol_parameter  SymbolKind = "parameter"
	symbol_local      SymbolKind = "local"
	symbol_field      SymbolKind = "field"
	symbol_method     SymbolKind = "method"
	symbol_namespace  SymbolKind = "namespace"
	symbol_error_type SymbolKind = "error_type"
)

// Symbol is a semantically resolved declaration available for lookup.
type Symbol struct {
	name               string
	kind               SymbolKind
	type_info          *SemanticType
	declaration        ast.Node
	namespace_name     string
	scope_id           int
	generic_parameters []string
	receiver_name      string
	error_type_id      int
}

// Scope models one lexical scope. Parent links implement ordinary lexical lookup.
type Scope struct {
	id        int
	parent_id int
	kind      string
	symbols   map[string]*Symbol
}

// Namespace contains the declarations exported by a source file or runtime module.
type Namespace struct {
	name            string
	source_path     string
	import_kind     ast.ImportKind
	symbols         map[string]*Symbol
	structs         map[string]*StructInfo
	protocols       map[string]*ProtocolInfo
	open_imports    []*Namespace
	alias_imports   map[string]*Namespace
	analyzed_bodies bool
}

// StructInfo stores fields and methods after declaration hoisting. Generic members
// retain their template types and are substituted when a specialization is used.
type StructInfo struct {
	name               string
	declaration        *ast.StructDecl
	intrinsic          bool
	generic_parameters []string
	fields             map[string]*Symbol
	field_order        []string
	methods            map[string]*Symbol
	constructors       []*SemanticCallable
	destructor         bool
}

// ProtocolInfo stores protocol method requirements for later conformance analysis.
type ProtocolInfo struct {
	name               string
	declaration        *ast.ProtocolDecl
	generic_parameters []string
	methods            map[string]*Symbol
}

// GenericSpecialization records every concrete generic type/expression requested
// by source. LLVM lowering can consume this metadata to choose deterministic names.
type GenericSpecialization struct {
	base_name      string
	concrete_name  string
	type_arguments []*SemanticType
	source_span    token.Span
	source_path    string
}

// SemanticError is a source-located semantic diagnostic.
type SemanticError struct {
	source_path string
	source_span token.Span
	message     string
}

func (semantic_error SemanticError) Error() string {
	return fmt.Sprintf("%s:%d:%d: %s", semantic_error.source_path, semantic_error.source_span.Start.Line, semantic_error.source_span.Start.Column, semantic_error.message)
}

// DiagnosticLevel identifies a non-fatal compiler diagnostic emitted after
// successful syntax/semantic validation.
type DiagnosticLevel string

const (
	diagnostic_info    DiagnosticLevel = "info"
	diagnostic_warning DiagnosticLevel = "warning"
)

// CompilerDiagnostic is a source-located informational or warning diagnostic.
type CompilerDiagnostic struct {
	level       DiagnosticLevel
	code        string
	message     string
	source_path string
	source_span token.Span
}

// MeccPolicy is the effective compiler policy attached to one lexical region.
// MECC availability itself is a build option; scoped directives only tune the
// behavior of estates when MECC is enabled.
type MeccPolicy struct {
	compacting bool
}

// LifetimeActionKind identifies a compiler-selected lifetime operation.
type LifetimeActionKind string

const (
	lifetime_cleanup       LifetimeActionKind = "cleanup"
	lifetime_explicit_free LifetimeActionKind = "explicit_free"
	lifetime_mecc          LifetimeActionKind = "mecc"
)

// LifetimeAction records one allocation-level action selected before lowering.
type LifetimeAction struct {
	kind       LifetimeActionKind
	allocation int
	node       ast.Node
	reason     string
	policy     MeccPolicy
}

// AllocationInfo records provenance and the finalized lifetime disposition for
// one `new` allocation expression.
type AllocationInfo struct {
	id                 int
	new_expr           *ast.NewExpr
	source_path        string
	source_span        token.Span
	type_info          *SemanticType
	policy             MeccPolicy
	escaped            bool
	mecc               bool
	explicitly_freed   bool
	destruction_reason string
}

// IndexSafety records whether semantic analysis could prove an indexing
// operation safe. Invalid fixed-array accesses are rejected during semantic
// analysis and therefore never reach lowering; the remaining states let the
// backend elide checks only when a proof actually exists.
type IndexSafety string

const (
	index_safety_runtime IndexSafety = "runtime"
	index_safety_proven  IndexSafety = "proven_safe"
)

// AnalysisResult is the immutable output consumed by symbol emission and lowering.
type AnalysisResult struct {
	root_path              string
	root_namespace         *Namespace
	scopes                 []*Scope
	namespaces             map[string]*Namespace
	specializations        []GenericSpecialization
	semantic_errors        []SemanticError
	expression_types       map[ast.Expr]*SemanticType
	mecc_enabled           bool
	mecc_macros_present    bool
	mecc_policy_by_node    map[ast.Node]MeccPolicy
	allocations            map[int]*AllocationInfo
	allocation_by_new      map[*ast.NewExpr]int
	expression_allocations map[ast.Expr]int
	lifetime_actions       []LifetimeAction
	compiler_diagnostics   []CompilerDiagnostic
	error_type_ids         map[string]int
	index_safety           map[*ast.IndexExpr]IndexSafety
}

func clone_type(type_info *SemanticType) *SemanticType {
	if type_info == nil {
		return nil
	}
	cloned := *type_info
	cloned.generic_args = append([]*SemanticType(nil), type_info.generic_args...)
	cloned.array_dimensions = append([]SemanticArrayDimension(nil), type_info.array_dimensions...)
	if type_info.callable != nil {
		callable := *type_info.callable
		callable.parameters = append([]*SemanticType(nil), type_info.callable.parameters...)
		cloned.callable = &callable
	}
	return &cloned
}

func type_name(type_info *SemanticType) string {
	if type_info == nil {
		return "unknown"
	}
	if type_info.kind == type_namespace {
		return "namespace(" + type_info.namespace_name + ")"
	}
	if type_info.callable != nil {
		parameter_names := make([]string, 0, len(type_info.callable.parameters))
		for _, parameter := range type_info.callable.parameters {
			parameter_names = append(parameter_names, type_name(parameter))
		}
		if type_info.callable.variadic {
			parameter_names = append(parameter_names, "...")
		}
		return fmt.Sprintf("%s(%s)", type_name(type_info.callable.return_type), strings.Join(parameter_names, ", "))
	}
	name := type_info.name
	if name == "" {
		name = string(type_info.kind)
	}
	if len(type_info.generic_args) > 0 {
		arguments := make([]string, 0, len(type_info.generic_args))
		for _, argument := range type_info.generic_args {
			arguments = append(arguments, type_name(argument))
		}
		name += "<" + strings.Join(arguments, ", ") + ">"
	}
	for _, dimension := range type_info.array_dimensions {
		if dimension.dynamic {
			name += "[]"
		} else {
			name += "[" + dimension.size + "]"
		}
	}
	if type_info.pointer_depth > 0 {
		name += strings.Repeat("*", type_info.pointer_depth)
	}
	if type_info.is_const {
		name = "const " + name
	}
	return name
}

func specialization_is_concrete(specialization GenericSpecialization) bool {
	if len(specialization.type_arguments) == 0 {
		return false
	}
	for _, argument := range specialization.type_arguments {
		if argument == nil || argument.kind == type_unknown {
			return false
		}
	}
	return true
}

func specialization_name(base_name string, arguments []*SemanticType) string {
	argument_names := make([]string, 0, len(arguments))
	for _, argument := range arguments {
		cleaned := strings.NewReplacer(" ", "_", "<", "_", ">", "", ",", "_", "[", "_", "]", "", "*", "p", "(", "_", ")", "").Replace(type_name(argument))
		argument_names = append(argument_names, cleaned)
	}
	return base_name + "__" + strings.Join(argument_names, "__")
}

func is_integer_type(type_info *SemanticType) bool {
	if type_info == nil {
		return false
	}
	if type_info.kind == type_integer {
		return true
	}
	switch type_info.name {
	case "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64":
		return true
	}
	return false
}

func is_integer_like(type_info *SemanticType) bool {
	return is_integer_type(type_info) || (type_info != nil && (type_info.kind == type_char || type_info.name == "char"))
}

func is_float_type(type_info *SemanticType) bool {
	return type_info != nil && (type_info.kind == type_float || type_info.name == "float32" || type_info.name == "float64")
}

func is_numeric_type(type_info *SemanticType) bool {
	return is_integer_type(type_info) || is_float_type(type_info)
}

func is_unknown_type(type_info *SemanticType) bool {
	return type_info == nil || type_info.kind == type_unknown
}

func types_compatible(target *SemanticType, value *SemanticType) bool {
	if is_unknown_type(target) || is_unknown_type(value) {
		return true
	}
	if value.kind == type_null {
		return target.pointer_depth > 0 || target.kind == type_pointer || target.name == "string" || target.name == "owned"
	}
	if len(target.array_dimensions) == len(value.array_dimensions) && len(target.array_dimensions) > 0 && is_integer_like(target) && is_integer_like(value) {
		return true
	}
	if len(target.array_dimensions) == 0 && len(value.array_dimensions) == 0 && is_integer_like(target) && is_integer_like(value) {
		return true
	}
	if value.untyped_literal && is_float_type(value) && is_float_type(target) {
		return true
	}
	// Native C bindings commonly expose predicate results as int32. Clyth permits
	// those values to initialize/assign bools at the FFI boundary.
	if (target.kind == type_bool && is_integer_type(value)) || (value.kind == type_bool && is_integer_type(target)) {
		return true
	}
	if target.callable != nil || value.callable != nil {
		return callables_compatible(target.callable, value.callable)
	}
	if target.name != value.name || target.pointer_depth != value.pointer_depth || len(target.array_dimensions) != len(value.array_dimensions) {
		return false
	}
	if len(target.generic_args) != len(value.generic_args) {
		return false
	}
	for index := range target.generic_args {
		if !types_compatible(target.generic_args[index], value.generic_args[index]) {
			return false
		}
	}
	return true
}

func callables_compatible(target *SemanticCallable, value *SemanticCallable) bool {
	if target == nil || value == nil {
		return false
	}
	if !types_compatible(target.return_type, value.return_type) || target.variadic != value.variadic || len(target.parameters) != len(value.parameters) {
		return false
	}
	for index := range target.parameters {
		if !types_compatible(target.parameters[index], value.parameters[index]) {
			return false
		}
	}
	return true
}

func semantic_type_json(type_info *SemanticType) any {
	if type_info == nil {
		return nil
	}
	result := map[string]any{
		"kind":    type_info.kind,
		"name":    type_info.name,
		"display": type_name(type_info),
		"const":   type_info.is_const,
	}
	if len(type_info.generic_args) > 0 {
		arguments := make([]any, 0, len(type_info.generic_args))
		for _, argument := range type_info.generic_args {
			arguments = append(arguments, semantic_type_json(argument))
		}
		result["generic_args"] = arguments
	}
	if type_info.callable != nil {
		parameters := make([]any, 0, len(type_info.callable.parameters))
		for _, parameter := range type_info.callable.parameters {
			parameters = append(parameters, semantic_type_json(parameter))
		}
		result["callable"] = map[string]any{
			"return_type": semantic_type_json(type_info.callable.return_type),
			"parameters":  parameters,
			"variadic":    type_info.callable.variadic,
		}
	}
	if type_info.namespace_name != "" {
		result["namespace"] = type_info.namespace_name
	}
	return result
}

func symbols_json(result *AnalysisResult) ([]byte, error) {
	namespace_names := make([]string, 0, len(result.namespaces))
	for name := range result.namespaces {
		namespace_names = append(namespace_names, name)
	}
	sort.Strings(namespace_names)

	namespaces := make([]any, 0, len(namespace_names))
	for _, namespace_name := range namespace_names {
		namespace := result.namespaces[namespace_name]
		symbol_names := make([]string, 0, len(namespace.symbols))
		for name := range namespace.symbols {
			symbol_names = append(symbol_names, name)
		}
		sort.Strings(symbol_names)
		symbols := make([]any, 0, len(symbol_names))
		for _, name := range symbol_names {
			symbol := namespace.symbols[name]
			symbols = append(symbols, map[string]any{
				"name":               symbol.name,
				"kind":               symbol.kind,
				"type":               semantic_type_json(symbol.type_info),
				"generic_parameters": symbol.generic_parameters,
				"receiver":           symbol.receiver_name,
			})
		}
		aliases := make(map[string]string, len(namespace.alias_imports))
		for alias, imported_namespace := range namespace.alias_imports {
			aliases[alias] = imported_namespace.name
		}
		open_imports := make([]string, 0, len(namespace.open_imports))
		for _, imported_namespace := range namespace.open_imports {
			open_imports = append(open_imports, imported_namespace.name)
		}
		namespaces = append(namespaces, map[string]any{
			"name":         namespace.name,
			"source_path":  namespace.source_path,
			"import_kind":  namespace.import_kind,
			"symbols":      symbols,
			"aliases":      aliases,
			"open_imports": open_imports,
		})
	}

	specializations := make([]any, 0, len(result.specializations))
	for _, specialization := range result.specializations {
		arguments := make([]string, 0, len(specialization.type_arguments))
		for _, argument := range specialization.type_arguments {
			arguments = append(arguments, type_name(argument))
		}
		specializations = append(specializations, map[string]any{
			"base_name":      specialization.base_name,
			"concrete_name":  specialization.concrete_name,
			"type_arguments": arguments,
			"source_path":    specialization.source_path,
			"span":           specialization.source_span,
		})
	}

	allocation_json := make([]any, 0, len(result.allocations))
	allocation_ids := make([]int, 0, len(result.allocations))
	for id := range result.allocations {
		allocation_ids = append(allocation_ids, id)
	}
	sort.Ints(allocation_ids)
	for _, id := range allocation_ids {
		allocation := result.allocations[id]
		allocation_json = append(allocation_json, map[string]any{
			"id": allocation.id, "type": type_name(allocation.type_info),
			"source_path": allocation.source_path, "span": allocation.source_span,
			"escaped": allocation.escaped, "mecc": allocation.mecc,
			"compacting":         allocation.policy.compacting,
			"explicitly_freed":   allocation.explicitly_freed,
			"destruction_reason": allocation.destruction_reason,
		})
	}
	payload := map[string]any{
		"root_path":           result.root_path,
		"root_namespace":      result.root_namespace.name,
		"namespaces":          namespaces,
		"specializations":     specializations,
		"scope_count":         len(result.scopes),
		"mecc_enabled":        result.mecc_enabled,
		"mecc_macros_present": result.mecc_macros_present,
		"allocations":         allocation_json,
	}
	return json.MarshalIndent(payload, "", "  ")
}
