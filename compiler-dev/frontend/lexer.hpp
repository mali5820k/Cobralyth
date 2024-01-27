#ifndef lexer_hpp
#define lexer_hpp

#include <string>
#include <vector>
#include <map>

enum TokenType {
    IF, ELSE, RETURN, CONTINUE, WHILE, FOR, BREAK, SWITCH, CASE, DEFAULT, 
    VAR,  INT8, INT16, INT, INT32, INT64, UINT8, UINT16, UINT, UINT32, UINT64,
    BOOL, CHAR, STRING, BYTE, FLOAT32, FLOAT64,

    IMPORT,
    CLASS, STRUCT, FUNCTION, METHOD,
    FIXED, CONST, PUBLIC, PRIVATE, PROTECTED, 
    LIST, ARRAY,

    PLUS, MINUS, MULTIPLY, DIVIDE, EXPONENT, MODULO,
    PLUS_EQUALS, MINUS_EQUALS, MULTIPLY_EQUALS, DIVIDE_EQUALS, MODULO_EQUALS,
    GT, LT, GTE, LTE, EQUALS, EQUALS_EQUALS, NOT,
    BIT_OR, BIT_AND, BIT_XOR, BIT_NOT,
    OR, AND,

    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET, RIGHT_BRACKET, LEFT_CURLY, RIGHT_CURLY,
    SEMICOLON, COLON, DOT, COMMENT, MULTI_COMMENT_START, MULTI_COMMENT_END,
    IDENTIFIER,

};

class Token {
    public:
        Token(int tokenLine, std::string tokenValue, TokenType tokenType);
        ~Token();

        std::string toString();
    
        int tokenLine;
        std::string tokenValue;
        TokenType tokenType;
};

class Lexer {
    public:
        Lexer();
        ~Lexer();
        std::vector<std::string> scannedContents();
        std::vector<Token> tokenList();
        bool scanFile(std::string filename);
        void consumeChar();
        void checkMatch(std::vector<std::string> keywordList);
        void lexLine();
        

    private:
        std::string filename;
        std::vector<Token> scannedTokens;
        std::string fileLine;
        int currentLineCharIndex;
        int lineCounter;
        char currentChar;
        char nextChar;
        bool endOfFileReached;
};

#endif