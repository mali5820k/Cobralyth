#ifndef CLYTH_LLVM_STUB_HPP
#define CLYTH_LLVM_STUB_HPP

#include "common.hpp"
#include "clyth_ast.hpp"
#include "clyth_semantic.hpp"
#include "clyth_lowering_plan.hpp"

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>

#include <unordered_map>

namespace clyth::llvm_stub {

// ============================================================
// LLVM Codegen
// ============================================================
//
// This is the first real LLVM backend for Clyth. It intentionally supports a
// conservative V1 subset first:
//   - extern C declarations such as printf
//   - function definitions
//   - blocks, expression statements, returns
//   - integer/bool/string/null literals
//   - local variables and simple assignment
//   - calls, unary operations, and common integer/bool binary operations
//
// Unsupported nodes produce explicit diagnostics instead of silently pretending
// that lowering succeeded.
// ============================================================

struct CodegenConfig {
    std::string module_name = "clyth_module";
    std::filesystem::path output_ir_path;
    std::filesystem::path output_binary_path;
    std::string linker_command = "zig cc";
    std::string link_target = "native-linux-musl";
    bool compile_to_executable = false;
    bool print_ir_to_stdout = false;
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
    struct FunctionScope {
        llvm::Function* function = nullptr;
        llvm::Type* return_type = nullptr;
        std::unordered_map<std::string, llvm::AllocaInst*> locals;
    };

    DiagnosticBag& diagnostics;

    llvm::LLVMContext context;
    std::unique_ptr<llvm::Module> module;
    llvm::IRBuilder<> builder;
    std::unordered_map<std::string, llvm::Function*> functions;
    std::unique_ptr<FunctionScope> current_scope;

    bool emit_program(const lowering::ClythLoweringPlan& plan, const semantic::SemanticResult& semantics);
    bool emit_top_level_node(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    bool declare_extern_function(const ast::NodePtr& node);
    bool declare_function_header(const ast::NodePtr& node);
    bool emit_function_body(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    bool emit_block(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    bool emit_statement(const ast::NodePtr& node, const semantic::SemanticResult& semantics);

    llvm::Value* emit_expression(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_literal(const ast::NodePtr& node);
    llvm::Value* emit_identifier(const ast::NodePtr& node);
    llvm::Value* emit_postfix(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_call_suffix(const std::string& callee_name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_unary(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_binary(const ast::NodePtr& node, const semantic::SemanticResult& semantics);

    llvm::Type* llvm_type_from_clyth_type(const std::string& type_name);
    llvm::AllocaInst* create_entry_alloca(llvm::Function* function, llvm::Type* type, const std::string& name);

    bool write_ir_file(const std::filesystem::path& output_path);
    bool link_executable(const CodegenConfig& config);

    void add_codegen_error(const ast::NodePtr& node, const std::string& message);
    static std::string shell_quote(const std::string& value);
};

} // namespace clyth::llvm_stub

#endif // CLYTH_LLVM_STUB_HPP
