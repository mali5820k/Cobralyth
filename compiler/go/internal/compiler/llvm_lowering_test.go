package compiler

import (
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/mali5820k/clyth-bootstrap/internal/frontend/ast"
	"github.com/mali5820k/clyth-bootstrap/internal/frontend/parser"
)

func lower_test_source(t *testing.T, source string) ([]byte, *AnalysisResult) {
	t.Helper()
	directory := t.TempDir()
	path := filepath.Join(directory, "main.clyth")
	if err := os.WriteFile(path, []byte(source), 0o644); err != nil {
		t.Fatal(err)
	}
	program, parse_errors, lex_errors := parser.Parse(source)
	if len(parse_errors) != 0 || len(lex_errors) != 0 {
		t.Fatalf("unexpected frontend errors: parse=%v lex=%v", parse_errors, lex_errors)
	}
	result, semantic_errors, err := analyze_file(path, program)
	if err != nil {
		t.Fatal(err)
	}
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
	ir, err := lower_llvm_ir(program, result, "x86_64-unknown-linux-musl")
	if err != nil {
		t.Fatal(err)
	}
	return ir, result
}

func Test_new_expression_resolves_to_raw_pointer(t *testing.T) {
	source := `
struct Thing { int32 value }
int32 main() {
    Thing *thing = new Thing()
    return 0
}
`
	result, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
	found := false
	for expression, type_info := range result.expression_types {
		if _, ok := expression.(*ast.NewExpr); ok {
			found = true
			if type_info == nil || type_info.pointer_depth == 0 {
				t.Fatalf("new expression should resolve to a raw pointer, got %#v", type_info)
			}
		}
	}
	if !found {
		t.Fatal("expected a new expression in semantic result")
	}
}

func Test_llvm_lowering_emits_native_main_and_control_flow(t *testing.T) {
	source := `
extern C int32 printf(string format, ...)
int32 main(string[] args) {
    int32 value = 0
    while value < 2 { value += 1 }
    printf("value=%d\\n", value)
    return value - 2
}

`
	result, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
	program, parse_errors, lex_errors := parser.Parse(source)
	if len(parse_errors) != 0 || len(lex_errors) != 0 {
		t.Fatalf("unexpected frontend errors: parse=%v lex=%v", parse_errors, lex_errors)
	}
	ir, err := lower_llvm_ir(program, result, "x86_64-unknown-linux-musl")
	if err != nil {
		t.Fatal(err)
	}
	text := string(ir)
	for _, expected := range []string{"target triple = \"x86_64-unknown-linux-musl\"", "define i32 @main(i32 %clyth.argc, ptr %clyth.argv)", "declare i32 @printf(ptr, ...)", "while.cond"} {
		if !strings.Contains(text, expected) {
			t.Fatalf("expected LLVM IR to contain %q\n%s", expected, text)
		}
	}
}

func Test_llvm_lowering_emits_runtime_global_initializer(t *testing.T) {
	source := `
int32 make_value() { return 42 }
int32 global_value = make_value()
int32 main() { return global_value - 42 }
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	for _, expected := range []string{
		"@global_value = global i32 0",
		"@llvm.global_ctors = appending global",
		"define internal void @__clyth_init_globals()",
		"call i32 @make_value()",
		"store i32",
	} {
		if !strings.Contains(text, expected) {
			t.Fatalf("expected runtime global initializer IR to contain %q\n%s", expected, text)
		}
	}
}

func Test_llvm_lowering_raw_array_index_in_catch_remains_unchecked(t *testing.T) {
	source := `
#ErrorType device_timeout
extern C int32 get_index()
int32 main() {
    int32[2] values = [10, 20]
    Error result = catch(() => {
        int32 value = values[get_index()]
    })
    if (result.error && result.error_type == bounds_error) { return 9 }
    return 0
}
`
	result, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
	program, parse_errors, lex_errors := parser.Parse(source)
	if len(parse_errors) != 0 || len(lex_errors) != 0 {
		t.Fatalf("unexpected frontend errors: parse=%v lex=%v", parse_errors, lex_errors)
	}
	ir, err := lower_llvm_ir(program, result, "x86_64-unknown-linux-musl")
	if err != nil {
		t.Fatal(err)
	}
	text := string(ir)
	if !strings.Contains(text, "catch.end") {
		t.Fatalf("expected catch body lowering to remain present\n%s", text)
	}
	for _, forbidden := range []string{"bounds.fail", "array or string index out of bounds", "icmp ult i64"} {
		if strings.Contains(text, forbidden) {
			t.Fatalf("raw array indexing must remain unchecked even inside catch; found %q\n%s", forbidden, text)
		}
	}
}

func Test_llvm_lowering_defaults_and_typed_variadic_stack_pack(t *testing.T) {
	source := `
int32 pick(int32 base = 40, int32... values) { return base }
int32 main() {
    int32 one = pick()
    int32 two = pick(41, 1, 2, 3)
    return one + two - 81
}
`
	result, semantic_errors := analyze_test_source(t, source, nil)
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
	program, parse_errors, lex_errors := parser.Parse(source)
	if len(parse_errors) != 0 || len(lex_errors) != 0 {
		t.Fatalf("unexpected frontend errors: parse=%v lex=%v", parse_errors, lex_errors)
	}
	ir, err := lower_llvm_ir(program, result, "x86_64-unknown-linux-musl")
	if err != nil {
		t.Fatal(err)
	}
	text := string(ir)
	for _, expected := range []string{
		"define i32 @pick(i32 %arg.0, ptr %arg.1)",
		"alloca [16 x i8]",
		"store i64 3",
		"call i32 @pick(i32 %v",
	} {
		if !strings.Contains(text, expected) {
			t.Fatalf("expected typed variadic/default lowering to contain %q\n%s", expected, text)
		}
	}
}

func Test_llvm_lowering_elides_proven_fixed_array_bounds_guard(t *testing.T) {
	source := `
int32 main() {
    int32[2] values = [10, 20]
    int32 limit = 2
    int32 total = 0
    for (int32 i = 0; i < limit; i++) { total += values[i] }
    return total - 30
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	if strings.Contains(text, "bounds.fail") {
		t.Fatalf("proven-safe fixed-array loop should not emit a bounds guard\n%s", text)
	}
}

func Test_llvm_lowering_runtime_raw_array_index_has_no_bounds_guard(t *testing.T) {
	source := `
extern C int32 get_index()
int32 main() {
    int32[2] values = [10, 20]
    return values[get_index()]
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	for _, forbidden := range []string{"bounds.fail", "icmp ult i64", "array or string index out of bounds"} {
		if strings.Contains(text, forbidden) {
			t.Fatalf("raw array runtime indexing must not emit an implicit bounds guard; found %q\n%s", forbidden, text)
		}
	}
	if !strings.Contains(text, "getelementptr i32") {
		t.Fatalf("raw array indexing should lower directly to element address arithmetic\n%s", text)
	}
}

func Test_llvm_lowering_catch_scope_cleanup_is_independent_of_raw_array_indexing(t *testing.T) {
	source := `
struct Thing {}
extern C int32 get_index()
int32 main() {
    int32[2] values = [10, 20]
    Error result = catch(() => {
        Thing *item = new Thing()
        int32 value = values[get_index()]
    })
    if (result.error) { return 0 }
    return 1
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	if strings.Contains(text, "bounds.fail") {
		t.Fatalf("raw array indexing inside catch must not invent a recoverable bounds failure\n%s", text)
	}
	free := strings.Index(text, "call void @free(ptr")
	end := strings.Index(text, "br label %catch.end")
	if free < 0 || end < 0 || free > end {
		t.Fatalf("ordinary scope cleanup must still happen before leaving the catch body\n%s", text)
	}
}

func Test_llvm_lowering_null_dereference_is_fatal_outside_catch(t *testing.T) {
	source := `
int32 main() {
    int32 *value = null
    return *value
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	for _, expected := range []string{"nullref.fail", "call void @abort()", "unreachable"} {
		if !strings.Contains(text, expected) {
			t.Fatalf("null dereference outside catch should contain %q\n%s", expected, text)
		}
	}
}

func Test_llvm_lowering_mecc_emits_graph_roots_edges_and_collapse(t *testing.T) {
	source := `
struct Thing { int32 value }
struct Holder { Thing *child }
void distribute(Holder *left, Holder *right) {
    Thing *item = new Thing()
    Thing *alias = item
    left.child = item
    right.child = alias
}
int32 main() { return 0 }
`
	directory := t.TempDir()
	path := filepath.Join(directory, "main.clyth")
	if err := os.WriteFile(path, []byte(source), 0o644); err != nil {
		t.Fatal(err)
	}
	program, parse_errors, lex_errors := parser.Parse(source)
	if len(parse_errors) != 0 || len(lex_errors) != 0 {
		t.Fatalf("unexpected frontend errors: parse=%v lex=%v", parse_errors, lex_errors)
	}
	result, semantic_errors, err := analyze_file_with_options(path, program, SemanticOptions{mecc_enabled: true})
	if err != nil {
		t.Fatal(err)
	}
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
	ir, err := lower_llvm_ir(program, result, "x86_64-unknown-linux-musl")
	if err != nil {
		t.Fatal(err)
	}
	text := string(ir)
	for _, expected := range []string{
		"declare ptr @clyth_mecc_adopt(ptr, i1, ptr)",
		"declare void @clyth_mecc_store(ptr, ptr, ptr)",
		"declare void @clyth_mecc_collapse()",
		"call ptr @clyth_mecc_adopt(",
		"call void @clyth_mecc_store(",
		"call void @clyth_mecc_collapse()",
	} {
		if !strings.Contains(text, expected) {
			t.Fatalf("expected MECC lowering to contain %q\n%s", expected, text)
		}
	}
}

func Test_llvm_lowering_mecc_adopt_preserves_destructor(t *testing.T) {
	source := `
struct Thing {
    int32 value
    on_free { value = 0 }
}
struct Holder { Thing *child }
void distribute(Holder *left, Holder *right) {
    Thing *item = new Thing()
    left.child = item
    right.child = item
}
int32 main() { return 0 }
`
	directory := t.TempDir()
	path := filepath.Join(directory, "main.clyth")
	if err := os.WriteFile(path, []byte(source), 0o644); err != nil {
		t.Fatal(err)
	}
	program, parse_errors, lex_errors := parser.Parse(source)
	if len(parse_errors) != 0 || len(lex_errors) != 0 {
		t.Fatalf("unexpected frontend errors: parse=%v lex=%v", parse_errors, lex_errors)
	}
	result, semantic_errors, err := analyze_file_with_options(path, program, SemanticOptions{mecc_enabled: true})
	if err != nil {
		t.Fatal(err)
	}
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
	ir, err := lower_llvm_ir(program, result, "x86_64-unknown-linux-musl")
	if err != nil {
		t.Fatal(err)
	}
	text := string(ir)
	if !strings.Contains(text, "ptr @Thing__dtor") {
		t.Fatalf("expected MECC adoption to carry Thing destructor callback\n%s", text)
	}
}

func Test_llvm_lowering_conditional_explicit_free_preserves_other_path_cleanup(t *testing.T) {
	source := `
extern C bool should_free()
struct Thing { int32 value }
int32 main() {
    Thing *item = new Thing()
    if should_free() { free(item) }
    return 0
}
`
	ir, result := lower_test_source(t, source)
	cleanup_actions := 0
	for _, action := range result.lifetime_actions {
		if action.kind == lifetime_cleanup {
			cleanup_actions++
		}
	}
	if cleanup_actions == 0 {
		t.Fatal("expected a cleanup action for the non-free branch")
	}
	text := string(ir)
	if !strings.Contains(text, "cleanup.free") {
		t.Fatalf("conditional explicit free must preserve cleanup on the other branch\n%s", text)
	}
	if !strings.Contains(text, "store ptr null, ptr") {
		t.Fatalf("explicit free should null the cleanup slot for exactly-once destruction\n%s", text)
	}
}

func Test_llvm_lowering_default_expression_is_reevaluated_per_omitted_call(t *testing.T) {
	source := `
int32 next_default() { return 7 }
int32 pick(int32 value = next_default()) { return value }
int32 main() {
    int32 first = pick()
    int32 second = pick()
    return first + second - 14
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	if calls := strings.Count(text, "call i32 @next_default()"); calls != 2 {
		t.Fatalf("default expression must be reevaluated at each omitted call site; got %d calls\n%s", calls, text)
	}
}

func Test_llvm_lowering_heterogeneous_auto_variadic_stack_descriptors(t *testing.T) {
	source := `
struct Point { int32 x }
void inspect(auto... values) {}
int32 main() {
    Point point
    inspect(42, "hello", point)
    return 0
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	for _, expected := range []string{
		"define void @inspect(ptr %arg.0)",
		"alloca [6 x i64]",
		"store i64 3",
		"ptrtoint ptr",
	} {
		if !strings.Contains(text, expected) {
			t.Fatalf("expected heterogeneous variadic lowering to contain %q\n%s", expected, text)
		}
	}
}

func Test_llvm_lowering_zero_length_typed_variadic_pack(t *testing.T) {
	source := `
void collect(int32... values) {}
int32 main() {
    collect()
    return 0
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	for _, expected := range []string{"alloca [1 x i32]", "store i64 0", "call void @collect(ptr"} {
		if !strings.Contains(text, expected) {
			t.Fatalf("expected zero-length typed variadic lowering to contain %q\n%s", expected, text)
		}
	}
}

func Test_llvm_lowering_postfix_increment_updates_storage(t *testing.T) {
	source := `
int32 main() {
    int32 i = 0
    i++
    return i - 1
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	if !strings.Contains(text, " = add i32 ") || strings.Count(text, "store i32") < 2 {
		t.Fatalf("postfix increment must update its lvalue\n%s", text)
	}
}

func Test_llvm_lowering_runtime_raw_array_indexed_write_has_no_bounds_guard(t *testing.T) {
	source := `
int32 dynamic_index() { return 1 }
int32 main() {
    int32[2] values = [10, 20]
    values[dynamic_index()] = 42
    return values[1] - 42
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	for _, forbidden := range []string{"bounds.fail", "icmp ult i64", "array or string index out of bounds"} {
		if strings.Contains(text, forbidden) {
			t.Fatalf("raw array indexed writes must not emit an implicit bounds guard; found %q\n%s", forbidden, text)
		}
	}
	if !strings.Contains(text, "store i32") || !strings.Contains(text, "getelementptr i32") {
		t.Fatalf("raw indexed write should lower directly to address arithmetic and store\n%s", text)
	}
}

func Test_llvm_lowering_pointer_member_write_gets_null_guard(t *testing.T) {
	source := `
struct Thing { int32 value }
int32 main() {
    Thing *thing = null
    thing.value = 1
    return 0
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	for _, expected := range []string{"nullref.fail", "icmp ne ptr", "call void @abort()"} {
		if !strings.Contains(text, expected) {
			t.Fatalf("pointer-member write should contain %q\n%s", expected, text)
		}
	}
}

func Test_llvm_lowering_pointer_dereference_assignment_gets_null_guard(t *testing.T) {
	source := `
int32 main() {
    int32 *value = null
    *value = 1
    return 0
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	for _, expected := range []string{"nullref.fail", "icmp ne ptr", "call void @abort()"} {
		if !strings.Contains(text, expected) {
			t.Fatalf("pointer-dereference assignment should contain %q\n%s", expected, text)
		}
	}
}

func Test_llvm_lowering_break_runs_exited_scope_cleanup(t *testing.T) {
	source := `
struct Thing { int32 value }
int32 main() {
    while (true) {
        Thing *item = new Thing()
        break
    }
    return 0
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	body := strings.Index(text, "while.body")
	branch := strings.Index(text[body:], "br label %while.end")
	if body < 0 || branch < 0 {
		t.Fatalf("expected while/break lowering\n%s", text)
	}
	segment := text[body : body+branch]
	if !strings.Contains(segment, "call void @free(ptr") {
		t.Fatalf("break must run exited lexical allocation cleanup before branching\n%s", text)
	}
}

func Test_llvm_lowering_continue_runs_exited_scope_cleanup(t *testing.T) {
	source := `
struct Thing { int32 value }
int32 main() {
    int32 i = 0
    while (i < 2) {
        Thing *item = new Thing()
        i++
        continue
    }
    return i - 2
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	body := strings.Index(text, "while.body")
	branch := strings.Index(text[body:], "br label %while.cond")
	if body < 0 || branch < 0 {
		t.Fatalf("expected while/continue lowering\n%s", text)
	}
	segment := text[body : body+branch]
	if !strings.Contains(segment, "call void @free(ptr") {
		t.Fatalf("continue must run exited lexical allocation cleanup before branching\n%s", text)
	}
}

func Test_llvm_lowering_value_struct_runs_lexical_destructor_and_storage_cleanup(t *testing.T) {
	source := `
struct Thing {
    int32 value
    on_free { value = 0 }
}
int32 main() {
    Thing thing
    return 0
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	if !strings.Contains(text, "call void @Thing__dtor(ptr") || !strings.Contains(text, "call void @free(ptr") {
		t.Fatalf("value struct should run on_free and reclaim compiler backing storage\n%s", text)
	}
}

func Test_llvm_lowering_value_variadic_copy_is_destroyed_after_call(t *testing.T) {
	source := `
struct Point { int32 x }
void consume(Point... values) {}
int32 main() {
    Point point
    consume(point)
    return 0
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	call := strings.Index(text, "call void @consume(ptr")
	if call < 0 {
		t.Fatalf("expected typed aggregate variadic call\n%s", text)
	}
	after := text[call:]
	if !strings.Contains(after, "call void @free(ptr") {
		t.Fatalf("typed aggregate variadic copy must be reclaimed after call\n%s", text)
	}
}

func Test_imported_enum_preserves_integer_abi(t *testing.T) {
	directory := t.TempDir()
	dep_path := filepath.Join(directory, "dep.clyth")
	main_path := filepath.Join(directory, "main.clyth")
	dep := `
enum Kind { Zero, One }
int32 consume(Kind kind) { return 0 }
`
	main_source := `
import "dep.clyth"
int32 main() { return consume(Kind.One) }
`
	if err := os.WriteFile(dep_path, []byte(dep), 0o644); err != nil {
		t.Fatal(err)
	}
	if err := os.WriteFile(main_path, []byte(main_source), 0o644); err != nil {
		t.Fatal(err)
	}
	program, parse_errors, lex_errors := parser.Parse(main_source)
	if len(parse_errors) != 0 || len(lex_errors) != 0 {
		t.Fatalf("unexpected frontend errors: parse=%v lex=%v", parse_errors, lex_errors)
	}
	result, semantic_errors, err := analyze_file(main_path, program)
	if err != nil {
		t.Fatal(err)
	}
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
	ir, err := lower_llvm_ir(program, result, "x86_64-unknown-linux-musl")
	if err != nil {
		t.Fatal(err)
	}
	text := string(ir)
	if !strings.Contains(text, "declare i32 @consume(i32)") {
		t.Fatalf("imported enum parameter must retain i32 ABI\n%s", text)
	}
	if strings.Contains(text, "declare i32 @consume(ptr)") {
		t.Fatalf("imported enum parameter incorrectly degraded to ptr ABI\n%s", text)
	}
}

func Test_local_function_using_imported_enum_preserves_integer_abi(t *testing.T) {
	directory := t.TempDir()
	dep_path := filepath.Join(directory, "dep.clyth")
	main_path := filepath.Join(directory, "main.clyth")
	dep := `enum Kind { Zero, One }`
	main_source := `
import "dep.clyth"
int32 classify(Kind kind) { return 0 }
int32 main() { return classify(Kind.One) }
`
	if err := os.WriteFile(dep_path, []byte(dep), 0o644); err != nil {
		t.Fatal(err)
	}
	if err := os.WriteFile(main_path, []byte(main_source), 0o644); err != nil {
		t.Fatal(err)
	}
	program, parse_errors, lex_errors := parser.Parse(main_source)
	if len(parse_errors) != 0 || len(lex_errors) != 0 {
		t.Fatalf("unexpected frontend errors: parse=%v lex=%v", parse_errors, lex_errors)
	}
	result, semantic_errors, err := analyze_file(main_path, program)
	if err != nil {
		t.Fatal(err)
	}
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
	ir, err := lower_llvm_ir(program, result, "x86_64-unknown-linux-musl")
	if err != nil {
		t.Fatal(err)
	}
	text := string(ir)
	if !strings.Contains(text, "define i32 @classify(i32 %arg.0)") {
		t.Fatalf("local function parameter using imported enum must retain i32 ABI\n%s", text)
	}
}

func Test_imported_enum_member_lowers_to_declared_integer_value(t *testing.T) {
	directory := t.TempDir()
	dep_path := filepath.Join(directory, "dep.clyth")
	main_path := filepath.Join(directory, "main.clyth")
	dep := `enum Kind { Illegal, EOF, Explicit = 7, Next }`
	main_source := `
import "dep.clyth"
int32 main() { return Kind.EOF + Kind.Explicit + Kind.Next - 16 }
`
	if err := os.WriteFile(dep_path, []byte(dep), 0o644); err != nil {
		t.Fatal(err)
	}
	if err := os.WriteFile(main_path, []byte(main_source), 0o644); err != nil {
		t.Fatal(err)
	}
	program, parse_errors, lex_errors := parser.Parse(main_source)
	if len(parse_errors) != 0 || len(lex_errors) != 0 {
		t.Fatalf("unexpected frontend errors: parse=%v lex=%v", parse_errors, lex_errors)
	}
	result, semantic_errors, err := analyze_file(main_path, program)
	if err != nil {
		t.Fatal(err)
	}
	if len(semantic_errors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semantic_errors)
	}
	ir, err := lower_llvm_ir(program, result, "x86_64-unknown-linux-musl")
	if err != nil {
		t.Fatal(err)
	}
	text := string(ir)
	if strings.Contains(text, "ptrtoint ptr null to i32") {
		t.Fatalf("imported enum member degraded to null/pointer zero\n%s", text)
	}
	for _, expected := range []string{"add i32 1, 7", "add i32 %v1, 8"} {
		if !strings.Contains(text, expected) {
			t.Fatalf("expected imported enum lowering %q in IR\n%s", expected, text)
		}
	}
}

func Test_llvm_lowering_indexable_protocol_dispatches_to_implementing_methods(t *testing.T) {
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
    pair[1] = 9
    return pair[0] + pair[1] - 16
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	for _, expected := range []string{
		"call void @Pair__on_write_index",
		"call i32 @Pair__on_read_index",
	} {
		if !strings.Contains(text, expected) {
			t.Fatalf("expected protocol indexing to lower through %q\n%s", expected, text)
		}
	}
}

func Test_intrinsic_struct_embeds_fixed_scalar_array_inline(t *testing.T) {
	lowerer := &llvm_lowerer{}
	char_array := &SemanticType{
		kind:             type_array,
		name:             "char",
		array_dimensions: []SemanticArrayDimension{{dynamic: false, size: "15"}},
	}
	info := &StructInfo{
		name:      "string",
		intrinsic: true,
		fields: map[string]*Symbol{
			"array_ptr":         {name: "array_ptr", kind: symbol_field, type_info: &SemanticType{kind: type_pointer, name: "char", pointer_depth: 1}},
			"local_stack_cache": {name: "local_stack_cache", kind: symbol_field, type_info: char_array},
			"length":            {name: "length", kind: symbol_field, type_info: &SemanticType{kind: type_integer, name: "uint64"}},
		},
		field_order: []string{"array_ptr", "local_stack_cache", "length"},
	}

	if got := lowerer.struct_field_offset(info, "array_ptr"); got != 0 {
		t.Fatalf("array_ptr offset = %d, want 0", got)
	}
	if got := lowerer.struct_field_offset(info, "local_stack_cache"); got != 8 {
		t.Fatalf("local_stack_cache offset = %d, want 8", got)
	}
	if got := lowerer.struct_field_offset(info, "length"); got != 24 {
		t.Fatalf("length offset = %d, want 24", got)
	}
	if got := lowerer.struct_storage_size(info); got != 32 {
		t.Fatalf("string intrinsic storage size = %d, want 32", got)
	}
}

func Test_llvm_lowering_raw_pointer_index_is_direct_element_access(t *testing.T) {
	source := `
char read_at(char* data, int64 index) {
    return data[index]
}
void write_at(char* data, int64 index, char value) {
    data[index] = value
}
int32 main() { return 0 }
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	if !strings.Contains(text, "getelementptr i8, ptr") {
		t.Fatalf("raw char* indexing should lower directly as i8 pointer arithmetic\n%s", text)
	}
	// A raw T* is not an array descriptor and therefore must not first load a
	// data pointer from offset zero before indexing.
	if strings.Contains(text, "getelementptr ptr, ptr") {
		t.Fatalf("raw char* indexing unexpectedly used pointer-width elements\n%s", text)
	}
}

func Test_core_string_literal_uses_descriptor_and_inline_sso(t *testing.T) {
	source := `
int32 main() {
    string value = "hello"
    if value.length != 5 { return 1 }
    return 0
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	for _, expected := range []string{
		`@.clyth.string.0 = private unnamed_addr constant { ptr, [15 x i8], i8, i64 }`,
		`[15 x i8] c"hello`,
		`i64 5 }`,
		`i64 24`,
	} {
		if !strings.Contains(text, expected) {
			t.Fatalf("expected Core string descriptor IR to contain %q\n%s", expected, text)
		}
	}
	if strings.Contains(text, "call i64 @strlen(ptr @.clyth.string.0)") {
		t.Fatalf("Clyth string.length should load descriptor metadata, not call strlen\n%s", text)
	}
}

func Test_native_main_argv_is_adapted_to_core_string_descriptors(t *testing.T) {
	source := `
int32 main(string[] args) {
    if args.length < 2 { return 1 }
    if args[1].length == 0 { return 2 }
    return 0
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	for _, expected := range []string{
		"main.argv.cond",
		"getelementptr ptr, ptr %clyth.argv",
		"call i64 @strlen(ptr",
		"mul i64",
	} {
		if !strings.Contains(text, expected) {
			t.Fatalf("expected native argv adapter IR to contain %q\n%s", expected, text)
		}
	}
}

func Test_scalar_primitives_remain_direct_llvm_primitives(t *testing.T) {
	source := `
bool take_bool(bool value) { return value }
char take_char(char value) { return value }
int8 take_i8(int8 value) { return value }
int16 take_i16(int16 value) { return value }
int32 take_i32(int32 value) { return value }
int64 take_i64(int64 value) { return value }
float32 take_f32(float32 value) { return value }
float64 take_f64(float64 value) { return value }
int32* take_ptr(int32* value) { return value }
int32 main() { return 0 }
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	for _, expected := range []string{
		"define i1 @take_bool(i1 %arg.0)",
		"define i8 @take_char(i8 %arg.0)",
		"define i8 @take_i8(i8 %arg.0)",
		"define i16 @take_i16(i16 %arg.0)",
		"define i32 @take_i32(i32 %arg.0)",
		"define i64 @take_i64(i64 %arg.0)",
		"define float @take_f32(float %arg.0)",
		"define double @take_f64(double %arg.0)",
		"define ptr @take_ptr(ptr %arg.0)",
	} {
		if !strings.Contains(text, expected) {
			t.Fatalf("expected primitive ABI IR to contain %q\n%s", expected, text)
		}
	}
}

func Test_raw_array_constructor_allocates_descriptor_and_storage(t *testing.T) {
	source := `
int32 main() {
    int32[] values = array<int32>(4)
    values[2] = 7
    return values.length - 4
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	for _, expected := range []string{
		"call ptr @calloc(i64 1, i64 16)",
		"call ptr @calloc(i64 4, i64 4)",
		"getelementptr i8, ptr",
		"i64 8",
	} {
		if !strings.Contains(text, expected) {
			t.Fatalf("expected raw-array constructor IR to contain %q\n%s", expected, text)
		}
	}
}

func Test_raw_array_copy_constructor_copies_retained_prefix(t *testing.T) {
	source := `
int32 main() {
    int32[] values = array<int32>(2)
    values[0] = 7
    int32[] grown = array<int32>(values, 4)
    return grown[0] - 7
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	if !strings.Contains(text, "@llvm.memcpy.p0.p0.i64") {
		t.Fatalf("expected raw-array copy constructor to emit llvm.memcpy\n%s", text)
	}
}

func Test_interpolated_string_uses_descriptor_for_format_and_result(t *testing.T) {
	source := `
int32 main() {
    string name = "world"
    string value = ` + "`${name} ${42}`" + `
    if value.length == 0 { return 1 }
    return 0
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	// The snprintf format is a C view materialized from a Clyth descriptor,
	// and the formatted bytes are copied into a fresh descriptor before the
	// temporary C buffer is released.
	if !strings.Contains(text, "call i32 (ptr, i64, ptr, ...) @snprintf") {
		t.Fatalf("interpolated string did not lower through snprintf\n%s", text)
	}
	if !strings.Contains(text, "call ptr @calloc(i64 1, i64 32)") {
		t.Fatalf("interpolated string result was not materialized as a Core string descriptor\n%s", text)
	}
	if !strings.Contains(text, "call void @free(ptr") {
		t.Fatalf("interpolated string temporary buffer was not released\n%s", text)
	}
}

func Test_string_array_length_uses_raw_array_descriptor(t *testing.T) {
	source := `
int64 inspect(string[] args) {
    return args.length
}
int32 main() {
    string[] values = array<string>(2)
    return inspect(values) - 2
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	start := strings.Index(text, "define i64 @inspect(")
	if start < 0 {
		t.Fatalf("expected inspect function in LLVM IR\n%s", text)
	}
	end := strings.Index(text[start:], "\n}")
	if end < 0 {
		t.Fatalf("expected inspect function terminator in LLVM IR\n%s", text[start:])
	}
	body := text[start : start+end]
	if !strings.Contains(body, "getelementptr i8, ptr") || !strings.Contains(body, "i64 8") {
		t.Fatalf("expected string[] length to read raw-array header offset 8\n%s", body)
	}
	// Aggregate parameter ownership now clones string[] values on function entry.
	// That clone legitimately touches offset 24 while deep-copying each string
	// element, so the old whole-function negative check for "i64 24" was too
	// broad. The actual array-length access is required to use header offset 8.
}

func Test_value_aggregate_parameter_gets_callee_owned_copy(t *testing.T) {
	source := `
struct Box { int64 value }
Box roundtrip(Box input) {
    input = Box()
    return input
}
int32 main() {
    Box value = Box()
    Box result = roundtrip(value)
    return 0
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	start := strings.Index(text, "define ptr @roundtrip(")
	if start < 0 {
		t.Fatalf("expected roundtrip function in LLVM IR\n%s", text)
	}
	end := strings.Index(text[start:], "\n}")
	if end < 0 {
		t.Fatalf("expected roundtrip function terminator in LLVM IR\n%s", text[start:])
	}
	body := text[start : start+end]
	if !strings.Contains(body, "icmp eq ptr %arg.0, null") {
		t.Fatalf("expected value aggregate parameter to be cloned on callee entry\n%s", body)
	}
	if !strings.Contains(body, "call void @free(ptr") {
		t.Fatalf("expected callee-owned aggregate parameter to be destroyed by function cleanup\n%s", body)
	}
}

func Test_default_string_compares_as_empty_without_null_dereference(t *testing.T) {
	source := `
struct Options { string Input }
int32 main() {
    Options options = Options()
    if options.Input != "" { return 1 }
    return 0
}
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	if !strings.Contains(text, "@.clyth.empty.string") {
		t.Fatalf("expected canonical empty-string descriptor in LLVM IR\n%s", text)
	}
	if !strings.Contains(text, "select i1") || !strings.Contains(text, "ptr @.clyth.empty.string") {
		t.Fatalf("expected null-safe string descriptor selection\n%s", text)
	}
}

func Test_pointer_element_array_remains_value_aggregate(t *testing.T) {
	source := `
struct Item { int64 value }
Item*[] copy_items(Item*[] values) {
    Item*[] out = array<Item*>(values.length)
    int64 i = 0
    while i < values.length {
        out[i] = values[i]
        i += 1
    }
    return out
}
int32 main() { return 0 }
`
	ir, _ := lower_test_source(t, source)
	text := string(ir)
	start := strings.Index(text, "define ptr @copy_items(")
	if start < 0 {
		t.Fatalf("expected copy_items function in LLVM IR\n%s", text)
	}
	end := strings.Index(text[start:], "\n}")
	if end < 0 {
		t.Fatalf("expected copy_items function terminator\n%s", text[start:])
	}
	body := text[start : start+end]
	if !strings.Contains(body, "aggregate.clone") {
		t.Fatalf("expected Item*[] parameter to be treated as a value aggregate and cloned on entry\n%s", body)
	}
	if !strings.Contains(body, "getelementptr i8, ptr") || !strings.Contains(body, "load ptr, ptr") {
		t.Fatalf("expected Item*[] indexing to load the raw-array data pointer from its descriptor\n%s", body)
	}
}

func Test_mecc_defers_collapse_for_pointer_backed_value_aggregate_return(t *testing.T) {
	source := `
struct Box { int32 value }
Box identity(Box input) { return input }
int32 main() { Box b = Box(); Box c = identity(b); return 0 }
`
	directory := t.TempDir()
	path := filepath.Join(directory, "main.clyth")
	if err := os.WriteFile(path, []byte(source), 0o644); err != nil {
		t.Fatal(err)
	}
	program, parseErrors, lexErrors := parser.Parse(source)
	if len(parseErrors) != 0 || len(lexErrors) != 0 {
		t.Fatalf("unexpected frontend errors: parse=%v lex=%v", parseErrors, lexErrors)
	}
	result, semanticErrors, err := analyze_file_with_options(path, program, SemanticOptions{mecc_enabled: true})
	if err != nil {
		t.Fatal(err)
	}
	if len(semanticErrors) != 0 {
		t.Fatalf("unexpected semantic errors: %v", semanticErrors)
	}
	ir, err := lower_llvm_ir(program, result, "x86_64-unknown-linux-musl")
	if err != nil {
		t.Fatal(err)
	}
	text := string(ir)
	start := strings.Index(text, "define ptr @identity(")
	if start < 0 {
		t.Fatalf("expected identity definition\n%s", text)
	}
	end := strings.Index(text[start:], "\n}")
	if end < 0 {
		t.Fatalf("expected identity terminator")
	}
	body := text[start : start+end]
	if strings.Contains(body, "call void @clyth_mecc_collapse()") {
		t.Fatalf("value-aggregate return must defer MECC collapse until caller establishes ownership\n%s", body)
	}
}
