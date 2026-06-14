#include "clythLLVMStub.hpp"

namespace clyth::llvmstub {

ClythLLVMCodegen::ClythLLVMCodegen(DiagnosticBag& diagnostics)
    : diagnostics(diagnostics) {}

bool ClythLLVMCodegen::emit(
    const lowering::ClythLoweringPlan& plan,
    const semantic::SemanticResult& semantics,
    const CodegenConfig& config
) {
    if (plan.empty()) {
        diagnostics.add_error(SourceLocation{}, "LLVM codegen received an empty lowering plan.");
        return false;
    }

    if (config.dump_codegen_plan) {
        fmt::print("LLVM codegen stub for module '{}'\n", config.module_name);
    }

    return emit_program(plan, semantics) && !diagnostics.has_errors();
}

bool ClythLLVMCodegen::emit_program(
    const lowering::ClythLoweringPlan& plan,
    const semantic::SemanticResult& semantics
) {
    for (const lowering::LinearNode& node : plan.nodes) {
        switch (node.kind) {
            case ast::NodeKind::StructDecl:
                if (!emit_struct_stub(node, semantics)) return false;
                break;

            case ast::NodeKind::FunctionDecl:
                if (!emit_function_stub(node, semantics)) return false;
                break;

            case ast::NodeKind::MethodDecl:
                if (!emit_method_stub(node, semantics)) return false;
                break;

            case ast::NodeKind::ReturnStmt:
            case ast::NodeKind::VarDeclStmt:
            case ast::NodeKind::AssignmentStmt:
            case ast::NodeKind::ExprStmt:
            case ast::NodeKind::IfStmt:
            case ast::NodeKind::WhileStmt:
            case ast::NodeKind::ForStmt:
            case ast::NodeKind::BreakStmt:
            case ast::NodeKind::ContinueStmt:
            case ast::NodeKind::MeccBlockStmt:
                if (!emit_statement_stub(node, semantics)) return false;
                break;

            case ast::NodeKind::AllocationExpr:
                if (!emit_mecc_allocation_stub(node, semantics)) return false;
                break;

            case ast::NodeKind::LiteralExpr:
            case ast::NodeKind::IdentifierExpr:
            case ast::NodeKind::BinaryExpr:
            case ast::NodeKind::UnaryExpr:
            case ast::NodeKind::CallExpr:
            case ast::NodeKind::MemberAccessExpr:
            case ast::NodeKind::IndexExpr:
            case ast::NodeKind::PostfixExpr:
            case ast::NodeKind::ListLiteralExpr:
            case ast::NodeKind::MapLiteralExpr:
            case ast::NodeKind::SetLiteralExpr:
                if (!emit_expression_stub(node, semantics)) return false;
                break;

            default:
                break;
        }
    }

    return true;
}

bool ClythLLVMCodegen::emit_function_stub(const lowering::LinearNode& node, const semantic::SemanticResult&) {
    (void)node;
    // TODO:
    // - create llvm::FunctionType from semantic return/param types
    // - create llvm::Function
    // - create entry BasicBlock
    // - register params in codegen scope
    return true;
}

bool ClythLLVMCodegen::emit_method_stub(const lowering::LinearNode& node, const semantic::SemanticResult&) {
    (void)node;
    // TODO:
    // - lower method name to stable symbol, e.g. MyStruct.method
    // - optionally pass implicit self pointer later
    // - enforce visibility only during semantic analysis, not here
    return true;
}

bool ClythLLVMCodegen::emit_struct_stub(const lowering::LinearNode& node, const semantic::SemanticResult&) {
    (void)node;
    // TODO:
    // - create llvm::StructType
    // - map field order from AST/semantic result
    // - store in codegen type table
    return true;
}

bool ClythLLVMCodegen::emit_statement_stub(const lowering::LinearNode& node, const semantic::SemanticResult&) {
    (void)node;
    // TODO:
    // - dispatch return/var/assignment/control flow
    // - create blocks for if/while/for
    // - use resolved symbols/types from SemanticResult
    return true;
}

bool ClythLLVMCodegen::emit_expression_stub(const lowering::LinearNode& node, const semantic::SemanticResult&) {
    (void)node;
    // TODO:
    // - emit constants
    // - emit loads
    // - emit binary/unary ops
    // - emit calls/member access/indexing
    return true;
}

bool ClythLLVMCodegen::emit_mecc_allocation_stub(const lowering::LinearNode& node, const semantic::SemanticResult&) {
    (void)node;
    // TODO:
    // - if attr estate=manual, lower to malloc/free flow
    // - if attr estate=current, lower to current estate allocation
    // - if attr estate=independent, lower to independent estate allocation
    // - emit retain/release counter calls once MECC runtime exists
    return true;
}

} // namespace clyth::llvmstub
