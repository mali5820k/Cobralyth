package cobralyth_dev

type TokenType int32

type Token struct {
	Type     TokenType
	File     string
	Line     int64
	Contents string
	Value    string
}

type Lexer struct {
	File          string
	lineNumber    int64
	scannedTokens []string
	currentChar   rune
	nextChar      rune
}
