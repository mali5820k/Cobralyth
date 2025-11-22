
export enum TokenType {
    TOKEN_IF="if", TOKEN_ELSE="else", TOKEN_WHILE="while", TOKEN_FOR="for", TOKEN_BREAK="break", TOKEN_RETURN="return", TOKEN_CONTINUE="continue",
    TOKEN_AUTO="auto", TOKEN_CONST="const", TOKEN_TRY="try", TOKEN_CATCH="catch",
    TOKEN_INT8="int8", TOKEN_INT16="int16", TOKEN_INT32="int32", TOKEN_INT64="int64",
    TOKEN_UINT8="uint8", TOKEN_UINT16="uint16", TOKEN_UINT32="uint32", TOKEN_UINT64="uint64",
    TOKEN_FLOAT32="float32", TOKEN_FLOAT64="float64",
    TOKEN_CHAR="char", TOKEN_STRING="string", TOKEN_STRING_LITERAL="<string literal>", TOKEN_CHAR_LITERAL="<char literal>", TOKEN_BOOL="bool",
    TOKEN_FUNCTION="function", TOKEN_SCOPE="scope", TOKEN_STRUCT="struct", TOKEN_OPEN_CURLY_BRACE="{", TOKEN_CLOSE_CURLY_BRACE="}", TOKEN_OPEN_PAREN="(", TOKEN_CLOSE_PAREN=")", TOKEN_OPEN_BRACE="[", TOKEN_CLOSE_BRACE="]", TOKEN_COMMA=",", TOKEN_DOT=".", TOKEN_COLON=":", TOKEN_SEMICOLON=";",
    TOKEN_ADD="+", TOKEN_SUBTRACT="-", TOKEN_MULT="*", TOKEN_DIV="/", TOKEN_ADD_EQUALS="+=", TOKEN_SUBTRACT_EQUALS="-=", TOKEN_MULT_EQUALS="*=", TOKEN_DIV_EQUALS="/=", TOKEN_MOD_EQUALS="%=", TOKEN_MOD="%", TOKEN_ASSIGN="=", TOKEN_EQUALS="==", TOKEN_GEQUALS=">=", TOKEN_LEQALS="<=", TOKEN_NOT_EQUALS="!=", TOKEN_GREATER=">", TOKEN_LESSER="<", TOKEN_BANG="!", TOKEN_AND="&&", TOKEN_OR="||", TOKEN_BIT_AND="&", TOKEN_BIT_OR="|", TOKEN_BIT_BANG="~", TOKEN_BIT_XOR="^", TOKEN_COMMENT="//", TOKEN_BLOCK_COMMENT_START="/*", TOKEN_BLOCK_COMMENT_END="*/",
    TOKEN_IDENTIFIER="Type identifier", TOKEN_IMPORT="import", TOKEN_NULL="null", TOKEN_COMPILER_NULL="COMPILER_NULL", TOKEN_ESCAPE="\\",
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

export class Lexer {
    public file_string: string
    private file_contents: string[] = []
    private current_pos: number
    private current_line_number: number
    private tokens_list: Token[] = []

    constructor(file_string: string = "") {
        this.file_string = file_string
        this.current_pos = 0
        this.current_line_number = 0
    }

    has_next_line() {
        if (this.current_line_number + 1 <= this.file_contents.length -1 ) {
            return true
        }
        return false
    }

    has_next_char_across_lines() {
        const max_line_length = this.file_contents[this.current_line_number].length
        if (this.current_pos + 1 < max_line_length - 1) {
            return this.file_contents[this.current_line_number][this.current_pos + 1]
        } else if (this.has_next_line() && this.file_contents[this.current_line_number+1].length > 0) {
            return this.file_contents[this.current_line_number+1][0]
        } else {
            return null
        }
    }

    has_next_char_inline() {
        const max_line_length = this.file_contents[this.current_line_number].length
        if (this.current_pos + 1 < max_line_length - 1) {
            return this.file_contents[this.current_line_number][this.current_pos + 1]
        } else {
            return null
        }
    }

    get_next_char_across_lines() {
        if (this.has_next_char_across_lines()) {
            this.current_pos += 1
            return this.file_contents[this.current_pos]
        } else if (this.has_next_line()) {
            this.current_pos = 0
            if (this.file_contents[this.current_line_number+1].length -1 >= this.current_pos){
                this.current_line_number += 1
                return this.file_contents[this.current_line_number][this.current_pos]
            }
        } else {
            return null
        }
    }

    get_next_char_inline() {
        if (this.has_next_char_inline()) {
            this.current_pos += 1
            return this.file_contents[this.current_pos]
        } else {
            return null
        }
    }

    is_float_literal(string_match: string) {
        const float_test = new RegExp('^[0-9]*.[0-9]+ | ^[0-9]*.[0-9]+f', 'u')
        if (float_test.test(string_match)) {
            return true
        }

        return false
    }

    is_int_literal(string_match: string) {
        const int_test = new RegExp('^[01-9]+', 'u')
        if (int_test.test(string_match)) {
            return true
        }

        return false
    }

    is_digit(string_match: string) {
        const digit_test = new RegExp('[01-9]', 'u')
        if (digit_test.test(string_match)) {
            return true
        }

        return false
    }
    
    // Ran after looping through string contents to verify string is indeed valid.
    is_string_literal(string_match: string) {
        // NOTE - must check for double-quotes before this function is called
        const string_test = new RegExp('("([\\].|[^"\n\r\t]|[\n\r\t])*?")|(`([\\].|[^`\n\r\t]|[\n\r\t])*?`)', 'u')
        if (string_test.test(string_match)) {
            return true
        }

        return false
    }

    is_identifier(string_match: string) {
        const identifier_test = new RegExp('^_*[a-z]*[A-Z]*', 'u')
        if (identifier_test.test(string_match)) {
            return true
        }

        return false
    }

    async tokenize_file(file_path ?: null | string) {
        if (file_path == null) {
            file_path = this.file_string
        }
        if (file_path.length == 0) {
            console.log("ERROR: empty file-path provided to Lexer!")
            return []
        }

        this.tokens_list = []

        this.file_contents = (await Deno.readTextFile(this.file_string)).split('\n')
        console.log(`The file contents are: ${this.file_contents}`)

        // Reset file-reader to the top of the file:
        this.current_line_number = 0
        this.current_pos = 0

        // Lex the file here and return the results;
        let char_buffer = ""

        // Reading the entire file line by line:
        while (this.current_line_number <= this.file_contents.length-1) {
            const next_char = this.get_next_char_across_lines()
            if (next_char == null) {
                return
            }
            char_buffer += next_char
            console.log("Current char_buffer is:", char_buffer)
            // Strings have their own completion-loop until the terminating symbols are encountered - ie double quotes or back-ticks for formatted strings:
            const string_token = new Token(TokenType.TOKEN_STRING_LITERAL, "", this.current_line_number+1, this.current_pos+1)

            if (char_buffer[0] == '"' || char_buffer[0] == '`') {
                console.log("Encountered a string\n")
                // Expect to parse the entire line until a double-quote is found to end (non-escaped):
                let string_closed = false
                const string_term_symbol = char_buffer[0]
                string_token.contents += char_buffer
                char_buffer = ""
                while (!string_closed) {
                    console.log("Processing a string...")
                    if (this.has_next_char_across_lines()) {
                        char_buffer += this.get_next_char_across_lines()
                        string_token.contents += char_buffer
                        console.log(`Current string token contents in string block: ${string_token.contents}`)
                        // Otherwise, consume all whitespace and intermediate characters that're before the last string_term_symbol:
                        switch (char_buffer) {
                            case " ":
                                char_buffer = ""
                                break
                            case "\t":
                                char_buffer = ""
                                break
                            case "\\":
                                if (this.has_next_char_across_lines()) {
                                    continue
                                } 
                                break
                            case "\n":
                                char_buffer = ""
                                break
                            case "\r\n":
                                char_buffer = ""
                                break
                            default:
                                if (char_buffer == string_term_symbol) {
                                    string_closed = true
                                    // Do a verification check that the string scanned in is actually a valid string:
                                    if (!this.is_string_literal(string_token.contents)) {
                                        // This should be an error...
                                        console.log(`ERROR: Invalid string at line: ${string_token.line} col: ${string_token.column}`)
                                    }
                                }
                                break
                        }
                    } else {
                        // This should be an error...
                        console.log(`ERROR: Incomplete string at line: ${string_token.line} col: ${string_token.column}`)
                    }
                }
            }
            // Consume whitespace and comments now that we know we're not in a string:
            while (char_buffer == " " || char_buffer == "\n" || char_buffer == "\t" || char_buffer == "\r\n") {
                char_buffer = ""
                console.log("Consuming whitespace...\n")
                continue
            }

            // Numeric literal check:
            if (this.is_digit(char_buffer)) {
                console.log("Is a digit - testing for float and int possibilities...")
                let is_float = false
                // Keep consuming characters until either a '.', space, or some other character is encountered which isn't that of an int
                for (const current_char = this.has_next_char_inline(); current_char != null;) {
                    if (!this.is_digit(current_char)) {
                        char_buffer += this.get_next_char_inline()
                        continue
                    } else if (current_char == "." || current_char == "f") {
                        // Now we hit a float so set the float-flag to true and break
                        is_float = true
                        console.log(`This is a float!: ${current_char}`)
                        break
                    } else {
                        // We just hit a regular integer:
                        const token_entry = new Token(TokenType.TOKEN_INT32, `${char_buffer}`, this.current_line_number+1, this.current_pos+1)    
                        this.tokens_list.push(token_entry)
                        console.log(`This is an int!: ${token_entry.contents}`)
                        continue
                    }
                }

                // Only continue processing as float if we know it's not an int from earlier:
                if (!is_float) {
                    continue;
                }
                // Read in the remainder of the numerical literal until the next character isn't a digit to process the float:
                for (const current_char = this.has_next_char_inline(); current_char != null;) {
                    if (!this.is_digit(current_char)) {
                        // previous position is the last index value of the float:
                        console.log(`This is a float!: ${char_buffer}`)
                        break
                    } else {
                        char_buffer += this.get_next_char_inline()
                        continue
                    }
                }
                const token_entry = new Token(TokenType.TOKEN_FLOAT32, `${char_buffer}`, this.current_line_number+1, this.current_pos+1)    
                        this.tokens_list.push(token_entry)
            }
            
            // Special symbols/operations check:
            console.log(`Checking if special symbol or operation: ${char_buffer}`)
            switch (char_buffer) {
                case "+":
                    if (this.has_next_char_inline() && this.has_next_char_inline() == "=") {
                        // Encountered a "+=" operator:
                        char_buffer += this.get_next_char_inline()
                        const token_entry = new Token(TokenType.TOKEN_ADD_EQUALS, char_buffer, this.current_line_number+1, this.current_pos+1)
                        this.tokens_list.push(token_entry)
                        char_buffer = ""
                    } else {
                        // Encountered a "+" operator:
                        const token_entry = new Token(TokenType.TOKEN_ADD, char_buffer, this.current_line_number+1, this.current_pos+1)
                        this.tokens_list.push(token_entry)
                        char_buffer = ""
                    }
                    continue
                case "-":
                    if (this.has_next_char_inline() && this.has_next_char_inline() == "=") {
                        // Encountered a "-=" operator:
                        char_buffer += this.get_next_char_inline()
                        const token_entry = new Token(TokenType.TOKEN_SUBTRACT_EQUALS, char_buffer, this.current_line_number+1, this.current_pos+1)
                        this.tokens_list.push(token_entry)
                        char_buffer = ""
                    } else {
                        // Encountered a "-" operator:
                        const token_entry = new Token(TokenType.TOKEN_SUBTRACT, char_buffer, this.current_line_number+1, this.current_pos+1)
                        this.tokens_list.push(token_entry)
                        char_buffer = ""
                    }
                    continue
                case "*":
                    if (this.has_next_char_inline() && this.has_next_char_inline() == "=") {
                        // Encountered a "*=" operator:
                        char_buffer += this.get_next_char_inline()
                        const token_entry = new Token(TokenType.TOKEN_MULT_EQUALS, char_buffer, this.current_line_number+1, this.current_pos+1)
                        this.tokens_list.push(token_entry)
                        char_buffer = ""
                    } else {
                        // Encountered a "*" operator:
                        const token_entry = new Token(TokenType.TOKEN_MULT, char_buffer, this.current_line_number+1, this.current_pos+1)
                        this.tokens_list.push(token_entry)
                        char_buffer = ""
                    }
                    continue
                case "/":
                    if (this.has_next_char_inline() && this.has_next_char_inline() == "=") {
                        // Encountered a "/=" operator:
                        char_buffer += this.get_next_char_inline()
                        const token_entry = new Token(TokenType.TOKEN_DIV_EQUALS, char_buffer, this.current_line_number+1, this.current_pos+1)
                        this.tokens_list.push(token_entry)
                        char_buffer = ""
                    } else {
                        // Encountered a "/" operator:
                        const token_entry = new Token(TokenType.TOKEN_DIV, char_buffer, this.current_line_number+1, this.current_pos+1)
                        this.tokens_list.push(token_entry)
                        char_buffer = ""
                    }
                    continue
                case "%":
                    if (this.has_next_char_inline() && this.has_next_char_inline() == "=") {
                        // Encountered a "%=" operator:
                        char_buffer += this.get_next_char_inline()
                        const token_entry = new Token(TokenType.TOKEN_MOD_EQUALS, char_buffer, this.current_line_number+1, this.current_pos+1)
                        this.tokens_list.push(token_entry)
                        char_buffer = ""
                    } else {
                        // Encountered a "%" operator:
                        const token_entry = new Token(TokenType.TOKEN_MOD, char_buffer, this.current_line_number+1, this.current_pos+1)
                        this.tokens_list.push(token_entry)
                        char_buffer = ""
                    }
                    continue
                case "(": {
                    const token_entry = new Token(TokenType.TOKEN_OPEN_PAREN, char_buffer, this.current_line_number+1, this.current_pos+1)
                    this.tokens_list.push(token_entry)
                    char_buffer = ""
                    continue
                }
                case ")": {
                    const token_entry = new Token(TokenType.TOKEN_CLOSE_PAREN, char_buffer, this.current_line_number+1, this.current_pos+1)
                    this.tokens_list.push(token_entry)
                    char_buffer = ""
                    continue
                }
                case "{": {
                    const token_entry = new Token(TokenType.TOKEN_OPEN_CURLY_BRACE, char_buffer, this.current_line_number+1, this.current_pos+1)
                    this.tokens_list.push(token_entry)
                    char_buffer = ""
                    continue
                }
                case "}": {
                    const token_entry = new Token(TokenType.TOKEN_CLOSE_CURLY_BRACE, char_buffer, this.current_line_number+1, this.current_pos+1)
                    this.tokens_list.push(token_entry)
                    char_buffer = ""
                    continue
                }
                case "[": {
                    const token_entry = new Token(TokenType.TOKEN_OPEN_BRACE, char_buffer, this.current_line_number+1, this.current_pos+1)
                    this.tokens_list.push(token_entry)
                    char_buffer = ""
                    continue
                }
                case "]": {
                    const token_entry = new Token(TokenType.TOKEN_CLOSE_BRACE, char_buffer, this.current_line_number+1, this.current_pos+1)
                    this.tokens_list.push(token_entry)
                    char_buffer = ""
                    continue
                }
                default:
                    break
            }

            // Identifier check:
            console.log("This may very well just be an identifier - we'll get the whole thing until the next character makes the whole thing not a valid identifier\n")

        }
        console.log(`Token list is: ${this.tokens_list}`)
        return this.tokens_list
    }
}
