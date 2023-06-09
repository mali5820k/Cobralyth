package cobralyth_dev

import (
	"bufio"
	"errors"
	"fmt"
	"os"
	"unicode"
)

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
	TOKEN_INT8
	TOKEN_INT16
	TOKEN_INT32
	TOKEN_INT64
	TOKEN_UINT8
	TOKEN_UINT16
	TOKEN_UINT32
	TOKEN_UINT64
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
	TOKEN_DOT
	TOKEN_STRUCT
	TOKEN_CLASS
	TOKEN_PUBLIC
	TOKEN_PRIVATE
	TOKEN_LITERAL
)

// Map for string to keyword token-type lookup
var languageDictionary = map[string]TokenType{
	"null":     TOKEN_NULL,
	"if":       TOKEN_IF,
	"else":     TOKEN_ELSE,
	"for":      TOKEN_FOR,
	"[":        TOKEN_SQUARE_LEFT_BRACKET,
	"]":        TOKEN_SQUARE_RIGHT_BRACKET,
	"{":        TOKEN_CURLY_LEFT_BRACKET,
	"}":        TOKEN_CURLY_RIGHT_BRACKET,
	"(":        TOKEN_PAREN_LEFT,
	")":        TOKEN_PAREN_RIGHT,
	"var":      TOKEN_VAR,
	"uUINT8":   TOKEN_INT8,
	"uUINT16":  TOKEN_INT16,
	"uUINT32":  TOKEN_INT32,
	"uUINT64":  TOKEN_INT64,
	"UINT8":    TOKEN_UINT8,
	"UINT16":   TOKEN_UINT16,
	"UINT32":   TOKEN_UINT32,
	"UINT64":   TOKEN_UINT64,
	"float":    TOKEN_FLOAT,
	"double":   TOKEN_DOUBLE,
	"char":     TOKEN_CHAR,
	"string":   TOKEN_STRING,
	"byte":     TOKEN_BYTE,
	"bool":     TOKEN_BOOL,
	"func":     TOKEN_FUNCTION,
	"/":        TOKEN_DIVIDE,
	"*":        TOKEN_MULTIPLY,
	"+":        TOKEN_PLUS,
	"-":        TOKEN_MINUS,
	"++":       TOKEN_PLUS_PLUS,
	"--":       TOKEN_MINUS_MINUS,
	"+=":       TOKEN_PLUS_EQUALS,
	"-=":       TOKEN_MINUS_EQUALS,
	"*=":       TOKEN_TIMES_EQUALS,
	"/=":       TOKEN_DIVIDE_EQUALS,
	"return":   TOKEN_RETURN,
	"break":    TOKEN_BREAK,
	"continue": TOKEN_CONTINUE,
	"switch":   TOKEN_SWITCH,
	"case":     TOKEN_CASE,
	"default":  TOKEN_DEFAULT,
	";":        TOKEN_SEMICOLON,
	"=":        TOKEN_EQUALS,
	"==":       TOKEN_EQUALS_EQUALS,
	">":        TOKEN_GREATER_THAN,
	"<":        TOKEN_LESS_THAN,
	">=":       TOKEN_GREATER_THAN_EQUALS,
	"<=":       TOKEN_LESS_THAN_EQUALS,
	"!=":       TOKEN_NOT_EQUALS,
	"or":       TOKEN_LOGIC_OR,
	"and":      TOKEN_LOGIC_AND,
	"bnot":     TOKEN_BIT_NOT,
	"bor":      TOKEN_BIT_OR,
	"band":     TOKEN_BIT_AND,
	"!":        TOKEN_LOGIC_NOT,
	"not":      TOKEN_LOGIC_NOT,
	"\"":       TOKEN_DOUBLE_QUOTE,
	"'":        TOKEN_SINGLE_QUOTE,
	"%":        TOKEN_MODULO,
	"//":       TOKEN_COMMENT_SINGLE,
	"/*":       TOKEN_COMMENT_MULTI_START,
	"*/":       TOKEN_COMMENT_MULTI_END,
	".":        TOKEN_DOT,
	"struct":   TOKEN_STRUCT,
	"class":    TOKEN_CLASS,
	"public":   TOKEN_PUBLIC,
	"private":  TOKEN_PRIVATE,
	"literal":  TOKEN_LITERAL,
}

var KeywordsTrie *TrieNode

func GenerateKeywordsTrie() (bool, error) {
	if KeywordsTrie == nil {
		KeywordsTrie = NewTrieNode('#', TOKEN_UNDEFINED, 0)
	}
	for key := range languageDictionary {
		success := addStringToTree(key, KeywordsTrie)
		if !success {
			return false, errors.New("ERROR: Failed to add partial or total keywords to trie root object")
		}
	}

	return true, nil
}

// String representation of all token-types for REPL
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
	case TOKEN_INT8:
		return "INT8"
	case TOKEN_INT16:
		return "INT16"
	case TOKEN_INT32:
		return "INT32"
	case TOKEN_INT64:
		return "INT64"
	case TOKEN_UINT8:
		return "UINT8"
	case TOKEN_UINT16:
		return "UINT16"
	case TOKEN_UINT32:
		return "UINT32"
	case TOKEN_UINT64:
		return "UINT64"
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
		return "LNOT"
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

// TokenType, Token-file, Token-line, Token-Contents, Token-Value
// Token-Value is the literal value of that token, so if an UINT, it's a value of type UINT.
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
	FilePath          string
	scannedTokens     []Token
	secondaryPaths    []string
	fileContentsIndex int64
	fileContents      []string
	lineNumber        int64
	scanBuffer        string
	errorLog          []string
}

// Returns a new Lexer object to scan and generate tokens from a particular file
// Variadic arguments allow for default variable assignments
// The order of param-evaluation is: filepath, line number, scanned tokens list, current character, and next character.
func NewLexer(FilePath string) Lexer {
	// Default parameters:
	myLexer := Lexer{}
	myLexer.FilePath = FilePath
	myLexer.lineNumber = 1
	myLexer.scannedTokens = []Token{}
	myLexer.secondaryPaths = []string{}
	myLexer.fileContentsIndex = 0
	myLexer.fileContents = []string{}
	myLexer.lineNumber = 0
	myLexer.scanBuffer = ""
	myLexer.errorLog = []string{}
	return myLexer
}

// All files stored in a directory can be imported separately from the package name: "PackageName.filename".
// All directories can be a package if they contain a package-header file: "PackageName.clythp".
// For package installations and package management, a particular directory can be defined.
// That way, all packages are stored in a standard location and can be referred to by name.
func (lexer *Lexer) ScanFile() bool {
	// Line buffers:
	var currentFileLines []string
	// Start by scanning the main file
	file, err := os.Open(lexer.FilePath)
	if err != nil {
		fmt.Printf("\n\nERROR: FILE NOT FOUND! Filename: %s", lexer.FilePath)
		return false
	}
	defer file.Close()

	fileScanner := bufio.NewScanner(file)

	// This will read in token by token.
	for fileScanner.Scan() {
		// Subsequent calls to fileScanner.Text() will return fully scanned lines.
		currentFileLines = append(currentFileLines, fileScanner.Text())
	}

	// Lex the input strings UINTo tokens:
	lexer.Tokenize(currentFileLines)

	// We've tokenized the main file
	return true
}

func (lexer *Lexer) lexingError(errorMessage string) {
	// Add error message to Lexer Logs:
	lexingError := errorMessage + "\n"
	lexer.errorLog = append(lexer.errorLog, lexingError)
}

// We'll append to the list of "other-files" for a package
// This function's approach is inspired by the approach written in "Crafting-Interpreters" from my own memories of what
// Robert (Bob) Nystrom explained, however, this approach is unique as it's traversing a Trie for language keywords and types.
func (lexer *Lexer) Tokenize(fileLines []string) (bool, error) {
	lexer.scanBuffer = ""
	currentTrieNode := KeywordsTrie
	var trackingType TokenType = TOKEN_UNDEFINED

	// For each line, we want to perform a Trie-traversal for each token encountered.
	for index, line := range fileLines {
		// Looping through each character in this current-line's loop iteration.
		for i := 0; i < len(line); i++ {
			currentChar := line[i]

			if unicode.IsDigit(rune(currentChar)) {
				// Need to know what type of digit we're dealing with:
				// IE is it a part of a literal, variable declaration, or by itself?
				// Since strings will be parsed without hitting this section at all
				if len(lexer.scannedTokens) == 0 || i == 0 {
					// This is a problem as it's a number on a line somewhere without anything prefixing it.
					// so we'll throw an error
					lexer.lexingError("LEXING ERROR: Found number without context: " + string(currentChar) + "\nOn line: " + string(index))
					continue
				}
				lastToken := lexer.scannedTokens[len(lexer.scannedTokens)-1]
				switch lastToken.Type {
				case TokenType(TOKEN_DOUBLE):
					trackingType = TOKEN_DOUBLE
					break
				case TOKEN_FLOAT:
					trackingType = TOKEN_FLOAT
					break
				case TOKEN_INT8:
					trackingType = TOKEN_UINT8
					break
				case TOKEN_INT16:
					trackingType = TOKEN_UINT16
					break
				case TOKEN_INT32:
					trackingType = TOKEN_UINT32
					break
				case TOKEN_INT64:
					trackingType = TOKEN_UINT64
					break
				case TOKEN_UINT8:
					trackingType = TOKEN_UINT8
					break
				case TOKEN_UINT16:
					trackingType = TOKEN_UINT16
					break
				case TOKEN_UINT32:
					trackingType = TOKEN_UINT32
					break
				case TOKEN_UINT64:
					trackingType = TOKEN_UINT64
					break
				default:
					lexer.lexingError("LEXING ERROR: Found invalid digit type: " + string(currentChar) + "\nOn line: " + string(index))
				}
			} else {
				nextTrieNode, found := currentTrieNode.getNeighborWith(rune(currentChar))

				if !found && len(line)-i > 1 {
					// Either we hit a literal, or
					// we didn't find any matches.
					if currentTrieNode.getTokenType() == TOKEN_LITERAL {
						// Then this is likely a literal or function call.
					}
					continue
				} else {
					lexer.scanBuffer += string(currentChar)
					currentTrieNode = nextTrieNode
					continue
				}
			}
		}
	}
	return true, nil
}

func isKeyword(wordBuffer string) (bool, error) {

	return false, nil
}

func (lexer Lexer) String() string {
	repr := fmt.Sprintf("Lexer:\n\tFile=%s\n\tlineNumber=%d\n\tscannedTokens=%v\n", lexer.FilePath, lexer.lineNumber, lexer.scannedTokens)
	return repr
}
