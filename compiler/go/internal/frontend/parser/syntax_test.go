package parser

import (
	"testing"

	"github.com/mali5820k/clyth-bootstrap/internal/frontend/ast"
)

func mustParse(t *testing.T, source string) *ast.Program {
	t.Helper()
	p, pe, le := Parse(source)
	if len(le) != 0 {
		t.Fatalf("lexer: %v", le)
	}
	if len(pe) != 0 {
		t.Fatalf("parser: %v", pe)
	}
	return p
}

func TestImportKinds(t *testing.T) {
	p := mustParse(t, "import file-io\nimport \"support/helpers.clyth\" as helpers\n")
	m := p.Declarations[0].(*ast.ImportDecl)
	f := p.Declarations[1].(*ast.ImportDecl)
	if m.Kind != ast.ImportModule || m.Target != "file-io" {
		t.Fatalf("module import = %#v", m)
	}
	if f.Kind != ast.ImportFile || f.Target != "support/helpers.clyth" || f.Alias != "helpers" {
		t.Fatalf("file import = %#v", f)
	}
}

func TestCallableTypeOnlyInDeclarationContext(t *testing.T) {
	p := mustParse(t, "int32 apply(int32 value, int32(int32) callback) { return callback(value) }\n")
	fn := p.Declarations[0].(*ast.FunctionDecl)
	typ := fn.Parameters[1].Type
	if typ.Callable == nil || typ.Callable.Return.Name != "int32" || len(typ.Callable.Parameters) != 1 || typ.Callable.Parameters[0].Name != "int32" {
		t.Fatalf("callable type = %#v", typ)
	}
	ret := fn.Body.Statements[0].(*ast.ReturnStmt)
	if _, ok := ret.Value.(*ast.CallExpr); !ok {
		t.Fatalf("callback(value) parsed as %T", ret.Value)
	}
}

func TestGenericCallPreservesTypeArguments(t *testing.T) {
	p := mustParse(t, "Widget make(Allocator allocator) { return allocator.alloc<Widget>(new Widget()) }\n")
	fn := p.Declarations[0].(*ast.FunctionDecl)
	ret := fn.Body.Statements[0].(*ast.ReturnStmt)
	call := ret.Value.(*ast.CallExpr)
	g, ok := call.Callee.(*ast.GenericSpecializationExpr)
	if !ok {
		t.Fatalf("generic callee parsed as %T", call.Callee)
	}
	if len(g.TypeArguments) != 1 || g.TypeArguments[0].Name != "Widget" {
		t.Fatalf("generic args = %#v", g.TypeArguments)
	}
}

func Test_enum_and_switch_surface(t *testing.T) {
	source := `
enum Color { red, green = 3, blue }
int32 main() {
  int32 value = 1
  switch (value) {
    case 1:
      value = 2
    case 2:
      value = 3
      break
    default:
      value = 4
  }
  return value
}
`
	_, parse_errors, lex_errors := Parse(source)
	if len(lex_errors) != 0 || len(parse_errors) != 0 {
		t.Fatalf("unexpected frontend errors: parse=%v lex=%v", parse_errors, lex_errors)
	}
}

func Test_lambda_v1_canonical_forms(t *testing.T) {
	program := mustParse(t, `
int32 main() {
    auto concise = void(int32 value) => { print(value) }
    void(int32) explicit = (int32 value) => { print(value) }
    void(int32) contextual = (value) => { print(value) }
    return 0
}
`)
	function := program.Declarations[0].(*ast.FunctionDecl)
	first := function.Body.Statements[0].(*ast.VarDeclStmt).Initializer.(*ast.LambdaExpr)
	if first.ReturnType == nil || first.ReturnType.Name != "void" || first.Parameters[0].Type == nil || first.Parameters[0].Type.Name != "int32" {
		t.Fatalf("fully typed lambda = %#v", first)
	}
	third := function.Body.Statements[2].(*ast.VarDeclStmt).Initializer.(*ast.LambdaExpr)
	if third.ReturnType != nil || third.Parameters[0].Type != nil || third.Parameters[0].Name != "value" {
		t.Fatalf("contextual lambda = %#v", third)
	}
}

func TestMeccConfigDirectiveSyntax(t *testing.T) {
	source := `
#MECC_config.compacting = true
int32 main() {
    #MECC_config.compacting = false
    return 0
}
`
	program, parse_errors, lex_errors := Parse(source)
	if len(lex_errors) != 0 || len(parse_errors) != 0 {
		t.Fatalf("unexpected diagnostics: lex=%v parse=%v", lex_errors, parse_errors)
	}
	if len(program.Declarations) != 2 {
		t.Fatalf("declarations = %d", len(program.Declarations))
	}
	directive, ok := program.Declarations[0].(*ast.MeccConfigDirectiveDecl)
	if !ok || directive.Property != "compacting" || !directive.Value {
		t.Fatalf("top-level directive = %#v", program.Declarations[0])
	}
	function := program.Declarations[1].(*ast.FunctionDecl)
	if len(function.Body.Statements) != 2 {
		t.Fatalf("function statements = %d", len(function.Body.Statements))
	}
	nested, ok := function.Body.Statements[0].(*ast.MeccConfigDirectiveStmt)
	if !ok || nested.Property != "compacting" || nested.Value {
		t.Fatalf("nested directive = %#v", function.Body.Statements[0])
	}
}

func TestErrorTypeDirectiveSyntax(t *testing.T) {
	program := mustParse(t, `
#ErrorType device_timeout
int32 main() { return 0 }
`)
	if len(program.Declarations) != 2 {
		t.Fatalf("declarations = %d", len(program.Declarations))
	}
	directive, ok := program.Declarations[0].(*ast.ErrorTypeDirectiveDecl)
	if !ok || directive.Name != "device_timeout" {
		t.Fatalf("error type directive = %#v", program.Declarations[0])
	}
}

func Test_default_and_typed_variadic_parameters(t *testing.T) {
	program := mustParse(t, `
void log(bool timestamp = true, string... messages) {}
void inspect(auto... values) {}
`)
	log_fn := program.Declarations[0].(*ast.FunctionDecl)
	if len(log_fn.Parameters) != 2 || log_fn.Parameters[0].DefaultValue == nil {
		t.Fatalf("default parameter metadata = %#v", log_fn.Parameters)
	}
	if !log_fn.Parameters[1].Variadic || log_fn.Parameters[1].Type == nil || log_fn.Parameters[1].Type.Name != "string" {
		t.Fatalf("typed variadic parameter = %#v", log_fn.Parameters[1])
	}
	inspect_fn := program.Declarations[1].(*ast.FunctionDecl)
	if !inspect_fn.Parameters[0].Variadic || inspect_fn.Parameters[0].Type == nil || inspect_fn.Parameters[0].Type.Name != "auto" {
		t.Fatalf("heterogeneous variadic parameter = %#v", inspect_fn.Parameters[0])
	}
}
