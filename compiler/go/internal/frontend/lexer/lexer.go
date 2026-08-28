package lexer

import (
	"fmt"
	"strings"
	"unicode/utf8"

	"github.com/mali5820k/clyth-bootstrap/internal/frontend/token"
)

var keywordTrie = newTrie(map[string]token.Kind{
	"import": token.Import, "auto": token.Auto, "extern": token.Extern, "struct": token.Struct, "intrinsic_struct": token.IntrinsicStruct, "protocol": token.Protocol, "enum": token.Enum,
	"constructor": token.Constructor, "on_free": token.OnFree, "private": token.Private, "public": token.Public,
	"return": token.Return, "if": token.If, "else": token.Else, "while": token.While, "for": token.For, "switch": token.Switch, "case": token.Case, "default": token.Default,
	"continue": token.Continue, "break": token.Break, "true": token.True, "false": token.False, "null": token.Null,
	"as": token.As, "new": token.New, "const": token.Const,
	"cast": token.Cast, "this": token.This,
})

var operatorTrie = newTrie(map[string]token.Kind{
	"...": token.Ellipsis, "==": token.Equal, "!=": token.NotEqual, "<=": token.LessEqual, ">=": token.GreaterEqual,
	"&&": token.AndAnd, "||": token.OrOr, "+=": token.PlusAssign, "-=": token.MinusAssign, "*=": token.StarAssign,
	"/=": token.SlashAssign, "%=": token.PercentAssign, "->": token.Arrow, "=>": token.FatArrow, "::": token.Scope,
	"++": token.Increment, "--": token.Decrement,
	"(": token.LParen, ")": token.RParen, "{": token.LBrace, "}": token.RBrace, "[": token.LBracket, "]": token.RBracket,
	"<": token.Less, ">": token.Greater, ",": token.Comma, ".": token.Dot, ":": token.Colon, ";": token.Semicolon,
	"+": token.Plus, "-": token.Minus, "*": token.Star, "/": token.Slash, "%": token.Percent, "=": token.Assign,
	"!": token.Bang, "&": token.Ampersand, "|": token.Pipe, "~": token.Tilde, "#": token.Hash,
})

type Error struct {
	Span    token.Span
	Message string
}

func (e Error) Error() string {
	return fmt.Sprintf("%d:%d: %s", e.Span.Start.Line, e.Span.Start.Column, e.Message)
}

type Lexer struct {
	source string
	offset int
	line   int
	column int
	errs   []Error
}

func New(source string) *Lexer   { return &Lexer{source: source, line: 1, column: 1} }
func (l *Lexer) Errors() []Error { return append([]Error(nil), l.errs...) }

func (l *Lexer) pos() token.Position {
	return token.Position{Offset: l.offset, Line: l.line, Column: l.column}
}
func (l *Lexer) atEnd() bool { return l.offset >= len(l.source) }
func (l *Lexer) peekByte(n int) byte {
	i := l.offset + n
	if i < 0 || i >= len(l.source) {
		return 0
	}
	return l.source[i]
}
func (l *Lexer) advanceByte() byte {
	if l.atEnd() {
		return 0
	}
	b := l.source[l.offset]
	l.offset++
	if b == '\n' {
		l.line++
		l.column = 1
	} else {
		l.column++
	}
	return b
}
func (l *Lexer) advanceRune() rune {
	if l.atEnd() {
		return 0
	}
	r, size := utf8.DecodeRuneInString(l.source[l.offset:])
	for range size {
		l.advanceByte()
	}
	return r
}
func (l *Lexer) make(kind token.Kind, start token.Position) token.Token {
	return token.Token{Kind: kind, Lexeme: l.source[start.Offset:l.offset], Span: token.Span{Start: start, End: l.pos()}}
}

func isIdentStart(r rune) bool {
	return r == '_' || r > 127 || ('a' <= r && r <= 'z') || ('A' <= r && r <= 'Z')
}
func isIdentContinue(r rune) bool { return isIdentStart(r) || ('0' <= r && r <= '9') }

func (l *Lexer) Next() token.Token {
	for !l.atEnd() {
		start := l.pos()
		b := l.peekByte(0)
		switch b {
		case ' ', '\t', '\r':
			l.advanceByte()
			continue
		case '\n':
			l.advanceByte()
			return l.make(token.Newline, start)
		case '/':
			if l.peekByte(1) == '/' {
				for !l.atEnd() && l.peekByte(0) != '\n' {
					l.advanceByte()
				}
				continue
			}
			if l.peekByte(1) == '*' {
				l.advanceByte()
				l.advanceByte()
				closed := false
				for !l.atEnd() {
					if l.peekByte(0) == '*' && l.peekByte(1) == '/' {
						l.advanceByte()
						l.advanceByte()
						closed = true
						break
					}
					l.advanceRune()
				}
				if !closed {
					sp := token.Span{Start: start, End: l.pos()}
					l.errs = append(l.errs, Error{sp, "unterminated block comment"})
					return token.Token{Kind: token.Illegal, Lexeme: l.source[start.Offset:l.offset], Span: sp}
				}
				continue
			}
		}
		if b == '"' {
			return l.scanQuoted(start, '"', token.String)
		}
		if b == '\'' {
			return l.scanQuoted(start, '\'', token.Character)
		}
		if b == '`' {
			return l.scanBacktick(start)
		}
		r, _ := utf8.DecodeRuneInString(l.source[l.offset:])
		if isIdentStart(r) {
			l.advanceRune()
			for !l.atEnd() {
				r, _ = utf8.DecodeRuneInString(l.source[l.offset:])
				if !isIdentContinue(r) {
					break
				}
				l.advanceRune()
			}
			text := l.source[start.Offset:l.offset]
			if kind, ok := keywordTrie.exact(text); ok {
				return l.make(kind, start)
			}
			return l.make(token.Identifier, start)
		}
		if b >= '0' && b <= '9' {
			return l.scanNumber(start)
		}
		if kind, end, ok := operatorTrie.longest(l.source, l.offset); ok {
			for l.offset < end {
				l.advanceByte()
			}
			return l.make(kind, start)
		}
		l.advanceRune()
		sp := token.Span{Start: start, End: l.pos()}
		err := Error{sp, fmt.Sprintf("unexpected character %q", l.source[start.Offset:l.offset])}
		l.errs = append(l.errs, err)
		return token.Token{Kind: token.Illegal, Lexeme: l.source[start.Offset:l.offset], Span: sp}
	}
	p := l.pos()
	return token.Token{Kind: token.EOF, Span: token.Span{Start: p, End: p}}
}

func (l *Lexer) scanQuoted(start token.Position, quote byte, kind token.Kind) token.Token {
	l.advanceByte()
	escaped := false
	for !l.atEnd() {
		b := l.advanceByte()
		if escaped {
			escaped = false
			continue
		}
		if b == '\\' {
			escaped = true
			continue
		}
		if b == quote {
			return l.make(kind, start)
		}
		if b == '\n' && quote == '\'' {
			break
		}
	}
	sp := token.Span{Start: start, End: l.pos()}
	l.errs = append(l.errs, Error{sp, "unterminated literal"})
	return token.Token{Kind: token.Illegal, Lexeme: l.source[start.Offset:l.offset], Span: sp}
}

func (l *Lexer) scanBacktick(start token.Position) token.Token {
	l.advanceByte()
	escaped := false
	for !l.atEnd() {
		b := l.advanceByte()
		if escaped {
			escaped = false
			continue
		}
		if b == '\\' {
			escaped = true
			continue
		}
		if b == '`' {
			return l.make(token.InterpolatedString, start)
		}
	}
	sp := token.Span{Start: start, End: l.pos()}
	l.errs = append(l.errs, Error{sp, "unterminated backtick string"})
	return token.Token{Kind: token.Illegal, Lexeme: l.source[start.Offset:l.offset], Span: sp}
}

func (l *Lexer) scanNumber(start token.Position) token.Token {
	kind := token.Integer
	if l.peekByte(0) == '0' && (l.peekByte(1) == 'x' || l.peekByte(1) == 'X' || l.peekByte(1) == 'b' || l.peekByte(1) == 'B' || l.peekByte(1) == 'o' || l.peekByte(1) == 'O') {
		l.advanceByte()
		l.advanceByte()
		for !l.atEnd() {
			b := l.peekByte(0)
			if !(b == '_' || (b >= '0' && b <= '9') || (b >= 'a' && b <= 'f') || (b >= 'A' && b <= 'F')) {
				break
			}
			l.advanceByte()
		}
		return l.make(kind, start)
	}
	for !l.atEnd() {
		b := l.peekByte(0)
		if !(b == '_' || (b >= '0' && b <= '9')) {
			break
		}
		l.advanceByte()
	}
	if l.peekByte(0) == '.' && l.peekByte(1) >= '0' && l.peekByte(1) <= '9' {
		kind = token.Float
		l.advanceByte()
		for !l.atEnd() {
			b := l.peekByte(0)
			if !(b == '_' || (b >= '0' && b <= '9')) {
				break
			}
			l.advanceByte()
		}
	}
	if b := l.peekByte(0); b == 'e' || b == 'E' {
		kind = token.Float
		l.advanceByte()
		if l.peekByte(0) == '+' || l.peekByte(0) == '-' {
			l.advanceByte()
		}
		for !l.atEnd() {
			b = l.peekByte(0)
			if !(b == '_' || (b >= '0' && b <= '9')) {
				break
			}
			l.advanceByte()
		}
	}
	return l.make(kind, start)
}

func Lex(source string) ([]token.Token, []Error) {
	l := New(source)
	out := make([]token.Token, 0, 128)
	for {
		t := l.Next()
		out = append(out, t)
		if t.Kind == token.EOF {
			break
		}
	}
	return out, l.Errors()
}

func Debug(tokens []token.Token) string {
	var b strings.Builder
	for _, t := range tokens {
		fmt.Fprintln(&b, t.String())
	}
	return b.String()
}
