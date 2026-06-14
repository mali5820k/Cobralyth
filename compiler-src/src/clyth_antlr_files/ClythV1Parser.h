
// Generated from ClythV1.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  ClythV1Parser : public antlr4::Parser {
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

  enum {
    RuleProgram = 0, RuleTopLevelItem = 1, RuleIncludeDecl = 2, RuleIncludeTarget = 3, 
    RuleExternDecl = 4, RuleExternAbi = 5, RuleExternParamList = 6, RuleExternParam = 7, 
    RuleStructDecl = 8, RuleStructField = 9, RuleMethodBlock = 10, RuleMethodDecl = 11, 
    RuleMethodSimpleName = 12, RuleFunctionDecl = 13, RuleParamList = 14, 
    RuleParam = 15, RuleBlock = 16, RuleStatement = 17, RuleVarDecl = 18, 
    RuleAssignmentStmt = 19, RuleAssignable = 20, RuleAssignmentOp = 21, 
    RuleExprStmt = 22, RuleReturnStmt = 23, RuleIfStmt = 24, RuleElseClause = 25, 
    RuleWhileStmt = 26, RuleForStmt = 27, RuleForEachHeader = 28, RuleForEachBinding = 29, 
    RuleForEachVar = 30, RuleForClassicHeader = 31, RuleForInit = 32, RuleVarDeclNoSemi = 33, 
    RuleAssignmentNoSemi = 34, RuleBreakStmt = 35, RuleContinueStmt = 36, 
    RuleMeccBlock = 37, RuleParenExpr = 38, RuleExpression = 39, RuleLogicalOr = 40, 
    RuleLogicalAnd = 41, RuleEquality = 42, RuleRelation = 43, RuleComparison = 44, 
    RuleAdditive = 45, RuleMultiplicative = 46, RuleUnary = 47, RulePostfix = 48, 
    RulePostfixSuffix = 49, RulePrimary = 50, RuleAllocationExpression = 51, 
    RuleArgumentList = 52, RuleLiteral = 53, RuleCollectionLiteral = 54, 
    RuleListLiteral = 55, RuleMapLiteral = 56, RuleMapEntryList = 57, RuleMapEntry = 58, 
    RuleSetLiteral = 59, RuleExpressionList = 60, RuleType = 61, RuleMapType = 62, 
    RuleCollectionType = 63, RuleBaseType = 64, RuleTypeSuffix = 65, RuleCommaOrSemi = 66
  };

  explicit ClythV1Parser(antlr4::TokenStream *input);

  ClythV1Parser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~ClythV1Parser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ProgramContext;
  class TopLevelItemContext;
  class IncludeDeclContext;
  class IncludeTargetContext;
  class ExternDeclContext;
  class ExternAbiContext;
  class ExternParamListContext;
  class ExternParamContext;
  class StructDeclContext;
  class StructFieldContext;
  class MethodBlockContext;
  class MethodDeclContext;
  class MethodSimpleNameContext;
  class FunctionDeclContext;
  class ParamListContext;
  class ParamContext;
  class BlockContext;
  class StatementContext;
  class VarDeclContext;
  class AssignmentStmtContext;
  class AssignableContext;
  class AssignmentOpContext;
  class ExprStmtContext;
  class ReturnStmtContext;
  class IfStmtContext;
  class ElseClauseContext;
  class WhileStmtContext;
  class ForStmtContext;
  class ForEachHeaderContext;
  class ForEachBindingContext;
  class ForEachVarContext;
  class ForClassicHeaderContext;
  class ForInitContext;
  class VarDeclNoSemiContext;
  class AssignmentNoSemiContext;
  class BreakStmtContext;
  class ContinueStmtContext;
  class MeccBlockContext;
  class ParenExprContext;
  class ExpressionContext;
  class LogicalOrContext;
  class LogicalAndContext;
  class EqualityContext;
  class RelationContext;
  class ComparisonContext;
  class AdditiveContext;
  class MultiplicativeContext;
  class UnaryContext;
  class PostfixContext;
  class PostfixSuffixContext;
  class PrimaryContext;
  class AllocationExpressionContext;
  class ArgumentListContext;
  class LiteralContext;
  class CollectionLiteralContext;
  class ListLiteralContext;
  class MapLiteralContext;
  class MapEntryListContext;
  class MapEntryContext;
  class SetLiteralContext;
  class ExpressionListContext;
  class TypeContext;
  class MapTypeContext;
  class CollectionTypeContext;
  class BaseTypeContext;
  class TypeSuffixContext;
  class CommaOrSemiContext; 

  class  ProgramContext : public antlr4::ParserRuleContext {
  public:
    ProgramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<TopLevelItemContext *> topLevelItem();
    TopLevelItemContext* topLevelItem(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProgramContext* program();

  class  TopLevelItemContext : public antlr4::ParserRuleContext {
  public:
    TopLevelItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IncludeDeclContext *includeDecl();
    ExternDeclContext *externDecl();
    StructDeclContext *structDecl();
    MethodBlockContext *methodBlock();
    FunctionDeclContext *functionDecl();
    MethodDeclContext *methodDecl();
    StatementContext *statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TopLevelItemContext* topLevelItem();

  class  IncludeDeclContext : public antlr4::ParserRuleContext {
  public:
    IncludeDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INCLUDE();
    IncludeTargetContext *includeTarget();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IncludeDeclContext* includeDecl();

  class  IncludeTargetContext : public antlr4::ParserRuleContext {
  public:
    IncludeTargetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *STRING_LITERAL();
    antlr4::tree::TerminalNode *TEMPLATE_STRING_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IncludeTargetContext* includeTarget();

  class  ExternDeclContext : public antlr4::ParserRuleContext {
  public:
    ExternDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXTERN();
    TypeContext *type();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    ExternAbiContext *externAbi();
    ExternParamListContext *externParamList();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExternDeclContext* externDecl();

  class  ExternAbiContext : public antlr4::ParserRuleContext {
  public:
    ExternAbiContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *C_ABI();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExternAbiContext* externAbi();

  class  ExternParamListContext : public antlr4::ParserRuleContext {
  public:
    ExternParamListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExternParamContext *> externParam();
    ExternParamContext* externParam(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *VARARGS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExternParamListContext* externParamList();

  class  ExternParamContext : public antlr4::ParserRuleContext {
  public:
    ExternParamContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExternParamContext* externParam();

  class  StructDeclContext : public antlr4::ParserRuleContext {
  public:
    StructDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRUCT();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *LBRACE();
    antlr4::tree::TerminalNode *RBRACE();
    std::vector<StructFieldContext *> structField();
    StructFieldContext* structField(size_t i);
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StructDeclContext* structDecl();

  class  StructFieldContext : public antlr4::ParserRuleContext {
  public:
    StructFieldContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    antlr4::tree::TerminalNode *IDENTIFIER();
    CommaOrSemiContext *commaOrSemi();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StructFieldContext* structField();

  class  MethodBlockContext : public antlr4::ParserRuleContext {
  public:
    MethodBlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *LBRACE();
    antlr4::tree::TerminalNode *RBRACE();
    std::vector<MethodDeclContext *> methodDecl();
    MethodDeclContext* methodDecl(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MethodBlockContext* methodBlock();

  class  MethodDeclContext : public antlr4::ParserRuleContext {
  public:
    MethodDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    MethodSimpleNameContext *methodSimpleName();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    BlockContext *block();
    antlr4::tree::TerminalNode *MECC();
    ParamListContext *paramList();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MethodDeclContext* methodDecl();

  class  MethodSimpleNameContext : public antlr4::ParserRuleContext {
  public:
    MethodSimpleNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *CONSTRUCTOR();
    antlr4::tree::TerminalNode *DESTRUCTOR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MethodSimpleNameContext* methodSimpleName();

  class  FunctionDeclContext : public antlr4::ParserRuleContext {
  public:
    FunctionDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    BlockContext *block();
    antlr4::tree::TerminalNode *MECC();
    ParamListContext *paramList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionDeclContext* functionDecl();

  class  ParamListContext : public antlr4::ParserRuleContext {
  public:
    ParamListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ParamContext *> param();
    ParamContext* param(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParamListContext* paramList();

  class  ParamContext : public antlr4::ParserRuleContext {
  public:
    ParamContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParamContext* param();

  class  BlockContext : public antlr4::ParserRuleContext {
  public:
    BlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LBRACE();
    antlr4::tree::TerminalNode *RBRACE();
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlockContext* block();

  class  StatementContext : public antlr4::ParserRuleContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarDeclContext *varDecl();
    AssignmentStmtContext *assignmentStmt();
    ExprStmtContext *exprStmt();
    ReturnStmtContext *returnStmt();
    IfStmtContext *ifStmt();
    WhileStmtContext *whileStmt();
    ForStmtContext *forStmt();
    BreakStmtContext *breakStmt();
    ContinueStmtContext *continueStmt();
    MeccBlockContext *meccBlock();
    BlockContext *block();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementContext* statement();

  class  VarDeclContext : public antlr4::ParserRuleContext {
  public:
    VarDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *ASSIGN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VarDeclContext* varDecl();

  class  AssignmentStmtContext : public antlr4::ParserRuleContext {
  public:
    AssignmentStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AssignableContext *assignable();
    AssignmentOpContext *assignmentOp();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignmentStmtContext* assignmentStmt();

  class  AssignableContext : public antlr4::ParserRuleContext {
  public:
    AssignableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    std::vector<PostfixSuffixContext *> postfixSuffix();
    PostfixSuffixContext* postfixSuffix(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignableContext* assignable();

  class  AssignmentOpContext : public antlr4::ParserRuleContext {
  public:
    AssignmentOpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASSIGN();
    antlr4::tree::TerminalNode *PLUS_ASSIGN();
    antlr4::tree::TerminalNode *MINUS_ASSIGN();
    antlr4::tree::TerminalNode *STAR_ASSIGN();
    antlr4::tree::TerminalNode *SLASH_ASSIGN();
    antlr4::tree::TerminalNode *PERCENT_ASSIGN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignmentOpContext* assignmentOp();

  class  ExprStmtContext : public antlr4::ParserRuleContext {
  public:
    ExprStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExprStmtContext* exprStmt();

  class  ReturnStmtContext : public antlr4::ParserRuleContext {
  public:
    ReturnStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RETURN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReturnStmtContext* returnStmt();

  class  IfStmtContext : public antlr4::ParserRuleContext {
  public:
    IfStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IF();
    ParenExprContext *parenExpr();
    BlockContext *block();
    ElseClauseContext *elseClause();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IfStmtContext* ifStmt();

  class  ElseClauseContext : public antlr4::ParserRuleContext {
  public:
    ElseClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ELSE();
    IfStmtContext *ifStmt();
    BlockContext *block();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElseClauseContext* elseClause();

  class  WhileStmtContext : public antlr4::ParserRuleContext {
  public:
    WhileStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHILE();
    ParenExprContext *parenExpr();
    BlockContext *block();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WhileStmtContext* whileStmt();

  class  ForStmtContext : public antlr4::ParserRuleContext {
  public:
    ForStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FOR();
    BlockContext *block();
    ForEachHeaderContext *forEachHeader();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    ForClassicHeaderContext *forClassicHeader();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ForStmtContext* forStmt();

  class  ForEachHeaderContext : public antlr4::ParserRuleContext {
  public:
    ForEachHeaderContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ForEachBindingContext *forEachBinding();
    antlr4::tree::TerminalNode *IN();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ForEachHeaderContext* forEachHeader();

  class  ForEachBindingContext : public antlr4::ParserRuleContext {
  public:
    ForEachBindingContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ForEachVarContext *> forEachVar();
    ForEachVarContext* forEachVar(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ForEachBindingContext* forEachBinding();

  class  ForEachVarContext : public antlr4::ParserRuleContext {
  public:
    ForEachVarContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ForEachVarContext* forEachVar();

  class  ForClassicHeaderContext : public antlr4::ParserRuleContext {
  public:
    ForClassicHeaderContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> SEMI();
    antlr4::tree::TerminalNode* SEMI(size_t i);
    ForInitContext *forInit();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ForClassicHeaderContext* forClassicHeader();

  class  ForInitContext : public antlr4::ParserRuleContext {
  public:
    ForInitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarDeclNoSemiContext *varDeclNoSemi();
    AssignmentNoSemiContext *assignmentNoSemi();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ForInitContext* forInit();

  class  VarDeclNoSemiContext : public antlr4::ParserRuleContext {
  public:
    VarDeclNoSemiContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *ASSIGN();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VarDeclNoSemiContext* varDeclNoSemi();

  class  AssignmentNoSemiContext : public antlr4::ParserRuleContext {
  public:
    AssignmentNoSemiContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AssignableContext *assignable();
    AssignmentOpContext *assignmentOp();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignmentNoSemiContext* assignmentNoSemi();

  class  BreakStmtContext : public antlr4::ParserRuleContext {
  public:
    BreakStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BREAK();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BreakStmtContext* breakStmt();

  class  ContinueStmtContext : public antlr4::ParserRuleContext {
  public:
    ContinueStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CONTINUE();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ContinueStmtContext* continueStmt();

  class  MeccBlockContext : public antlr4::ParserRuleContext {
  public:
    MeccBlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MECC();
    BlockContext *block();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MeccBlockContext* meccBlock();

  class  ParenExprContext : public antlr4::ParserRuleContext {
  public:
    ParenExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RPAREN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParenExprContext* parenExpr();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LogicalOrContext *logicalOr();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExpressionContext* expression();

  class  LogicalOrContext : public antlr4::ParserRuleContext {
  public:
    LogicalOrContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LogicalAndContext *> logicalAnd();
    LogicalAndContext* logicalAnd(size_t i);
    std::vector<antlr4::tree::TerminalNode *> OR();
    antlr4::tree::TerminalNode* OR(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LogicalOrContext* logicalOr();

  class  LogicalAndContext : public antlr4::ParserRuleContext {
  public:
    LogicalAndContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<EqualityContext *> equality();
    EqualityContext* equality(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AND();
    antlr4::tree::TerminalNode* AND(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LogicalAndContext* logicalAnd();

  class  EqualityContext : public antlr4::ParserRuleContext {
  public:
    EqualityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<RelationContext *> relation();
    RelationContext* relation(size_t i);
    std::vector<antlr4::tree::TerminalNode *> EQ();
    antlr4::tree::TerminalNode* EQ(size_t i);
    std::vector<antlr4::tree::TerminalNode *> NEQ();
    antlr4::tree::TerminalNode* NEQ(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EqualityContext* equality();

  class  RelationContext : public antlr4::ParserRuleContext {
  public:
    RelationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ComparisonContext *> comparison();
    ComparisonContext* comparison(size_t i);
    std::vector<antlr4::tree::TerminalNode *> IS();
    antlr4::tree::TerminalNode* IS(size_t i);
    std::vector<antlr4::tree::TerminalNode *> IN();
    antlr4::tree::TerminalNode* IN(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RelationContext* relation();

  class  ComparisonContext : public antlr4::ParserRuleContext {
  public:
    ComparisonContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AdditiveContext *> additive();
    AdditiveContext* additive(size_t i);
    std::vector<antlr4::tree::TerminalNode *> GT();
    antlr4::tree::TerminalNode* GT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LT();
    antlr4::tree::TerminalNode* LT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> GTE();
    antlr4::tree::TerminalNode* GTE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LTE();
    antlr4::tree::TerminalNode* LTE(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComparisonContext* comparison();

  class  AdditiveContext : public antlr4::ParserRuleContext {
  public:
    AdditiveContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<MultiplicativeContext *> multiplicative();
    MultiplicativeContext* multiplicative(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PLUS();
    antlr4::tree::TerminalNode* PLUS(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AdditiveContext* additive();

  class  MultiplicativeContext : public antlr4::ParserRuleContext {
  public:
    MultiplicativeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<UnaryContext *> unary();
    UnaryContext* unary(size_t i);
    std::vector<antlr4::tree::TerminalNode *> STAR();
    antlr4::tree::TerminalNode* STAR(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SLASH();
    antlr4::tree::TerminalNode* SLASH(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PERCENT();
    antlr4::tree::TerminalNode* PERCENT(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MultiplicativeContext* multiplicative();

  class  UnaryContext : public antlr4::ParserRuleContext {
  public:
    UnaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnaryContext *unary();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *BANG();
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *TILDE();
    antlr4::tree::TerminalNode *AMP();
    PostfixContext *postfix();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnaryContext* unary();

  class  PostfixContext : public antlr4::ParserRuleContext {
  public:
    PostfixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PrimaryContext *primary();
    std::vector<PostfixSuffixContext *> postfixSuffix();
    PostfixSuffixContext* postfixSuffix(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PostfixContext* postfix();

  class  PostfixSuffixContext : public antlr4::ParserRuleContext {
  public:
    PostfixSuffixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *LBRACKET();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RBRACKET();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    ArgumentListContext *argumentList();
    antlr4::tree::TerminalNode *INCREMENT();
    antlr4::tree::TerminalNode *DECREMENT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PostfixSuffixContext* postfixSuffix();

  class  PrimaryContext : public antlr4::ParserRuleContext {
  public:
    PrimaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LiteralContext *literal();
    CollectionLiteralContext *collectionLiteral();
    AllocationExpressionContext *allocationExpression();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *LPAREN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RPAREN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrimaryContext* primary();

  class  AllocationExpressionContext : public antlr4::ParserRuleContext {
  public:
    AllocationExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    TypeContext *type();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *MALLOC();
    antlr4::tree::TerminalNode *ISO_MALLOC();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AllocationExpressionContext* allocationExpression();

  class  ArgumentListContext : public antlr4::ParserRuleContext {
  public:
    ArgumentListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArgumentListContext* argumentList();

  class  LiteralContext : public antlr4::ParserRuleContext {
  public:
    LiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TEMPLATE_STRING_LITERAL();
    antlr4::tree::TerminalNode *STRING_LITERAL();
    antlr4::tree::TerminalNode *NUMERIC_LITERAL();
    antlr4::tree::TerminalNode *BOOLEAN_LITERAL();
    antlr4::tree::TerminalNode *NULL_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LiteralContext* literal();

  class  CollectionLiteralContext : public antlr4::ParserRuleContext {
  public:
    CollectionLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MapLiteralContext *mapLiteral();
    SetLiteralContext *setLiteral();
    ListLiteralContext *listLiteral();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CollectionLiteralContext* collectionLiteral();

  class  ListLiteralContext : public antlr4::ParserRuleContext {
  public:
    ListLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LBRACKET();
    antlr4::tree::TerminalNode *RBRACKET();
    ExpressionListContext *expressionList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ListLiteralContext* listLiteral();

  class  MapLiteralContext : public antlr4::ParserRuleContext {
  public:
    MapLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LBRACE();
    antlr4::tree::TerminalNode *RBRACE();
    MapEntryListContext *mapEntryList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MapLiteralContext* mapLiteral();

  class  MapEntryListContext : public antlr4::ParserRuleContext {
  public:
    MapEntryListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<MapEntryContext *> mapEntry();
    MapEntryContext* mapEntry(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MapEntryListContext* mapEntryList();

  class  MapEntryContext : public antlr4::ParserRuleContext {
  public:
    MapEntryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *COLON();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MapEntryContext* mapEntry();

  class  SetLiteralContext : public antlr4::ParserRuleContext {
  public:
    SetLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LBRACE();
    ExpressionListContext *expressionList();
    antlr4::tree::TerminalNode *RBRACE();
    antlr4::tree::TerminalNode *COMMA();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetLiteralContext* setLiteral();

  class  ExpressionListContext : public antlr4::ParserRuleContext {
  public:
    ExpressionListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExpressionListContext* expressionList();

  class  TypeContext : public antlr4::ParserRuleContext {
  public:
    TypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MapTypeContext *mapType();
    CollectionTypeContext *collectionType();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeContext* type();

  class  MapTypeContext : public antlr4::ParserRuleContext {
  public:
    MapTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<CollectionTypeContext *> collectionType();
    CollectionTypeContext* collectionType(size_t i);
    antlr4::tree::TerminalNode *COLON();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MapTypeContext* mapType();

  class  CollectionTypeContext : public antlr4::ParserRuleContext {
  public:
    CollectionTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BaseTypeContext *baseType();
    std::vector<TypeSuffixContext *> typeSuffix();
    TypeSuffixContext* typeSuffix(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CollectionTypeContext* collectionType();

  class  BaseTypeContext : public antlr4::ParserRuleContext {
  public:
    BaseTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BUILTIN_TYPE();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BaseTypeContext* baseType();

  class  TypeSuffixContext : public antlr4::ParserRuleContext {
  public:
    TypeSuffixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LBRACKET();
    antlr4::tree::TerminalNode *RBRACKET();
    antlr4::tree::TerminalNode *NUMERIC_LITERAL();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeSuffixContext* typeSuffix();

  class  CommaOrSemiContext : public antlr4::ParserRuleContext {
  public:
    CommaOrSemiContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *SEMI();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CommaOrSemiContext* commaOrSemi();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

