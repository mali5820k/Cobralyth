package lexer

import (
	"testing"

	"github.com/mali5820k/clyth-bootstrap/internal/frontend/token"
)

func significant(ts []token.Token) []token.Token {
	out := make([]token.Token, 0, len(ts))
	for _, x := range ts {
		if x.Kind != token.Newline && x.Kind != token.EOF {
			out = append(out, x)
		}
	}
	return out
}

func TestTrieRecognizesLongestOperators(t *testing.T) {
	ts, errs := Lex("a += b == c => d ...")
	if len(errs) != 0 {
		t.Fatal(errs)
	}
	ts = significant(ts)
	want := []token.Kind{token.Identifier, token.PlusAssign, token.Identifier, token.Equal, token.Identifier, token.FatArrow, token.Identifier, token.Ellipsis}
	if len(ts) != len(want) {
		t.Fatalf("got %d tokens, want %d", len(ts), len(want))
	}
	for i, k := range want {
		if ts[i].Kind != k {
			t.Fatalf("token %d: got %s want %s", i, ts[i].Kind, k)
		}
	}
}

func TestHyphenRemainsOperatorOutsideImport(t *testing.T) {
	ts, errs := Lex("left-right")
	if len(errs) != 0 {
		t.Fatal(errs)
	}
	ts = significant(ts)
	if len(ts) != 3 || ts[0].Kind != token.Identifier || ts[1].Kind != token.Minus || ts[2].Kind != token.Identifier {
		t.Fatalf("unexpected tokens: %#v", ts)
	}
}
