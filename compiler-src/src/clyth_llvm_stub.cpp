#include "clyth_llvm_stub.hpp"

namespace clyth::llvm_stub {

ClythLLVMCodegen::ClythLLVMCodegen(DiagnosticBag& diagnostics)
    : diagnostics(diagnostics),
      module(std::make_unique<llvm::Module>("clyth_module", context)),
      builder(context) {}

bool ClythLLVMCodegen::emit(
    const lowering::ClythLoweringPlan& plan,
    const semantic::SemanticResult& semantics,
    const CodegenConfig& config
) {
    if (plan.empty()) {
        diagnostics.add_error(SourceLocation{}, "LLVM codegen received an empty lowering plan.");
        return false;
    }

    module->setModuleIdentifier(config.module_name);
    module->setSourceFileName(config.module_name);

    if (config.dump_codegen_plan) {
        fmt::print("LLVM codegen stub for module '{}'\n", config.module_name);
    }

    const bool ok = emit_program(plan, semantics);
    if (!ok || diagnostics.has_errors()) {
        return false;
    }

    if (llvm::verifyModule(*module, &llvm::errs())) {
        diagnostics.add_error(SourceLocation{}, "LLVM module verification failed.");
        return false;
    }

    module->print(llvm::outs(), nullptr);

    return true;
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

            case ast::NodeKind::ExternDecl:
                if (!emit_extern_function_stub(node, semantics)) return false;
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
            case ast::NodeKind::CurlyLiteralExpr:
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

bool ClythLLVMCodegen::emit_extern_function_stub(const lowering::LinearNode& node, const semantic::SemanticResult&) {
    (void)node;
    // TODO:
    // - create llvm::FunctionType from semantic return/param types
    // - create llvm::Function
    // - create entry BasicBlock
    // - register params in codegen scope
    if (node.kind == ast::NodeKind::ExternDecl && node.text.find("printf") != std::string::npos) {
        emit_printf_decl();
    }
    if (node.kind == ast::NodeKind::FunctionDecl && node.text.find("main") != std::string::npos) {
        return emit_main_function_stub(node, semantics);
    }
    return true;
}

bool ClythLLVMCodegen::emit_printf_decl() {
    llvm::Type* int32_type = llvm::Type::getInt32Ty(context);
    llvm::Type* string_type = llvm::PointerType::get(context, 0);
    std::vector<llvm::Type*> params;
    params.push_back(string_type);
    llvm::FunctionType* printf_type = llvm::FunctionType::get(int32_type, params, true);
    llvm::Function* printf_function = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, "printf", module.get());
    functions["printf"] = printf_function;
    return true;
}

bool ClythLLVMCodegen::emit_main_function_stub(const lowering::LinearNode& node, const semantic::SemanticResult&) {
    (void)node;
    llvm::Type* int32_type = llvm::Type::getInt32Ty(context);
    llvm::FunctionType::get(int32_type, false);
    llvm::Function* main_function = llvm::Function::Create(main_type, llvm::Function::ExternalLinkage, "main", module.get());
    functions["main"] = main_function;
    llvm::BasicBlock* entry_block = llvm::BasicBlock::Create(context, "entry", main_function);
    builder.SetInsertPoint(entry_block);
    llvm::Function* printf_function = functions.at("printf");
    llvm::Value* msg = builder.CreateGlobalStringPtr("\nSimplest extern-C function hookup\n");
    builder.CreateCall(printf_function, {msg});
    builder.CreateRet(llvm::ConstnatInt::get(int32_type, 0));
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

} // namespace clyth::llvm_stub
