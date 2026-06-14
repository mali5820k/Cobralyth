
// Generated from ClythV1.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  ClythV1Lexer : public antlr4::Lexer {
public:
  enum {
    INCLUDE = 1, EXTERN = 2, C_ABI = 3, STRUCT = 4, MECC = 5, MALLOC = 6, 
    ISO_MALLOC = 7, CONSTRUCTOR = 8, DESTRUCTOR = 9, IF = 10, ELSE = 11, 
    FOR = 12, WHILE = 13, RETURN = 14, BREAK = 15, CONTINUE = 16, IS = 17, 
    IN = 18, NOT = 19, BOOLEAN_LITERAL = 20, NULL_LITERAL = 21, LPAREN = 22, 
    RPAREN = 23, LBRACE = 24, RBRACE = 25, LBRACKET = 26, RBRACKET = 27, 
    COMMA = 28, COLON = 29, SEMI = 30, DOT = 31, OR = 32, AND = 33, EQ = 34, 
    NEQ = 35, GTE = 36, LTE = 37, GT = 38, LT = 39, INCREMENT = 40, DECREMENT = 41, 
    PLUS_ASSIGN = 42, MINUS_ASSIGN = 43, STAR_ASSIGN = 44, SLASH_ASSIGN = 45, 
    PERCENT_ASSIGN = 46, ASSIGN = 47, PLUS = 48, MINUS = 49, STAR = 50, 
    SLASH = 51, PERCENT = 52, BANG = 53, TILDE = 54, AMP = 55, VARARGS = 56, 
    BUILTIN_TYPE = 57, NUMERIC_LITERAL = 58, TEMPLATE_STRING_LITERAL = 59, 
    STRING_LITERAL = 60, IDENTIFIER = 61, SINGLE_LINE_COMMENT = 62, MULTI_LINE_COMMENT = 63, 
    WS = 64
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

