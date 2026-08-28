package lsp

import "testing"

func TestDiagnoseBalanced(t *testing.T) {
	got := diagnose("int32 main(string[] args) { return 0 }\n")
	if len(got) != 0 {
		t.Fatalf("expected no diagnostics, got %#v", got)
	}
}

func TestDiagnoseUnclosedDelimiter(t *testing.T) {
	got := diagnose("int32 main() {\n")
	if len(got) != 1 {
		t.Fatalf("expected one diagnostic, got %#v", got)
	}
}

func TestScanSymbols(t *testing.T) {
	d := document{URI: "file:///test.clyth", Text: "struct Point {\n}\nint32 add(int32 a, int32 b) { return a + b }\n"}
	got := scanSymbols(d)
	names := map[string]bool{}
	for _, x := range got {
		names[x.Name] = true
	}
	if !names["Point"] || !names["add"] {
		t.Fatalf("missing symbols: %#v", got)
	}
}

func TestSemanticCallableTypeVsCall(t *testing.T) {
	text := "bool(Request, Response)[] filters\nauthenticate(request, response)\nPoint(1, 2)\n"
	toks := scanSemanticTokens(text)
	seen := map[string]bool{}
	for _, tok := range toks {
		lines := []string{"bool(Request, Response)[] filters", "authenticate(request, response)", "Point(1, 2)"}
		if tok.line < len(lines) && tok.start+tok.length <= len(lines[tok.line]) {
			lex := lines[tok.line][tok.start : tok.start+tok.length]
			seen[lex+":"+semanticTokenTypes[tok.typ]] = true
		}
	}
	for _, want := range []string{"bool:type", "Request:type", "Response:type", "filters:variable", "authenticate:function", "Point:type"} {
		if !seen[want] {
			t.Fatalf("missing semantic token %s: %#v", want, seen)
		}
	}
}

func TestSemanticImportKinds(t *testing.T) {
	text := "import file-io\nimport \"support/helper.clyth\"\n"
	toks := scanSemanticTokens(text)
	seenNamespace, seenString := false, false
	for _, tok := range toks {
		if semanticTokenTypes[tok.typ] == "namespace" {
			seenNamespace = true
		}
		if semanticTokenTypes[tok.typ] == "string" {
			seenString = true
		}
	}
	if !seenNamespace || !seenString {
		t.Fatalf("expected module namespace and source path tokens: %#v", toks)
	}
}

func TestSemanticExplicitLambdaSignature(t *testing.T) {
	text := "auto callback = void(int32 value) => { print(value) }\n"
	toks := scanSemanticTokens(text)
	seen := map[string]bool{}
	for _, tok := range toks {
		if tok.line == 0 && tok.start+tok.length <= len(text) {
			lex := text[tok.start : tok.start+tok.length]
			seen[lex+":"+semanticTokenTypes[tok.typ]] = true
		}
	}
	for _, want := range []string{"void:type", "int32:type", "value:variable"} {
		if !seen[want] {
			t.Fatalf("missing semantic token %s: %#v", want, seen)
		}
	}
}
