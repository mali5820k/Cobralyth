#ifndef CLYTH_LOWERING_PLAN_HPP
#define CLYTH_LOWERING_PLAN_HPP

#include "common.hpp"
#include "clyth_ast.hpp"
#include "clyth_semantic.hpp"

#include <optional>
#include <unordered_map>

namespace clyth::lowering {

// ============================================================
// Linear Lowering Plan
// ============================================================
//
// This is NOT LLVM IR yet.
//
// This is a codegen-friendly linearized view of the AST so LLVM lowering can
// iterate through stable records instead of recursively wandering the AST.
//
// The original AST remains attached by shared_ptr so LLVM codegen can inspect
// details when needed. The linear layer gives you:
//   - stable ordering
//   - parent index
//   - depth
//   - kind/label/text
//   - resolved type when semantic analysis knows it
//
// Later LLVM codegen can consume this as:
//
//   for (const LinearNode& op : plan.nodes) {
//       switch (op.kind) {
//           case FunctionDecl: create LLVM function
//           case BlockStmt:    create block/scope
//           case ReturnStmt:   emit ret
//           ...
//       }
//   }
// ============================================================

struct LinearNode {
    std::size_t index = 0;
    std::optional<std::size_t> parent_index;
    std::size_t depth = 0;

    ast::NodeKind kind = ast::NodeKind::Generic;
    std::string kind_name;
    std::string label;
    std::string text;
    SourceLocation location {};

    std::string resolved_type = "unknown";
    std::map<std::string, std::string> attributes;

    ast::NodePtr ast_node;
};

struct ClythLoweringPlan {
    std::vector<LinearNode> nodes;

    bool empty() const;
    void print() const;
};

class ClythLoweringPlanBuilder {
public:
    static ClythLoweringPlan build(
        const ast::ProgramPtr& program,
        const semantic::SemanticResult& semantics,
        DiagnosticBag& diagnostics
    );

private:
    ClythLoweringPlanBuilder(
        const semantic::SemanticResult& semantics,
        DiagnosticBag& diagnostics
    );

    void walk(
        const ast::NodePtr& node,
        std::optional<std::size_t> parent_index,
        std::size_t depth
    );

    const semantic::SemanticResult& semantics;
    DiagnosticBag& diagnostics;
    ClythLoweringPlan plan;
};

} // namespace clyth::lowering

#endif // CLYTH_LOWERING_PLAN_HPP
