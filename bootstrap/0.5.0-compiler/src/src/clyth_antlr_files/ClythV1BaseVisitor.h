
// Generated from ClythV1.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "ClythV1Visitor.h"


/**
 * This class provides an empty implementation of ClythV1Visitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  ClythV1BaseVisitor : public ClythV1Visitor {
public:

  virtual std::any visitProgram(ClythV1Parser::ProgramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTopLevelItem(ClythV1Parser::TopLevelItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIncludeDecl(ClythV1Parser::IncludeDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIncludeTarget(ClythV1Parser::IncludeTargetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExternDecl(ClythV1Parser::ExternDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExternAbi(ClythV1Parser::ExternAbiContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExternParamList(ClythV1Parser::ExternParamListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExternParam(ClythV1Parser::ExternParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStructDecl(ClythV1Parser::StructDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGenericParamList(ClythV1Parser::GenericParamListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStructField(ClythV1Parser::StructFieldContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVisibilityModifier(ClythV1Parser::VisibilityModifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMethodBlock(ClythV1Parser::MethodBlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMethodDecl(ClythV1Parser::MethodDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMethodSimpleName(ClythV1Parser::MethodSimpleNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionDecl(ClythV1Parser::FunctionDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParamList(ClythV1Parser::ParamListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParam(ClythV1Parser::ParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(ClythV1Parser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatement(ClythV1Parser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarDecl(ClythV1Parser::VarDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignmentStmt(ClythV1Parser::AssignmentStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignable(ClythV1Parser::AssignableContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignmentOp(ClythV1Parser::AssignmentOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprStmt(ClythV1Parser::ExprStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnStmt(ClythV1Parser::ReturnStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIfStmt(ClythV1Parser::IfStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitElseClause(ClythV1Parser::ElseClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWhileStmt(ClythV1Parser::WhileStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForStmt(ClythV1Parser::ForStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForEachHeader(ClythV1Parser::ForEachHeaderContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForEachBinding(ClythV1Parser::ForEachBindingContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForEachVar(ClythV1Parser::ForEachVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForClassicHeader(ClythV1Parser::ForClassicHeaderContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForInit(ClythV1Parser::ForInitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarDeclNoSemi(ClythV1Parser::VarDeclNoSemiContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignmentNoSemi(ClythV1Parser::AssignmentNoSemiContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBreakStmt(ClythV1Parser::BreakStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitContinueStmt(ClythV1Parser::ContinueStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMeccBlock(ClythV1Parser::MeccBlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParenExpr(ClythV1Parser::ParenExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpression(ClythV1Parser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLogicalOr(ClythV1Parser::LogicalOrContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLogicalAnd(ClythV1Parser::LogicalAndContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEquality(ClythV1Parser::EqualityContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelation(ClythV1Parser::RelationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitComparison(ClythV1Parser::ComparisonContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAdditive(ClythV1Parser::AdditiveContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMultiplicative(ClythV1Parser::MultiplicativeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnary(ClythV1Parser::UnaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPostfix(ClythV1Parser::PostfixContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPostfixSuffix(ClythV1Parser::PostfixSuffixContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimary(ClythV1Parser::PrimaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLambdaExpression(ClythV1Parser::LambdaExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLambdaParamList(ClythV1Parser::LambdaParamListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLambdaParam(ClythV1Parser::LambdaParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArgumentList(ClythV1Parser::ArgumentListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral(ClythV1Parser::LiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCollectionLiteral(ClythV1Parser::CollectionLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitListLiteral(ClythV1Parser::ListLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCurlyLiteral(ClythV1Parser::CurlyLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCurlyEntryList(ClythV1Parser::CurlyEntryListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCurlyEntry(ClythV1Parser::CurlyEntryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionList(ClythV1Parser::ExpressionListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitType(ClythV1Parser::TypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFixedArrayType(ClythV1Parser::FixedArrayTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDynamicArrayType(ClythV1Parser::DynamicArrayTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionType(ClythV1Parser::FunctionTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionParamTypeList(ClythV1Parser::FunctionParamTypeListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGenericType(ClythV1Parser::GenericTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGenericArgList(ClythV1Parser::GenericArgListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypeAtom(ClythV1Parser::TypeAtomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBaseType(ClythV1Parser::BaseTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCommaOrSemi(ClythV1Parser::CommaOrSemiContext *ctx) override {
    return visitChildren(ctx);
  }


};

