#include "clyth_llvm_stub.hpp"

#include <cerrno>
#include <cctype>
#include <cstdlib>
#include <sstream>

namespace clyth::llvm_stub {
namespace {

bool is_token(const ast::NodePtr& node, const std::string& text) {
    return node && node->kind == ast::NodeKind::Token && node->text == text;
}

std::optional<std::string> attr(const ast::NodePtr& node, const std::string& key) {
    if (!node) {
        return std::nullopt;
    }

    auto it = node->attributes.find(key);
    if (it == node->attributes.end()) {
        return std::nullopt;
    }

    return it->second;
}

std::vector<ast::NodePtr> children_of_kind(const ast::NodePtr& node, ast::NodeKind kind) {
    std::vector<ast::NodePtr> result;

    if (!node) {
        return result;
    }

    for (const auto& child : node->children) {
        if (child && child->kind == kind) {
            result.push_back(child);
        }
    }

    return result;
}

ast::NodePtr first_child_of_kind(const ast::NodePtr& node, ast::NodeKind kind) {
    if (!node) {
        return nullptr;
    }

    for (const auto& child : node->children) {
        if (child && child->kind == kind) {
            return child;
        }
    }

    return nullptr;
}

ast::NodePtr first_child_with_label(const ast::NodePtr& node, const std::string& label) {
    if (!node) {
        return nullptr;
    }

    for (const auto& child : node->children) {
        if (child && child->label == label) {
            return child;
        }
    }

    return nullptr;
}

std::vector<ast::NodePtr> expression_children(const ast::NodePtr& node) {
    std::vector<ast::NodePtr> result;

    if (!node) {
        return result;
    }

    for (const auto& child : node->children) {
        if (!child || child->kind == ast::NodeKind::Token || child->kind == ast::NodeKind::Type) {
            continue;
        }

        result.push_back(child);
    }

    return result;
}

std::optional<std::string> first_token_text(const ast::NodePtr& node) {
    if (!node) {
        return std::nullopt;
    }

    for (const auto& child : node->children) {
        if (!child) {
            continue;
        }

        if (child->kind == ast::NodeKind::Token && !child->text.empty()) {
            return child->text;
        }

        if (auto nested = first_token_text(child)) {
            return nested;
        }
    }

    return std::nullopt;
}

std::optional<std::string> declared_name(const ast::NodePtr& node) {
    return semantic::query::declared_name(node);
}

std::string declared_type_name(const ast::NodePtr& node) {
    const ast::NodePtr type_node = first_child_of_kind(node, ast::NodeKind::Type);

    if (!type_node) {
        return "void";
    }

    if (auto name = attr(type_node, "name")) {
        return *name;
    }

    return type_node->text;
}

std::string unescape_clyth_string_literal(std::string literal) {
    if (literal.size() >= 2 && literal.front() == '"' && literal.back() == '"') {
        literal = literal.substr(1, literal.size() - 2);
    }

    std::string out;
    out.reserve(literal.size());

    for (std::size_t i = 0; i < literal.size(); ++i) {
        const char ch = literal[i];
        if (ch != '\\' || i + 1 >= literal.size()) {
            out.push_back(ch);
            continue;
        }

        const char escaped = literal[++i];
        switch (escaped) {
            case 'n': out.push_back('\n'); break;
            case 'r': out.push_back('\r'); break;
            case 't': out.push_back('\t'); break;
            case '0': out.push_back('\0'); break;
            case '\\': out.push_back('\\'); break;
            case '"': out.push_back('"'); break;
            default:
                out.push_back(escaped);
                break;
        }
    }

    return out;
}

bool is_integer_literal(const std::string& text) {
    if (text.empty()) {
        return false;
    }

    std::size_t i = 0;
    if (text[0] == '-' || text[0] == '+') {
        i = 1;
    }

    if (i >= text.size()) {
        return false;
    }

    for (; i < text.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(text[i]))) {
            return false;
        }
    }

    return true;
}

bool is_wrapper_node(const ast::NodePtr& node) {
    if (!node) {
        return false;
    }

    if (node->kind == ast::NodeKind::Generic) {
        return true;
    }

    switch (node->kind) {
        case ast::NodeKind::BinaryExpr:
        case ast::NodeKind::UnaryExpr:
        case ast::NodeKind::PostfixExpr:
            return node->children.size() == 1;
        default:
            return false;
    }
}


bool is_expression_like(const ast::NodePtr& node) {
    if (!node) {
        return false;
    }

    return semantic::query::is_expression_node(node) ||
           node->kind == ast::NodeKind::Generic ||
           node->kind == ast::NodeKind::LiteralExpr ||
           node->kind == ast::NodeKind::IdentifierExpr;
}

ast::NodePtr first_expression_like_child(const ast::NodePtr& node) {
    if (!node) {
        return nullptr;
    }

    for (const auto& child : node->children) {
        if (is_expression_like(child)) {
            return child;
        }
    }

    return nullptr;
}

std::vector<ast::NodePtr> direct_children_of_kind(const ast::NodePtr& node, ast::NodeKind kind) {
    std::vector<ast::NodePtr> result;
    if (!node) {
        return result;
    }

    for (const auto& child : node->children) {
        if (child && child->kind == kind) {
            result.push_back(child);
        }
    }

    return result;
}


std::vector<ast::NodePtr> direct_params(const ast::NodePtr& node) {
    std::vector<ast::NodePtr> params;
    if (!node) {
        return params;
    }
    for (const auto& child : node->children) {
        if (!child || (child->label != "paramList" && child->label != "externParamList")) {
            continue;
        }
        for (const auto& param : child->children) {
            if (param && param->kind == ast::NodeKind::Param) {
                params.push_back(param);
            }
        }
    }
    return params;
}

bool is_dynamic_array_type_name(const std::string& type_name) {
    return type_name.size() >= 2 && type_name.compare(type_name.size() - 2, 2, "[]") == 0;
}

struct FixedArrayTypeInfo {
    std::string element_type_name;
    std::uint64_t length = 0;
};

std::optional<FixedArrayTypeInfo> parse_fixed_array_type_name(const std::string& type_name) {
    const std::size_t left = type_name.rfind('[');
    if (left == std::string::npos || type_name.empty() || type_name.back() != ']') {
        return std::nullopt;
    }

    const std::string size_text = type_name.substr(left + 1, type_name.size() - left - 2);
    if (size_text.empty()) {
        return std::nullopt;
    }

    for (char ch : size_text) {
        if (!std::isdigit(static_cast<unsigned char>(ch))) {
            return std::nullopt;
        }
    }

    FixedArrayTypeInfo info;
    info.element_type_name = type_name.substr(0, left);
    info.length = static_cast<std::uint64_t>(std::stoull(size_text));
    return info;
}


ast::NodePtr first_descendant_of_kind(const ast::NodePtr& node, ast::NodeKind kind) {
    if (!node) {
        return nullptr;
    }

    if (node->kind == kind) {
        return node;
    }

    for (const auto& child : node->children) {
        if (auto found = first_descendant_of_kind(child, kind)) {
            return found;
        }
    }

    return nullptr;
}

std::vector<ast::NodePtr> list_literal_elements(const ast::NodePtr& list_node) {
    std::vector<ast::NodePtr> result;
    if (!list_node) {
        return result;
    }

    for (const auto& child : list_node->children) {
        if (!child) {
            continue;
        }

        if (child->label == "expressionList") {
            for (const auto& expr : child->children) {
                if (is_expression_like(expr)) {
                    result.push_back(expr);
                }
            }
        } else if (is_expression_like(child)) {
            result.push_back(child);
        }
    }

    return result;
}

bool is_pointer_generic_type_name(const std::string& type_name) {
    return type_name.rfind("pointer<", 0) == 0 && !type_name.empty() && type_name.back() == '>';
}

ast::NodePtr else_body_child(const ast::NodePtr& else_node) {
    if (!else_node) {
        return nullptr;
    }

    for (const auto& child : else_node->children) {
        if (child && (child->kind == ast::NodeKind::IfStmt || child->kind == ast::NodeKind::BlockStmt)) {
            return child;
        }
    }

    return nullptr;
}

std::vector<ast::NodePtr> direct_top_level_nodes(const lowering::ClythLoweringPlan& plan) {
    std::vector<ast::NodePtr> result;

    for (const lowering::LinearNode& node : plan.nodes) {
        if (node.depth == 1 && node.ast_node) {
            result.push_back(node.ast_node);
        }
    }

    return result;
}

} // namespace

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
        fmt::print("LLVM codegen for module '{}'\n", config.module_name);
    }

    const bool ok = emit_program(plan, semantics);
    if (!ok || diagnostics.has_errors()) {
        return false;
    }

    if (llvm::verifyModule(*module, &llvm::errs())) {
        diagnostics.add_error(SourceLocation{}, "LLVM module verification failed.");
        return false;
    }

    if (config.print_ir_to_stdout || config.output_ir_path.empty()) {
        module->print(llvm::outs(), nullptr);
    }

    if (!config.output_ir_path.empty() && !write_ir_file(config.output_ir_path)) {
        return false;
    }

    if (config.compile_to_executable && !link_executable(config)) {
        return false;
    }

    return !diagnostics.has_errors();
}

bool ClythLLVMCodegen::emit_program(
    const lowering::ClythLoweringPlan& plan,
    const semantic::SemanticResult& semantics
) {
    const std::vector<ast::NodePtr> top_level = direct_top_level_nodes(plan);

    // First pass: create all externally callable/function symbols so calls can
    // resolve independent of source order.
    for (const auto& node : top_level) {
        if (!node) {
            continue;
        }

        if (node->kind == ast::NodeKind::ExternDecl) {
            if (!declare_extern_function(node)) {
                return false;
            }
        } else if (node->kind == ast::NodeKind::FunctionDecl) {
            if (!declare_function_header(node)) {
                return false;
            }
        } else if (node->kind == ast::NodeKind::VarDeclStmt) {
            if (!declare_global_variable(node)) {
                return false;
            }
        }
    }

    // Second pass: emit bodies for Clyth-defined functions.
    for (const auto& node : top_level) {
        if (!emit_top_level_node(node, semantics)) {
            return false;
        }
    }

    return true;
}

bool ClythLLVMCodegen::emit_top_level_node(const ast::NodePtr& node, const semantic::SemanticResult& semantics) {
    if (!node) {
        return true;
    }

    switch (node->kind) {
        case ast::NodeKind::ExternDecl:
            return true;

        case ast::NodeKind::FunctionDecl:
            return emit_function_body(node, semantics);

        case ast::NodeKind::StructDecl:
        case ast::NodeKind::MethodDecl:
        case ast::NodeKind::MethodBlock:
            // Parsed and semantically checked; full layout/method lowering comes later.
            return true;

        case ast::NodeKind::IncludeDecl:
            return true;

        case ast::NodeKind::VarDeclStmt:
            return true;

        default:
            add_codegen_error(node, fmt::format("Unsupported top-level node '{}'.", ast::node_kind_name(node->kind)));
            return false;
    }
}

bool ClythLLVMCodegen::declare_extern_function(const ast::NodePtr& node) {
    const auto maybe_name = declared_name(node);
    if (!maybe_name) {
        add_codegen_error(node, "Extern declaration is missing a function name.");
        return false;
    }

    const std::string function_name = *maybe_name;
    if (functions.find(function_name) != functions.end()) {
        return true;
    }

    const std::string return_type_name = declared_type_name(node);
    llvm::Type* return_type = llvm_type_from_clyth_type(return_type_name);
    if (return_type == nullptr) {
        add_codegen_error(node, fmt::format("Unsupported extern return type '{}'.", return_type_name));
        return false;
    }

    std::vector<llvm::Type*> parameter_types;
    for (const auto& child : node->children) {
        if (!child || child->label != "externParamList") {
            continue;
        }

        for (const auto& param : child->children) {
            if (!param || param->kind != ast::NodeKind::Param) {
                continue;
            }

            const std::string param_type_name = declared_type_name(param);
            llvm::Type* param_type = llvm_type_from_clyth_type(param_type_name);
            if (param_type == nullptr) {
                add_codegen_error(param, fmt::format("Unsupported extern parameter type '{}'.", param_type_name));
                return false;
            }
            parameter_types.push_back(param_type);
        }
    }

    const bool is_varargs = node->text.find("...") != std::string::npos;
    llvm::FunctionType* function_type = llvm::FunctionType::get(return_type, parameter_types, is_varargs);
    llvm::Function* function = llvm::Function::Create(
        function_type,
        llvm::Function::ExternalLinkage,
        function_name,
        module.get()
    );

    functions[function_name] = function;
    return true;
}

bool ClythLLVMCodegen::declare_function_header(const ast::NodePtr& node) {
    const auto maybe_name = declared_name(node);
    if (!maybe_name) {
        add_codegen_error(node, "Function declaration is missing a name.");
        return false;
    }

    const std::string function_name = *maybe_name;
    if (functions.find(function_name) != functions.end()) {
        return true;
    }

    const std::string return_type_name = declared_type_name(node);
    llvm::Type* return_type = llvm_type_from_clyth_type(return_type_name);
    if (return_type == nullptr) {
        add_codegen_error(node, fmt::format("Unsupported function return type '{}'.", return_type_name));
        return false;
    }

    std::vector<llvm::Type*> parameter_types;
    const bool modern_main = is_modern_main_signature(node);

    if (modern_main) {
        // Clyth exposes main(string[] args), but native process startup still
        // uses the C ABI: int main(int argc, char** argv). The hidden argc is
        // retained for future args.length support; the visible args parameter
        // is bound to argv for this pass.
        parameter_types.push_back(llvm::Type::getInt32Ty(context));
        parameter_types.push_back(llvm::PointerType::get(context, 0));
    } else {
        for (const auto& param : direct_params(node)) {
            const std::string param_type_name = declared_type_name(param);
            llvm::Type* param_type = llvm_type_from_clyth_type(param_type_name);
            if (param_type == nullptr) {
                add_codegen_error(param, fmt::format("Unsupported parameter type '{}'.", param_type_name));
                return false;
            }
            parameter_types.push_back(param_type);
        }
    }

    llvm::FunctionType* function_type = llvm::FunctionType::get(return_type, parameter_types, false);
    llvm::Function* function = llvm::Function::Create(
        function_type,
        llvm::Function::ExternalLinkage,
        function_name,
        module.get()
    );

    std::size_t arg_index = 0;
    for (llvm::Argument& arg : function->args()) {
        if (modern_main && arg_index == 0) {
            arg.setName("__clyth_argc");
        } else if (modern_main && arg_index == 1) {
            arg.setName("args");
        } else {
            arg.setName(fmt::format("arg{}", arg_index));
        }
        ++arg_index;
    }

    functions[function_name] = function;
    return true;
}

bool ClythLLVMCodegen::declare_global_variable(const ast::NodePtr& node) {
    const auto maybe_name = declared_name(node);
    if (!maybe_name) {
        add_codegen_error(node, "Global variable declaration is missing a name.");
        return false;
    }

    const std::string name = *maybe_name;
    if (globals.find(name) != globals.end()) {
        return true;
    }

    const std::string type_name = declared_type_name(node);
    llvm::Type* type = llvm_type_from_clyth_type(type_name);
    if (type == nullptr) {
        add_codegen_error(node, fmt::format("Unsupported global variable type '{}'.", type_name));
        return false;
    }

    llvm::Constant* initializer = emit_global_constant_initializer(type, node);
    if (initializer == nullptr) {
        if (type->isIntegerTy()) {
            initializer = llvm::ConstantInt::get(type, 0);
        } else if (type->isFloatingPointTy()) {
            initializer = llvm::ConstantFP::get(type, 0.0);
        } else if (type->isPointerTy()) {
            initializer = llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(type));
        } else {
            add_codegen_error(node, fmt::format("Global variable '{}' has unsupported default initializer type.", name));
            return false;
        }
    }

    auto* global = new llvm::GlobalVariable(
        *module,
        type,
        false,
        llvm::GlobalValue::ExternalLinkage,
        initializer,
        name
    );

    globals[name] = global;
    return true;
}

bool ClythLLVMCodegen::is_modern_main_signature(const ast::NodePtr& node) const {
    const auto maybe_name = declared_name(node);
    if (!maybe_name || *maybe_name != "main") {
        return false;
    }

    const std::vector<ast::NodePtr> params = direct_params(node);
    if (params.size() != 1) {
        return false;
    }

    return declared_type_name(params.front()) == "string[]";
}

bool ClythLLVMCodegen::emit_function_body(const ast::NodePtr& node, const semantic::SemanticResult& semantics) {
    const auto maybe_name = declared_name(node);
    if (!maybe_name) {
        add_codegen_error(node, "Function body is missing a name.");
        return false;
    }

    auto it = functions.find(*maybe_name);
    if (it == functions.end()) {
        add_codegen_error(node, fmt::format("Function '{}' was not declared before body emission.", *maybe_name));
        return false;
    }

    llvm::Function* function = it->second;
    if (!function->empty()) {
        return true;
    }

    llvm::BasicBlock* entry_block = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(entry_block);

    current_scope = std::make_unique<FunctionScope>();
    current_scope->function = function;
    current_scope->return_type = function->getReturnType();
    push_local_scope();

    // Materialize arguments into allocas so later assignment/load logic can be uniform.
    const std::vector<ast::NodePtr> params = direct_params(node);
    const bool modern_main = is_modern_main_signature(node);

    std::size_t index = 0;
    for (llvm::Argument& arg : function->args()) {
        if (modern_main && index == 0) {
            llvm::AllocaInst* argc_alloca = create_entry_alloca(function, arg.getType(), "__clyth_argc");
            builder.CreateStore(&arg, argc_alloca);
            register_local("__clyth_argc", argc_alloca);
            ++index;
            continue;
        }

        std::string name = fmt::format("arg{}", index);
        if (modern_main && index == 1 && !params.empty()) {
            if (auto param_name = declared_name(params.front())) {
                name = *param_name;
            } else {
                name = "args";
            }
        } else if (!modern_main && index < params.size()) {
            if (auto param_name = declared_name(params[index])) {
                name = *param_name;
            }
        }
        arg.setName(name);

        llvm::AllocaInst* alloca = create_entry_alloca(function, arg.getType(), name);
        builder.CreateStore(&arg, alloca);
        register_local(name, alloca);
        ++index;
    }

    const ast::NodePtr body = first_child_of_kind(node, ast::NodeKind::BlockStmt);
    if (!body) {
        add_codegen_error(node, fmt::format("Function '{}' is missing a body block.", *maybe_name));
        current_scope.reset();
        return false;
    }

    const bool ok = emit_block(body, semantics);

    if (ok && !builder.GetInsertBlock()->getTerminator()) {
        if (current_scope->return_type->isVoidTy()) {
            builder.CreateRetVoid();
        } else if (current_scope->return_type->isIntegerTy()) {
            builder.CreateRet(llvm::ConstantInt::get(current_scope->return_type, 0));
        } else {
            add_codegen_error(node, fmt::format("Function '{}' has no return and unsupported default return type.", *maybe_name));
            current_scope.reset();
            return false;
        }
    }

    current_scope.reset();
    return ok;
}

bool ClythLLVMCodegen::emit_block(const ast::NodePtr& node, const semantic::SemanticResult& semantics) {
    if (!node) {
        return true;
    }

    push_local_scope();
    bool ok = true;

    for (const auto& child : node->children) {
        if (!emit_statement(child, semantics)) {
            ok = false;
            break;
        }

        if (builder.GetInsertBlock() && builder.GetInsertBlock()->getTerminator()) {
            break;
        }
    }

    pop_local_scope();
    return ok;
}

bool ClythLLVMCodegen::emit_statement(const ast::NodePtr& node, const semantic::SemanticResult& semantics) {
    if (!node) {
        return true;
    }

    switch (node->kind) {
        case ast::NodeKind::BlockStmt:
            return emit_block(node, semantics);

        case ast::NodeKind::ExprStmt: {
            const auto exprs = expression_children(node);
            if (!exprs.empty()) {
                emit_expression(exprs.front(), semantics);
                return !diagnostics.has_errors();
            }
            return true;
        }

        case ast::NodeKind::ReturnStmt: {
            const auto exprs = expression_children(node);
            if (exprs.empty()) {
                builder.CreateRetVoid();
                return true;
            }

            llvm::Value* value = emit_expression(exprs.front(), semantics);
            if (value == nullptr) {
                return false;
            }

            if (current_scope && current_scope->return_type && value->getType() != current_scope->return_type) {
                if (value->getType()->isIntegerTy() && current_scope->return_type->isIntegerTy()) {
                    value = builder.CreateIntCast(value, current_scope->return_type, true, "retcast");
                }
            }

            builder.CreateRet(value);
            return true;
        }

        case ast::NodeKind::VarDeclStmt: {
            if (!current_scope) {
                add_codegen_error(node, "Variable declaration outside of function scope is not supported yet.");
                return false;
            }

            const auto maybe_name = declared_name(node);
            if (!maybe_name) {
                add_codegen_error(node, "Variable declaration is missing a name.");
                return false;
            }

            const std::string type_name = declared_type_name(node);
            llvm::Type* type = llvm_type_from_clyth_type(type_name);
            if (type == nullptr) {
                add_codegen_error(node, fmt::format("Unsupported variable type '{}'.", type_name));
                return false;
            }

            llvm::AllocaInst* alloca = create_entry_alloca(current_scope->function, type, *maybe_name);
            if (!register_local(*maybe_name, alloca)) { return false; }

            if (auto fixed_array = parse_fixed_array_type_name(type_name)) {
                llvm::Type* element_type = llvm_type_from_clyth_type(fixed_array->element_type_name);
                if (element_type == nullptr) {
                    add_codegen_error(node, fmt::format("Unsupported fixed array element type '{}'.", fixed_array->element_type_name));
                    return false;
                }
                if (!register_local_array(*maybe_name, LocalArrayInfo{alloca, element_type, fixed_array->length, true})) {
                    return false;
                }

                const auto exprs = expression_children(node);
                if (!exprs.empty()) {
                    ast::NodePtr list_initializer = first_descendant_of_kind(exprs.front(), ast::NodeKind::ListLiteralExpr);
                    if (!list_initializer) {
                        add_codegen_error(exprs.front(), "Fixed array initializer must be a list literal in this backend pass.");
                        return false;
                    }
                    return emit_fixed_array_initializer(*maybe_name, list_initializer, semantics);
                }

                builder.CreateStore(llvm::ConstantAggregateZero::get(type), alloca);
                return true;
            }

            const auto exprs = expression_children(node);
            if (!exprs.empty()) {
                llvm::Value* initializer = emit_expression(exprs.front(), semantics);
                if (initializer == nullptr) {
                    return false;
                }
                if (initializer->getType() != type && initializer->getType()->isIntegerTy() && type->isIntegerTy()) {
                    initializer = builder.CreateIntCast(initializer, type, true, "initcast");
                }
                builder.CreateStore(initializer, alloca);
            } else if (type->isIntegerTy()) {
                builder.CreateStore(llvm::ConstantInt::get(type, 0), alloca);
            } else if (type->isFloatingPointTy()) {
                builder.CreateStore(llvm::ConstantFP::get(type, 0.0), alloca);
            } else if (type->isPointerTy()) {
                builder.CreateStore(llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(type)), alloca);
            }

            return true;
        }

        case ast::NodeKind::AssignmentStmt: {
            if (!current_scope) {
                add_codegen_error(node, "Assignment outside of function scope is not supported yet.");
                return false;
            }

            const ast::NodePtr assignable = first_child_with_label(node, "assignable");
            const auto maybe_name = first_token_text(assignable);
            if (!maybe_name) {
                add_codegen_error(node, "Assignment target could not be resolved.");
                return false;
            }

            llvm::Type* target_type = nullptr;
            llvm::Value* target_address = emit_lvalue_address(assignable, semantics, &target_type);

            if (target_address == nullptr || target_type == nullptr) {
                return false;
            }

            const auto exprs = expression_children(node);
            if (exprs.empty()) {
                add_codegen_error(node, "Assignment is missing a right-hand expression.");
                return false;
            }

            llvm::Value* value = emit_expression(exprs.back(), semantics);
            if (value == nullptr) {
                return false;
            }

            std::string assignment_operator = "=";
            if (const ast::NodePtr op_node = first_child_with_label(node, "assignmentOp")) {
                if (auto op_text = attr(op_node, "operator")) {
                    assignment_operator = *op_text;
                }
            }

            if (assignment_operator != "=") {
                llvm::Value* existing = builder.CreateLoad(target_type, target_address, *maybe_name);
                if (existing->getType()->isIntegerTy() && value->getType()->isIntegerTy() && existing->getType() != value->getType()) {
                    value = builder.CreateIntCast(value, existing->getType(), true, "assigncast");
                }

                if (assignment_operator == "+=") {
                    value = builder.CreateAdd(existing, value, "addassigntmp");
                } else if (assignment_operator == "-=") {
                    value = builder.CreateSub(existing, value, "subassigntmp");
                } else if (assignment_operator == "*=") {
                    value = builder.CreateMul(existing, value, "mulassigntmp");
                } else if (assignment_operator == "/=") {
                    value = builder.CreateSDiv(existing, value, "divassigntmp");
                } else if (assignment_operator == "%=") {
                    value = builder.CreateSRem(existing, value, "modassigntmp");
                } else {
                    add_codegen_error(node, fmt::format("Unsupported assignment operator '{}'.", assignment_operator));
                    return false;
                }
            }

            if (value->getType() != target_type && value->getType()->isIntegerTy() && target_type->isIntegerTy()) {
                value = builder.CreateIntCast(value, target_type, true, "assigncast");
            }
            builder.CreateStore(value, target_address);
            return true;
        }

        case ast::NodeKind::IfStmt:
            return emit_if_statement(node, semantics);

        case ast::NodeKind::WhileStmt:
            return emit_while_statement(node, semantics);

        case ast::NodeKind::BreakStmt:
            if (loop_stack.empty()) {
                add_codegen_error(node, "'break' used outside of a loop.");
                return false;
            }
            builder.CreateBr(loop_stack.back().break_block);
            return true;

        case ast::NodeKind::ContinueStmt:
            if (loop_stack.empty()) {
                add_codegen_error(node, "'continue' used outside of a loop.");
                return false;
            }
            builder.CreateBr(loop_stack.back().continue_block);
            return true;

        case ast::NodeKind::MeccBlockStmt:
            // Until MECC estate lowering exists, a mecc block behaves like a
            // normal lexical block for codegen purposes. Semantic analysis
            // still records that MECC was requested.
            return emit_block(first_child_of_kind(node, ast::NodeKind::BlockStmt), semantics);

        case ast::NodeKind::ForStmt:
            add_codegen_error(node, "'for' loop codegen is not implemented yet; use while loops in this backend pass.");
            return false;

        default:
            if (semantic::query::is_expression_node(node)) {
                emit_expression(node, semantics);
                return !diagnostics.has_errors();
            }
            return true;
    }
}


bool ClythLLVMCodegen::emit_if_statement(const ast::NodePtr& node, const semantic::SemanticResult& semantics) {
    llvm::Value* condition = emit_condition_value(first_expression_like_child(node), semantics);
    if (condition == nullptr) {
        return false;
    }

    llvm::Function* function = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* then_block = llvm::BasicBlock::Create(context, "if.then", function);
    const ast::NodePtr else_clause = first_child_of_kind(node, ast::NodeKind::ElseClause);
    llvm::BasicBlock* else_block = else_clause
        ? llvm::BasicBlock::Create(context, "if.else", function)
        : nullptr;
    llvm::BasicBlock* merge_block = llvm::BasicBlock::Create(context, "if.end", function);

    builder.CreateCondBr(condition, then_block, else_clause ? else_block : merge_block);

    builder.SetInsertPoint(then_block);
    const auto blocks = direct_children_of_kind(node, ast::NodeKind::BlockStmt);
    if (blocks.empty()) {
        add_codegen_error(node, "if statement is missing a then-block.");
        return false;
    }
    if (!emit_block(blocks.front(), semantics)) {
        return false;
    }
    if (!builder.GetInsertBlock()->getTerminator()) {
        builder.CreateBr(merge_block);
    }

    if (else_clause) {
        builder.SetInsertPoint(else_block);
        const ast::NodePtr else_body = else_body_child(else_clause);
        if (!else_body) {
            add_codegen_error(else_clause, "else clause is missing a block or nested if statement.");
            return false;
        }
        if (else_body->kind == ast::NodeKind::IfStmt) {
            if (!emit_if_statement(else_body, semantics)) {
                return false;
            }
        } else if (!emit_block(else_body, semantics)) {
            return false;
        }
        if (!builder.GetInsertBlock()->getTerminator()) {
            builder.CreateBr(merge_block);
        }
    }

    builder.SetInsertPoint(merge_block);
    return true;
}

bool ClythLLVMCodegen::emit_while_statement(const ast::NodePtr& node, const semantic::SemanticResult& semantics) {
    llvm::Function* function = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* condition_block = llvm::BasicBlock::Create(context, "while.cond", function);
    llvm::BasicBlock* body_block = llvm::BasicBlock::Create(context, "while.body", function);
    llvm::BasicBlock* end_block = llvm::BasicBlock::Create(context, "while.end", function);

    builder.CreateBr(condition_block);
    builder.SetInsertPoint(condition_block);

    llvm::Value* condition = emit_condition_value(first_expression_like_child(node), semantics);
    if (condition == nullptr) {
        return false;
    }
    builder.CreateCondBr(condition, body_block, end_block);

    builder.SetInsertPoint(body_block);
    loop_stack.push_back(LoopScope{condition_block, end_block});

    const ast::NodePtr body = first_child_of_kind(node, ast::NodeKind::BlockStmt);
    if (!body) {
        add_codegen_error(node, "while statement is missing a body block.");
        loop_stack.pop_back();
        return false;
    }
    const bool ok = emit_block(body, semantics);
    loop_stack.pop_back();
    if (!ok) {
        return false;
    }

    if (!builder.GetInsertBlock()->getTerminator()) {
        builder.CreateBr(condition_block);
    }

    builder.SetInsertPoint(end_block);
    return true;
}

llvm::Value* ClythLLVMCodegen::emit_expression(const ast::NodePtr& node, const semantic::SemanticResult& semantics) {
    if (!node) {
        return nullptr;
    }

    if (is_wrapper_node(node)) {
        const auto exprs = expression_children(node);
        if (exprs.size() == 1) {
            return emit_expression(exprs.front(), semantics);
        }
    }

    switch (node->kind) {
        case ast::NodeKind::LiteralExpr:
            return emit_literal(node);

        case ast::NodeKind::IdentifierExpr:
            return emit_identifier(node);

        case ast::NodeKind::PostfixExpr:
            return emit_postfix(node, semantics);

        case ast::NodeKind::UnaryExpr:
            return emit_unary(node, semantics);

        case ast::NodeKind::BinaryExpr:
            return emit_binary(node, semantics);

        case ast::NodeKind::Generic: {
            if (auto token = first_token_text(node)) {
                if (current_scope && lookup_local(*token) != nullptr) {
                    return emit_identifier(node);
                }
            }

            const auto exprs = expression_children(node);
            if (!exprs.empty()) {
                return emit_expression(exprs.front(), semantics);
            }

            add_codegen_error(node, "Unable to lower generic expression node.");
            return nullptr;
        }

        case ast::NodeKind::ListLiteralExpr:
        case ast::NodeKind::CurlyLiteralExpr:
        case ast::NodeKind::AllocationExpr:
        case ast::NodeKind::MemberAccessExpr:
        case ast::NodeKind::IndexExpr:
            add_codegen_error(node, fmt::format("Expression codegen for '{}' is not implemented yet.", ast::node_kind_name(node->kind)));
            return nullptr;

        default:
            add_codegen_error(node, fmt::format("Unsupported expression node '{}'.", ast::node_kind_name(node->kind)));
            return nullptr;
    }
}

llvm::Value* ClythLLVMCodegen::emit_literal(const ast::NodePtr& node) {
    const std::string literal = attr(node, "literal").value_or(node ? node->text : "");

    if (literal == "true") {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), 1);
    }

    if (literal == "false") {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), 0);
    }

    if (literal == "null") {
        return llvm::ConstantPointerNull::get(llvm::PointerType::get(context, 0));
    }

    if (!literal.empty() && literal.front() == '"') {
        return builder.CreateGlobalStringPtr(unescape_clyth_string_literal(literal));
    }

    if (is_integer_literal(literal)) {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), std::stoll(literal), true);
    }

    if (literal.find('.') != std::string::npos) {
        try {
            return llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), std::stod(literal));
        } catch (const std::exception&) {
            // Fall through to diagnostic below.
        }
    }

    add_codegen_error(node, fmt::format("Unsupported literal '{}'.", literal));
    return nullptr;
}

llvm::Value* ClythLLVMCodegen::emit_identifier(const ast::NodePtr& node) {
    const auto maybe_name = first_token_text(node);
    if (!maybe_name) {
        add_codegen_error(node, "Identifier expression has no identifier token.");
        return nullptr;
    }

    if (!current_scope) {
        add_codegen_error(node, fmt::format("Identifier '{}' used outside function scope.", *maybe_name));
        return nullptr;
    }

    if (llvm::AllocaInst* local = lookup_local(*maybe_name)) {
        return builder.CreateLoad(local->getAllocatedType(), local, *maybe_name);
    }

    auto global_it = globals.find(*maybe_name);
    if (global_it != globals.end()) {
        llvm::GlobalVariable* global = global_it->second;
        return builder.CreateLoad(global->getValueType(), global, *maybe_name);
    }

    add_codegen_error(node, fmt::format("Unknown identifier '{}'.", *maybe_name));
    return nullptr;
}

llvm::Value* ClythLLVMCodegen::emit_postfix(const ast::NodePtr& node, const semantic::SemanticResult& semantics) {
    const auto maybe_base_name = first_token_text(node);
    if (!maybe_base_name) {
        add_codegen_error(node, "Postfix expression is missing a base identifier.");
        return nullptr;
    }

    llvm::Value* current_value = nullptr;
    bool consumed_suffix = false;

    for (const auto& child : node->children) {
        if (!child) {
            continue;
        }

        if (child->kind == ast::NodeKind::CallExpr) {
            current_value = emit_call_suffix(*maybe_base_name, child, semantics);
            consumed_suffix = true;
            continue;
        }

        if (child->kind == ast::NodeKind::IndexExpr) {
            llvm::Type* element_type = nullptr;
            llvm::Value* element_address = emit_fixed_array_element_address(*maybe_base_name, child, semantics, &element_type);
            if (element_address != nullptr && element_type != nullptr) {
                current_value = builder.CreateLoad(element_type, element_address, "arrayindexload");
                consumed_suffix = true;
                continue;
            }

            if (!current_scope) {
                add_codegen_error(child, "Indexing requires a function scope.");
                return nullptr;
            }
            llvm::AllocaInst* local = lookup_local(*maybe_base_name);
            if (local == nullptr) {
                add_codegen_error(child, fmt::format("Unknown indexed value '{}'.", *maybe_base_name));
                return nullptr;
            }

            llvm::Value* base_pointer = builder.CreateLoad(local->getAllocatedType(), local, *maybe_base_name);
            llvm::Value* index_value = emit_expression(first_expression_like_child(child), semantics);
            if (index_value == nullptr) {
                return nullptr;
            }
            if (!index_value->getType()->isIntegerTy(32)) {
                index_value = builder.CreateIntCast(index_value, llvm::Type::getInt32Ty(context), true, "idxcast");
            }

            // Pass 2 models string[] args as native argv for startup interop: an
            // opaque pointer to an array of opaque pointers. args[i] therefore
            // loads one C-string pointer. Native Clyth string arrays will later
            // lower through the runtime string/array representation.
            element_type = llvm::PointerType::get(context, 0);
            llvm::Value* slot = builder.CreateGEP(element_type, base_pointer, index_value, "indexslot");
            current_value = builder.CreateLoad(element_type, slot, "indexload");
            consumed_suffix = true;
            continue;
        }

        if (child->kind == ast::NodeKind::MemberAccessExpr) {
            const std::string member_text = child->text;
            if (member_text == ".length" || member_text == "length" || first_token_text(child).value_or("") == "length") {
                if (auto info = lookup_local_array(*maybe_base_name)) {
                    current_value = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), info->length);
                    consumed_suffix = true;
                    continue;
                }
            }

            add_codegen_error(child, fmt::format("Postfix suffix '{}' is not implemented yet.", ast::node_kind_name(child->kind)));
            return nullptr;
        }
    }

    if (consumed_suffix) {
        return current_value;
    }

    return emit_identifier(node);
}


llvm::Value* ClythLLVMCodegen::emit_fixed_array_element_address(
    const std::string& name,
    const ast::NodePtr& index_node,
    const semantic::SemanticResult& semantics,
    llvm::Type** out_type
) {
    auto info = lookup_local_array(name);
    if (!info || info->alloca == nullptr || info->element_type == nullptr) {
        return nullptr;
    }

    llvm::Value* index_value = emit_expression(first_expression_like_child(index_node), semantics);
    if (index_value == nullptr) {
        return nullptr;
    }

    if (!index_value->getType()->isIntegerTy(64)) {
        index_value = builder.CreateIntCast(index_value, llvm::Type::getInt64Ty(context), true, "arrayidxcast");
    }

    llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), 0);
    llvm::Value* element_address = builder.CreateInBoundsGEP(
        info->alloca->getAllocatedType(),
        info->alloca,
        {zero, index_value},
        fmt::format("{}.elem", name)
    );

    if (out_type != nullptr) {
        *out_type = info->element_type;
    }

    return element_address;
}

llvm::Value* ClythLLVMCodegen::emit_lvalue_address(
    const ast::NodePtr& node,
    const semantic::SemanticResult& semantics,
    llvm::Type** out_type
) {
    const auto maybe_name = first_token_text(node);
    if (!maybe_name) {
        add_codegen_error(node, "Assignment target could not be resolved.");
        return nullptr;
    }

    for (const auto& child : node->children) {
        if (child && child->kind == ast::NodeKind::IndexExpr) {
            return emit_fixed_array_element_address(*maybe_name, child, semantics, out_type);
        }
    }

    if (llvm::AllocaInst* local = lookup_local(*maybe_name)) {
        if (out_type != nullptr) {
            *out_type = local->getAllocatedType();
        }
        return local;
    }

    auto global_it = globals.find(*maybe_name);
    if (global_it != globals.end()) {
        if (out_type != nullptr) {
            *out_type = global_it->second->getValueType();
        }
        return global_it->second;
    }

    add_codegen_error(node, fmt::format("Unknown assignment target '{}'.", *maybe_name));
    return nullptr;
}

bool ClythLLVMCodegen::emit_fixed_array_initializer(
    const std::string& name,
    const ast::NodePtr& list_node,
    const semantic::SemanticResult& semantics
) {
    auto info = lookup_local_array(name);
    if (!info || info->alloca == nullptr || info->element_type == nullptr) {
        add_codegen_error(list_node, fmt::format("Internal codegen error: '{}' is not a fixed array.", name));
        return false;
    }

    llvm::Constant* zero_initializer = llvm::ConstantAggregateZero::get(info->alloca->getAllocatedType());
    builder.CreateStore(zero_initializer, info->alloca);

    const std::vector<ast::NodePtr> elements = list_literal_elements(list_node);
    if (elements.size() > info->length) {
        add_codegen_error(list_node, fmt::format("Array initializer for '{}' has {} elements but array length is {}.", name, elements.size(), info->length));
        return false;
    }

    for (std::size_t index = 0; index < elements.size(); ++index) {
        llvm::Value* index_value = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), index);
        llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), 0);
        llvm::Value* slot = builder.CreateInBoundsGEP(
            info->alloca->getAllocatedType(),
            info->alloca,
            {zero, index_value},
            fmt::format("{}.init{}", name, index)
        );

        llvm::Value* value = emit_expression(elements[index], semantics);
        if (value == nullptr) {
            return false;
        }
        if (value->getType() != info->element_type && value->getType()->isIntegerTy() && info->element_type->isIntegerTy()) {
            value = builder.CreateIntCast(value, info->element_type, true, "arrayinitcast");
        }
        builder.CreateStore(value, slot);
    }

    return true;
}

llvm::Value* ClythLLVMCodegen::emit_call_suffix(
    const std::string& callee_name,
    const ast::NodePtr& call_node,
    const semantic::SemanticResult& semantics
) {
    auto function_it = functions.find(callee_name);
    if (function_it == functions.end()) {
        add_codegen_error(call_node, fmt::format("Unknown function '{}'.", callee_name));
        return nullptr;
    }

    std::vector<llvm::Value*> args;
    const ast::NodePtr arg_list = first_child_with_label(call_node, "argumentList");
    if (arg_list) {
        for (const auto& arg : expression_children(arg_list)) {
            llvm::Value* arg_value = emit_expression(arg, semantics);
            if (arg_value == nullptr) {
                return nullptr;
            }
            args.push_back(arg_value);
        }
    }

    return builder.CreateCall(function_it->second, args, callee_name == "printf" ? "" : "calltmp");
}

llvm::Value* ClythLLVMCodegen::emit_unary(const ast::NodePtr& node, const semantic::SemanticResult& semantics) {
    if (node->children.size() == 1) {
        return emit_expression(node->children.front(), semantics);
    }

    std::string op;
    ast::NodePtr operand_node;

    for (const auto& child : node->children) {
        if (!child) {
            continue;
        }
        if (child->kind == ast::NodeKind::Token && op.empty()) {
            op = child->text;
        } else if (child->kind != ast::NodeKind::Token) {
            operand_node = child;
        }
    }

    llvm::Value* operand = emit_expression(operand_node, semantics);
    if (operand == nullptr) {
        return nullptr;
    }

    if (op == "-") {
        return builder.CreateNeg(operand, "negtmp");
    }
    if (op == "!" || op == "not") {
        return builder.CreateNot(operand, "nottmp");
    }
    if (op == "~") {
        return builder.CreateNot(operand, "bwnottmp");
    }
    if (op == "&") {
        add_codegen_error(node, "Address-of operator is not implemented yet.");
        return nullptr;
    }

    add_codegen_error(node, fmt::format("Unsupported unary operator '{}'.", op));
    return nullptr;
}

llvm::Value* ClythLLVMCodegen::emit_binary(const ast::NodePtr& node, const semantic::SemanticResult& semantics) {
    std::vector<ast::NodePtr> operands;
    std::vector<std::string> operators;

    for (const auto& child : node->children) {
        if (!child) {
            continue;
        }

        if (child->kind == ast::NodeKind::Token) {
            operators.push_back(child->text);
        } else {
            operands.push_back(child);
        }
    }

    if (operands.empty()) {
        add_codegen_error(node, "Binary expression has no operands.");
        return nullptr;
    }

    llvm::Value* lhs = emit_expression(operands.front(), semantics);
    if (lhs == nullptr) {
        return nullptr;
    }

    auto value_to_bool = [&](llvm::Value* value) -> llvm::Value* {
        if (value == nullptr) {
            return nullptr;
        }
        if (value->getType()->isIntegerTy(1)) {
            return value;
        }
        if (value->getType()->isIntegerTy()) {
            return builder.CreateICmpNE(value, llvm::ConstantInt::get(value->getType(), 0), "booltmp");
        }
        if (value->getType()->isPointerTy()) {
            return builder.CreateICmpNE(value, llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(value->getType())), "boolptrtmp");
        }
        if (value->getType()->isFloatingPointTy()) {
            return builder.CreateFCmpONE(value, llvm::ConstantFP::get(value->getType(), 0.0), "boolfloattmp");
        }
        return nullptr;
    };

    for (std::size_t i = 1; i < operands.size(); ++i) {
        llvm::Value* rhs = emit_expression(operands[i], semantics);
        if (rhs == nullptr) {
            return nullptr;
        }

        const std::string op = i - 1 < operators.size() ? operators[i - 1] : "";

        const bool numeric_float = lhs->getType()->isFloatingPointTy() || rhs->getType()->isFloatingPointTy();
        if (numeric_float) {
            llvm::Type* target_type = (lhs->getType()->isDoubleTy() || rhs->getType()->isDoubleTy())
                ? llvm::Type::getDoubleTy(context)
                : llvm::Type::getFloatTy(context);
            if (lhs->getType()->isIntegerTy()) {
                lhs = builder.CreateSIToFP(lhs, target_type, "lhsfpcast");
            } else if (lhs->getType() != target_type) {
                lhs = builder.CreateFPCast(lhs, target_type, "lhsfpcast");
            }
            if (rhs->getType()->isIntegerTy()) {
                rhs = builder.CreateSIToFP(rhs, target_type, "rhsfpcast");
            } else if (rhs->getType() != target_type) {
                rhs = builder.CreateFPCast(rhs, target_type, "rhsfpcast");
            }
        } else if (lhs->getType()->isIntegerTy() && rhs->getType()->isIntegerTy() && lhs->getType() != rhs->getType()) {
            const unsigned lhs_bits = lhs->getType()->getIntegerBitWidth();
            const unsigned rhs_bits = rhs->getType()->getIntegerBitWidth();
            llvm::Type* target_type = lhs_bits >= rhs_bits ? lhs->getType() : rhs->getType();
            lhs = builder.CreateIntCast(lhs, target_type, true, "lhscast");
            rhs = builder.CreateIntCast(rhs, target_type, true, "rhscast");
        }

        if (op == "+") {
            lhs = numeric_float ? builder.CreateFAdd(lhs, rhs, "faddtmp") : builder.CreateAdd(lhs, rhs, "addtmp");
        } else if (op == "-") {
            lhs = numeric_float ? builder.CreateFSub(lhs, rhs, "fsubtmp") : builder.CreateSub(lhs, rhs, "subtmp");
        } else if (op == "*") {
            lhs = numeric_float ? builder.CreateFMul(lhs, rhs, "fmultmp") : builder.CreateMul(lhs, rhs, "multmp");
        } else if (op == "/") {
            lhs = numeric_float ? builder.CreateFDiv(lhs, rhs, "fdivtmp") : builder.CreateSDiv(lhs, rhs, "divtmp");
        } else if (op == "%") {
            if (numeric_float) {
                add_codegen_error(node, "Modulo is not supported for floating-point operands.");
                return nullptr;
            }
            lhs = builder.CreateSRem(lhs, rhs, "modtmp");
        } else if (op == "==") {
            lhs = numeric_float ? builder.CreateFCmpOEQ(lhs, rhs, "feqtmp") : builder.CreateICmpEQ(lhs, rhs, "eqtmp");
        } else if (op == "!=") {
            lhs = numeric_float ? builder.CreateFCmpONE(lhs, rhs, "fnetmp") : builder.CreateICmpNE(lhs, rhs, "netmp");
        } else if (op == "<") {
            lhs = numeric_float ? builder.CreateFCmpOLT(lhs, rhs, "flttmp") : builder.CreateICmpSLT(lhs, rhs, "lttmp");
        } else if (op == "<=") {
            lhs = numeric_float ? builder.CreateFCmpOLE(lhs, rhs, "fletmp") : builder.CreateICmpSLE(lhs, rhs, "letmp");
        } else if (op == ">") {
            lhs = numeric_float ? builder.CreateFCmpOGT(lhs, rhs, "fgttmp") : builder.CreateICmpSGT(lhs, rhs, "gttmp");
        } else if (op == ">=") {
            lhs = numeric_float ? builder.CreateFCmpOGE(lhs, rhs, "fgetmp") : builder.CreateICmpSGE(lhs, rhs, "getmp");
        } else if (op == "&&" || op == "and") {
            llvm::Value* lhs_bool = value_to_bool(lhs);
            llvm::Value* rhs_bool = value_to_bool(rhs);
            if (lhs_bool == nullptr || rhs_bool == nullptr) {
                add_codegen_error(node, "Logical and requires boolean-compatible operands.");
                return nullptr;
            }
            lhs = builder.CreateAnd(lhs_bool, rhs_bool, "andtmp");
        } else if (op == "||" || op == "or") {
            llvm::Value* lhs_bool = value_to_bool(lhs);
            llvm::Value* rhs_bool = value_to_bool(rhs);
            if (lhs_bool == nullptr || rhs_bool == nullptr) {
                add_codegen_error(node, "Logical or requires boolean-compatible operands.");
                return nullptr;
            }
            lhs = builder.CreateOr(lhs_bool, rhs_bool, "ortmp");
        } else {
            add_codegen_error(node, fmt::format("Unsupported binary operator '{}'.", op));
            return nullptr;
        }
    }

    return lhs;
}


llvm::Value* ClythLLVMCodegen::emit_condition_value(const ast::NodePtr& node, const semantic::SemanticResult& semantics) {
    llvm::Value* value = emit_expression(node, semantics);
    if (value == nullptr) {
        return nullptr;
    }

    if (value->getType()->isIntegerTy(1)) {
        return value;
    }

    if (value->getType()->isIntegerTy()) {
        return builder.CreateICmpNE(value, llvm::ConstantInt::get(value->getType(), 0), "condtmp");
    }

    if (value->getType()->isPointerTy()) {
        return builder.CreateICmpNE(value, llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(value->getType())), "condptrtmp");
    }

    if (value->getType()->isFloatingPointTy()) {
        return builder.CreateFCmpONE(value, llvm::ConstantFP::get(value->getType(), 0.0), "condfloattmp");
    }

    add_codegen_error(node, "Condition expression does not lower to a supported truthy type.");
    return nullptr;
}

llvm::Constant* ClythLLVMCodegen::emit_global_constant_initializer(llvm::Type* type, const ast::NodePtr& node) {
    const auto exprs = expression_children(node);
    if (exprs.empty()) {
        return nullptr;
    }

    const ast::NodePtr expr = exprs.front();
    if (!expr || expr->kind != ast::NodeKind::LiteralExpr) {
        add_codegen_error(node, "Global variable initializers currently support literals only.");
        return nullptr;
    }

    const std::string literal = attr(expr, "literal").value_or(expr->text);

    if (type->isIntegerTy(1)) {
        if (literal == "true") {
            return llvm::ConstantInt::get(type, 1);
        }
        if (literal == "false") {
            return llvm::ConstantInt::get(type, 0);
        }
    }

    if (type->isIntegerTy() && is_integer_literal(literal)) {
        return llvm::ConstantInt::get(type, std::stoll(literal), true);
    }

    if (type->isFloatingPointTy()) {
        try {
            return llvm::ConstantFP::get(type, std::stod(literal));
        } catch (const std::exception&) {
            add_codegen_error(node, fmt::format("Invalid floating-point global initializer '{}'.", literal));
            return nullptr;
        }
    }

    if (type->isPointerTy()) {
        if (literal == "null") {
            return llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(type));
        }

        if (!literal.empty() && literal.front() == '"') {
            const std::string unescaped = unescape_clyth_string_literal(literal);
            llvm::Constant* raw_string = llvm::ConstantDataArray::getString(context, unescaped, true);
            auto* global_string = new llvm::GlobalVariable(
                *module,
                raw_string->getType(),
                true,
                llvm::GlobalValue::PrivateLinkage,
                raw_string,
                ".global_string"
            );
            global_string->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);
            global_string->setAlignment(llvm::Align(1));

            llvm::Constant* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
            llvm::Constant* indices[] = {zero, zero};
            return llvm::ConstantExpr::getInBoundsGetElementPtr(raw_string->getType(), global_string, indices);
        }
    }

    add_codegen_error(node, "Unsupported global variable initializer for this type.");
    return nullptr;
}

llvm::Type* ClythLLVMCodegen::llvm_type_from_clyth_type(const std::string& type_name) {
    if (type_name == "void") {
        return llvm::Type::getVoidTy(context);
    }
    if (type_name == "bool") {
        return llvm::Type::getInt1Ty(context);
    }
    if (type_name == "char" || type_name == "uint8" || type_name == "int8") {
        return llvm::Type::getInt8Ty(context);
    }
    if (type_name == "uint16" || type_name == "int16") {
        return llvm::Type::getInt16Ty(context);
    }
    if (type_name == "uint32" || type_name == "int32" || type_name == "int" || type_name == "uint" || type_name == "numeric") {
        return llvm::Type::getInt32Ty(context);
    }
    if (type_name == "uint64" || type_name == "int64") {
        return llvm::Type::getInt64Ty(context);
    }
    if (type_name == "float32" || type_name == "float") {
        return llvm::Type::getFloatTy(context);
    }
    if (type_name == "float64" || type_name == "double") {
        return llvm::Type::getDoubleTy(context);
    }
    if (type_name == "string") {
        // Pass 2 still treats string values at extern-C boundaries as C string
        // pointers. Native Clyth string layout is planned as char[]-backed
        // runtime data in a later pass.
        return llvm::PointerType::get(context, 0);
    }
    if (auto fixed_array = parse_fixed_array_type_name(type_name)) {
        llvm::Type* element_type = llvm_type_from_clyth_type(fixed_array->element_type_name);
        if (element_type == nullptr || fixed_array->length == 0) {
            return nullptr;
        }
        return llvm::ArrayType::get(element_type, fixed_array->length);
    }
    if (is_dynamic_array_type_name(type_name) || is_pointer_generic_type_name(type_name)) {
        return llvm::PointerType::get(context, 0);
    }

    if (type_name.find('[') != std::string::npos || type_name.find('<') != std::string::npos || type_name.find('(') != std::string::npos) {
        // Runtime containers and fixed arrays need layout/runtime lowering. Use
        // opaque pointers only when they appear in extern boundaries for now.
        return llvm::PointerType::get(context, 0);
    }

    return nullptr;
}

llvm::AllocaInst* ClythLLVMCodegen::create_entry_alloca(llvm::Function* function, llvm::Type* type, const std::string& name) {
    llvm::IRBuilder<> temporary_builder(&function->getEntryBlock(), function->getEntryBlock().begin());
    return temporary_builder.CreateAlloca(type, nullptr, name);
}

void ClythLLVMCodegen::push_local_scope() {
    if (!current_scope) {
        return;
    }

    current_scope->local_scopes.emplace_back();
    current_scope->local_array_scopes.emplace_back();
}

void ClythLLVMCodegen::pop_local_scope() {
    if (!current_scope || current_scope->local_scopes.empty()) {
        return;
    }

    current_scope->local_scopes.pop_back();
    if (!current_scope->local_array_scopes.empty()) {
        current_scope->local_array_scopes.pop_back();
    }
}

bool ClythLLVMCodegen::register_local(const std::string& name, llvm::AllocaInst* alloca) {
    if (!current_scope || current_scope->local_scopes.empty()) {
        diagnostics.add_error(SourceLocation{}, fmt::format("Internal codegen error: no local scope exists while registering '{}'.", name));
        return false;
    }

    auto& scope = current_scope->local_scopes.back();
    if (scope.find(name) != scope.end()) {
        diagnostics.add_error(SourceLocation{}, fmt::format("Duplicate local variable '{}' in the same lexical scope.", name));
        return false;
    }

    scope[name] = alloca;
    return true;
}


bool ClythLLVMCodegen::register_local_array(const std::string& name, const LocalArrayInfo& info) {
    if (!current_scope || current_scope->local_array_scopes.empty()) {
        diagnostics.add_error(SourceLocation{}, fmt::format("Internal codegen error: no array scope exists while registering '{}'.", name));
        return false;
    }

    current_scope->local_array_scopes.back()[name] = info;
    return true;
}

std::optional<ClythLLVMCodegen::LocalArrayInfo> ClythLLVMCodegen::lookup_local_array(const std::string& name) const {
    if (!current_scope) {
        return std::nullopt;
    }

    for (auto it = current_scope->local_array_scopes.rbegin(); it != current_scope->local_array_scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second;
        }
    }

    return std::nullopt;
}

llvm::AllocaInst* ClythLLVMCodegen::lookup_local(const std::string& name) const {
    if (!current_scope) {
        return nullptr;
    }

    for (auto it = current_scope->local_scopes.rbegin(); it != current_scope->local_scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second;
        }
    }

    return nullptr;
}

bool ClythLLVMCodegen::write_ir_file(const std::filesystem::path& output_path) {
    std::error_code ec;
    llvm::raw_fd_ostream output(output_path.string(), ec, llvm::sys::fs::OF_Text);

    if (ec) {
        diagnostics.add_error(SourceLocation{}, fmt::format("Failed to open LLVM IR output file '{}': {}", output_path.string(), ec.message()));
        return false;
    }

    module->print(output, nullptr);
    fmt::print("Wrote LLVM IR: {}\n", output_path.string());
    return true;
}

bool ClythLLVMCodegen::link_executable(const CodegenConfig& config) {
    if (config.output_binary_path.empty()) {
        diagnostics.add_error(SourceLocation{}, "Executable linking requested but output_binary_path is empty.");
        return false;
    }

    if (config.output_ir_path.empty()) {
        diagnostics.add_error(SourceLocation{}, "Executable linking requested but output_ir_path is empty.");
        return false;
    }

    std::ostringstream command;
    command << config.linker_command;
    if (!config.link_target.empty()) {
        command << " -target " << shell_quote(config.link_target);
    }
    command << " -static ";
    command << shell_quote(config.output_ir_path.string());
    command << " -o " << shell_quote(config.output_binary_path.string());

    fmt::print("Linking executable: {}\n", config.output_binary_path.string());
    const int rc = std::system(command.str().c_str());
    if (rc != 0) {
        diagnostics.add_error(SourceLocation{}, fmt::format("Link command failed with exit code {}: {}", rc, command.str()));
        return false;
    }

    fmt::print("Wrote executable: {}\n", config.output_binary_path.string());
    return true;
}

void ClythLLVMCodegen::add_codegen_error(const ast::NodePtr& node, const std::string& message) {
    diagnostics.add_error(node ? node->location : SourceLocation{}, message);
}

std::string ClythLLVMCodegen::shell_quote(const std::string& value) {
    std::string result = "'";
    for (char ch : value) {
        if (ch == '\'') {
            result += "'\\''";
        } else {
            result.push_back(ch);
        }
    }
    result.push_back('\'');
    return result;
}

} // namespace clyth::llvm_stub
