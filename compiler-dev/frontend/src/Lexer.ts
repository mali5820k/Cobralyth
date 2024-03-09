import {TokenType, Token} from "./Token"
class Lexer  {
    private current_file:string = ""
    private module_import_entries: string[] = []
    private current_token = ""
    private token_line = ""
    private token_line_number = -1
    private token_type: TokenType = TokenType.COMPILER_NOT_SET
    private tokens_list: Token[] = []
    private active_file: string[] = []

    constructor() {
        
    }
    
    /**
     * Reads in the specified file for lexing (the entrypoint)
     * @param file_name File path to read in
     * @returns returns a map of type <string, Token[]>, file-paths to their lexed Tokens.
     */
    scanFile(file_name: string) {
        let lexed_files = new Map<string, Token[]>([])
        return lexed_files
    }

    /**
     * Lexes the current line in the active_file being read in.
     */
    lexLine() {}

    /**
     * Consumes the current character and grabs the next character.
     * Returns an object containing new character and a flag if end-of-line is reached.
     */
    consumeChar() {
        return { current_character: "", end_of_line: false }
    }

    /**
     * Takes the currently-collected characters and traverses the KeywordTrie to determine
     * if it is a keyword or not.
     */
    checkMatch() {}
}


export default Lexer;