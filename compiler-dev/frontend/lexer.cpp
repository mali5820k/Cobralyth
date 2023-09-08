#include "lexer.hpp"

Token::Token(int tokenLine, std::string tokenValue, TokenType tokenType) {
    this->tokenLine = tokenLine;
    this->tokenValue = tokenValue;
    this->tokenType = tokenType;
}

std::string Token::toString() {
    std::string representation("Token line: " + std::to_string(this->tokenLine) + "\nToken type: "+ std::to_string(this->tokenType) + "\nToken value:" + this->tokenValue + "\n\n");
    return representation;
}
