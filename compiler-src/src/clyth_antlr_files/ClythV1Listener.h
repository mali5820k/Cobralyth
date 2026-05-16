
// Generated from ../ClythV1.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "ClythV1Parser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by ClythV1Parser.
 */
class  ClythV1Listener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProgram(ClythV1Parser::ProgramContext *ctx) = 0;
  virtual void exitProgram(ClythV1Parser::ProgramContext *ctx) = 0;

  virtual void enterTopLevelItem(ClythV1Parser::TopLevelItemContext *ctx) = 0;
  virtual void exitTopLevelItem(ClythV1Parser::TopLevelItemContext *ctx) = 0;

  virtual void enterIncludeDecl(ClythV1Parser::IncludeDeclContext *ctx) = 0;
  virtual void exitIncludeDecl(ClythV1Parser::IncludeDeclContext *ctx) = 0;

  virtual void enterIncludeTarget(ClythV1Parser::IncludeTargetContext *ctx) = 0;
  virtual void exitIncludeTarget(ClythV1Parser::IncludeTargetContext *ctx) = 0;

  virtual void enterExternDecl(ClythV1Parser::ExternDeclContext *ctx) = 0;
  virtual void exitExternDecl(ClythV1Parser::ExternDeclContext *ctx) = 0;

  virtual void enterExternAbi(ClythV1Parser::ExternAbiContext *ctx) = 0;
  virtual void exitExternAbi(ClythV1Parser::ExternAbiContext *ctx) = 0;

  virtual void enterExternParamList(ClythV1Parser::ExternParamListContext *ctx) = 0;
  virtual void exitExternParamList(ClythV1Parser::ExternParamListContext *ctx) = 0;

  virtual void enterExternParam(ClythV1Parser::ExternParamContext *ctx) = 0;
  virtual void exitExternParam(ClythV1Parser::ExternParamContext *ctx) = 0;

  virtual void enterStructDecl(ClythV1Parser::StructDeclContext *ctx) = 0;
  virtual void exitStructDecl(ClythV1Parser::StructDeclContext *ctx) = 0;

  virtual void enterStructField(ClythV1Parser::StructFieldContext *ctx) = 0;
  virtual void exitStructField(ClythV1Parser::StructFieldContext *ctx) = 0;

  virtual void enterFunctionDecl(ClythV1Parser::FunctionDeclContext *ctx) = 0;
  virtual void exitFunctionDecl(ClythV1Parser::FunctionDeclContext *ctx) = 0;

  virtual void enterParamList(ClythV1Parser::ParamListContext *ctx) = 0;
  virtual void exitParamList(ClythV1Parser::ParamListContext *ctx) = 0;

  virtual void enterParam(ClythV1Parser::ParamContext *ctx) = 0;
  virtual void exitParam(ClythV1Parser::ParamContext *ctx) = 0;

  virtual void enterBlock(ClythV1Parser::BlockContext *ctx) = 0;
  virtual void exitBlock(ClythV1Parser::BlockContext *ctx) = 0;

  virtual void enterStatement(ClythV1Parser::StatementContext *ctx) = 0;
  virtual void exitStatement(ClythV1Parser::StatementContext *ctx) = 0;

  virtual void enterVarDecl(ClythV1Parser::VarDeclContext *ctx) = 0;
  virtual void exitVarDecl(ClythV1Parser::VarDeclContext *ctx) = 0;

  virtual void enterAssignmentStmt(ClythV1Parser::AssignmentStmtContext *ctx) = 0;
  virtual void exitAssignmentStmt(ClythV1Parser::AssignmentStmtContext *ctx) = 0;

  virtual void enterAssignable(ClythV1Parser::AssignableContext *ctx) = 0;
  virtual void exitAssignable(ClythV1Parser::AssignableContext *ctx) = 0;

  virtual void enterAssignmentOp(ClythV1Parser::AssignmentOpContext *ctx) = 0;
  virtual void exitAssignmentOp(ClythV1Parser::AssignmentOpContext *ctx) = 0;

  virtual void enterExprStmt(ClythV1Parser::ExprStmtContext *ctx) = 0;
  virtual void exitExprStmt(ClythV1Parser::ExprStmtContext *ctx) = 0;

  virtual void enterReturnStmt(ClythV1Parser::ReturnStmtContext *ctx) = 0;
  virtual void exitReturnStmt(ClythV1Parser::ReturnStmtContext *ctx) = 0;

  virtual void enterIfStmt(ClythV1Parser::IfStmtContext *ctx) = 0;
  virtual void exitIfStmt(ClythV1Parser::IfStmtContext *ctx) = 0;

  virtual void enterElseClause(ClythV1Parser::ElseClauseContext *ctx) = 0;
  virtual void exitElseClause(ClythV1Parser::ElseClauseContext *ctx) = 0;

  virtual void enterWhileStmt(ClythV1Parser::WhileStmtContext *ctx) = 0;
  virtual void exitWhileStmt(ClythV1Parser::WhileStmtContext *ctx) = 0;

  virtual void enterForStmt(ClythV1Parser::ForStmtContext *ctx) = 0;
  virtual void exitForStmt(ClythV1Parser::ForStmtContext *ctx) = 0;

  virtual void enterForEachHeader(ClythV1Parser::ForEachHeaderContext *ctx) = 0;
  virtual void exitForEachHeader(ClythV1Parser::ForEachHeaderContext *ctx) = 0;

  virtual void enterForEachBinding(ClythV1Parser::ForEachBindingContext *ctx) = 0;
  virtual void exitForEachBinding(ClythV1Parser::ForEachBindingContext *ctx) = 0;

  virtual void enterForEachVar(ClythV1Parser::ForEachVarContext *ctx) = 0;
  virtual void exitForEachVar(ClythV1Parser::ForEachVarContext *ctx) = 0;

  virtual void enterForClassicHeader(ClythV1Parser::ForClassicHeaderContext *ctx) = 0;
  virtual void exitForClassicHeader(ClythV1Parser::ForClassicHeaderContext *ctx) = 0;

  virtual void enterForInit(ClythV1Parser::ForInitContext *ctx) = 0;
  virtual void exitForInit(ClythV1Parser::ForInitContext *ctx) = 0;

  virtual void enterVarDeclNoSemi(ClythV1Parser::VarDeclNoSemiContext *ctx) = 0;
  virtual void exitVarDeclNoSemi(ClythV1Parser::VarDeclNoSemiContext *ctx) = 0;

  virtual void enterAssignmentNoSemi(ClythV1Parser::AssignmentNoSemiContext *ctx) = 0;
  virtual void exitAssignmentNoSemi(ClythV1Parser::AssignmentNoSemiContext *ctx) = 0;

  virtual void enterBreakStmt(ClythV1Parser::BreakStmtContext *ctx) = 0;
  virtual void exitBreakStmt(ClythV1Parser::BreakStmtContext *ctx) = 0;

  virtual void enterContinueStmt(ClythV1Parser::ContinueStmtContext *ctx) = 0;
  virtual void exitContinueStmt(ClythV1Parser::ContinueStmtContext *ctx) = 0;

  virtual void enterParenExpr(ClythV1Parser::ParenExprContext *ctx) = 0;
  virtual void exitParenExpr(ClythV1Parser::ParenExprContext *ctx) = 0;

  virtual void enterExpression(ClythV1Parser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(ClythV1Parser::ExpressionContext *ctx) = 0;

  virtual void enterLogicalOr(ClythV1Parser::LogicalOrContext *ctx) = 0;
  virtual void exitLogicalOr(ClythV1Parser::LogicalOrContext *ctx) = 0;

  virtual void enterLogicalAnd(ClythV1Parser::LogicalAndContext *ctx) = 0;
  virtual void exitLogicalAnd(ClythV1Parser::LogicalAndContext *ctx) = 0;

  virtual void enterEquality(ClythV1Parser::EqualityContext *ctx) = 0;
  virtual void exitEquality(ClythV1Parser::EqualityContext *ctx) = 0;

  virtual void enterRelation(ClythV1Parser::RelationContext *ctx) = 0;
  virtual void exitRelation(ClythV1Parser::RelationContext *ctx) = 0;

  virtual void enterComparison(ClythV1Parser::ComparisonContext *ctx) = 0;
  virtual void exitComparison(ClythV1Parser::ComparisonContext *ctx) = 0;

  virtual void enterAdditive(ClythV1Parser::AdditiveContext *ctx) = 0;
  virtual void exitAdditive(ClythV1Parser::AdditiveContext *ctx) = 0;

  virtual void enterMultiplicative(ClythV1Parser::MultiplicativeContext *ctx) = 0;
  virtual void exitMultiplicative(ClythV1Parser::MultiplicativeContext *ctx) = 0;

  virtual void enterUnary(ClythV1Parser::UnaryContext *ctx) = 0;
  virtual void exitUnary(ClythV1Parser::UnaryContext *ctx) = 0;

  virtual void enterPostfix(ClythV1Parser::PostfixContext *ctx) = 0;
  virtual void exitPostfix(ClythV1Parser::PostfixContext *ctx) = 0;

  virtual void enterPostfixSuffix(ClythV1Parser::PostfixSuffixContext *ctx) = 0;
  virtual void exitPostfixSuffix(ClythV1Parser::PostfixSuffixContext *ctx) = 0;

  virtual void enterPrimary(ClythV1Parser::PrimaryContext *ctx) = 0;
  virtual void exitPrimary(ClythV1Parser::PrimaryContext *ctx) = 0;

  virtual void enterArgumentList(ClythV1Parser::ArgumentListContext *ctx) = 0;
  virtual void exitArgumentList(ClythV1Parser::ArgumentListContext *ctx) = 0;

  virtual void enterLiteral(ClythV1Parser::LiteralContext *ctx) = 0;
  virtual void exitLiteral(ClythV1Parser::LiteralContext *ctx) = 0;

  virtual void enterCollectionLiteral(ClythV1Parser::CollectionLiteralContext *ctx) = 0;
  virtual void exitCollectionLiteral(ClythV1Parser::CollectionLiteralContext *ctx) = 0;

  virtual void enterListLiteral(ClythV1Parser::ListLiteralContext *ctx) = 0;
  virtual void exitListLiteral(ClythV1Parser::ListLiteralContext *ctx) = 0;

  virtual void enterMapLiteral(ClythV1Parser::MapLiteralContext *ctx) = 0;
  virtual void exitMapLiteral(ClythV1Parser::MapLiteralContext *ctx) = 0;

  virtual void enterMapEntryList(ClythV1Parser::MapEntryListContext *ctx) = 0;
  virtual void exitMapEntryList(ClythV1Parser::MapEntryListContext *ctx) = 0;

  virtual void enterMapEntry(ClythV1Parser::MapEntryContext *ctx) = 0;
  virtual void exitMapEntry(ClythV1Parser::MapEntryContext *ctx) = 0;

  virtual void enterSetLiteral(ClythV1Parser::SetLiteralContext *ctx) = 0;
  virtual void exitSetLiteral(ClythV1Parser::SetLiteralContext *ctx) = 0;

  virtual void enterExpressionList(ClythV1Parser::ExpressionListContext *ctx) = 0;
  virtual void exitExpressionList(ClythV1Parser::ExpressionListContext *ctx) = 0;

  virtual void enterType(ClythV1Parser::TypeContext *ctx) = 0;
  virtual void exitType(ClythV1Parser::TypeContext *ctx) = 0;

  virtual void enterMapType(ClythV1Parser::MapTypeContext *ctx) = 0;
  virtual void exitMapType(ClythV1Parser::MapTypeContext *ctx) = 0;

  virtual void enterCollectionType(ClythV1Parser::CollectionTypeContext *ctx) = 0;
  virtual void exitCollectionType(ClythV1Parser::CollectionTypeContext *ctx) = 0;

  virtual void enterBaseType(ClythV1Parser::BaseTypeContext *ctx) = 0;
  virtual void exitBaseType(ClythV1Parser::BaseTypeContext *ctx) = 0;

  virtual void enterTypeSuffix(ClythV1Parser::TypeSuffixContext *ctx) = 0;
  virtual void exitTypeSuffix(ClythV1Parser::TypeSuffixContext *ctx) = 0;

  virtual void enterCommaOrSemi(ClythV1Parser::CommaOrSemiContext *ctx) = 0;
  virtual void exitCommaOrSemi(ClythV1Parser::CommaOrSemiContext *ctx) = 0;


};

