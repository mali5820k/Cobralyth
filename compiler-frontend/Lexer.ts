import { Trie } from 'npm:@kamilmielnik/trie' // May need to come up with my own Trie implementation so I can use the LexerTrieEntry type instead of just strings;

export enum TokenType {
    TOKEN_IF="if", TOKEN_ELSE="else", TOKEN_WHILE="while", TOKEN_FOR="for", TOKEN_BREAK="break", TOKEN_RETURN="return", TOKEN_CONTINUE="continue",
    TOKEN_AUTO="auto", TOKEN_CONST="const", TOKEN_TRY="try", TOKEN_CATCH="catch",
    TOKEN_INT8="int8", TOKEN_INT16="int16", TOKEN_INT32="int32", TOKEN_INT64="int64",
    TOKEN_UINT8="uint8", TOKEN_UINT16="uint16", TOKEN_UINT32="uint32", TOKEN_UINT64="uint64",
    TOKEN_FLOAT32="float32", TOKEN_FLOAT64="float64",
    TOKEN_CHAR="char", TOKEN_STRING="string", TOKEN_STRING_LITERAL="<string literal>", TOKEN_CHAR_LITERAL="<char literal>", TOKEN_BOOL="bool", TOKEN_DOUBLE_QUOTE="\"", TOKEN_SINGLE_QUOTE="'",
    TOKEN_FUNCTION="function", TOKEN_SCOPE="scope", TOKEN_STRUCT="struct", TOKEN_OPEN_CURLY_BRACE="{", TOKEN_CLOSE_CURLY_BRACE="}", TOKEN_OPEN_PAREN="(", TOKEN_CLOSE_PAREN=")", TOKEN_OPEN_BRACE="[", TOKEN_CLOSE_BRACE="]", TOKEN_COMMA=",", TOKEN_DOT=".", TOKEN_COLON=":", TOKEN_SEMICOLON=";",
    TOKEN_ADD="+", TOKEN_SUBTRACT="-", TOKEN_MULT="*", TOKEN_DIV="/", TOKEN_REMAINDER="%", TOKEN_ASSIGN="=", TOKEN_EQUALS="==", TOKEN_GEQUALS=">=", TOKEN_LEQALS="<=", TOKEN_GREATER=">", TOKEN_LESSER="<", TOKEN_BANG="!", TOKEN_AND="&&", TOKEN_OR="||", TOKEN_BIT_AND="&", TOKEN_BIT_OR="|", TOKEN_BIT_BANG="~", TOKEN_BIT_XOR="^", TOKEN_COMMENT="//", TOKEN_BLOCK_COMMENT_START="/*", TOKEN_BLOCK_COMMENT_END="*/",
    TOKEN_IDENTIFIER="Type identifier", TOKEN_IMPORT="import", TOKEN_NULL="null", TOKEN_COMPILER_NULL="COMPILER_NULL"
}

export class Token {
    public token_type: TokenType
    public contents: string
    public line: number
    public column: number

    constructor(token_type: TokenType, contents: string, line: number, column: number) {
        this.token_type = token_type
        this.contents = contents
        this.line = line
        this.column = column
    }
}

class LexerTrieEntry {
    public character :string = ""
    public mapped_token_type = TokenType.TOKEN_COMPILER_NULL
    constructor(character: string, mapped_token_type: TokenType) {
        this.character = character
        this.mapped_token_type = mapped_token_type
    }
}

export class Lexer {
    public file_string: string
    private current_pos: number
    private current_line_number: number
    private lexer_trie = new Trie()

    constructor(file_string: string = "") {
        this.file_string = file_string
        this.current_pos = 0
        this.current_line_number = 0
        this.lexer_trie.add(new LexerTrieEntry("if", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("else", TokenType.TOKEN_ELSE))
        this.lexer_trie.add(new LexerTrieEntry("for", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("while", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("break", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("return", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("continue", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("function", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("string", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("char", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("int8", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("int16", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("int32", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("int", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("int64", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("uint8", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("uint16", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("uint32", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("uint", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("uint64", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("float32", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("float64", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("bool", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("struct", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("import", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry(".", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry(",", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry(";", TokenType.TOKEN_IF))
        this.lexer_trie.add(new LexerTrieEntry("'", TokenType.TOKEN_SINGLE_QUOTE))
        this.lexer_trie.add(new LexerTrieEntry("\"", TokenType.TOKEN_DOUBLE_QUOTE))
    }

    lookahead() {
        if (this.current_pos + 1 < this.file_string.length) {
            return this.file_string[this.current_pos + 1]
        }

        return null
    }

    tokenize_string(file_string ?: null | string) {
        if (file_string == null) {
            file_string = this.file_string
        }
        if (file_string.length == 0) {
            console.log("ERROR: empty token string received by lexer!")
            return []
        }

        // Lex the string here and return the results;
        while (this.lookahead() != null) {
            break;
        }
        console.log(lexer_trie)
        return []
    }
}
