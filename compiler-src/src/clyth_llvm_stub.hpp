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
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>

#include <unordered_map>
#include <optional>
#include <vector>

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
    bool preserve_ir_file = false;
    std::vector<std::filesystem::path> extra_link_inputs;
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
    struct StructInfo {
        std::string name;
        llvm::StructType* type = nullptr;
        std::vector<std::string> field_names;
        std::vector<std::string> field_type_names;
        std::unordered_map<std::string, std::size_t> field_indices;
    };

    struct MethodInfo {
        std::string owner_type_name;
        std::string method_name;
        std::string lowered_name;
        ast::NodePtr declaration;
    };

    struct LocalArrayInfo {
        llvm::AllocaInst* alloca = nullptr;
        llvm::Type* element_type = nullptr;
        std::uint64_t length = 0;
        bool fixed = false;
    };

    struct LocalDynamicArrayInfo {
        llvm::AllocaInst* alloca = nullptr;
        llvm::StructType* array_type = nullptr;
        llvm::Type* element_type = nullptr;
        std::string element_type_name;
        bool has_capacity = false;
    };

    struct LocalStringInfo {
        llvm::AllocaInst* alloca = nullptr;
    };

    struct LocalMapInfo {
        llvm::AllocaInst* alloca = nullptr;
        llvm::StructType* array_type = nullptr;
        llvm::StructType* entry_type = nullptr;
        llvm::Type* key_type = nullptr;
        llvm::Type* value_type = nullptr;
        std::string key_type_name;
        std::string value_type_name;
    };

    struct FunctionScope {
        llvm::Function* function = nullptr;
        llvm::Type* return_type = nullptr;
        std::vector<std::unordered_map<std::string, llvm::AllocaInst*>> local_scopes;
        std::vector<std::unordered_map<std::string, std::string>> local_type_scopes;
        std::vector<std::unordered_map<std::string, LocalArrayInfo>> local_array_scopes;
        std::vector<std::unordered_map<std::string, LocalDynamicArrayInfo>> local_dynamic_array_scopes;
        std::vector<std::unordered_map<std::string, LocalStringInfo>> local_string_scopes;
        std::vector<std::unordered_map<std::string, LocalMapInfo>> local_map_scopes;
        std::string this_type_name;
    };

    struct LoopScope {
        llvm::BasicBlock* continue_block = nullptr;
        llvm::BasicBlock* break_block = nullptr;
    };

    DiagnosticBag& diagnostics;

    llvm::LLVMContext context;
    std::unique_ptr<llvm::Module> module;
    llvm::IRBuilder<> builder;
    std::unordered_map<std::string, llvm::Function*> functions;
    std::unordered_map<std::string, llvm::GlobalVariable*> globals;
    std::unordered_map<std::string, std::string> global_type_names;
    std::unordered_map<std::string, StructInfo> structs;
    std::unordered_map<std::string, MethodInfo> methods;
    std::unordered_map<std::string, llvm::StructType*> dynamic_array_types;
    std::unordered_map<std::string, llvm::StructType*> map_entry_types;
    llvm::StructType* native_string_type = nullptr;
    std::unique_ptr<FunctionScope> current_scope;
    std::vector<LoopScope> loop_stack;

    bool emit_program(const lowering::ClythLoweringPlan& plan, const semantic::SemanticResult& semantics);
    bool emit_top_level_node(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    bool declare_struct_shell(const ast::NodePtr& node);
    bool define_struct_body(const ast::NodePtr& node);
    bool declare_extern_function(const ast::NodePtr& node);
    bool declare_function_header(const ast::NodePtr& node);
    bool declare_method_header(const ast::NodePtr& node, const std::string& owner_type_name = {});
    bool emit_method_body(const ast::NodePtr& node, const semantic::SemanticResult& semantics, const std::string& owner_type_name = {});
    bool declare_global_variable(const ast::NodePtr& node);
    bool is_modern_main_signature(const ast::NodePtr& node) const;
    bool emit_function_body(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    bool emit_block(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    bool emit_statement(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    bool emit_if_statement(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    bool emit_while_statement(const ast::NodePtr& node, const semantic::SemanticResult& semantics);

    llvm::Value* emit_expression(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_literal(const ast::NodePtr& node);
    llvm::Value* emit_identifier(const ast::NodePtr& node);
    llvm::Value* emit_postfix(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_call_suffix(const std::string& callee_name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_method_call_suffix(const std::string& receiver_name, const std::string& method_name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_lvalue_address(const ast::NodePtr& node, const semantic::SemanticResult& semantics, llvm::Type** out_type = nullptr);
    llvm::Value* emit_struct_field_address(const std::string& base_name, const ast::NodePtr& member_node, llvm::Type** out_type = nullptr);
    llvm::Value* emit_this_field_address(const std::string& field_name, const ast::NodePtr& node, llvm::Type** out_type = nullptr);
    bool maybe_emit_default_constructor_call(const std::string& type_name, llvm::Value* receiver_address, const ast::NodePtr& node);
    llvm::Value* emit_fixed_array_element_address(const std::string& name, const ast::NodePtr& index_node, const semantic::SemanticResult& semantics, llvm::Type** out_type = nullptr);
    bool emit_fixed_array_initializer(const std::string& name, const ast::NodePtr& list_node, const semantic::SemanticResult& semantics);
    llvm::StructType* dynamic_array_type_for(llvm::Type* element_type, const std::string& element_type_name, bool has_capacity = false);
    llvm::StructType* map_entry_type_for(llvm::Type* key_type, llvm::Type* value_type, const std::string& key_type_name, const std::string& value_type_name);
    llvm::StructType* string_type_for();
    bool emit_string_literal_initializer(const std::string& name, const ast::NodePtr& literal_node);
    llvm::Value* emit_string_data_pointer(llvm::Value* string_value, const std::string& name_hint = "string.data");
    llvm::Value* emit_string_field_load(const LocalStringInfo& info, std::uint32_t field_index, const std::string& name_hint);
    llvm::Value* emit_string_index_address(const std::string& name, const ast::NodePtr& index_node, const semantic::SemanticResult& semantics, llvm::Type** out_type = nullptr);
    llvm::Function* declare_libc_malloc();
    llvm::Function* declare_libc_realloc();
    bool emit_dynamic_array_initializer(const std::string& name, const ast::NodePtr& list_node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_dynamic_array_element_address(const std::string& name, const ast::NodePtr& index_node, const semantic::SemanticResult& semantics, llvm::Type** out_type = nullptr);
    llvm::Value* emit_dynamic_array_length(const LocalDynamicArrayInfo& info);
    llvm::Value* emit_dynamic_array_capacity(const LocalDynamicArrayInfo& info);
    bool dynamic_array_has_capacity(const LocalDynamicArrayInfo& info) const;
    bool emit_dynamic_array_push(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_dynamic_array_pop(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_dynamic_array_contains(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics);
    bool emit_set_insert(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics);
    bool emit_map_initializer(const std::string& name, const ast::NodePtr& keyed_literal_node, const semantic::SemanticResult& semantics);
    bool emit_map_put(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics);
    bool emit_map_put_values(const std::string& name, const ast::NodePtr& diagnostic_node, const ast::NodePtr& key_node, const ast::NodePtr& value_node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_map_get(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_map_contains_key(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_unary(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_binary(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    llvm::Value* emit_condition_value(const ast::NodePtr& node, const semantic::SemanticResult& semantics);
    llvm::Constant* emit_global_constant_initializer(llvm::Type* type, const ast::NodePtr& node);

    llvm::Type* llvm_type_from_clyth_type(const std::string& type_name);
    llvm::AllocaInst* create_entry_alloca(llvm::Function* function, llvm::Type* type, const std::string& name);

    void push_local_scope();
    void pop_local_scope();
    bool register_local(const std::string& name, llvm::AllocaInst* alloca);
    bool register_local_type(const std::string& name, const std::string& type_name);
    llvm::AllocaInst* lookup_local(const std::string& name) const;
    std::optional<std::string> lookup_symbol_type_name(const std::string& name) const;
    bool register_local_array(const std::string& name, const LocalArrayInfo& info);
    std::optional<LocalArrayInfo> lookup_local_array(const std::string& name) const;
    bool register_local_dynamic_array(const std::string& name, const LocalDynamicArrayInfo& info);
    std::optional<LocalDynamicArrayInfo> lookup_local_dynamic_array(const std::string& name) const;
    bool register_local_string(const std::string& name, const LocalStringInfo& info);
    std::optional<LocalStringInfo> lookup_local_string(const std::string& name) const;
    bool register_local_map(const std::string& name, const LocalMapInfo& info);
    std::optional<LocalMapInfo> lookup_local_map(const std::string& name) const;

    bool write_ir_file(const std::filesystem::path& output_path);
    bool link_executable(const CodegenConfig& config);

    void add_codegen_error(const ast::NodePtr& node, const std::string& message);
    static std::string shell_quote(const std::string& value);
};

} // namespace clyth::llvm_stub

#endif // CLYTH_LLVM_STUB_HPP
