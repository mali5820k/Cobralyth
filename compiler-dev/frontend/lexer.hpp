#ifndef lexer_hpp
#define lexer_hpp
#include "common.hpp"
#include "token.hpp"

class Lexer {
    public:
        std::vector<Token> lex_string(std::string code);
        std::vector<Token> lex_file(std::vector<std::string> code);
};

#endif