package compiler

import (
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/mali5820k/clyth-bootstrap/internal/frontend/parser"
)

// analyze_test_source parses and semantically analyzes one temporary Clyth file.
func analyze_test_source(t *testing.T, source string, extra_files map[string]string) (*AnalysisResult, []SemanticError) {
	t.Helper()
	directory := t.TempDir()
	root_path := filepath.Join(directory, "main.clyth")
	if err := os.WriteFile(root_path, []byte(source), 0o644); err != nil {
		t.Fatal(err)
	}
	for name, content := range extra_files {
		path := filepath.Join(directory, name)
		if err := os.MkdirAll(filepath.Dir(path), 0o755); err != nil {
			t.Fatal(err)
		}
		if err := os.WriteFile(path, []byte(content), 0o644); err != nil {
			t.Fatal(err)
		}
	}
	program, parse_errors, lex_errors := parser.Parse(source)
	if len(parse_errors) != 0 || len(lex_errors) != 0 {
		t.Fatalf("unexpected frontend errors: parse=%v lex=%v", parse_errors, lex_errors)
	}
	result, semantic_errors, err := analyze_file(root_path, program)
	if err != nil {
		t.Fatal(err)
	}
	return result, semantic_errors
}

func Test_semantic_hoisting_and_shadowing(t *testing.T) {
	source := `
int32 main() {
    int32 result = add(global_value, 2)
    { int32 result = 7 }
    return result - 42
}
int32 add(int32 a, int32 b) { return a + b }
int32 global_value = 40
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
}

func Test_semantic_import_alias_namespace(t *testing.T) {
	source := `
import "support/helper.clyth" as helper
int32 main() { return helper.answer() - 42 }
`
	helper := `int32 answer() { return 42 }`
	_, semantic_errors := analyze_test_source(t, source, map[string]string{"support/helper.clyth": helper})
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
}

func Test_semantic_generic_specialization_metadata(t *testing.T) {
	source := `
struct Box<T> { T value }
int32 main() { Box<int32> value; value.value = 42; return 0 }
`
	result, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
	found := false
	for _, specialization := range result.specializations {
		if specialization.base_name == "Box" && strings.Contains(specialization.concrete_name, "Box__int32") {
			found = true
		}
	}
	if !found {
		t.Fatalf("expected Box<int32> specialization metadata, got %#v", result.specializations)
	}
}

func Test_semantic_rejects_callable_mismatch(t *testing.T) {
	source := `
int32 apply(int32(int32) callback) { return callback(1) }
void wrong(int32 value) {}
int32 main() { return apply(wrong) }
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) == 0 {
		t.Fatal("expected callable mismatch")
	}
}

func Test_semantic_accepts_const_pointer_parameter(t *testing.T) {
	source := `
void inspect(const int32 *value) {}
int32 main() { int32 value = 1; inspect(&value); return 0 }
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
}

func Test_semantic_rejects_duplicate_local(t *testing.T) {
	source := `
int32 main() { int32 value = 1; int32 value = 2; return value }
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) == 0 {
		t.Fatal("expected duplicate local error")
	}
}

func Test_semantic_rejects_type_mismatch(t *testing.T) {
	source := `
int32 main() { string value = 42; return 0 }
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) == 0 {
		t.Fatal("expected type mismatch")
	}
}

func Test_semantic_lambda_v1_inference_forms(t *testing.T) {
	source := `
extern C void print(int32 value)
int32 main() {
    auto concise = void(int32 value) => { print(value) }
    void(int32) explicit = (int32 value) => { print(value) }
    void(int32) contextual = (value) => { print(value) }
    concise(1)
    explicit(2)
    contextual(3)
    return 0
}
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
}

func Test_semantic_rejects_untyped_lambda_without_context(t *testing.T) {
	source := `
int32 main() {
    auto invalid = (value) => { return value }
    return 0
}
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) == 0 {
		t.Fatal("expected missing contextual lambda parameter type error")
	}
}

func Test_semantic_private_members_visible_inside_declaring_struct(t *testing.T) {
	source := `
struct Counter {
    private int32 value
    private void increment() { value += 1 }
    int32 next() { increment(); return value }
}
int32 main() { Counter counter = Counter(); return counter.next() - 1 }
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
}

func Test_semantic_rejects_private_member_access_outside_declaring_struct(t *testing.T) {
	source := `
struct Counter {
    private int32 value
    private void increment() { value += 1 }
}
int32 main() {
    Counter counter = Counter()
    counter.increment()
    return counter.value
}
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) < 2 {
		t.Fatalf("expected private method and field access errors, got %v", semantic_errors)
	}
}

func Test_generic_substitution_preserves_array_modifiers(t *testing.T) {
	input := &SemanticType{kind: type_array, name: "T", array_dimensions: []SemanticArrayDimension{{dynamic: true}}}
	result := substitute_type(input, map[string]*SemanticType{"T": {kind: type_integer, name: "int32"}})
	if result.kind != type_array || result.name != "int32" || len(result.array_dimensions) != 1 {
		t.Fatalf("expected substituted int32[] type, got %#v", result)
	}
}

func analyze_test_source_with_options(t *testing.T, source string, options SemanticOptions) (*AnalysisResult, []SemanticError) {
	t.Helper()
	directory := t.TempDir()
	root_path := filepath.Join(directory, "main.clyth")
	if err := os.WriteFile(root_path, []byte(source), 0o644); err != nil {
		t.Fatal(err)
	}
	program, parse_errors, lex_errors := parser.Parse(source)
	if len(parse_errors) != 0 || len(lex_errors) != 0 {
		t.Fatalf("unexpected frontend errors: parse=%v lex=%v", parse_errors, lex_errors)
	}
	result, semantic_errors, err := analyze_file_with_options(root_path, program, options)
	if err != nil {
		t.Fatal(err)
	}
	return result, semantic_errors
}

func Test_mecc_directive_is_inert_and_reported_when_disabled(t *testing.T) {
	source := `
#MECC_config.compacting = true
int32 main() { return 0 }
`
	result, semantic_errors := analyze_test_source_with_options(t, source, SemanticOptions{mecc_enabled: false})
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
	count := 0
	for _, diagnostic := range result.compiler_diagnostics {
		if diagnostic.code == "MECC_CONFIG_IGNORED" {
			count++
		}
	}
	if count != 1 {
		t.Fatalf("expected one project-level ignored-MECC info, got %d", count)
	}
}

func Test_local_aliases_do_not_require_mecc(t *testing.T) {
	source := `
struct Thing {}
struct Holder { Thing *child }
int32 main() {
    Holder left
    Holder right
    Thing *item = new Thing()
    Thing *alias = item
    left.child = item
    right.child = alias
    return 0
}
`
	result, semantic_errors := analyze_test_source_with_options(t, source, SemanticOptions{mecc_enabled: false})
	if len(semantic_errors) != 0 {
		t.Fatalf("local aliases should remain statically cleanable: %v", semantic_errors)
	}
	for _, allocation := range result.allocations {
		if allocation != nil && allocation.mecc {
			t.Fatal("non-escaping local aliases must not select MECC")
		}
	}
}

func Test_cross_domain_escape_requires_or_selects_mecc(t *testing.T) {
	source := `
struct Thing {}
struct Holder { Thing *child }
void distribute(Holder *left, Holder *right) {
    Thing *item = new Thing()
    Thing *alias = item
    left.child = item
    right.child = alias
}
int32 main() { return 0 }
`
	_, static_errors := analyze_test_source_with_options(t, source, SemanticOptions{mecc_enabled: false})
	if len(static_errors) == 0 {
		t.Fatal("expected static lifetime error for simultaneous caller domains")
	}
	result, managed_errors := analyze_test_source_with_options(t, source, SemanticOptions{mecc_enabled: true})
	if len(managed_errors) != 0 {
		t.Fatalf("MECC build should accept unresolved cross-domain lifetime: %v", managed_errors)
	}
	promoted := false
	for _, allocation := range result.allocations {
		if allocation != nil && allocation.mecc {
			promoted = true
		}
	}
	if !promoted {
		t.Fatal("expected MECC promotion metadata")
	}
}

func Test_explicit_free_rejects_double_free_and_use_after_free(t *testing.T) {
	double_free := `
struct Thing {}
int32 main() {
    Thing *item = new Thing()
    free(item)
    free(item)
    return 0
}
`
	_, errors := analyze_test_source(t, double_free, nil)
	if len(errors) == 0 {
		t.Fatal("expected double-free diagnostic")
	}

	use_after_free := `
struct Thing { int32 value }
int32 main() {
    Thing *item = new Thing()
    Thing *alias = item
    free(item)
    return alias.value
}
`
	_, errors = analyze_test_source(t, use_after_free, nil)
	if len(errors) == 0 {
		t.Fatal("expected use-after-free diagnostic through alias")
	}
}

func Test_freeing_container_reference_is_warning_not_error(t *testing.T) {
	source := `
struct Thing {}
struct Box { Thing *child }
int32 main() {
    Box box
    Thing *item = new Thing()
    box.child = item
    free(item)
    return 0
}
`
	result, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("freeing a mutable referenced allocation is legal: %v", semantic_errors)
	}
	found := false
	for _, diagnostic := range result.compiler_diagnostics {
		if diagnostic.code == "LIFETIME_STALE_CONTAINER_REF" {
			found = true
		}
	}
	if !found {
		t.Fatal("expected stale-container-reference warning")
	}
}

func TestCatchAndCustomErrorTypeSemanticSurface(t *testing.T) {
	source := `
#ErrorType device_timeout
int32 main() {
    Error result = catch(() => {
        raise(device_timeout, "Device timed out")
    })
    if (result.error && result.error_type == device_timeout) { return 7 }
    return 0
}
`
	result, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected catch/ErrorType semantic errors: %v", semantic_errors)
	}
	if result.error_type_ids["root::device_timeout"] == 0 {
		t.Fatalf("custom ErrorType identity was not registered: %#v", result.error_type_ids)
	}
}

func Test_semantic_defaults_and_variadic_conversion_contracts(t *testing.T) {
	source := `
struct Point {
    int32 x
    string to_string() { return "point" }
}
void connect(string host, int32 port = 443, bool tls = true) {}
void print_all(string... values) {}
void inspect(auto... values) {}
int32 main() {
    Point point
    connect("example.com")
    connect("example.com", 8080)
    print_all("position", 42, point)
    inspect(42, "hello", point)
    return 0
}
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected default/variadic semantic errors: %v", semantic_errors)
	}
}

func Test_semantic_static_bounds_for_known_loop_range(t *testing.T) {
	source := `
int32 main() {
    int32[4] values = [1, 2, 3, 4]
    for int32 i = 0; i < 10; i++ {
        int32 x = values[i]
    }
    return 0
}
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) == 0 {
		t.Fatal("expected static loop bounds error")
	}
	found := false
	for _, semantic_error := range semantic_errors {
		if strings.Contains(semantic_error.message, "max-iterator invocation: 9") {
			found = true
			break
		}
	}
	if !found {
		t.Fatalf("expected iterator-range diagnostic, got %v", semantic_errors)
	}
}

func Test_semantic_static_bounds_for_known_while_range(t *testing.T) {
	source := `
int32 main() {
    int32[4] values = [1, 2, 3, 4]
    int32 i = 0
    while (i < 10) {
        int32 x = values[i]
        i++
    }
    return 0
}
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) == 0 {
		t.Fatal("expected static while-loop bounds error")
	}
	found := false
	for _, semantic_error := range semantic_errors {
		if strings.Contains(semantic_error.message, "max-iterator invocation: 9") {
			found = true
			break
		}
	}
	if !found {
		t.Fatalf("expected while iterator-range diagnostic, got %v", semantic_errors)
	}
}

func Test_semantic_diagnoses_use_after_free_through_known_aggregate_field(t *testing.T) {
	source := `
struct Thing { int32 value }
struct Box { Thing *child }
int32 main() {
    Box box
    Thing *item = new Thing()
    box.child = item
    free(item)
    return box.child.value
}
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) == 0 {
		t.Fatal("expected use-after-free diagnostic through known aggregate field")
	}
	found := false
	for _, semantic_error := range semantic_errors {
		if strings.Contains(semantic_error.message, "provable use after free through 'box.child'") {
			found = true
			break
		}
	}
	if !found {
		t.Fatalf("expected aggregate-field UAF diagnostic, got %v", semantic_errors)
	}
}

func Test_semantic_clearing_aggregate_field_removes_known_stale_path(t *testing.T) {
	source := `
struct Thing { int32 value }
struct Box { Thing *child }
int32 main() {
    Box box
    Thing *item = new Thing()
    box.child = item
    free(item)
    box.child = null
    return 0
}
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("clearing a stale aggregate field without reading it should be legal: %v", semantic_errors)
	}
}

func Test_lifetime_field_clear_releases_final_known_reference(t *testing.T) {
	source := `
struct Thing {}
struct Box { Thing *child }
int32 main() {
    Box box
    Thing *item = new Thing()
    box.child = item
    item = null
    box.child = null
    return 0
}
`
	result, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected lifetime errors: %v", semantic_errors)
	}
	found := false
	for _, action := range result.lifetime_actions {
		if action.kind == lifetime_cleanup && strings.Contains(action.reason, "no remaining live alias") {
			found = true
			break
		}
	}
	if !found {
		t.Fatalf("expected cleanup when the final known aggregate field reference is cleared: %#v", result.lifetime_actions)
	}
}

func Test_semantic_default_parameter_does_not_skip_into_variadic_position(t *testing.T) {
	source := `
void log(bool timestamp = true, string... messages) {}
int32 main() {
    log("message")
    return 0
}
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) == 0 {
		t.Fatal("expected strict positional type error for default-before-variadic call")
	}
	found := false
	for _, semantic_error := range semantic_errors {
		if strings.Contains(semantic_error.message, "argument") || strings.Contains(semantic_error.message, "bool") {
			found = true
			break
		}
	}
	if !found {
		t.Fatalf("expected positional argument mismatch, got %v", semantic_errors)
	}
}

func Test_semantic_rejects_non_lvalue_increment_and_assignment(t *testing.T) {
	for _, source := range []string{
		`int32 main() { 1++; return 0 }`,
		`int32 main() { 1 = 2; return 0 }`,
		`int32 main() { string text = "x"; text++; return 0 }`,
	} {
		_, semantic_errors := analyze_test_source(t, source, nil)
		if len(semantic_errors) == 0 {
			t.Fatalf("expected invalid mutation target/type to be rejected: %s", source)
		}
	}
}

func Test_semantic_rejects_const_mutation(t *testing.T) {
	source := `
int32 main() {
    const int32 value = 1
    value = 2
    return value
}
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) == 0 {
		t.Fatal("expected mutation of const local to be rejected")
	}
	found := false
	for _, semantic_error := range semantic_errors {
		if strings.Contains(semantic_error.message, "const") {
			found = true
			break
		}
	}
	if !found {
		t.Fatalf("expected const diagnostic, got %v", semantic_errors)
	}
}

func Test_indexed_pointer_store_preserves_allocation_lifetime(t *testing.T) {
	source := `
struct Node { Node*[4] children }
void install(Node *node) {
    node.children[0] = new Node()
}
int32 main() { return 0 }
`
	result, semantic_errors := analyze_test_source_with_options(t, source, SemanticOptions{mecc_enabled: false})
	if len(semantic_errors) != 0 {
		t.Fatalf("indexed pointer store should be accepted: %v", semantic_errors)
	}
	found := false
	for _, allocation := range result.allocations {
		if allocation != nil && allocation.escaped {
			found = true
		}
	}
	if !found {
		t.Fatal("allocation stored through caller-owned indexed aggregate must escape local cleanup")
	}
}

func Test_array_exposes_only_length_member(t *testing.T) {
	source := `
int32 main() {
    int32[] values = [1, 2, 3]
    int64 n = values.length
    values.resize(4)
    return n - 3
}
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) == 0 {
		t.Fatal("expected raw array resize to be rejected")
	}
	found := false
	for _, semantic_error := range semantic_errors {
		if strings.Contains(semantic_error.message, "the only built-in array member is 'length'") {
			found = true
			break
		}
	}
	if !found {
		t.Fatalf("expected array-member contract diagnostic, got %v", semantic_errors)
	}
}

func Test_semantic_protocol_infers_index_and_value_types_from_methods(t *testing.T) {
	source := `
protocol Indexable {
    auto on_read_index(auto index)
    void on_write_index(auto index, auto value)
}
struct Pair : Indexable {
    int32 left
    int32 right
    int32 on_read_index(uint64 index) { if index == 0 { return left }; return right }
    void on_write_index(uint64 index, int32 value) { if index == 0 { left = value; return }; right = value }
}
int32 main() {
    Pair pair
    pair[0] = 7
    return pair[0] - 7
}
`
	_, semanticErrors := analyze_test_source(t, source, nil)
	if len(semanticErrors) != 0 {
		t.Fatalf("Indexable must infer its concrete index/value types from implementation methods: %v", semanticErrors)
	}
}

func Test_semantic_indexable_requires_coherent_read_write_signatures(t *testing.T) {
	source := `
protocol Indexable {
    auto on_read_index(auto index)
    void on_write_index(auto index, auto value)
}
struct Broken : Indexable {
    int32 on_read_index(uint64 index) { return 0 }
    void on_write_index(int32 index, int32 value) {}
}
int32 main() { return 0 }
`
	_, semanticErrors := analyze_test_source(t, source, nil)
	if len(semanticErrors) == 0 {
		t.Fatal("expected incompatible Indexable method signatures to fail conformance")
	}
	found := false
	for _, semanticError := range semanticErrors {
		if strings.Contains(semanticError.message, "Indexable read/write index parameter types must match") {
			found = true
			break
		}
	}
	if !found {
		t.Fatalf("expected Indexable signature-coherence diagnostic, got %v", semanticErrors)
	}
}

func Test_semantic_intrinsic_struct_is_reserved_to_clyth_core(t *testing.T) {
	source := `
intrinsic_struct PretendPrimitive { int32 value }
int32 main() { return 0 }
`
	_, semanticErrors := analyze_test_source(t, source, nil)
	if len(semanticErrors) == 0 {
		t.Fatal("user source must not be able to define intrinsic_struct")
	}
	found := false
	for _, semanticError := range semanticErrors {
		if strings.Contains(semanticError.message, "reserved for the protected Clyth Core module") {
			found = true
			break
		}
	}
	if !found {
		t.Fatalf("expected protected-core diagnostic, got %v", semanticErrors)
	}
}

func Test_raw_array_constructor_requires_integer_length(t *testing.T) {
	source := `
int32 main() {
    int32[] values = array<int32>("four")
    return 0
}
`
	_, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) == 0 {
		t.Fatal("expected non-integer raw-array constructor length to be rejected")
	}
	found := false
	for _, semantic_error := range semantic_errors {
		if strings.Contains(semantic_error.message, "requires an integer length") {
			found = true
			break
		}
	}
	if !found {
		t.Fatalf("expected array-constructor length diagnostic, got %v", semantic_errors)
	}
}

func Test_raw_array_copy_constructor_accepts_matching_source(t *testing.T) {
	source := `
int32 main() {
    int32[] values = array<int32>(2)
    int32[] grown = array<int32>(values, 4)
    return grown.length - 4
}
`
	_, semanticErrors := analyze_test_source(t, source, nil)
	if len(semanticErrors) != 0 {
		t.Fatalf("expected matching raw-array copy construction to pass, got %v", semanticErrors)
	}
}
