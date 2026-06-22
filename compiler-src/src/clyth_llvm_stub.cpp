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
    for (const auto& child : node->children) {
        if (!child || child->label != "paramList") {
            continue;
        }

        for (const auto& param : child->children) {
            if (!param || param->kind != ast::NodeKind::Param) {
                continue;
            }

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
        arg.setName(fmt::format("arg{}", arg_index++));
    }

    functions[function_name] = function;
    return true;
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

    // Materialize arguments into allocas so later assignment/load logic can be uniform.
    std::vector<ast::NodePtr> params;
    for (const auto& child : node->children) {
        if (!child || child->label != "paramList") {
            continue;
        }
        for (const auto& param : child->children) {
            if (param && param->kind == ast::NodeKind::Param) {
                params.push_back(param);
            }
        }
    }

    std::size_t index = 0;
    for (llvm::Argument& arg : function->args()) {
        std::string name = fmt::format("arg{}", index);
        if (index < params.size()) {
            if (auto param_name = declared_name(params[index])) {
                name = *param_name;
            }
        }
        arg.setName(name);

        llvm::AllocaInst* alloca = create_entry_alloca(function, arg.getType(), name);
        builder.CreateStore(&arg, alloca);
        current_scope->locals[name] = alloca;
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

    for (const auto& child : node->children) {
        if (!emit_statement(child, semantics)) {
            return false;
        }

        if (builder.GetInsertBlock() && builder.GetInsertBlock()->getTerminator()) {
            break;
        }
    }

    return true;
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
            current_scope->locals[*maybe_name] = alloca;

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

            const auto maybe_name = first_token_text(first_child_with_label(node, "assignable"));
            if (!maybe_name) {
                add_codegen_error(node, "Assignment target could not be resolved.");
                return false;
            }

            auto local_it = current_scope->locals.find(*maybe_name);
            if (local_it == current_scope->locals.end()) {
                add_codegen_error(node, fmt::format("Unknown assignment target '{}'.", *maybe_name));
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

            builder.CreateStore(value, local_it->second);
            return true;
        }

        case ast::NodeKind::IfStmt:
        case ast::NodeKind::WhileStmt:
        case ast::NodeKind::ForStmt:
        case ast::NodeKind::BreakStmt:
        case ast::NodeKind::ContinueStmt:
        case ast::NodeKind::MeccBlockStmt:
            add_codegen_error(node, fmt::format("Codegen for '{}' is not implemented in this V1 backend subset yet.", ast::node_kind_name(node->kind)));
            return false;

        default:
            if (semantic::query::is_expression_node(node)) {
                emit_expression(node, semantics);
                return !diagnostics.has_errors();
            }
            return true;
    }
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
                if (current_scope && current_scope->locals.count(*token) > 0) {
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

    auto it = current_scope->locals.find(*maybe_name);
    if (it == current_scope->locals.end()) {
        add_codegen_error(node, fmt::format("Unknown identifier '{}'.", *maybe_name));
        return nullptr;
    }

    return builder.CreateLoad(it->second->getAllocatedType(), it->second, *maybe_name);
}

llvm::Value* ClythLLVMCodegen::emit_postfix(const ast::NodePtr& node, const semantic::SemanticResult& semantics) {
    const auto maybe_base_name = first_token_text(node);
    if (!maybe_base_name) {
        add_codegen_error(node, "Postfix expression is missing a base identifier.");
        return nullptr;
    }

    llvm::Value* current_value = nullptr;
    bool consumed_call = false;

    for (const auto& child : node->children) {
        if (!child) {
            continue;
        }

        if (child->kind == ast::NodeKind::CallExpr) {
            current_value = emit_call_suffix(*maybe_base_name, child, semantics);
            consumed_call = true;
            continue;
        }

        if (child->kind == ast::NodeKind::MemberAccessExpr || child->kind == ast::NodeKind::IndexExpr) {
            add_codegen_error(child, fmt::format("Postfix suffix '{}' is not implemented yet.", ast::node_kind_name(child->kind)));
            return nullptr;
        }
    }

    if (consumed_call) {
        return current_value;
    }

    return emit_identifier(node);
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

    for (std::size_t i = 1; i < operands.size(); ++i) {
        llvm::Value* rhs = emit_expression(operands[i], semantics);
        if (rhs == nullptr) {
            return nullptr;
        }

        const std::string op = i - 1 < operators.size() ? operators[i - 1] : "";

        if (lhs->getType()->isIntegerTy() && rhs->getType()->isIntegerTy() && lhs->getType() != rhs->getType()) {
            const unsigned lhs_bits = lhs->getType()->getIntegerBitWidth();
            const unsigned rhs_bits = rhs->getType()->getIntegerBitWidth();
            llvm::Type* target_type = lhs_bits >= rhs_bits ? lhs->getType() : rhs->getType();
            lhs = builder.CreateIntCast(lhs, target_type, true, "lhscast");
            rhs = builder.CreateIntCast(rhs, target_type, true, "rhscast");
        }

        if (op == "+") {
            lhs = builder.CreateAdd(lhs, rhs, "addtmp");
        } else if (op == "-") {
            lhs = builder.CreateSub(lhs, rhs, "subtmp");
        } else if (op == "*") {
            lhs = builder.CreateMul(lhs, rhs, "multmp");
        } else if (op == "/") {
            lhs = builder.CreateSDiv(lhs, rhs, "divtmp");
        } else if (op == "%") {
            lhs = builder.CreateSRem(lhs, rhs, "modtmp");
        } else if (op == "==") {
            lhs = builder.CreateICmpEQ(lhs, rhs, "eqtmp");
        } else if (op == "!=") {
            lhs = builder.CreateICmpNE(lhs, rhs, "netmp");
        } else if (op == "<") {
            lhs = builder.CreateICmpSLT(lhs, rhs, "lttmp");
        } else if (op == "<=") {
            lhs = builder.CreateICmpSLE(lhs, rhs, "letmp");
        } else if (op == ">") {
            lhs = builder.CreateICmpSGT(lhs, rhs, "gttmp");
        } else if (op == ">=") {
            lhs = builder.CreateICmpSGE(lhs, rhs, "getmp");
        } else if (op == "&&" || op == "and") {
            lhs = builder.CreateAnd(lhs, rhs, "andtmp");
        } else if (op == "||" || op == "or") {
            lhs = builder.CreateOr(lhs, rhs, "ortmp");
        } else {
            add_codegen_error(node, fmt::format("Unsupported binary operator '{}'.", op));
            return nullptr;
        }
    }

    return lhs;
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
