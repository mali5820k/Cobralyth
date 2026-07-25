
// Generated from ClythV1.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "ClythV1Parser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by ClythV1Parser.
 */
class  ClythV1Visitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by ClythV1Parser.
   */
    virtual std::any visitProgram(ClythV1Parser::ProgramContext *context) = 0;

    virtual std::any visitTopLevelItem(ClythV1Parser::TopLevelItemContext *context) = 0;

    virtual std::any visitIncludeDecl(ClythV1Parser::IncludeDeclContext *context) = 0;

    virtual std::any visitIncludeTarget(ClythV1Parser::IncludeTargetContext *context) = 0;

    virtual std::any visitExternDecl(ClythV1Parser::ExternDeclContext *context) = 0;

    virtual std::any visitExternAbi(ClythV1Parser::ExternAbiContext *context) = 0;

    virtual std::any visitExternParamList(ClythV1Parser::ExternParamListContext *context) = 0;

    virtual std::any visitExternParam(ClythV1Parser::ExternParamContext *context) = 0;

    virtual std::any visitStructDecl(ClythV1Parser::StructDeclContext *context) = 0;

    virtual std::any visitGenericParamList(ClythV1Parser::GenericParamListContext *context) = 0;

    virtual std::any visitStructField(ClythV1Parser::StructFieldContext *context) = 0;

    virtual std::any visitVisibilityModifier(ClythV1Parser::VisibilityModifierContext *context) = 0;

    virtual std::any visitMethodBlock(ClythV1Parser::MethodBlockContext *context) = 0;

    virtual std::any visitMethodDecl(ClythV1Parser::MethodDeclContext *context) = 0;

    virtual std::any visitMethodSimpleName(ClythV1Parser::MethodSimpleNameContext *context) = 0;

    virtual std::any visitFunctionDecl(ClythV1Parser::FunctionDeclContext *context) = 0;

    virtual std::any visitParamList(ClythV1Parser::ParamListContext *context) = 0;

    virtual std::any visitParam(ClythV1Parser::ParamContext *context) = 0;

    virtual std::any visitBlock(ClythV1Parser::BlockContext *context) = 0;

    virtual std::any visitStatement(ClythV1Parser::StatementContext *context) = 0;

    virtual std::any visitVarDecl(ClythV1Parser::VarDeclContext *context) = 0;

    virtual std::any visitAssignmentStmt(ClythV1Parser::AssignmentStmtContext *context) = 0;

    virtual std::any visitAssignable(ClythV1Parser::AssignableContext *context) = 0;

    virtual std::any visitAssignmentOp(ClythV1Parser::AssignmentOpContext *context) = 0;

    virtual std::any visitExprStmt(ClythV1Parser::ExprStmtContext *context) = 0;

    virtual std::any visitReturnStmt(ClythV1Parser::ReturnStmtContext *context) = 0;

    virtual std::any visitIfStmt(ClythV1Parser::IfStmtContext *context) = 0;

    virtual std::any visitElseClause(ClythV1Parser::ElseClauseContext *context) = 0;

    virtual std::any visitWhileStmt(ClythV1Parser::WhileStmtContext *context) = 0;

    virtual std::any visitForStmt(ClythV1Parser::ForStmtContext *context) = 0;

    virtual std::any visitForEachHeader(ClythV1Parser::ForEachHeaderContext *context) = 0;

    virtual std::any visitForEachBinding(ClythV1Parser::ForEachBindingContext *context) = 0;

    virtual std::any visitForEachVar(ClythV1Parser::ForEachVarContext *context) = 0;

    virtual std::any visitForClassicHeader(ClythV1Parser::ForClassicHeaderContext *context) = 0;

    virtual std::any visitForInit(ClythV1Parser::ForInitContext *context) = 0;

    virtual std::any visitVarDeclNoSemi(ClythV1Parser::VarDeclNoSemiContext *context) = 0;

    virtual std::any visitAssignmentNoSemi(ClythV1Parser::AssignmentNoSemiContext *context) = 0;

    virtual std::any visitBreakStmt(ClythV1Parser::BreakStmtContext *context) = 0;

    virtual std::any visitContinueStmt(ClythV1Parser::ContinueStmtContext *context) = 0;

    virtual std::any visitMeccBlock(ClythV1Parser::MeccBlockContext *context) = 0;

    virtual std::any visitParenExpr(ClythV1Parser::ParenExprContext *context) = 0;

    virtual std::any visitExpression(ClythV1Parser::ExpressionContext *context) = 0;

    virtual std::any visitLogicalOr(ClythV1Parser::LogicalOrContext *context) = 0;

    virtual std::any visitLogicalAnd(ClythV1Parser::LogicalAndContext *context) = 0;

    virtual std::any visitEquality(ClythV1Parser::EqualityContext *context) = 0;

    virtual std::any visitRelation(ClythV1Parser::RelationContext *context) = 0;

    virtual std::any visitComparison(ClythV1Parser::ComparisonContext *context) = 0;

    virtual std::any visitAdditive(ClythV1Parser::AdditiveContext *context) = 0;

    virtual std::any visitMultiplicative(ClythV1Parser::MultiplicativeContext *context) = 0;

    virtual std::any visitUnary(ClythV1Parser::UnaryContext *context) = 0;

    virtual std::any visitPostfix(ClythV1Parser::PostfixContext *context) = 0;

    virtual std::any visitPostfixSuffix(ClythV1Parser::PostfixSuffixContext *context) = 0;

    virtual std::any visitPrimary(ClythV1Parser::PrimaryContext *context) = 0;

    virtual std::any visitLambdaExpression(ClythV1Parser::LambdaExpressionContext *context) = 0;

    virtual std::any visitLambdaParamList(ClythV1Parser::LambdaParamListContext *context) = 0;

    virtual std::any visitLambdaParam(ClythV1Parser::LambdaParamContext *context) = 0;

    virtual std::any visitArgumentList(ClythV1Parser::ArgumentListContext *context) = 0;

    virtual std::any visitLiteral(ClythV1Parser::LiteralContext *context) = 0;

    virtual std::any visitCollectionLiteral(ClythV1Parser::CollectionLiteralContext *context) = 0;

    virtual std::any visitListLiteral(ClythV1Parser::ListLiteralContext *context) = 0;

    virtual std::any visitCurlyLiteral(ClythV1Parser::CurlyLiteralContext *context) = 0;

    virtual std::any visitCurlyEntryList(ClythV1Parser::CurlyEntryListContext *context) = 0;

    virtual std::any visitCurlyEntry(ClythV1Parser::CurlyEntryContext *context) = 0;

    virtual std::any visitExpressionList(ClythV1Parser::ExpressionListContext *context) = 0;

    virtual std::any visitType(ClythV1Parser::TypeContext *context) = 0;

    virtual std::any visitFixedArrayType(ClythV1Parser::FixedArrayTypeContext *context) = 0;

    virtual std::any visitDynamicArrayType(ClythV1Parser::DynamicArrayTypeContext *context) = 0;

    virtual std::any visitFunctionType(ClythV1Parser::FunctionTypeContext *context) = 0;

    virtual std::any visitFunctionParamTypeList(ClythV1Parser::FunctionParamTypeListContext *context) = 0;

    virtual std::any visitGenericType(ClythV1Parser::GenericTypeContext *context) = 0;

    virtual std::any visitGenericArgList(ClythV1Parser::GenericArgListContext *context) = 0;

    virtual std::any visitTypeAtom(ClythV1Parser::TypeAtomContext *context) = 0;

    virtual std::any visitBaseType(ClythV1Parser::BaseTypeContext *context) = 0;

    virtual std::any visitCommaOrSemi(ClythV1Parser::CommaOrSemiContext *context) = 0;


};

