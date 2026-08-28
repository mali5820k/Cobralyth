package token

import "fmt"

type Kind uint16

const (
	Illegal Kind = iota
	EOF
	Newline
	Identifier
	Integer
	Float
	String
	InterpolatedString
	Character

	// Keywords.
	Import
	Auto
	Extern
	Struct
	IntrinsicStruct
	Protocol
	Enum
	Constructor
	OnFree
	Private
	Public
	Return
	If
	Else
	While
	For
	Switch
	Case
	Default
	Continue
	Break
	True
	False
	Null
	As
	New
	Const
	Cast
	This

	// Punctuation/operators.
	LParen
	RParen
	LBrace
	RBrace
	LBracket
	RBracket
	Less
	Greater
	Comma
	Dot
	Colon
	Semicolon
	Plus
	Minus
	Star
	Slash
	Percent
	Assign
	Bang
	Ampersand
	Pipe
	Tilde
	Equal
	NotEqual
	LessEqual
	GreaterEqual
	AndAnd
	OrOr
	PlusAssign
	MinusAssign
	StarAssign
	SlashAssign
	PercentAssign
	Arrow
	FatArrow
	Scope
	Ellipsis
	Increment
	Decrement
	Hash
)

type Position struct {
	Offset int `json:"offset"`
	Line   int `json:"line"`
	Column int `json:"column"`
}

type Span struct {
	Start Position `json:"start"`
	End   Position `json:"end"`
}

func Merge(a, b Span) Span { return Span{Start: a.Start, End: b.End} }

type Token struct {
	Kind   Kind   `json:"kind"`
	Lexeme string `json:"lexeme"`
	Span   Span   `json:"span"`
}

func (t Token) String() string {
	return fmt.Sprintf("%s(%q)@%d:%d", t.Kind, t.Lexeme, t.Span.Start.Line, t.Span.Start.Column)
}

var names = map[Kind]string{
	Illegal: "illegal", EOF: "eof", Newline: "newline", Identifier: "identifier", Integer: "integer", Float: "float", String: "string", InterpolatedString: "interpolated-string", Character: "character",
	Import: "import", Auto: "auto", Extern: "extern", Struct: "struct", IntrinsicStruct: "intrinsic_struct", Protocol: "protocol", Enum: "enum", Constructor: "constructor", OnFree: "on_free", Private: "private", Public: "public", Return: "return", If: "if", Else: "else", While: "while", For: "for", Switch: "switch", Case: "case", Default: "default", Continue: "continue", Break: "break", True: "true", False: "false", Null: "null", As: "as", New: "new", Const: "const", Cast: "cast", This: "this",
	LParen: "(", RParen: ")", LBrace: "{", RBrace: "}", LBracket: "[", RBracket: "]", Less: "<", Greater: ">", Comma: ",", Dot: ".", Colon: ":", Semicolon: ";", Plus: "+", Minus: "-", Star: "*", Slash: "/", Percent: "%", Assign: "=", Bang: "!", Ampersand: "&", Pipe: "|", Tilde: "~", Equal: "==", NotEqual: "!=", LessEqual: "<=", GreaterEqual: ">=", AndAnd: "&&", OrOr: "||", PlusAssign: "+=", MinusAssign: "-=", StarAssign: "*=", SlashAssign: "/=", PercentAssign: "%=", Arrow: "->", FatArrow: "=>", Scope: "::", Ellipsis: "...", Increment: "++", Decrement: "--", Hash: "#",
}

func (k Kind) String() string {
	if s, ok := names[k]; ok {
		return s
	}
	return fmt.Sprintf("kind(%d)", k)
}
