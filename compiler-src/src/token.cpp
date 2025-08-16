#include "token.hpp"

Token::Token(std::string value, TokenType type) {
    this->value = value;
    this->type = type;
}
Token::~Token(){}