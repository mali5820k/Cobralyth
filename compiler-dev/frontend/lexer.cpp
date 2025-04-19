#include "lexer.hpp"
#include "trie.hpp"

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

    for (int i, j = 0; i < file_contents.size(); i++) {
        char currentChar = file_contents[i][j];
        switch (currentChar) {
            /**
             * For strings, " and ' tokens:
             */
            case '"': {

            }
            case '\'': {

            }

            /**
             * For numeric literals, ints and float types;
             */
            case 
        }
    }

    return true;
}

void Lexer::lexString() {

}

TokenType getTokenType(std::string input_string) {
    if (input_string.length() < 0) {
        return UNDEFINED;
    }
    // This is a numeric type;
    if (std::isdigit(input_string[0])) {
        
    }
    for (int i = 0; i < input_string.length(); i++) {
        if (std::isdigit[i]) {

        }
    }
    if (std::isalpha(input_string.c_str()) && !std::isdigit(input_string[0])) {

    } else if (std::isalnum(input_string) && !std::isdigit(input_string[0])) {
        // this is an identifier;
    }

    return IDENTIFIER;
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