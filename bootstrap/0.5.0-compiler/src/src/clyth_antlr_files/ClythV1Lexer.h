
// Generated from ClythV1.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  ClythV1Lexer : public antlr4::Lexer {
public:
  enum {
    INCLUDE = 1, EXTERN = 2, C_ABI = 3, PUBLIC = 4, PRIVATE = 5, STRUCT = 6, 
    FUNCTION = 7, MECC = 8, CONSTRUCTOR = 9, DESTRUCTOR = 10, IF = 11, ELSE = 12, 
    FOR = 13, WHILE = 14, RETURN = 15, BREAK = 16, CONTINUE = 17, IS = 18, 
    IN = 19, NOT = 20, BOOLEAN_LITERAL = 21, NULL_LITERAL = 22, LPAREN = 23, 
    RPAREN = 24, LBRACE = 25, RBRACE = 26, LBRACKET = 27, RBRACKET = 28, 
    COMMA = 29, COLON = 30, SEMI = 31, DOT = 32, OR = 33, AND = 34, EQ = 35, 
    NEQ = 36, GTE = 37, LTE = 38, GT = 39, LT = 40, INCREMENT = 41, DECREMENT = 42, 
    PLUS_ASSIGN = 43, MINUS_ASSIGN = 44, STAR_ASSIGN = 45, SLASH_ASSIGN = 46, 
    PERCENT_ASSIGN = 47, ARROW = 48, ASSIGN = 49, PLUS = 50, MINUS = 51, 
    STAR = 52, SLASH = 53, PERCENT = 54, BANG = 55, TILDE = 56, AMP = 57, 
    VARARGS = 58, BUILTIN_TYPE = 59, NUMERIC_LITERAL = 60, TEMPLATE_STRING_LITERAL = 61, 
    STRING_LITERAL = 62, IDENTIFIER = 63, SINGLE_LINE_COMMENT = 64, MULTI_LINE_COMMENT = 65, 
    WS = 66
  };

  explicit ClythV1Lexer(antlr4::CharStream *input);

  ~ClythV1Lexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

