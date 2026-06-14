#include "clythLoweringPlan.hpp"

namespace clyth::lowering {

bool ClythLoweringPlan::empty() const {
    return nodes.empty();
}

void ClythLoweringPlan::print() const {
    fmt::print("Linear lowering plan:\n");

    for (const LinearNode& node : nodes) {
        const std::string indent(node.depth * 2, ' ');

        fmt::print(
            "{}#{} {}",
            indent,
            node.index,
            node.kind_name
        );

        if (!node.label.empty()) {
            fmt::print(" [{}]", node.label);
        }

        if (!node.resolved_type.empty() && node.resolved_type != "unknown") {
            fmt::print(" : {}", node.resolved_type);
        }

        if (!node.text.empty()) {
            fmt::print(" text='{}'", node.text);
        }

        if (node.parent_index) {
            fmt::print(" parent=#{}", *node.parent_index);
        }

        fmt::print(" @{}:{}\n", node.location.line, node.location.column);
    }
}

ClythLoweringPlan ClythLoweringPlanBuilder::build(
    const ast::ProgramPtr& program,
    const semantic::SemanticResult& semantics,
    DiagnosticBag& diagnostics
) {
    ClythLoweringPlanBuilder builder(semantics, diagnostics);

    if (!program) {
        diagnostics.add_error(SourceLocation{}, "Cannot build lowering plan from null AST program.");
        return {};
    }

    builder.walk(program, std::nullopt, 0);
    return std::move(builder.plan);
}

ClythLoweringPlanBuilder::ClythLoweringPlanBuilder(
    const semantic::SemanticResult& semantics,
    DiagnosticBag& diagnostics
) : semantics(semantics),
    diagnostics(diagnostics) {}

void ClythLoweringPlanBuilder::walk(
    const ast::NodePtr& node,
    std::optional<std::size_t> parent_index,
    std::size_t depth
) {
    if (!node) {
        return;
    }

    LinearNode linear;
    linear.index = plan.nodes.size();
    linear.parent_index = parent_index;
    linear.depth = depth;
    linear.kind = node->kind;
    linear.kind_name = ast::node_kind_name(node->kind);
    linear.label = node->label;
    linear.text = node->text;
    linear.location = node->location;
    linear.attributes = node->attributes;
    linear.ast_node = node;

    auto type_it = semantics.node_types.find(node.get());

    if (type_it != semantics.node_types.end()) {
        linear.resolved_type = type_it->second.name;
    }

    const std::size_t current_index = linear.index;
    plan.nodes.push_back(std::move(linear));

    for (const auto& child : node->children) {
        walk(child, current_index, depth + 1);
    }
}

} // namespace clyth::lowering
