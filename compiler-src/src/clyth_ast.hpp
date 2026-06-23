#ifndef CLYTH_AST_HPP
#define CLYTH_AST_HPP

#include "common.hpp"
#include <any>

// ANTLR runtime.
#include "antlr4-runtime.h"

// Generated ANTLR parser/visitor files.
// Do not modify generated ANTLR files. Extend them from here instead.
#include "clyth_antlr_files/ClythV1Parser.h"
#include "clyth_antlr_files/ClythV1BaseVisitor.h"

namespace clyth {

// ============================================================
// Diagnostics
// ============================================================

struct SourceLocation {
    std::size_t line = 0;
    std::size_t column = 0;
};

struct Diagnostic {
    enum class Severity {
        Info,
        Warning,
        Error
    };

    Severity severity = Severity::Error;
    SourceLocation location {};
    std::string message;
};

class DiagnosticBag {
public:
    void add_error(SourceLocation location, std::string message);
    void add_warning(SourceLocation location, std::string message);
    void add_info(SourceLocation location, std::string message);

    bool has_errors() const;
    std::size_t error_count() const;
    const std::vector<Diagnostic>& all() const;

    void print_to_stderr(const std::filesystem::path& file = {}) const;

private:
    std::vector<Diagnostic> diagnostics;
};

// ANTLR syntax-error hook.
// This catches lexer/parser syntax errors before AST generation.
class ClythSyntaxErrorListener : public antlr4::BaseErrorListener {
public:
    explicit ClythSyntaxErrorListener(DiagnosticBag& diagnostics);

    void syntaxError(
        antlr4::Recognizer* recognizer,
        antlr4::Token* offending_symbol,
        std::size_t line,
        std::size_t char_position_in_line,
        const std::string& msg,
        std::exception_ptr e
    ) override;

private:
    DiagnosticBag& diagnostics;
};

// ============================================================
// AST Nodes
// ============================================================

namespace ast {

enum class NodeKind {
    Program,

    IncludeDecl,
    ExternDecl,
    StructDecl,
    StructField,
    MethodBlock,
    MethodDecl,
    FunctionDecl,
    Param,

    BlockStmt,
    VarDeclStmt,
    AssignmentStmt,
    ExprStmt,
    ReturnStmt,
    IfStmt,
    ElseClause,
    WhileStmt,
    ForStmt,
    BreakStmt,
    ContinueStmt,
    MeccBlockStmt,

    Type,
    LiteralExpr,
    IdentifierExpr,
    BinaryExpr,
    UnaryExpr,
    CallExpr,
    MemberAccessExpr,
    IndexExpr,
    PostfixExpr,
    AllocationExpr,

    ListLiteralExpr,
    CurlyLiteralExpr,
    CurlyEntry,

    Generic,
    Token
};

std::string node_kind_name(NodeKind kind);

struct ASTNode {
    NodeKind kind = NodeKind::Generic;
    SourceLocation location {};
    std::string label;
    std::string text;
    std::map<std::string, std::string> attributes;
    std::vector<std::shared_ptr<ASTNode>> children;

    ASTNode() = default;
    ASTNode(NodeKind kind, SourceLocation location, std::string label = {});
};

using NodePtr = std::shared_ptr<ASTNode>;
using ProgramPtr = NodePtr;

void print_ast(const NodePtr& node, int indent = 0);

std::string to_json(const NodePtr& node, int indent = 0);
std::string to_clyth_bytecode(const NodePtr& node);
bool write_json_file(const NodePtr& node, const std::filesystem::path& path);
bool write_bytecode_file(const NodePtr& node, const std::filesystem::path& path);

} // namespace ast

// ============================================================
// AST Builder Visitor
// ============================================================
//
// ClythAST translates ANTLR's parse tree into Clyth's own AST.
//
// ANTLR parse tree = grammar-shaped.
// Clyth AST        = compiler-shaped.
//
// For now, this builder creates a clean structural AST that is good enough
// for the next phase: semantic analysis. More specialized node classes can
// be introduced later without changing the overall pipeline.
// ============================================================

class ClythAST : public ClythV1BaseVisitor {
public:
    explicit ClythAST(DiagnosticBag& diagnostics);

    ast::ProgramPtr build(ClythV1Parser::ProgramContext* ctx);

    std::any visitProgram(ClythV1Parser::ProgramContext* ctx) override;
    std::any visitTopLevelItem(ClythV1Parser::TopLevelItemContext* ctx) override;
    std::any visitIncludeDecl(ClythV1Parser::IncludeDeclContext* ctx) override;
    std::any visitIncludeTarget(ClythV1Parser::IncludeTargetContext* ctx) override;
    std::any visitExternDecl(ClythV1Parser::ExternDeclContext* ctx) override;
    std::any visitExternAbi(ClythV1Parser::ExternAbiContext* ctx) override;
    std::any visitExternParamList(ClythV1Parser::ExternParamListContext* ctx) override;
    std::any visitExternParam(ClythV1Parser::ExternParamContext* ctx) override;
    std::any visitStructDecl(ClythV1Parser::StructDeclContext* ctx) override;
    std::any visitStructField(ClythV1Parser::StructFieldContext* ctx) override;

    // Requires grammar regeneration after adding methodBlock/methodDecl rules.
    std::any visitMethodBlock(ClythV1Parser::MethodBlockContext* ctx) override;
    std::any visitMethodDecl(ClythV1Parser::MethodDeclContext* ctx) override;

    std::any visitFunctionDecl(ClythV1Parser::FunctionDeclContext* ctx) override;
    std::any visitParamList(ClythV1Parser::ParamListContext* ctx) override;
    std::any visitParam(ClythV1Parser::ParamContext* ctx) override;
    std::any visitBlock(ClythV1Parser::BlockContext* ctx) override;
    std::any visitStatement(ClythV1Parser::StatementContext* ctx) override;
    std::any visitVarDecl(ClythV1Parser::VarDeclContext* ctx) override;
    std::any visitAssignmentStmt(ClythV1Parser::AssignmentStmtContext* ctx) override;
    std::any visitAssignable(ClythV1Parser::AssignableContext* ctx) override;
    std::any visitAssignmentOp(ClythV1Parser::AssignmentOpContext* ctx) override;
    std::any visitExprStmt(ClythV1Parser::ExprStmtContext* ctx) override;
    std::any visitReturnStmt(ClythV1Parser::ReturnStmtContext* ctx) override;
    std::any visitIfStmt(ClythV1Parser::IfStmtContext* ctx) override;
    std::any visitElseClause(ClythV1Parser::ElseClauseContext* ctx) override;
    std::any visitWhileStmt(ClythV1Parser::WhileStmtContext* ctx) override;
    std::any visitForStmt(ClythV1Parser::ForStmtContext* ctx) override;
    std::any visitForEachHeader(ClythV1Parser::ForEachHeaderContext* ctx) override;
    std::any visitForEachBinding(ClythV1Parser::ForEachBindingContext* ctx) override;
    std::any visitForEachVar(ClythV1Parser::ForEachVarContext* ctx) override;
    std::any visitForClassicHeader(ClythV1Parser::ForClassicHeaderContext* ctx) override;
    std::any visitForInit(ClythV1Parser::ForInitContext* ctx) override;
    std::any visitVarDeclNoSemi(ClythV1Parser::VarDeclNoSemiContext* ctx) override;
    std::any visitAssignmentNoSemi(ClythV1Parser::AssignmentNoSemiContext* ctx) override;
    std::any visitBreakStmt(ClythV1Parser::BreakStmtContext* ctx) override;
    std::any visitContinueStmt(ClythV1Parser::ContinueStmtContext* ctx) override;
    std::any visitMeccBlock(ClythV1Parser::MeccBlockContext* ctx) override;
    std::any visitParenExpr(ClythV1Parser::ParenExprContext* ctx) override;
    std::any visitExpression(ClythV1Parser::ExpressionContext* ctx) override;
    std::any visitLogicalOr(ClythV1Parser::LogicalOrContext* ctx) override;
    std::any visitLogicalAnd(ClythV1Parser::LogicalAndContext* ctx) override;
    std::any visitEquality(ClythV1Parser::EqualityContext* ctx) override;
    std::any visitRelation(ClythV1Parser::RelationContext* ctx) override;
    std::any visitComparison(ClythV1Parser::ComparisonContext* ctx) override;
    std::any visitAdditive(ClythV1Parser::AdditiveContext* ctx) override;
    std::any visitMultiplicative(ClythV1Parser::MultiplicativeContext* ctx) override;
    std::any visitUnary(ClythV1Parser::UnaryContext* ctx) override;
    std::any visitPostfix(ClythV1Parser::PostfixContext* ctx) override;
    std::any visitPostfixSuffix(ClythV1Parser::PostfixSuffixContext* ctx) override;
    std::any visitPrimary(ClythV1Parser::PrimaryContext* ctx) override;
    std::any visitAllocationExpression(ClythV1Parser::AllocationExpressionContext* ctx) override;
    std::any visitArgumentList(ClythV1Parser::ArgumentListContext* ctx) override;
    std::any visitLiteral(ClythV1Parser::LiteralContext* ctx) override;
    std::any visitCollectionLiteral(ClythV1Parser::CollectionLiteralContext* ctx) override;
    std::any visitListLiteral(ClythV1Parser::ListLiteralContext* ctx) override;
    std::any visitCurlyLiteral(ClythV1Parser::CurlyLiteralContext* ctx) override;
    std::any visitCurlyEntryList(ClythV1Parser::CurlyEntryListContext* ctx) override;
    std::any visitCurlyEntry(ClythV1Parser::CurlyEntryContext* ctx) override;
    std::any visitExpressionList(ClythV1Parser::ExpressionListContext* ctx) override;
    std::any visitType(ClythV1Parser::TypeContext* ctx) override;
    std::any visitFixedArrayType(ClythV1Parser::FixedArrayTypeContext* ctx) override;
    std::any visitDynamicArrayType(ClythV1Parser::DynamicArrayTypeContext* ctx) override;
    std::any visitGenericType(ClythV1Parser::GenericTypeContext* ctx) override;
    std::any visitGenericArgList(ClythV1Parser::GenericArgListContext* ctx) override;
    std::any visitTypeAtom(ClythV1Parser::TypeAtomContext* ctx) override;
    std::any visitBaseType(ClythV1Parser::BaseTypeContext* ctx) override;
    std::any visitCommaOrSemi(ClythV1Parser::CommaOrSemiContext* ctx) override;

private:
    DiagnosticBag& diagnostics;

    SourceLocation location_from(antlr4::ParserRuleContext* ctx) const;

    ast::NodePtr make_node(
        ast::NodeKind kind,
        antlr4::ParserRuleContext* ctx,
        std::string label = {}
    ) const;

    ast::NodePtr make_token_node(antlr4::tree::TerminalNode* terminal) const;

    ast::NodePtr any_to_node(const std::any& value, const std::string& caller);
    ast::NodePtr visit_single_rule_child(antlr4::ParserRuleContext* ctx);
    ast::NodePtr build_generic(
        ast::NodeKind kind,
        antlr4::ParserRuleContext* ctx,
        std::string label = {}
    );

    std::vector<ast::NodePtr> visit_rule_children(antlr4::ParserRuleContext* ctx);

    bool is_ignored_terminal(antlr4::tree::TerminalNode* terminal) const;
};

// ============================================================
// Semantic Analysis Stub
// ============================================================
//
// This is intentionally lightweight right now.
// Next step after AST generation:
// - symbol table
// - duplicate declaration checks
// - type resolution
// - expression typing
// - return validation
// - MECC-scope validation
// ============================================================

class ClythSemanticAnalyzer {
public:
    explicit ClythSemanticAnalyzer(DiagnosticBag& diagnostics);

    bool analyze(const ast::ProgramPtr& program);

private:
    DiagnosticBag& diagnostics;

    void analyze_node(const ast::NodePtr& node);
    void analyze_program(const ast::NodePtr& node);
    void analyze_function_decl(const ast::NodePtr& node);
    void analyze_struct_decl(const ast::NodePtr& node);
    void analyze_var_decl(const ast::NodePtr& node);
};

} // namespace clyth

#endif // CLYTH_AST_HPP
