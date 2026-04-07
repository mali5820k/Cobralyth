#include "token.hpp"

Token::Token(std::string value, TokenType type, std::string file_name, int64_t line_number) {
    this->value = value;
    this->type = type;
    this->file_name = file_name;
    this->line_number = line_number;
}
Token::~Token(){}

std::string Token::to_string() {
    std::string assembled_string = fmt::format("Token Type: {this->type}, Token Value: {this->value}");
    return assembled_string;
}