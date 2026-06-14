#ifndef CLYTH_LLVM_STUB_HPP
#define CLYTH_LLVM_STUB_HPP

#include "common.hpp"
#include "clythAST.hpp"
#include "clythSemantic.hpp"
#include "clythLoweringPlan.hpp"

namespace clyth::llvmstub {

// ============================================================
// LLVM Codegen Stub
// ============================================================
//
// This intentionally does NOT emit LLVM IR yet.
//
// It exists so the compiler pipeline has a final stable stage where LLVM work
// can begin without reworking AST/semantic/lowering code.
//
// Future implementation target:
//
//   lowering::ClythLoweringPlan
//        + semantic::SemanticResult
//        -> llvm::Module
//        -> verify
//        -> emit .ll / .o
//        -> link through zig/clang/lld
// ============================================================

struct CodegenConfig {
    std::string module_name = "clyth_module";
    std::filesystem::path output_ir_path;
    bool dump_codegen_plan = false;
};

class ClythLLVMCodegen {
public:
    explicit ClythLLVMCodegen(DiagnosticBag& diagnostics);

    bool emit(
        const lowering::ClythLoweringPlan& plan,
        const semantic::SemanticResult& semantics,
        const CodegenConfig& config = {}
    );

private:
    DiagnosticBag& diagnostics;

    bool emit_program(const lowering::ClythLoweringPlan& plan, const semantic::SemanticResult& semantics);
    bool emit_function_stub(const lowering::LinearNode& node, const semantic::SemanticResult& semantics);
    bool emit_method_stub(const lowering::LinearNode& node, const semantic::SemanticResult& semantics);
    bool emit_struct_stub(const lowering::LinearNode& node, const semantic::SemanticResult& semantics);
    bool emit_statement_stub(const lowering::LinearNode& node, const semantic::SemanticResult& semantics);
    bool emit_expression_stub(const lowering::LinearNode& node, const semantic::SemanticResult& semantics);
    bool emit_mecc_allocation_stub(const lowering::LinearNode& node, const semantic::SemanticResult& semantics);
};

} // namespace clyth::llvmstub

#endif // CLYTH_LLVM_STUB_HPP
