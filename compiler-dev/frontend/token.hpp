#ifndef token_hpp
#define token_hpp
#include "common.hpp"
#include "tokenType.hpp"

class Token {
    public:
        int line_number;
        int line_col_position;
        std::string token_text;
        Token_Type token_type;
};

#endif