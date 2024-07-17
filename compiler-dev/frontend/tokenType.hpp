#ifndef token_type_hpp
#define token_type_hpp
#include "common.hpp"

enum Token_Type {
    IF, ELSE, LET, CONST, COLON, SEMICOLON, DOT, CURLY_BRACKET_OPEN, CURLY_BRACKET_CLOSE,
    SQUARE_BRACKET_OPEN, SQUARE_BRACKET_CLOSE, PARENTHESIS_OPEN, PARENTHESIS_CLOSE,
    EQUALS, EQUALS_EQUALS, GREATER_THAN, LESS_THAN, BANG, NOT, AND, OR, BIN_NOT, BIN_AND, BIN_OR, BIN_XOR,
    FORWARD_SLASH, STAR, MINUS_SIGN, MODULO, COMMA, LITERAL, IDENTIFIER,
};

/**
 * Allows for lookup of token_types given a particular string.
 */
std::map<std::string, Token_Type> token_lookup_table {
    {std::string("if"), IF},
    {std::string("else"), ELSE},
};

#endif