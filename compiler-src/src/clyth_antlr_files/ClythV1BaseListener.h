
// Generated from ../ClythV1.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "ClythV1Listener.h"


/**
 * This class provides an empty implementation of ClythV1Listener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  ClythV1BaseListener : public ClythV1Listener {
public:

  virtual void enterProgram(ClythV1Parser::ProgramContext * /*ctx*/) override { }
  virtual void exitProgram(ClythV1Parser::ProgramContext * /*ctx*/) override { }

  virtual void enterTopLevelItem(ClythV1Parser::TopLevelItemContext * /*ctx*/) override { }
  virtual void exitTopLevelItem(ClythV1Parser::TopLevelItemContext * /*ctx*/) override { }

  virtual void enterIncludeDecl(ClythV1Parser::IncludeDeclContext * /*ctx*/) override { }
  virtual void exitIncludeDecl(ClythV1Parser::IncludeDeclContext * /*ctx*/) override { }

  virtual void enterIncludeTarget(ClythV1Parser::IncludeTargetContext * /*ctx*/) override { }
  virtual void exitIncludeTarget(ClythV1Parser::IncludeTargetContext * /*ctx*/) override { }

  virtual void enterExternDecl(ClythV1Parser::ExternDeclContext * /*ctx*/) override { }
  virtual void exitExternDecl(ClythV1Parser::ExternDeclContext * /*ctx*/) override { }

  virtual void enterExternAbi(ClythV1Parser::ExternAbiContext * /*ctx*/) override { }
  virtual void exitExternAbi(ClythV1Parser::ExternAbiContext * /*ctx*/) override { }

  virtual void enterExternParamList(ClythV1Parser::ExternParamListContext * /*ctx*/) override { }
  virtual void exitExternParamList(ClythV1Parser::ExternParamListContext * /*ctx*/) override { }

  virtual void enterExternParam(ClythV1Parser::ExternParamContext * /*ctx*/) override { }
  virtual void exitExternParam(ClythV1Parser::ExternParamContext * /*ctx*/) override { }

  virtual void enterStructDecl(ClythV1Parser::StructDeclContext * /*ctx*/) override { }
  virtual void exitStructDecl(ClythV1Parser::StructDeclContext * /*ctx*/) override { }

  virtual void enterStructField(ClythV1Parser::StructFieldContext * /*ctx*/) override { }
  virtual void exitStructField(ClythV1Parser::StructFieldContext * /*ctx*/) override { }

  virtual void enterFunctionDecl(ClythV1Parser::FunctionDeclContext * /*ctx*/) override { }
  virtual void exitFunctionDecl(ClythV1Parser::FunctionDeclContext * /*ctx*/) override { }

  virtual void enterParamList(ClythV1Parser::ParamListContext * /*ctx*/) override { }
  virtual void exitParamList(ClythV1Parser::ParamListContext * /*ctx*/) override { }

  virtual void enterParam(ClythV1Parser::ParamContext * /*ctx*/) override { }
  virtual void exitParam(ClythV1Parser::ParamContext * /*ctx*/) override { }

  virtual void enterBlock(ClythV1Parser::BlockContext * /*ctx*/) override { }
  virtual void exitBlock(ClythV1Parser::BlockContext * /*ctx*/) override { }

  virtual void enterStatement(ClythV1Parser::StatementContext * /*ctx*/) override { }
  virtual void exitStatement(ClythV1Parser::StatementContext * /*ctx*/) override { }

  virtual void enterVarDecl(ClythV1Parser::VarDeclContext * /*ctx*/) override { }
  virtual void exitVarDecl(ClythV1Parser::VarDeclContext * /*ctx*/) override { }

  virtual void enterAssignmentStmt(ClythV1Parser::AssignmentStmtContext * /*ctx*/) override { }
  virtual void exitAssignmentStmt(ClythV1Parser::AssignmentStmtContext * /*ctx*/) override { }

  virtual void enterAssignable(ClythV1Parser::AssignableContext * /*ctx*/) override { }
  virtual void exitAssignable(ClythV1Parser::AssignableContext * /*ctx*/) override { }

  virtual void enterAssignmentOp(ClythV1Parser::AssignmentOpContext * /*ctx*/) override { }
  virtual void exitAssignmentOp(ClythV1Parser::AssignmentOpContext * /*ctx*/) override { }

  virtual void enterExprStmt(ClythV1Parser::ExprStmtContext * /*ctx*/) override { }
  virtual void exitExprStmt(ClythV1Parser::ExprStmtContext * /*ctx*/) override { }

  virtual void enterReturnStmt(ClythV1Parser::ReturnStmtContext * /*ctx*/) override { }
  virtual void exitReturnStmt(ClythV1Parser::ReturnStmtContext * /*ctx*/) override { }

  virtual void enterIfStmt(ClythV1Parser::IfStmtContext * /*ctx*/) override { }
  virtual void exitIfStmt(ClythV1Parser::IfStmtContext * /*ctx*/) override { }

  virtual void enterElseClause(ClythV1Parser::ElseClauseContext * /*ctx*/) override { }
  virtual void exitElseClause(ClythV1Parser::ElseClauseContext * /*ctx*/) override { }

  virtual void enterWhileStmt(ClythV1Parser::WhileStmtContext * /*ctx*/) override { }
  virtual void exitWhileStmt(ClythV1Parser::WhileStmtContext * /*ctx*/) override { }

  virtual void enterForStmt(ClythV1Parser::ForStmtContext * /*ctx*/) override { }
  virtual void exitForStmt(ClythV1Parser::ForStmtContext * /*ctx*/) override { }

  virtual void enterForEachHeader(ClythV1Parser::ForEachHeaderContext * /*ctx*/) override { }
  virtual void exitForEachHeader(ClythV1Parser::ForEachHeaderContext * /*ctx*/) override { }

  virtual void enterForEachBinding(ClythV1Parser::ForEachBindingContext * /*ctx*/) override { }
  virtual void exitForEachBinding(ClythV1Parser::ForEachBindingContext * /*ctx*/) override { }

  virtual void enterForEachVar(ClythV1Parser::ForEachVarContext * /*ctx*/) override { }
  virtual void exitForEachVar(ClythV1Parser::ForEachVarContext * /*ctx*/) override { }

  virtual void enterForClassicHeader(ClythV1Parser::ForClassicHeaderContext * /*ctx*/) override { }
  virtual void exitForClassicHeader(ClythV1Parser::ForClassicHeaderContext * /*ctx*/) override { }

  virtual void enterForInit(ClythV1Parser::ForInitContext * /*ctx*/) override { }
  virtual void exitForInit(ClythV1Parser::ForInitContext * /*ctx*/) override { }

  virtual void enterVarDeclNoSemi(ClythV1Parser::VarDeclNoSemiContext * /*ctx*/) override { }
  virtual void exitVarDeclNoSemi(ClythV1Parser::VarDeclNoSemiContext * /*ctx*/) override { }

  virtual void enterAssignmentNoSemi(ClythV1Parser::AssignmentNoSemiContext * /*ctx*/) override { }
  virtual void exitAssignmentNoSemi(ClythV1Parser::AssignmentNoSemiContext * /*ctx*/) override { }

  virtual void enterBreakStmt(ClythV1Parser::BreakStmtContext * /*ctx*/) override { }
  virtual void exitBreakStmt(ClythV1Parser::BreakStmtContext * /*ctx*/) override { }

  virtual void enterContinueStmt(ClythV1Parser::ContinueStmtContext * /*ctx*/) override { }
  virtual void exitContinueStmt(ClythV1Parser::ContinueStmtContext * /*ctx*/) override { }

  virtual void enterParenExpr(ClythV1Parser::ParenExprContext * /*ctx*/) override { }
  virtual void exitParenExpr(ClythV1Parser::ParenExprContext * /*ctx*/) override { }

  virtual void enterExpression(ClythV1Parser::ExpressionContext * /*ctx*/) override { }
  virtual void exitExpression(ClythV1Parser::ExpressionContext * /*ctx*/) override { }

  virtual void enterLogicalOr(ClythV1Parser::LogicalOrContext * /*ctx*/) override { }
  virtual void exitLogicalOr(ClythV1Parser::LogicalOrContext * /*ctx*/) override { }

  virtual void enterLogicalAnd(ClythV1Parser::LogicalAndContext * /*ctx*/) override { }
  virtual void exitLogicalAnd(ClythV1Parser::LogicalAndContext * /*ctx*/) override { }

  virtual void enterEquality(ClythV1Parser::EqualityContext * /*ctx*/) override { }
  virtual void exitEquality(ClythV1Parser::EqualityContext * /*ctx*/) override { }

  virtual void enterRelation(ClythV1Parser::RelationContext * /*ctx*/) override { }
  virtual void exitRelation(ClythV1Parser::RelationContext * /*ctx*/) override { }

  virtual void enterComparison(ClythV1Parser::ComparisonContext * /*ctx*/) override { }
  virtual void exitComparison(ClythV1Parser::ComparisonContext * /*ctx*/) override { }

  virtual void enterAdditive(ClythV1Parser::AdditiveContext * /*ctx*/) override { }
  virtual void exitAdditive(ClythV1Parser::AdditiveContext * /*ctx*/) override { }

  virtual void enterMultiplicative(ClythV1Parser::MultiplicativeContext * /*ctx*/) override { }
  virtual void exitMultiplicative(ClythV1Parser::MultiplicativeContext * /*ctx*/) override { }

  virtual void enterUnary(ClythV1Parser::UnaryContext * /*ctx*/) override { }
  virtual void exitUnary(ClythV1Parser::UnaryContext * /*ctx*/) override { }

  virtual void enterPostfix(ClythV1Parser::PostfixContext * /*ctx*/) override { }
  virtual void exitPostfix(ClythV1Parser::PostfixContext * /*ctx*/) override { }

  virtual void enterPostfixSuffix(ClythV1Parser::PostfixSuffixContext * /*ctx*/) override { }
  virtual void exitPostfixSuffix(ClythV1Parser::PostfixSuffixContext * /*ctx*/) override { }

  virtual void enterPrimary(ClythV1Parser::PrimaryContext * /*ctx*/) override { }
  virtual void exitPrimary(ClythV1Parser::PrimaryContext * /*ctx*/) override { }

  virtual void enterArgumentList(ClythV1Parser::ArgumentListContext * /*ctx*/) override { }
  virtual void exitArgumentList(ClythV1Parser::ArgumentListContext * /*ctx*/) override { }

  virtual void enterLiteral(ClythV1Parser::LiteralContext * /*ctx*/) override { }
  virtual void exitLiteral(ClythV1Parser::LiteralContext * /*ctx*/) override { }

  virtual void enterCollectionLiteral(ClythV1Parser::CollectionLiteralContext * /*ctx*/) override { }
  virtual void exitCollectionLiteral(ClythV1Parser::CollectionLiteralContext * /*ctx*/) override { }

  virtual void enterListLiteral(ClythV1Parser::ListLiteralContext * /*ctx*/) override { }
  virtual void exitListLiteral(ClythV1Parser::ListLiteralContext * /*ctx*/) override { }

  virtual void enterMapLiteral(ClythV1Parser::MapLiteralContext * /*ctx*/) override { }
  virtual void exitMapLiteral(ClythV1Parser::MapLiteralContext * /*ctx*/) override { }

  virtual void enterMapEntryList(ClythV1Parser::MapEntryListContext * /*ctx*/) override { }
  virtual void exitMapEntryList(ClythV1Parser::MapEntryListContext * /*ctx*/) override { }

  virtual void enterMapEntry(ClythV1Parser::MapEntryContext * /*ctx*/) override { }
  virtual void exitMapEntry(ClythV1Parser::MapEntryContext * /*ctx*/) override { }

  virtual void enterSetLiteral(ClythV1Parser::SetLiteralContext * /*ctx*/) override { }
  virtual void exitSetLiteral(ClythV1Parser::SetLiteralContext * /*ctx*/) override { }

  virtual void enterExpressionList(ClythV1Parser::ExpressionListContext * /*ctx*/) override { }
  virtual void exitExpressionList(ClythV1Parser::ExpressionListContext * /*ctx*/) override { }

  virtual void enterType(ClythV1Parser::TypeContext * /*ctx*/) override { }
  virtual void exitType(ClythV1Parser::TypeContext * /*ctx*/) override { }

  virtual void enterMapType(ClythV1Parser::MapTypeContext * /*ctx*/) override { }
  virtual void exitMapType(ClythV1Parser::MapTypeContext * /*ctx*/) override { }

  virtual void enterCollectionType(ClythV1Parser::CollectionTypeContext * /*ctx*/) override { }
  virtual void exitCollectionType(ClythV1Parser::CollectionTypeContext * /*ctx*/) override { }

  virtual void enterBaseType(ClythV1Parser::BaseTypeContext * /*ctx*/) override { }
  virtual void exitBaseType(ClythV1Parser::BaseTypeContext * /*ctx*/) override { }

  virtual void enterTypeSuffix(ClythV1Parser::TypeSuffixContext * /*ctx*/) override { }
  virtual void exitTypeSuffix(ClythV1Parser::TypeSuffixContext * /*ctx*/) override { }

  virtual void enterCommaOrSemi(ClythV1Parser::CommaOrSemiContext * /*ctx*/) override { }
  virtual void exitCommaOrSemi(ClythV1Parser::CommaOrSemiContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

