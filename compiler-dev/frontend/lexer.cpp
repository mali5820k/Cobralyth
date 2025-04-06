#include "lexer.hpp"

// Lexer Class methods:
Lexer::Lexer() {
    addKeywords();
    addSymbols();
}
Lexer::~Lexer() {}

bool Lexer::lexFile(std::string file_name) {
    if (!this->file_scanner->readFile(file_name)) {
        this->lexer_error_string.push_back(std::string("ERROR: Could not open file for lexing... " + file_name));
        return false;
    }
    if (!this->lexInput(this->file_scanner->getContents())) {
        this->lexer_error_string.push_back("ERROR: Could not lex scanned input.");
        return false;
    }

    return true;
}

bool Lexer::lexInput(std::vector<std::string> file_contents) {
    std::string current_input_string = "";
    bool in_string = false;
    int j = 0;

    Token last_valid_token = Token(current_input_string, NULL_);
    for (int i = 0; i < file_contents.size(); i++) {
        // Strings:
        std::vector<std::string> current_line; // todo: split current line by white-spaces if not enclosed by string quotes.
        if (file_contents[i][j] == char("\"") || file_contents[i][j] == char("'") && !in_string) {
            std::string quotes_type = "\"";
            quotes_type = file_contents[i][j];
            in_string = true;
            // Consume string until next double-quotes in current line - ignoring escape characters;
            while (j + 1 < file_contents.size()) {
                if (!(file_contents[i][j+1] == char("\"") && file_contents[i][j] != char("\\"))) {
                    current_input_string += file_contents[i][j];
                    j++;
                    continue;
                } else {
                    break;
                }
            }
            current_input_string += file_contents[i];
            this->tokens_list.push_back(Token(current_input_string, STRING));
            in_string = false;
            current_input_string = "";
        } else if (this->keywords_lookup_map.find(current_input_string) != this->keywords_lookup_map.end()) {
            last_valid_token = Token(current_input_string, this->keywords_lookup_map[current_input_string.c_str()]);
        } else if (this->symbols_lookup_map.find(current_input_string) != this->symbols_lookup_map.end()) {
            last_valid_token = Token(current_input_string, this->keywords_lookup_map[current_input_string.c_str()]);
        } else {

        }
    }
    return true;
}

bool Lexer::addKeywords() {
    this->keywords_lookup_map = {
        {"(", LEFT_PAREN},
        {")", RIGHT_PAREN},
        {"[", LEFT_BRACK},
        {"]", RIGHT_BRACK},
        {"{", LEFT_CURLY},
        {"}", RIGHT_CURLY},
        {".", DOT},
        {",", COMMA},
        {";", SEMICOLON},
        {":", COLON},
        {"\"", DOUBLE_QUOTE},
        {"'", SINGLE_QUOTE},
        {"*", STAR},
        {"*=", STAR_EQUALS},
        {"**", STAR_STAR},
        {"/", FORWARDS_SLASH},
        {"\\", BACKWARDS_SLASH},
        {"+", PLUS},
        {"-", MINUS},
        {"--", MINUS_MINUS},
        {"%", MODULO},
        {"%=", MODULO_EQUALS},
        {"/=", SLASH_EQUALS},
        {"=", EQUAL},
        {"==", EQUAL_EQUAL},
        {">", GREATER_THAN},
        {"<", LESS_THAN},
        {">=", GREATER_EQUAL},
        {"<=", LESSER_EQUAL},
        {"!", NOT},
        {"!=", NOT_EQUAL},
        {"&&", AND},
        {"||", OR},
        {"&", BIT_AND},
        {"|", BIT_OR},
        {"^", BIT_XOR},
        {"~", BIT_NOT},
        {"#", POUND},
    };

    return true;
}

bool Lexer::addSymbols() {
    this->symbols_lookup_map = {
        {"if", IF},
        {"else", ELSE},
        {"try", TRY},
        {"catch", CATCH},
        {"switch", SWITCH},
        {"case", CASE},
        {"default", DEFAULT},
        {"for", FOR},
        {"while", WHILE},
        {"break", BREAK},
        {"continue", CONTINUE},
        {"return", RETURN},
        {"auto", AUTO},
        {"const", CONST},
        {"null", NULL_},
        {"new", NEW},
        {"free", FREE},
        {"int8", INT8},
        {"int16", INT16},
        {"int32", INT32},
        {"int64", INT64},
        {"uin8", UINT8},
        {"uint16", UINT16},
        {"uint32", UINT32},
        {"uint64", UINT64},
        {"float32", FLOAT32},
        {"float64", FLOAT64},
        {"char", CHAR},
        {"string", STRING},
    };

    return true;
}

std::string Lexer::toString() {
    std::string token_string = "";
    for (auto &token : this->tokens_list) {
        token_string += token.value;
    }
    return token_string;
}