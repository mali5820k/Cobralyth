#include "lexer.hpp"
#include <ctype.h>

// Lexer Class methods:
Lexer::Lexer() {
    
}
Lexer::~Lexer() {}

bool Lexer::lex_file(std::string file_name) {
    if (!this->file_scanner.read_file(file_name)) {
        this->lexer_error_string.push_back(std::string("ERROR: Could not open file for lexing... " + file_name));
        return false;
    }
    this->current_file_name = file_name;
    if (!this->lex_input(this->file_scanner.get_contents())) {
        this->lexer_error_string.push_back("ERROR: Could not lex scanned input.");
        return false;
    }

    return true;
}

bool Lexer::lex_input(std::vector<std::string> file_contents) {
    std::string current_input_string = "";
    bool in_string = false;
    fmt::println("Starting lex_input...");
    while (this->current_line_idx < file_contents.size()) {
        fmt::println("The current-Char is: {}", this->current_character());
        switch (this->current_character()[0]) {
            /**
             * For strings using double-quotes and backticks, and characters with single-quotes:
             */

            case '"': {
                this->lex_string();
                break;
            }
            case '`': {
                this->lex_backtick_string();
                break;
            }
            case '\'': {
                this->lex_char();
                break;
            }
            case 'i':
                // peek next char to check if it's a keyword:
                break;

            default : {
                break;
            }
        }
    }

    return true;
}

std::string Lexer::look_ahead() {
    std::string current_line = this->file_scanner.get_contents()[this->current_line_idx];

    // In cases where we're not done reading all chars in the current line, we can easily read the next char.
    if (current_line.length() > this->current_char_idx + 1) {
        fmt::println("Look ahead 1 char: '{current_line[this->current_char_idx + 1]}'");
        return fmt::format("{}", current_line[this->current_char_idx + 1]);
    }
    
    // For cases when we've reached the end of the current line and need to wrap around to the next line (if there is one)
    if (this->file_scanner.get_contents().size() > this->current_line_idx + 1) {
        current_line = this->file_scanner.get_contents()[this->current_line_idx + 1];
        fmt::println("Look ahead 1 char: '{current_line[0]}'");
        return fmt::format("{}", current_line[0]);
    }
    fmt::println("Reached end of file.");
    return ""; // empty string;
}

std::string Lexer::current_character() {
    std::string character = fmt::format("{}", this->file_scanner.get_contents()[this->current_line_idx][this->current_char_idx]);
    return character;
}

std::string Lexer::consume_character() {
    std::string character = this->current_character();
    fmt::println("Consuming the current character - checking if can move to next char...");
    if (this->file_scanner.get_contents()[this->current_line_idx].length() > this->current_char_idx) {
        fmt::println("Moving to next-char in line: {}", this->current_char_idx+1);
        this->current_line_idx += 1;
    } else {
        fmt::println("Moving to next-line: {}", this->current_line_idx+1);
        this->current_line_idx += 1;
        this->current_char_idx = 0;
    }
    fmt::println("Consumed character: {}", character);
    return character;
}

/**
 * ToString representation of the lexer class.
 * @returns std::string representation.
 */
std::string Lexer::to_string() {
    std::string token_string = "";
    for (auto &token : this->tokens_list) {
        token_string += token.value;
    }
    return token_string;
}

void Lexer::lex_string() {
    fmt::println("Parsing a string...");
    std::string string_value = this->consume_character();
    while (this->current_character() != "\"") {

        std::string consumed_char = this->consume_character();
        if (consumed_char != "\0") {
            string_value += consumed_char;
        } else {
            break;
        }
    }
    Token string_token = Token(string_value, TokenType::STRING, this->current_file_name, this->current_line_idx+1);
    fmt::println("Current double-quote-string lexed: {}", string_value);
}

void Lexer::lex_backtick_string() {
    std::string string_value = this->consume_character();
    while (this->current_character() != "`") {
        string_value += this->consume_character();
    }
    Token string_token = Token(string_value, TokenType::STRING, this->current_file_name, this->current_line_idx+1);
    fmt::println("Current backtick-string lexed: {}", string_value);
}

void Lexer::lex_char() {
    std::string string_value = this->consume_character();
    while (this->current_character() != "'") {
        string_value += this->consume_character();
    }
    Token string_token = Token(string_value, TokenType::CHAR, this->current_file_name, this->current_line_idx+1);
    fmt::println("Current char lexed: {}", string_value);
}

/**
 * Parses a given string to deduce the tokentype for it.
 * @param input_string of type std::string.
 * @returns TokenType enum value.
 */
TokenType get_token_type(std::string input_string) {
    if (input_string.length() < 0) {
        return TokenType::UNDEFINED;
    }
    if (std::isalpha(input_string[0]) && !std::isdigit(input_string[0])) {
        // This is an identifier
        return TokenType::IDENTIFIER;
    } else {
        // Check if this is a numeric type;
        bool numeric_value = true;
        for (int i = 0; i < input_string.length(); i++) {
            if (!std::isdigit(input_string[i])) {
                // This is an identifer 
                return TokenType::IDENTIFIER;
            }
        }
        if (!numeric_value) {
            return TokenType::UNDEFINED; // this->symbols_lookup_map.get(input_string);
        }

    }
    
    return TokenType::IDENTIFIER;
}

