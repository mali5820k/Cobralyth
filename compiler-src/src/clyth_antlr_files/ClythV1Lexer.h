
// Generated from ../ClythV1.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  ClythV1Lexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    INCLUDE = 15, EXTERN = 16, C_ABI = 17, STRUCT = 18, IF = 19, ELSE = 20, 
    FOR = 21, WHILE = 22, RETURN = 23, BREAK = 24, CONTINUE = 25, IS = 26, 
    IN = 27, NOT = 28, SEMI = 29, OR = 30, AND = 31, EQ = 32, NEQ = 33, 
    GTE = 34, LTE = 35, GT = 36, LT = 37, INCREMENT = 38, DECREMENT = 39, 
    PLUS = 40, MINUS = 41, STAR = 42, SLASH = 43, PERCENT = 44, BANG = 45, 
    TILDE = 46, AMP = 47, DOT = 48, VARARGS = 49, BUILTIN_TYPE = 50, BOOLEAN_LITERAL = 51, 
    NULL_LITERAL = 52, NUMERIC_LITERAL = 53, TEMPLATE_STRING_LITERAL = 54, 
    STRING_LITERAL = 55, IDENTIFIER = 56, SINGLE_LINE_COMMENT = 57, MULTI_LINE_COMMENT = 58, 
    WS = 59
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

