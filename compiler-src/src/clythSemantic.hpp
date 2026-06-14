#ifndef CLYTH_SEMANTIC_HPP
#define CLYTH_SEMANTIC_HPP

#include "common.hpp"
#include "clythAST.hpp"

#include <optional>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace clyth::semantic {

// ============================================================
// Semantic Model
// ============================================================
//
// This file intentionally keeps semantic analysis separated from:
//   - ANTLR parse-tree handling
//   - AST construction
//   - LLVM IR generation
//
// The goal is a pass-pipeline style design:
//
//   AST
//    -> declaration collection
//    -> type collection/checking
//    -> scope/symbol checking
//    -> control-flow checks
//    -> MECC annotation checks
//    -> semantic result consumed by LLVM lowering later
//
// Adding a new semantic rule should usually mean:
//   1. Create another ISemanticPass implementation.
//   2. Register it in ClythSemanticPipeline::register_default_passes().
//   3. Keep AST building and LLVM codegen untouched.
// ============================================================

enum class SymbolKind {
    Include,
    Struct,
    Field,
    Method,
    Function,
    ExternFunction,
    GlobalVariable,
    LocalVariable,
    Parameter
};

std::string symbol_kind_name(SymbolKind kind);

struct TypeInfo {
    std::string name = "unknown";
    bool is_builtin = false;
    bool is_void = false;
    bool is_auto = false;
    bool is_error = false;

    static TypeInfo unknown();
    static TypeInfo error(std::string name = "<error>");

    bool operator==(const TypeInfo& other) const;
    bool operator!=(const TypeInfo& other) const;
};

struct Symbol {
    std::string name;
    SymbolKind kind = SymbolKind::LocalVariable;
    TypeInfo type;
    SourceLocation location {};
    ast::NodePtr declaration;
};

struct Scope {
    std::string name;
    Scope* parent = nullptr;
    std::unordered_map<std::string, Symbol> symbols;

    Scope(std::string name, Scope* parent = nullptr);

    bool declare(const Symbol& symbol, DiagnosticBag& diagnostics);
    Symbol* find_local(const std::string& symbol_name);
    Symbol* find_recursive(const std::string& symbol_name);
};

struct FunctionFrame {
    std::string name;
    TypeInfo return_type;
    SourceLocation location {};
};

struct MeccAllocationInfo {
    const ast::ASTNode* node = nullptr;
    std::string variable_name;
    std::string allocation_kind; // malloc, iso_malloc
    std::string estate;          // manual, current, independent
    bool requires_estate_counter = false;
    bool requires_shared_handle = false;
    bool escapes_current_scope = false;
    SourceLocation location {};
};

struct SemanticResult {
    bool ok = false;

    std::vector<MeccAllocationInfo> mecc_allocations;
    std::vector<Symbol> global_symbols;
    std::unordered_map<const ast::ASTNode*, TypeInfo> node_types;
    std::unordered_map<const ast::ASTNode*, const Symbol*> resolved_symbols;

    void print_summary() const;
};

class SemanticContext {
public:
    explicit SemanticContext(DiagnosticBag& diagnostics);

    DiagnosticBag& diagnostics;

    SemanticResult result;

    // Global namespace of known structs/functions/externs.
    Scope global_scope;

    // Owned scopes are kept alive for symbol pointers.
    std::vector<std::unique_ptr<Scope>> owned_scopes;
    Scope* current_scope = nullptr;

    std::vector<FunctionFrame> function_stack;
    std::size_t loop_depth = 0;
    std::size_t mecc_depth = 0;

    Scope& push_scope(const std::string& name);
    void pop_scope();

    bool declare_global(const Symbol& symbol);
    bool declare_local(const Symbol& symbol);

    Symbol* find_symbol(const std::string& name);
    bool is_known_type(const TypeInfo& type) const;

    TypeInfo parse_type_text(const std::string& text) const;
    TypeInfo type_of_node(const ast::NodePtr& node) const;
    void set_node_type(const ast::NodePtr& node, const TypeInfo& type);

    bool in_function() const;
    const FunctionFrame* current_function() const;
    void push_function(FunctionFrame frame);
    void pop_function();
};

// ============================================================
// AST Query Helpers
// ============================================================
//
// These helpers intentionally work with the generic AST shape created by
// ClythAST. If later you replace generic ASTNode with specialized C++ classes,
// this helper layer is the main area to adapt.
// ============================================================

namespace query {

bool has_label(const ast::NodePtr& node, const std::string& label);
bool text_is(const ast::NodePtr& node, const std::string& text);

std::optional<std::string> attr(const ast::NodePtr& node, const std::string& key);

std::vector<ast::NodePtr> children_of_kind(const ast::NodePtr& node, ast::NodeKind kind);
ast::NodePtr first_child_of_kind(const ast::NodePtr& node, ast::NodeKind kind);
ast::NodePtr first_child_with_label(const ast::NodePtr& node, const std::string& label);

std::vector<ast::NodePtr> token_children(const ast::NodePtr& node);
std::vector<std::string> token_texts(const ast::NodePtr& node);

bool contains_token_text(const ast::NodePtr& node, const std::string& token_text);

std::optional<std::string> declared_name(const ast::NodePtr& node);
std::optional<TypeInfo> declared_type(const ast::NodePtr& node, const SemanticContext& context);

bool is_expression_node(const ast::NodePtr& node);
bool is_statement_node(const ast::NodePtr& node);
bool is_declaration_node(const ast::NodePtr& node);

} // namespace query

// ============================================================
// Pass Interface
// ============================================================

class ISemanticPass {
public:
    virtual ~ISemanticPass() = default;

    virtual std::string name() const = 0;
    virtual void run(SemanticContext& context, const ast::ProgramPtr& program) = 0;
};

// Collects top-level includes, structs, functions, externs, and globals.
class TopLevelDeclarationPass final : public ISemanticPass {
public:
    std::string name() const override;
    void run(SemanticContext& context, const ast::ProgramPtr& program) override;
};

// Checks struct field names and field type existence.
class StructValidationPass final : public ISemanticPass {
public:
    std::string name() const override;
    void run(SemanticContext& context, const ast::ProgramPtr& program) override;
};

// Checks function signatures: return type, params, duplicate params.
class FunctionSignaturePass final : public ISemanticPass {
public:
    std::string name() const override;
    void run(SemanticContext& context, const ast::ProgramPtr& program) override;
};

// Builds local scopes and checks duplicate local variables / basic identifiers.
class ScopeAndSymbolPass final : public ISemanticPass {
public:
    std::string name() const override;
    void run(SemanticContext& context, const ast::ProgramPtr& program) override;

private:
    void visit_node(SemanticContext& context, const ast::NodePtr& node);
    void visit_function(SemanticContext& context, const ast::NodePtr& node);
    void visit_block(SemanticContext& context, const ast::NodePtr& node, const std::string& scope_name);
    void visit_var_decl(SemanticContext& context, const ast::NodePtr& node);
    void visit_assignment(SemanticContext& context, const ast::NodePtr& node);
};

// Return/break/continue sanity checks.
class ControlFlowPass final : public ISemanticPass {
public:
    std::string name() const override;
    void run(SemanticContext& context, const ast::ProgramPtr& program) override;

private:
    void visit_node(SemanticContext& context, const ast::NodePtr& node);
    void visit_function(SemanticContext& context, const ast::NodePtr& node);
};

// MECC-specific annotation/check placeholder pass.
class MethodValidationPass final : public ISemanticPass {
public:
    std::string name() const override;
    void run(SemanticContext& context, const ast::ProgramPtr& program) override;
};

class MeccSemanticPass final : public ISemanticPass {
public:
    std::string name() const override;
    void run(SemanticContext& context, const ast::ProgramPtr& program) override;

private:
    void visit_node(SemanticContext& context, const ast::NodePtr& node);
};

// ============================================================
// Semantic Pipeline
// ============================================================

class ClythSemanticPipeline {
public:
    explicit ClythSemanticPipeline(DiagnosticBag& diagnostics);

    SemanticResult analyze(const ast::ProgramPtr& program);

    void add_pass(std::unique_ptr<ISemanticPass> pass);
    void register_default_passes();

private:
    DiagnosticBag& diagnostics;
    std::vector<std::unique_ptr<ISemanticPass>> passes;
};

} // namespace clyth::semantic

#endif // CLYTH_SEMANTIC_HPP
