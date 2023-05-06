package cobralyth_dev

import "fmt"

type TokenType int32

const (
	TOKEN_NULL TokenType = iota
	TOKEN_UNDEFINED
	TOKEN_IF
	TOKEN_ELSE
	TOKEN_FOR
	TOKEN_WHILE
	TOKEN_SQUARE_LEFT_BRACKET
	TOKEN_SQUARE_RIGHT_BRACKET
	TOKEN_CURLY_LEFT_BRACKET
	TOKEN_CURLY_RIGHT_BRACKET
	TOKEN_PAREN_LEFT
	TOKEN_PAREN_RIGHT
	TOKEN_VAR
	TOKEN_UINT8
	TOKEN_UINT16
	TOKEN_UINT32
	TOKEN_UINT64
	TOKEN_INT8
	TOKEN_INT16
	TOKEN_INT32
	TOKEN_INT64
	TOKEN_FLOAT
	TOKEN_DOUBLE
	TOKEN_CHAR
	TOKEN_STRING
	TOKEN_BYTE
	TOKEN_BOOL
	TOKEN_FUNCTION
	TOKEN_DIVIDE
	TOKEN_MULTIPLY
	TOKEN_PLUS
	TOKEN_MINUS
	TOKEN_PLUS_PLUS
	TOKEN_MINUS_MINUS
	TOKEN_PLUS_EQUALS
	TOKEN_MINUS_EQUALS
	TOKEN_TIMES_EQUALS
	TOKEN_DIVIDE_EQUALS
	TOKEN_RETURN
	TOKEN_BREAK
	TOKEN_CONTINUE
	TOKEN_SWITCH
	TOKEN_CASE
	TOKEN_DEFAULT
	TOKEN_SEMICOLON
	TOKEN_EQUALS
	TOKEN_EQUALS_EQUALS
	TOKEN_GREATER_THAN
	TOKEN_LESS_THAN
	TOKEN_GREATER_THAN_EQUALS
	TOKEN_LESS_THAN_EQUALS
	TOKEN_NOT_EQUALS
	TOKEN_LOGIC_OR
	TOKEN_LOGIC_AND
	TOKEN_LOGIC_NOT
	TOKEN_BIT_OR
	TOKEN_BIT_AND
	TOKEN_BIT_NOT
	TOKEN_DOUBLE_QUOTE
	TOKEN_SINGLE_QUOTE
	TOKEN_MODULO
	TOKEN_COMMENT_SINGLE
	TOKEN_COMMENT_MULTI_START
	TOKEN_COMMENT_MULTI_END
	TOKEN_DOCUMENT_START
	TOKEN_DOCUMENT_END
	TOKEN_DOT
	TOKEN_STRUCT
	TOKEN_CLASS
	TOKEN_PUBLIC
	TOKEN_PRIVATE
)

/**
 * String representation of all token-types for REPL
 */
func (tokenType TokenType) String() string {
	switch tokenType {
	case TOKEN_NULL:
		return "NULL"
	case TOKEN_UNDEFINED:
		return "UNDEFINED"
	case TOKEN_IF:
		return "IF"
	case TOKEN_ELSE:
		return "ELSE"
	case TOKEN_FOR:
		return "FOR"
	case TOKEN_WHILE:
		return "WHILE"
	case TOKEN_SQUARE_LEFT_BRACKET:
		return "["
	case TOKEN_SQUARE_RIGHT_BRACKET:
		return "]"
	case TOKEN_CURLY_LEFT_BRACKET:
		return "{"
	case TOKEN_CURLY_RIGHT_BRACKET:
		return "}"
	case TOKEN_PAREN_LEFT:
		return "("
	case TOKEN_PAREN_RIGHT:
		return ")"
	case TOKEN_VAR:
		return "VAR"
	case TOKEN_UINT8:
		return "UINT8"
	case TOKEN_UINT16:
		return "UINT16"
	case TOKEN_UINT32:
		return "UINT32"
	case TOKEN_UINT64:
		return "UINT64"
	case TOKEN_INT8:
		return "INT8"
	case TOKEN_INT16:
		return "INT16"
	case TOKEN_INT32:
		return "INT32"
	case TOKEN_INT64:
		return "INT64"
	case TOKEN_FLOAT:
		return "FLOAT"
	case TOKEN_DOUBLE:
		return "DOUBLE"
	case TOKEN_CHAR:
		return "CHAR"
	case TOKEN_STRING:
		return "STRING"
	case TOKEN_BYTE:
		return "BYTE"
	case TOKEN_BOOL:
		return "BOOL"
	case TOKEN_FUNCTION:
		return "FUNC"
	case TOKEN_DIVIDE:
		return "/"
	case TOKEN_MULTIPLY:
		return "*"
	case TOKEN_PLUS:
		return "+"
	case TOKEN_MINUS:
		return "-"
	case TOKEN_PLUS_PLUS:
		return "++"
	case TOKEN_MINUS_MINUS:
		return "--"
	case TOKEN_PLUS_EQUALS:
		return "+="
	case TOKEN_MINUS_EQUALS:
		return "-="
	case TOKEN_TIMES_EQUALS:
		return "*="
	case TOKEN_DIVIDE_EQUALS:
		return "/="
	case TOKEN_RETURN:
		return "RETURN"
	case TOKEN_BREAK:
		return "BREAK"
	case TOKEN_CONTINUE:
		return "CONTINUE"
	case TOKEN_SWITCH:
		return "SWITCH"
	case TOKEN_CASE:
		return "CASE"
	case TOKEN_DEFAULT:
		return "DEFAULT"
	case TOKEN_SEMICOLON:
		return ";"
	case TOKEN_EQUALS:
		return "="
	case TOKEN_EQUALS_EQUALS:
		return "=="
	case TOKEN_GREATER_THAN:
		return ">"
	case TOKEN_LESS_THAN:
		return "<"
	case TOKEN_GREATER_THAN_EQUALS:
		return ">="
	case TOKEN_LESS_THAN_EQUALS:
		return "<="
	case TOKEN_NOT_EQUALS:
		return "!="
	case TOKEN_LOGIC_OR:
		return "OR"
	case TOKEN_LOGIC_AND:
		return "AND"
	case TOKEN_LOGIC_NOT:
		return "BNOT"
	case TOKEN_BIT_OR:
		return "BOR"
	case TOKEN_BIT_AND:
		return "BAND"
	case TOKEN_BIT_NOT:
		return "BNOT"
	case TOKEN_DOUBLE_QUOTE:
		return "\""
	case TOKEN_SINGLE_QUOTE:
		return "'"
	case TOKEN_MODULO:
		return "%"
	case TOKEN_COMMENT_SINGLE:
		return "//"
	case TOKEN_COMMENT_MULTI_START:
		return "/*"
	case TOKEN_COMMENT_MULTI_END:
		return "*/"
	case TOKEN_DOCUMENT_START:
		return "/**"
	case TOKEN_DOCUMENT_END:
		return "*/"
	case TOKEN_DOT:
		return "."
	case TOKEN_STRUCT:
		return "STRUCT"
	case TOKEN_CLASS:
		return "CLASS"
	case TOKEN_PUBLIC:
		return "PUBLIC"
	case TOKEN_PRIVATE:
		return "PRIVATE"
	default:
		return ""
	}
}

type Token struct {
	Type     TokenType
	File     string
	Line     int64
	Contents string
	Value    string
}

func NewToken(args ...interface{}) *Token {
	// Default parameters:
	myToken := Token{TOKEN_UNDEFINED, "", 1, "", ""}
	if len(args) == 0 {
		return &myToken
	}

	switch len(args) {
	case 5:
		if x, isValid := args[4].(string); isValid {
			myToken.Value = x
		}
	case 4:
		if x, isValid := args[3].(string); isValid {
			myToken.Contents = x
		}
	case 3:
		if x, isValid := args[2].(int64); isValid {
			myToken.Line = x
		}
	case 2:
		// Check if the first argument/param is indeed a TokenType type.
		if x, isValid := args[1].(string); isValid {
			myToken.File = x
		}
	default:
		// Check if the first argument/param is indeed a TokenType type.
		if x, isValid := args[0].(TokenType); isValid {
			myToken.Type = x
		}
	}
	return &myToken
}

type Lexer struct {
	File          string
	lineNumber    int64
	scannedTokens []string
	currentChar   string
	nextChar      string
}

func NewLexer(args ...interface{}) *Lexer {
	// Default parameters:
	myLexer := Lexer{}
	myLexer.File = ""
	myLexer.lineNumber = 1
	myLexer.scannedTokens = []string{}
	myLexer.currentChar = ""
	myLexer.nextChar = ""

	if len(args) == 0 {
		return &myLexer
	}

	switch len(args) {
	case 5:
		if x, isValid := args[4].(string); isValid {
			myLexer.nextChar = x
		}
	case 4:
		if x, isValid := args[3].(string); isValid {
			myLexer.currentChar = x
		}
	case 3:
		if x, isValid := args[2].([]string); isValid {
			myLexer.scannedTokens = x
		}
	case 2:
		// Check if the first argument/param is indeed a LexerType type.
		if x, isValid := args[1].(int64); isValid {
			myLexer.lineNumber = x
		}
	default:
		// Check if the first argument/param is indeed a LexerType type.
		if x, isValid := args[0].(string); isValid {
			myLexer.File = x
		}
	}
	return &myLexer
}

func (lexer Lexer) String() string {
	repr := fmt.Sprintf("Lexer:\n\tFile=%s\n\tlineNumber=%d\n\tscannedTokens=%v\n\tcurrentChar=%s\n\tnextChar=%s\n", lexer.File, lexer.lineNumber, lexer.scannedTokens, lexer.currentChar, lexer.nextChar)
	return repr
}
