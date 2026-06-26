#include "clyth_llvm_stub.hpp"

#include <algorithm>
#include <cerrno>
#include <cstdint>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <string_view>

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

std::vector<ast::NodePtr> call_arguments(const ast::NodePtr& call_node) {
    if (!call_node) {
        return {};
    }

    const ast::NodePtr arg_list = first_child_with_label(call_node, "argumentList");
    return arg_list ? expression_children(arg_list) : std::vector<ast::NodePtr>{};
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

std::optional<std::string> parse_dynamic_array_element_type_name(const std::string& type_name) {
    if (!is_dynamic_array_type_name(type_name)) {
        return std::nullopt;
    }
    std::string element = type_name.substr(0, type_name.size() - 2);
    if (element.empty()) {
        return std::nullopt;
    }
    return element;
}

std::optional<std::string> parse_list_element_type_name(const std::string& type_name) {
    constexpr std::string_view prefix = "List<";
    if (type_name.rfind(std::string(prefix), 0) != 0 || type_name.empty() || type_name.back() != '>') {
        return std::nullopt;
    }

    std::string element = type_name.substr(prefix.size(), type_name.size() - prefix.size() - 1);
    if (element.empty()) {
        return std::nullopt;
    }

    return element;
}

bool is_list_type_name(const std::string& type_name) {
    return parse_list_element_type_name(type_name).has_value();
}

std::optional<std::string> parse_set_element_type_name(const std::string& type_name) {
    std::size_t prefix_size = 0;
    if (type_name.rfind("Set<", 0) == 0) {
        prefix_size = 4;
    } else if (type_name.rfind("set<", 0) == 0) {
        prefix_size = 4;
    } else {
        return std::nullopt;
    }
    if (type_name.empty() || type_name.back() != '>') {
        return std::nullopt;
    }

    std::string element = type_name.substr(prefix_size, type_name.size() - prefix_size - 1);
    if (element.empty()) {
        return std::nullopt;
    }

    return element;
}

bool is_set_type_name(const std::string& type_name) {
    return parse_set_element_type_name(type_name).has_value();
}

std::vector<std::string> split_top_level_generic_args(const std::string& text) {
    std::vector<std::string> result;
    std::string current;
    int depth = 0;

    for (char ch : text) {
        if (ch == '<') {
            ++depth;
            current.push_back(ch);
            continue;
        }
        if (ch == '>') {
            --depth;
            current.push_back(ch);
            continue;
        }
        if ((ch == ',' || ch == ':') && depth == 0) {
            if (!current.empty()) {
                result.push_back(current);
            }
            current.clear();
            continue;
        }
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            current.push_back(ch);
        }
    }

    if (!current.empty()) {
        result.push_back(current);
    }

    return result;
}

struct MapTypeInfo {
    std::string key_type_name;
    std::string value_type_name;
};

std::optional<MapTypeInfo> parse_map_type_name(const std::string& type_name) {
    std::size_t prefix_size = 0;
    if (type_name.rfind("Map<", 0) == 0) {
        prefix_size = 4;
    } else {
        return std::nullopt;
    }

    if (type_name.empty() || type_name.back() != '>') {
        return std::nullopt;
    }

    const std::string inner = type_name.substr(prefix_size, type_name.size() - prefix_size - 1);
    const auto args = split_top_level_generic_args(inner);
    if (args.size() != 2 || args[0].empty() || args[1].empty()) {
        return std::nullopt;
    }

    return MapTypeInfo{args[0], args[1]};
}

bool is_map_type_name(const std::string& type_name) {
    return parse_map_type_name(type_name).has_value();
}

std::string sanitize_type_fragment_for_symbol(std::string value) {
    for (char& ch : value) {
        if (!(std::isalnum(static_cast<unsigned char>(ch)) || ch == '_')) {
            ch = '_';
        }
    }
    return value;
}

std::uint64_t conservative_element_size_bytes(llvm::Type* type) {
    if (type == nullptr) {
        return 8;
    }
    if (type->isIntegerTy()) {
        return std::max<std::uint64_t>(1, type->getIntegerBitWidth() / 8);
    }
    if (type->isFloatTy()) {
        return 4;
    }
    if (type->isDoubleTy()) {
        return 8;
    }
    if (type->isPointerTy()) {
        return 8;
    }
    if (auto* struct_type = llvm::dyn_cast<llvm::StructType>(type)) {
        std::uint64_t total = 0;
        for (llvm::Type* element : struct_type->elements()) {
            total += conservative_element_size_bytes(element);
        }
        return std::max<std::uint64_t>(1, total);
    }
    return 8;
}

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

std::vector<ast::NodePtr> keyed_literal_entries(const ast::NodePtr& keyed_node) {
    std::vector<ast::NodePtr> result;
    if (!keyed_node) {
        return result;
    }

    if (keyed_node->kind == ast::NodeKind::CurlyEntry) {
        result.push_back(keyed_node);
        return result;
    }

    for (const auto& child : keyed_node->children) {
        if (!child) {
            continue;
        }
        if (child->kind == ast::NodeKind::CurlyEntry) {
            result.push_back(child);
            continue;
        }
        auto nested = keyed_literal_entries(child);
        result.insert(result.end(), nested.begin(), nested.end());
    }

    return result;
}

std::pair<ast::NodePtr, ast::NodePtr> keyed_entry_expressions(const ast::NodePtr& entry) {
    auto exprs = expression_children(entry);
    if (exprs.size() >= 2) {
        return {exprs[0], exprs[1]};
    }
    return {nullptr, nullptr};
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


std::optional<std::string> member_name_from_member_access(const ast::NodePtr& node) {
    if (!node) {
        return std::nullopt;
    }

    if (!node->text.empty()) {
        const std::size_t dot = node->text.rfind('.');
        if (dot != std::string::npos && dot + 1 < node->text.size()) {
            return node->text.substr(dot + 1);
        }
    }

    std::optional<std::string> last_identifier;
    for (const auto& child : node->children) {
        if (!child) {
            continue;
        }
        if (child->kind == ast::NodeKind::Token && !child->text.empty() && child->text != ".") {
            last_identifier = child->text;
        }
    }
    return last_identifier;
}



bool is_identifier_like(const std::string& token) {
    if (token.empty()) {
        return false;
    }

    const unsigned char first = static_cast<unsigned char>(token.front());
    if (!(std::isalpha(first) || token.front() == '_')) {
        return false;
    }

    for (char raw_char : token) {
        const unsigned char c = static_cast<unsigned char>(raw_char);
        if (!(std::isalnum(c) || raw_char == '_')) {
            return false;
        }
    }

    return true;
}


std::string method_key(const std::string& owner_type_name, const std::string& method_name) {
    return owner_type_name + "." + method_name;
}

std::string lowered_method_name(const std::string& owner_type_name, const std::string& method_name) {
    return owner_type_name + "." + method_name;
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

    if (config.compile_to_executable && !config.preserve_ir_file && !config.output_ir_path.empty()) {
        std::error_code ec;
        std::filesystem::remove(config.output_ir_path, ec);
        if (!ec) {
            fmt::print("Removed temporary LLVM IR: {}\n", config.output_ir_path.string());
        }
    }

    return !diagnostics.has_errors();
}

bool ClythLLVMCodegen::emit_program(
    const lowering::ClythLoweringPlan& plan,
    const semantic::SemanticResult& semantics
) {
    const std::vector<ast::NodePtr> top_level = direct_top_level_nodes(plan);

    // Pass 0: create opaque LLVM struct shells so field types can reference
    // structs independent of top-level source order.
    for (const auto& node : top_level) {
        if (node && node->kind == ast::NodeKind::StructDecl && !declare_struct_shell(node)) {
            return false;
        }
    }

    // Pass 1: populate struct field layouts once all struct names are known.
    for (const auto& node : top_level) {
        if (node && node->kind == ast::NodeKind::StructDecl && !define_struct_body(node)) {
            return false;
        }
    }

    // Pass 2: create all externally callable/function/global symbols so calls
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
        } else if (node->kind == ast::NodeKind::MethodBlock) {
            const std::string owner = attr(node, "owner").value_or("");
            for (const auto& method : node->children) {
                if (method && method->kind == ast::NodeKind::MethodDecl && !declare_method_header(method, owner)) {
                    return false;
                }
            }
        } else if (node->kind == ast::NodeKind::MethodDecl) {
            const std::string owner = attr(node, "owner").value_or("");
            if (!owner.empty() && !declare_method_header(node, owner)) {
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
            return true;

        case ast::NodeKind::MethodDecl: {
            const std::string owner = attr(node, "owner").value_or("");
            if (!owner.empty()) {
                return emit_method_body(node, semantics, owner);
            }
            return true;
        }

        case ast::NodeKind::MethodBlock: {
            const std::string owner = attr(node, "owner").value_or("");
            for (const auto& method : node->children) {
                if (method && method->kind == ast::NodeKind::MethodDecl && !emit_method_body(method, semantics, owner)) {
                    return false;
                }
            }
            return true;
        }

        case ast::NodeKind::IncludeDecl:
            return true;

        case ast::NodeKind::VarDeclStmt:
            return true;

        default:
            add_codegen_error(node, fmt::format("Unsupported top-level node '{}'.", ast::node_kind_name(node->kind)));
            return false;
    }
}


bool ClythLLVMCodegen::declare_struct_shell(const ast::NodePtr& node) {
    const auto maybe_name = declared_name(node);
    if (!maybe_name) {
        add_codegen_error(node, "Struct declaration is missing a name.");
        return false;
    }

    if (structs.find(*maybe_name) != structs.end()) {
        return true;
    }

    StructInfo info;
    info.name = *maybe_name;
    info.type = llvm::StructType::create(context, fmt::format("struct.{}", *maybe_name));
    structs[*maybe_name] = std::move(info);
    return true;
}

bool ClythLLVMCodegen::define_struct_body(const ast::NodePtr& node) {
    const auto maybe_name = declared_name(node);
    if (!maybe_name) {
        add_codegen_error(node, "Struct declaration is missing a name.");
        return false;
    }

    auto struct_it = structs.find(*maybe_name);
    if (struct_it == structs.end() || struct_it->second.type == nullptr) {
        add_codegen_error(node, fmt::format("Struct '{}' was not declared before layout emission.", *maybe_name));
        return false;
    }

    StructInfo& info = struct_it->second;
    if (!info.field_names.empty() || !info.field_indices.empty()) {
        return true;
    }

    std::vector<llvm::Type*> field_types;
    for (const auto& child : node->children) {
        if (!child || child->kind != ast::NodeKind::StructField) {
            continue;
        }

        const auto maybe_field_name = declared_name(child);
        if (!maybe_field_name) {
            add_codegen_error(child, fmt::format("Struct '{}' contains a field without a name.", *maybe_name));
            return false;
        }

        const std::string field_type_name = declared_type_name(child);
        llvm::Type* field_type = llvm_type_from_clyth_type(field_type_name);
        if (field_type == nullptr) {
            add_codegen_error(child, fmt::format("Unsupported field type '{}' for '{}.{}'.", field_type_name, *maybe_name, *maybe_field_name));
            return false;
        }

        if (field_type == info.type) {
            add_codegen_error(child, fmt::format("Struct '{}' cannot contain itself by value; use pointer<{}> later if recursive layout is needed.", *maybe_name, *maybe_name));
            return false;
        }

        const std::size_t index = info.field_names.size();
        info.field_indices[*maybe_field_name] = index;
        info.field_names.push_back(*maybe_field_name);
        info.field_type_names.push_back(field_type_name);
        field_types.push_back(field_type);
    }

    info.type->setBody(field_types, false);
    return true;
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
    llvm::Type* return_type = return_type_name == "string" ? llvm::PointerType::get(context, 0) : llvm_type_from_clyth_type(return_type_name);
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
            llvm::Type* param_type = param_type_name == "string" ? llvm::PointerType::get(context, 0) : llvm_type_from_clyth_type(param_type_name);
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


bool ClythLLVMCodegen::declare_method_header(const ast::NodePtr& node, const std::string& owner_type_name) {
    const std::string owner = !owner_type_name.empty() ? owner_type_name : attr(node, "owner").value_or("");
    if (owner.empty()) {
        add_codegen_error(node, "Method declaration is missing an owner type.");
        return false;
    }

    auto struct_it = structs.find(owner);
    if (struct_it == structs.end() || struct_it->second.type == nullptr) {
        add_codegen_error(node, fmt::format("Method owner '{}' is not a known struct.", owner));
        return false;
    }

    const auto maybe_name = declared_name(node);
    if (!maybe_name) {
        add_codegen_error(node, fmt::format("Method declaration for '{}' is missing a name.", owner));
        return false;
    }

    const std::string name = *maybe_name;
    const std::string key = method_key(owner, name);
    const std::string lowered = lowered_method_name(owner, name);

    if (functions.find(lowered) != functions.end()) {
        return true;
    }

    const std::string return_type_name = declared_type_name(node);
    llvm::Type* return_type = llvm_type_from_clyth_type(return_type_name);
    if (return_type == nullptr) {
        add_codegen_error(node, fmt::format("Unsupported method return type '{}'.", return_type_name));
        return false;
    }

    std::vector<llvm::Type*> parameter_types;
    parameter_types.push_back(llvm::PointerType::get(context, 0));
    for (const auto& param : direct_params(node)) {
        const std::string param_type_name = declared_type_name(param);
        llvm::Type* param_type = llvm_type_from_clyth_type(param_type_name);
        if (param_type == nullptr) {
            add_codegen_error(param, fmt::format("Unsupported method parameter type '{}'.", param_type_name));
            return false;
        }
        parameter_types.push_back(param_type);
    }

    llvm::FunctionType* function_type = llvm::FunctionType::get(return_type, parameter_types, false);
    llvm::Function* function = llvm::Function::Create(
        function_type,
        llvm::Function::ExternalLinkage,
        lowered,
        module.get()
    );

    std::size_t index = 0;
    for (llvm::Argument& arg : function->args()) {
        arg.setName(index == 0 ? "this" : fmt::format("arg{}", index - 1));
        ++index;
    }

    functions[lowered] = function;
    methods[key] = MethodInfo{owner, name, lowered, node};
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
        } else if (type->isStructTy() || type->isArrayTy()) {
            initializer = llvm::ConstantAggregateZero::get(type);
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
    global_type_names[name] = type_name;
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
        if (modern_main && index == 1 && !params.empty()) {
            register_local_type(name, declared_type_name(params.front()));
        } else if (!modern_main && index < params.size()) {
            register_local_type(name, declared_type_name(params[index]));
        }
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


bool ClythLLVMCodegen::emit_method_body(
    const ast::NodePtr& node,
    const semantic::SemanticResult& semantics,
    const std::string& owner_type_name
) {
    const std::string owner = !owner_type_name.empty() ? owner_type_name : attr(node, "owner").value_or("");
    const auto maybe_name = declared_name(node);
    if (owner.empty() || !maybe_name) {
        add_codegen_error(node, "Method body is missing an owner or method name.");
        return false;
    }

    const std::string lowered = lowered_method_name(owner, *maybe_name);
    auto it = functions.find(lowered);
    if (it == functions.end()) {
        add_codegen_error(node, fmt::format("Method '{}.{}' was not declared before body emission.", owner, *maybe_name));
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
    current_scope->this_type_name = owner;
    push_local_scope();

    const std::vector<ast::NodePtr> params = direct_params(node);
    std::size_t index = 0;
    for (llvm::Argument& arg : function->args()) {
        std::string name;
        std::string type_name;
        if (index == 0) {
            name = "this";
            type_name = owner;
        } else {
            const std::size_t param_index = index - 1;
            name = fmt::format("arg{}", param_index);
            if (param_index < params.size()) {
                if (auto param_name = declared_name(params[param_index])) {
                    name = *param_name;
                }
                type_name = declared_type_name(params[param_index]);
            }
        }

        arg.setName(name);
        llvm::AllocaInst* alloca = create_entry_alloca(function, arg.getType(), name);
        builder.CreateStore(&arg, alloca);
        register_local(name, alloca);
        register_local_type(name, type_name);
        ++index;
    }

    const ast::NodePtr body = first_child_of_kind(node, ast::NodeKind::BlockStmt);
    if (!body) {
        add_codegen_error(node, fmt::format("Method '{}.{}' is missing a body block.", owner, *maybe_name));
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
            add_codegen_error(node, fmt::format("Method '{}.{}' has no return and unsupported default return type.", owner, *maybe_name));
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
            if (!register_local_type(*maybe_name, type_name)) { return false; }

            if (type_name == "string") {
                if (!register_local_string(*maybe_name, LocalStringInfo{alloca})) {
                    return false;
                }

                const auto exprs = expression_children(node);
                if (!exprs.empty()) {
                    ast::NodePtr literal_initializer = first_descendant_of_kind(exprs.front(), ast::NodeKind::LiteralExpr);
                    if (!literal_initializer) {
                        add_codegen_error(exprs.front(), "String initializer must be a string literal in this backend pass.");
                        return false;
                    }
                    return emit_string_literal_initializer(*maybe_name, literal_initializer);
                }

                builder.CreateStore(llvm::ConstantAggregateZero::get(type), alloca);
                return true;
            }

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

            if (auto dynamic_element = parse_dynamic_array_element_type_name(type_name)) {
                llvm::Type* element_type = llvm_type_from_clyth_type(*dynamic_element);
                if (element_type == nullptr) {
                    add_codegen_error(node, fmt::format("Unsupported dynamic array element type '{}'.", *dynamic_element));
                    return false;
                }
                llvm::StructType* array_type = dynamic_array_type_for(element_type, *dynamic_element, false);
                if (array_type == nullptr) {
                    add_codegen_error(node, fmt::format("Unable to create dynamic array type for '{}'.", type_name));
                    return false;
                }
                if (!register_local_dynamic_array(*maybe_name, LocalDynamicArrayInfo{alloca, array_type, element_type, *dynamic_element, false})) {
                    return false;
                }

                const auto exprs = expression_children(node);
                if (!exprs.empty()) {
                    ast::NodePtr list_initializer = first_descendant_of_kind(exprs.front(), ast::NodeKind::ListLiteralExpr);
                    if (!list_initializer) {
                        add_codegen_error(exprs.front(), "Dynamic array initializer must be a list literal in this backend pass.");
                        return false;
                    }
                    return emit_dynamic_array_initializer(*maybe_name, list_initializer, semantics);
                }

                builder.CreateStore(llvm::ConstantAggregateZero::get(type), alloca);
                return true;
            }

            if (auto list_element = parse_list_element_type_name(type_name)) {
                llvm::Type* element_type = llvm_type_from_clyth_type(*list_element);
                if (element_type == nullptr) {
                    add_codegen_error(node, fmt::format("Unsupported List element type '{}'.", *list_element));
                    return false;
                }
                llvm::StructType* list_type = dynamic_array_type_for(element_type, *list_element, true);
                if (list_type == nullptr) {
                    add_codegen_error(node, fmt::format("Unable to create List storage for '{}'.", type_name));
                    return false;
                }
                if (!register_local_dynamic_array(*maybe_name, LocalDynamicArrayInfo{alloca, list_type, element_type, *list_element, true})) {
                    return false;
                }

                const auto exprs = expression_children(node);
                if (!exprs.empty()) {
                    ast::NodePtr list_initializer = first_descendant_of_kind(exprs.front(), ast::NodeKind::ListLiteralExpr);
                    if (!list_initializer) {
                        add_codegen_error(exprs.front(), "List initializer must be a list literal in this backend pass.");
                        return false;
                    }
                    return emit_dynamic_array_initializer(*maybe_name, list_initializer, semantics);
                }

                builder.CreateStore(llvm::ConstantAggregateZero::get(type), alloca);
                return true;
            }

            if (auto set_element = parse_set_element_type_name(type_name)) {
                llvm::Type* element_type = llvm_type_from_clyth_type(*set_element);
                if (element_type == nullptr) {
                    add_codegen_error(node, fmt::format("Unsupported Set element type '{}'.", *set_element));
                    return false;
                }
                llvm::StructType* set_type = dynamic_array_type_for(element_type, *set_element, true);
                if (set_type == nullptr) {
                    add_codegen_error(node, fmt::format("Unable to create Set storage for '{}'.", type_name));
                    return false;
                }
                if (!register_local_dynamic_array(*maybe_name, LocalDynamicArrayInfo{alloca, set_type, element_type, *set_element, true})) {
                    return false;
                }

                const auto exprs = expression_children(node);
                if (!exprs.empty()) {
                    ast::NodePtr list_initializer = first_descendant_of_kind(exprs.front(), ast::NodeKind::ListLiteralExpr);
                    if (!list_initializer) {
                        add_codegen_error(exprs.front(), "Set initializer must be a list literal in this backend pass.");
                        return false;
                    }
                    if (!emit_dynamic_array_initializer(*maybe_name, list_initializer, semantics)) {
                        return false;
                    }
                    // Alpha 0.3 Set<T> validates uniqueness for insert(); literal initializers are accepted as-is for now.
                    return true;
                }

                builder.CreateStore(llvm::ConstantAggregateZero::get(type), alloca);
                return true;
            }

            if (auto map_info = parse_map_type_name(type_name)) {
                llvm::Type* key_type = llvm_type_from_clyth_type(map_info->key_type_name);
                llvm::Type* value_type = llvm_type_from_clyth_type(map_info->value_type_name);
                if (key_type == nullptr || value_type == nullptr) {
                    add_codegen_error(node, fmt::format("Unsupported Map key/value types '{}', '{}'.", map_info->key_type_name, map_info->value_type_name));
                    return false;
                }
                llvm::StructType* entry_type = map_entry_type_for(key_type, value_type, map_info->key_type_name, map_info->value_type_name);
                if (entry_type == nullptr) {
                    add_codegen_error(node, fmt::format("Unable to create Map entry storage for '{}'.", type_name));
                    return false;
                }
                const std::string entry_name = "MapEntry<" + map_info->key_type_name + ":" + map_info->value_type_name + ">";
                llvm::StructType* map_type = dynamic_array_type_for(entry_type, entry_name, true);
                if (map_type == nullptr) {
                    add_codegen_error(node, fmt::format("Unable to create Map storage for '{}'.", type_name));
                    return false;
                }
                LocalDynamicArrayInfo dynamic_info{alloca, map_type, entry_type, entry_name, true};
                if (!register_local_dynamic_array(*maybe_name, dynamic_info)) {
                    return false;
                }
                if (!register_local_map(*maybe_name, LocalMapInfo{alloca, map_type, entry_type, key_type, value_type, map_info->key_type_name, map_info->value_type_name})) {
                    return false;
                }

                builder.CreateStore(llvm::ConstantAggregateZero::get(type), alloca);

                const auto exprs = expression_children(node);
                if (!exprs.empty()) {
                    ast::NodePtr keyed_initializer = first_descendant_of_kind(exprs.front(), ast::NodeKind::CurlyLiteralExpr);
                    if (!keyed_initializer) {
                        add_codegen_error(exprs.front(), "Map initializer must be a keyed-array literal like [key: value, ...] in this backend pass.");
                        return false;
                    }
                    return emit_map_initializer(*maybe_name, keyed_initializer, semantics);
                }

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
            } else if (type->isStructTy() || type->isArrayTy()) {
                builder.CreateStore(llvm::ConstantAggregateZero::get(type), alloca);
            }

            if (structs.find(type_name) != structs.end()) {
                if (!maybe_emit_default_constructor_call(type_name, alloca, node)) {
                    return false;
                }
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

            if (target_type == string_type_for()) {
                std::string string_assignment_operator = "=";
                if (const ast::NodePtr op_node = first_child_with_label(node, "assignmentOp")) {
                    if (auto op_text = attr(op_node, "operator")) {
                        string_assignment_operator = *op_text;
                    }
                }
                if (string_assignment_operator != "=") {
                    add_codegen_error(node, "Compound assignment for string values is not implemented yet.");
                    return false;
                }
                ast::NodePtr literal_initializer = first_descendant_of_kind(exprs.back(), ast::NodeKind::LiteralExpr);
                if (!literal_initializer) {
                    add_codegen_error(exprs.back(), "String assignment currently requires a string literal.");
                    return false;
                }
                if (!maybe_name || !emit_string_literal_initializer(*maybe_name, literal_initializer)) {
                    return false;
                }
                return true;
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
                // In expression position, a generic wrapper whose first token is an
                // identifier should be lowered as an identifier.  That lets normal
                // locals/globals resolve, and also lets method bodies fall through
                // to implicit this.field lookup for field names such as `value`.
                if (is_identifier_like(*token)) {
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

    if (current_scope && !current_scope->this_type_name.empty()) {
        llvm::Type* field_type = nullptr;
        llvm::Value* field_address = emit_this_field_address(*maybe_name, node, &field_type);
        if (field_address != nullptr && field_type != nullptr) {
            return builder.CreateLoad(field_type, field_address, fmt::format("this.{}", *maybe_name));
        }
        if (!diagnostics.has_errors()) {
            // fall through to the ordinary unknown identifier diagnostic
        } else {
            return nullptr;
        }
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
    std::string pending_method_name;

    for (const auto& child : node->children) {
        if (!child) {
            continue;
        }

        if (child->kind == ast::NodeKind::CallExpr) {
            if (!pending_method_name.empty()) {
                if (lookup_local_map(*maybe_base_name)) {
                    if (pending_method_name == "put") {
                        if (!emit_map_put(*maybe_base_name, child, semantics)) {
                            return nullptr;
                        }
                        current_value = llvm::Constant::getNullValue(llvm::Type::getInt32Ty(context));
                    } else if (pending_method_name == "get") {
                        current_value = emit_map_get(*maybe_base_name, child, semantics);
                    } else if (pending_method_name == "contains_key") {
                        current_value = emit_map_contains_key(*maybe_base_name, child, semantics);
                    } else {
                        add_codegen_error(child, fmt::format("Map '{}' has no method named '{}'.", *maybe_base_name, pending_method_name));
                        return nullptr;
                    }
                } else if (auto dynamic_container_info = lookup_local_dynamic_array(*maybe_base_name)) {
                    if (!dynamic_container_info->has_capacity) {
                        add_codegen_error(child, fmt::format("'{}' is an array; mutating container methods are available on List<T> and Set<T>, not plain T[].", *maybe_base_name));
                        return nullptr;
                    }
                    if (pending_method_name == "push") {
                        if (!emit_dynamic_array_push(*maybe_base_name, child, semantics)) {
                            return nullptr;
                        }
                        current_value = llvm::Constant::getNullValue(llvm::Type::getInt32Ty(context));
                    } else if (pending_method_name == "insert") {
                        auto receiver_type = lookup_symbol_type_name(*maybe_base_name);
                        if (!receiver_type || !parse_set_element_type_name(*receiver_type)) {
                            add_codegen_error(child, fmt::format("insert(value) is currently available on Set<T>; '{}' is '{}'.", *maybe_base_name, receiver_type.value_or("<unknown>")));
                            return nullptr;
                        }
                        if (!emit_set_insert(*maybe_base_name, child, semantics)) {
                            return nullptr;
                        }
                        current_value = llvm::Constant::getNullValue(llvm::Type::getInt32Ty(context));
                    } else if (pending_method_name == "pop") {
                        current_value = emit_dynamic_array_pop(*maybe_base_name, child, semantics);
                    } else if (pending_method_name == "contains") {
                        current_value = emit_dynamic_array_contains(*maybe_base_name, child, semantics);
                    } else {
                        add_codegen_error(child, fmt::format("Dynamic array/List/Set '{}' has no method named '{}'.", *maybe_base_name, pending_method_name));
                        return nullptr;
                    }
                } else {
                    current_value = emit_method_call_suffix(*maybe_base_name, pending_method_name, child, semantics);
                }
                pending_method_name.clear();
            } else {
                current_value = emit_call_suffix(*maybe_base_name, child, semantics);
            }
            consumed_suffix = true;
            continue;
        }

        if (child->kind == ast::NodeKind::IndexExpr) {
            llvm::Type* element_type = nullptr;
            llvm::Value* element_address = emit_string_index_address(*maybe_base_name, child, semantics, &element_type);
            if (element_address != nullptr && element_type != nullptr) {
                current_value = builder.CreateLoad(element_type, element_address, "stringindexload");
                consumed_suffix = true;
                continue;
            }

            element_address = emit_fixed_array_element_address(*maybe_base_name, child, semantics, &element_type);
            if (element_address != nullptr && element_type != nullptr) {
                current_value = builder.CreateLoad(element_type, element_address, "arrayindexload");
                consumed_suffix = true;
                continue;
            }

            element_address = emit_dynamic_array_element_address(*maybe_base_name, child, semantics, &element_type);
            if (element_address != nullptr && element_type != nullptr) {
                current_value = builder.CreateLoad(element_type, element_address, "dynarrayindexload");
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
            const std::string member = member_name_from_member_access(child).value_or("");
            if (member == "length") {
                if (auto string_info = lookup_local_string(*maybe_base_name)) {
                    current_value = emit_string_field_load(*string_info, 1, *maybe_base_name + ".length");
                    consumed_suffix = true;
                    continue;
                }
                if (auto info = lookup_local_array(*maybe_base_name)) {
                    current_value = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), info->length);
                    consumed_suffix = true;
                    continue;
                }
                if (auto dynamic_info = lookup_local_dynamic_array(*maybe_base_name)) {
                    current_value = emit_dynamic_array_length(*dynamic_info);
                    consumed_suffix = true;
                    continue;
                }
            }

            if (member == "capacity") {
                if (auto string_info = lookup_local_string(*maybe_base_name)) {
                    current_value = emit_string_field_load(*string_info, 2, *maybe_base_name + ".capacity");
                    consumed_suffix = true;
                    continue;
                }
                if (auto dynamic_info = lookup_local_dynamic_array(*maybe_base_name)) {
                    if (!dynamic_info->has_capacity) {
                        add_codegen_error(child, fmt::format("'{}' is an array; capacity is available on List<T>, Set<T>, and Map<K,V> containers, not plain T[].", *maybe_base_name));
                        return nullptr;
                    }
                    current_value = emit_dynamic_array_capacity(*dynamic_info);
                    consumed_suffix = true;
                    continue;
                }
            }

            if (auto map_info = lookup_local_map(*maybe_base_name)) {
                (void)map_info;
                if (member == "put" || member == "get" || member == "contains_key") {
                    pending_method_name = member;
                    consumed_suffix = true;
                    continue;
                }
            }

            if (auto dynamic_info = lookup_local_dynamic_array(*maybe_base_name)) {
                (void)dynamic_info;
                if (member == "push" || member == "pop" || member == "contains") {
                    pending_method_name = member;
                    consumed_suffix = true;
                    continue;
                }
                if (member == "insert") {
                    auto receiver_type = lookup_symbol_type_name(*maybe_base_name);
                    if (receiver_type && parse_set_element_type_name(*receiver_type)) {
                        pending_method_name = member;
                        consumed_suffix = true;
                        continue;
                    }
                }
            }

            if (auto receiver_type = lookup_symbol_type_name(*maybe_base_name)) {
                if (methods.find(method_key(*receiver_type, member)) != methods.end()) {
                    pending_method_name = member;
                    consumed_suffix = true;
                    continue;
                }
            }

            llvm::Type* field_type = nullptr;
            llvm::Value* field_address = emit_struct_field_address(*maybe_base_name, child, &field_type);
            if (field_address == nullptr || field_type == nullptr) {
                return nullptr;
            }

            current_value = builder.CreateLoad(field_type, field_address, fmt::format("{}.{}", *maybe_base_name, member));
            consumed_suffix = true;
            continue;
        }
    }

    if (consumed_suffix) {
        return current_value;
    }

    return emit_identifier(node);
}



llvm::Value* ClythLLVMCodegen::emit_struct_field_address(
    const std::string& base_name,
    const ast::NodePtr& member_node,
    llvm::Type** out_type
) {
    const auto maybe_type_name = lookup_symbol_type_name(base_name);
    if (!maybe_type_name) {
        add_codegen_error(member_node, fmt::format("Cannot resolve type for '{}'.", base_name));
        return nullptr;
    }

    auto struct_it = structs.find(*maybe_type_name);
    if (struct_it == structs.end()) {
        add_codegen_error(member_node, fmt::format("Type '{}' for '{}' is not a struct.", *maybe_type_name, base_name));
        return nullptr;
    }

    const std::string field_name = member_name_from_member_access(member_node).value_or("");
    if (field_name.empty()) {
        add_codegen_error(member_node, fmt::format("Member access on '{}' is missing a field name.", base_name));
        return nullptr;
    }

    const StructInfo& info = struct_it->second;
    auto field_it = info.field_indices.find(field_name);
    if (field_it == info.field_indices.end()) {
        add_codegen_error(member_node, fmt::format("Struct '{}' has no field named '{}'.", info.name, field_name));
        return nullptr;
    }

    llvm::Value* base_address = nullptr;
    if (llvm::AllocaInst* local = lookup_local(base_name)) {
        if (local->getAllocatedType()->isPointerTy()) {
            base_address = builder.CreateLoad(local->getAllocatedType(), local, fmt::format("{}.ptr", base_name));
        } else {
            base_address = local;
        }
    } else {
        auto global_it = globals.find(base_name);
        if (global_it != globals.end()) {
            base_address = global_it->second;
        }
    }

    if (base_address == nullptr) {
        add_codegen_error(member_node, fmt::format("Unknown struct value '{}'.", base_name));
        return nullptr;
    }

    llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* field_index = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), field_it->second);
    llvm::Value* address = builder.CreateInBoundsGEP(
        info.type,
        base_address,
        {zero, field_index},
        fmt::format("{}.{}.addr", base_name, field_name)
    );

    if (out_type != nullptr) {
        *out_type = info.type->getElementType(static_cast<unsigned>(field_it->second));
    }

    return address;
}


llvm::Value* ClythLLVMCodegen::emit_this_field_address(
    const std::string& field_name,
    const ast::NodePtr& node,
    llvm::Type** out_type
) {
    if (!current_scope || current_scope->this_type_name.empty()) {
        return nullptr;
    }

    auto struct_it = structs.find(current_scope->this_type_name);
    if (struct_it == structs.end()) {
        add_codegen_error(node, fmt::format("Current method receiver type '{}' is not a known struct.", current_scope->this_type_name));
        return nullptr;
    }

    const StructInfo& info = struct_it->second;
    auto field_it = info.field_indices.find(field_name);
    if (field_it == info.field_indices.end()) {
        return nullptr;
    }

    llvm::AllocaInst* this_alloca = lookup_local("this");
    if (this_alloca == nullptr) {
        add_codegen_error(node, "Internal codegen error: method scope has no 'this' value.");
        return nullptr;
    }

    llvm::Value* this_address = builder.CreateLoad(this_alloca->getAllocatedType(), this_alloca, "this.ptr");
    llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* field_index = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), field_it->second);
    llvm::Value* address = builder.CreateInBoundsGEP(
        info.type,
        this_address,
        {zero, field_index},
        fmt::format("this.{}.addr", field_name)
    );

    if (out_type != nullptr) {
        *out_type = info.type->getElementType(static_cast<unsigned>(field_it->second));
    }

    return address;
}

bool ClythLLVMCodegen::maybe_emit_default_constructor_call(
    const std::string& type_name,
    llvm::Value* receiver_address,
    const ast::NodePtr& node
) {
    const std::string key = method_key(type_name, "constructor");
    auto method_it = methods.find(key);
    if (method_it == methods.end()) {
        return true;
    }

    llvm::Function* constructor = functions[method_it->second.lowered_name];
    if (constructor == nullptr) {
        add_codegen_error(node, fmt::format("Constructor for '{}' was registered but not lowered.", type_name));
        return false;
    }

    if (constructor->arg_size() != 1) {
        // Only default constructors are auto-invoked by a bare local declaration
        // in Alpha 0.2.0. Parameterized constructor syntax is future work.
        return true;
    }

    builder.CreateCall(constructor, {receiver_address});
    return true;
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
        if (!child) {
            continue;
        }
        if (child->kind == ast::NodeKind::MemberAccessExpr) {
            return emit_struct_field_address(*maybe_name, child, out_type);
        }
        if (child->kind == ast::NodeKind::IndexExpr) {
            if (llvm::Value* fixed_address = emit_fixed_array_element_address(*maybe_name, child, semantics, out_type)) {
                return fixed_address;
            }
            return emit_dynamic_array_element_address(*maybe_name, child, semantics, out_type);
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

    if (current_scope && !current_scope->this_type_name.empty()) {
        llvm::Value* field_address = emit_this_field_address(*maybe_name, node, out_type);
        if (field_address != nullptr) {
            return field_address;
        }
        if (diagnostics.has_errors()) {
            return nullptr;
        }
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

    llvm::Function* callee = function_it->second;
    const std::size_t fixed_param_count = callee->getFunctionType()->getNumParams();
    const bool is_vararg = callee->getFunctionType()->isVarArg();
    std::vector<llvm::Value*> args;
    const ast::NodePtr arg_list = first_child_with_label(call_node, "argumentList");
    if (arg_list) {
        for (const auto& arg : expression_children(arg_list)) {
            llvm::Value* arg_value = emit_expression(arg, semantics);
            if (arg_value == nullptr) {
                return nullptr;
            }
            if (arg_value->getType() == string_type_for()) {
                arg_value = emit_string_data_pointer(arg_value, "call.string.data");
            }
            if (is_vararg && args.size() >= fixed_param_count) {
                if (arg_value->getType()->isIntegerTy() && arg_value->getType()->getIntegerBitWidth() < 32) {
                    arg_value = builder.CreateIntCast(arg_value, llvm::Type::getInt32Ty(context), true, "vararg_int_promote");
                } else if (arg_value->getType()->isFloatTy()) {
                    arg_value = builder.CreateFPExt(arg_value, llvm::Type::getDoubleTy(context), "vararg_float_promote");
                }
            }
            args.push_back(arg_value);
        }
    }

    return builder.CreateCall(callee, args, (callee_name == "printf" || callee->getReturnType()->isVoidTy()) ? "" : "calltmp");
}


llvm::Value* ClythLLVMCodegen::emit_method_call_suffix(
    const std::string& receiver_name,
    const std::string& method_name,
    const ast::NodePtr& call_node,
    const semantic::SemanticResult& semantics
) {
    const auto maybe_receiver_type = lookup_symbol_type_name(receiver_name);
    if (!maybe_receiver_type) {
        add_codegen_error(call_node, fmt::format("Cannot resolve receiver type for '{}.{}'.", receiver_name, method_name));
        return nullptr;
    }

    const std::string key = method_key(*maybe_receiver_type, method_name);
    auto method_it = methods.find(key);
    if (method_it == methods.end()) {
        add_codegen_error(call_node, fmt::format("Type '{}' has no method named '{}'.", *maybe_receiver_type, method_name));
        return nullptr;
    }

    llvm::Value* receiver_address = nullptr;
    if (llvm::AllocaInst* local = lookup_local(receiver_name)) {
        if (local->getAllocatedType()->isPointerTy()) {
            receiver_address = builder.CreateLoad(local->getAllocatedType(), local, fmt::format("{}.ptr", receiver_name));
        } else {
            receiver_address = local;
        }
    } else {
        auto global_it = globals.find(receiver_name);
        if (global_it != globals.end()) {
            receiver_address = global_it->second;
        }
    }

    if (receiver_address == nullptr) {
        add_codegen_error(call_node, fmt::format("Unknown method receiver '{}'.", receiver_name));
        return nullptr;
    }

    std::vector<llvm::Value*> args;
    args.push_back(receiver_address);

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

    auto function_it = functions.find(method_it->second.lowered_name);
    if (function_it == functions.end()) {
        add_codegen_error(call_node, fmt::format("Lowered method '{}' was not declared.", method_it->second.lowered_name));
        return nullptr;
    }

    llvm::Function* callee = function_it->second;
    return builder.CreateCall(callee, args, callee->getReturnType()->isVoidTy() ? "" : "methodcalltmp");
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
    if (type_name == "uint64" || type_name == "int64" || type_name == "uintptr" || type_name == "usize" || type_name == "size_t") {
        return llvm::Type::getInt64Ty(context);
    }
    if (type_name == "float32" || type_name == "float") {
        return llvm::Type::getFloatTy(context);
    }
    if (type_name == "float64" || type_name == "double") {
        return llvm::Type::getDoubleTy(context);
    }
    auto struct_it = structs.find(type_name);
    if (struct_it != structs.end()) {
        return struct_it->second.type;
    }

    if (type_name == "string") {
        return string_type_for();
    }
    if (auto fixed_array = parse_fixed_array_type_name(type_name)) {
        llvm::Type* element_type = llvm_type_from_clyth_type(fixed_array->element_type_name);
        if (element_type == nullptr || fixed_array->length == 0) {
            return nullptr;
        }
        return llvm::ArrayType::get(element_type, fixed_array->length);
    }
    if (auto dynamic_element = parse_dynamic_array_element_type_name(type_name)) {
        // Modern process startup still treats string[] args specially as argv in
        // main lowering. Plain T[] arrays now use the compact runtime array
        // layout: { data, length }; growable containers such as List<T>,
        // Set<T>, and Map<K,V> use { data, length, capacity }.
        llvm::Type* element_type = llvm_type_from_clyth_type(*dynamic_element);
        if (element_type == nullptr) {
            return nullptr;
        }
        return dynamic_array_type_for(element_type, *dynamic_element, false);
    }

    if (auto list_element = parse_list_element_type_name(type_name)) {
        llvm::Type* element_type = llvm_type_from_clyth_type(*list_element);
        if (element_type == nullptr) {
            return nullptr;
        }
        return dynamic_array_type_for(element_type, *list_element, true);
    }

    if (auto set_element = parse_set_element_type_name(type_name)) {
        llvm::Type* element_type = llvm_type_from_clyth_type(*set_element);
        if (element_type == nullptr) {
            return nullptr;
        }
        return dynamic_array_type_for(element_type, *set_element, true);
    }

    if (auto map_info = parse_map_type_name(type_name)) {
        llvm::Type* key_type = llvm_type_from_clyth_type(map_info->key_type_name);
        llvm::Type* value_type = llvm_type_from_clyth_type(map_info->value_type_name);
        if (key_type == nullptr || value_type == nullptr) {
            return nullptr;
        }
        llvm::StructType* entry_type = map_entry_type_for(key_type, value_type, map_info->key_type_name, map_info->value_type_name);
        if (entry_type == nullptr) {
            return nullptr;
        }
        return dynamic_array_type_for(entry_type, "MapEntry<" + map_info->key_type_name + ":" + map_info->value_type_name + ">", true);
    }

    if (is_pointer_generic_type_name(type_name)) {
        return llvm::PointerType::get(context, 0);
    }

    if (type_name.find('[') != std::string::npos || type_name.find('<') != std::string::npos || type_name.find('(') != std::string::npos) {
        // Runtime containers and fixed arrays need layout/runtime lowering. Use
        // opaque pointers only when they appear in extern boundaries for now.
        return llvm::PointerType::get(context, 0);
    }

    return nullptr;
}


llvm::StructType* ClythLLVMCodegen::dynamic_array_type_for(llvm::Type* element_type, const std::string& element_type_name, bool has_capacity) {
    if (element_type == nullptr) {
        return nullptr;
    }

    const std::string type_symbol = "clyth.array." + sanitize_type_fragment_for_symbol(element_type_name);
    auto found = dynamic_array_types.find(type_symbol);
    if (found != dynamic_array_types.end()) {
        return found->second;
    }

    llvm::StructType* array_type = llvm::StructType::create(context, type_symbol);
    llvm::Type* ptr_type = llvm::PointerType::get(context, 0);
    llvm::Type* i64 = llvm::Type::getInt64Ty(context);
    if (has_capacity) {
        array_type->setBody({ptr_type, i64, i64}, false);
    } else {
        array_type->setBody({ptr_type, i64}, false);
    }
    dynamic_array_types[type_symbol] = array_type;
    return array_type;
}

llvm::StructType* ClythLLVMCodegen::map_entry_type_for(llvm::Type* key_type, llvm::Type* value_type, const std::string& key_type_name, const std::string& value_type_name) {
    if (key_type == nullptr || value_type == nullptr) {
        return nullptr;
    }

    const std::string type_symbol = "clyth.map.entry." + sanitize_type_fragment_for_symbol(key_type_name) + "." + sanitize_type_fragment_for_symbol(value_type_name);
    auto found = map_entry_types.find(type_symbol);
    if (found != map_entry_types.end()) {
        return found->second;
    }

    llvm::StructType* entry_type = llvm::StructType::create(context, type_symbol);
    entry_type->setBody({key_type, value_type}, false);
    map_entry_types[type_symbol] = entry_type;
    return entry_type;
}


llvm::StructType* ClythLLVMCodegen::string_type_for() {
    if (native_string_type != nullptr) {
        return native_string_type;
    }

    native_string_type = llvm::StructType::create(context, "clyth.string");
    llvm::Type* ptr_type = llvm::PointerType::get(context, 0);
    llvm::Type* i64 = llvm::Type::getInt64Ty(context);
    native_string_type->setBody({ptr_type, i64, i64}, false);
    return native_string_type;
}

bool ClythLLVMCodegen::emit_string_literal_initializer(const std::string& name, const ast::NodePtr& literal_node) {
    auto info = lookup_local_string(name);
    if (!info || info->alloca == nullptr) {
        add_codegen_error(literal_node, fmt::format("Internal codegen error: '{}' is not a string.", name));
        return false;
    }

    const std::string literal = attr(literal_node, "literal").value_or(literal_node ? literal_node->text : "");
    if (literal.empty() || literal.front() != '"') {
        add_codegen_error(literal_node, "String initializer must be a string literal in this backend pass.");
        return false;
    }

    const std::string unescaped = unescape_clyth_string_literal(literal);
    llvm::Value* data = builder.CreateGlobalStringPtr(unescaped, name + ".literal");
    llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* data_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* len_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 1);
    llvm::Value* cap_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 2);
    llvm::Type* i64 = llvm::Type::getInt64Ty(context);
    llvm::StructType* string_type = string_type_for();

    llvm::Value* data_addr = builder.CreateInBoundsGEP(string_type, info->alloca, {zero, data_idx}, name + ".data.addr");
    llvm::Value* len_addr = builder.CreateInBoundsGEP(string_type, info->alloca, {zero, len_idx}, name + ".length.addr");
    llvm::Value* cap_addr = builder.CreateInBoundsGEP(string_type, info->alloca, {zero, cap_idx}, name + ".capacity.addr");
    builder.CreateStore(data, data_addr);
    builder.CreateStore(llvm::ConstantInt::get(i64, unescaped.size()), len_addr);
    builder.CreateStore(llvm::ConstantInt::get(i64, unescaped.size()), cap_addr);
    return true;
}

llvm::Value* ClythLLVMCodegen::emit_string_data_pointer(llvm::Value* string_value, const std::string& name_hint) {
    if (string_value == nullptr) {
        return nullptr;
    }
    if (string_value->getType()->isPointerTy()) {
        return string_value;
    }
    if (string_value->getType() == string_type_for()) {
        return builder.CreateExtractValue(string_value, {0}, name_hint);
    }
    return string_value;
}

llvm::Value* ClythLLVMCodegen::emit_string_field_load(const LocalStringInfo& info, std::uint32_t field_index, const std::string& name_hint) {
    llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), field_index);
    llvm::Value* address = builder.CreateInBoundsGEP(string_type_for(), info.alloca, {zero, idx}, name_hint + ".addr");
    llvm::Type* field_type = field_index == 0 ? static_cast<llvm::Type*>(llvm::PointerType::get(context, 0)) : static_cast<llvm::Type*>(llvm::Type::getInt64Ty(context));
    return builder.CreateLoad(field_type, address, name_hint);
}

llvm::Value* ClythLLVMCodegen::emit_string_index_address(
    const std::string& name,
    const ast::NodePtr& index_node,
    const semantic::SemanticResult& semantics,
    llvm::Type** out_type
) {
    auto info = lookup_local_string(name);
    if (!info || info->alloca == nullptr) {
        return nullptr;
    }

    llvm::Value* index_value = emit_expression(first_expression_like_child(index_node), semantics);
    if (index_value == nullptr) {
        return nullptr;
    }
    if (!index_value->getType()->isIntegerTy(64)) {
        index_value = builder.CreateIntCast(index_value, llvm::Type::getInt64Ty(context), true, "stringidxcast");
    }

    llvm::Value* data_ptr = emit_string_field_load(*info, 0, name + ".data");
    if (out_type != nullptr) {
        *out_type = llvm::Type::getInt8Ty(context);
    }
    return builder.CreateGEP(llvm::Type::getInt8Ty(context), data_ptr, index_value, name + ".char.addr");
}

llvm::Function* ClythLLVMCodegen::declare_libc_malloc() {
    if (llvm::Function* existing = module->getFunction("malloc")) {
        return existing;
    }
    llvm::Type* ptr_type = llvm::PointerType::get(context, 0);
    llvm::Type* i64 = llvm::Type::getInt64Ty(context);
    llvm::FunctionType* malloc_type = llvm::FunctionType::get(ptr_type, {i64}, false);
    return llvm::Function::Create(malloc_type, llvm::Function::ExternalLinkage, "malloc", module.get());
}

llvm::Function* ClythLLVMCodegen::declare_libc_realloc() {
    if (llvm::Function* existing = module->getFunction("realloc")) {
        return existing;
    }
    llvm::Type* ptr_type = llvm::PointerType::get(context, 0);
    llvm::Type* i64 = llvm::Type::getInt64Ty(context);
    llvm::FunctionType* realloc_type = llvm::FunctionType::get(ptr_type, {ptr_type, i64}, false);
    return llvm::Function::Create(realloc_type, llvm::Function::ExternalLinkage, "realloc", module.get());
}

llvm::Value* ClythLLVMCodegen::emit_dynamic_array_length(const LocalDynamicArrayInfo& info) {
    llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* one = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 1);
    llvm::Value* address = builder.CreateInBoundsGEP(info.array_type, info.alloca, {zero, one}, "dynarray.length.addr");
    return builder.CreateLoad(llvm::Type::getInt64Ty(context), address, "dynarray.length");
}

bool ClythLLVMCodegen::dynamic_array_has_capacity(const LocalDynamicArrayInfo& info) const {
    return info.has_capacity;
}

llvm::Value* ClythLLVMCodegen::emit_dynamic_array_capacity(const LocalDynamicArrayInfo& info) {
    if (!dynamic_array_has_capacity(info)) {
        return nullptr;
    }
    llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* two = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 2);
    llvm::Value* address = builder.CreateInBoundsGEP(info.array_type, info.alloca, {zero, two}, "dynarray.capacity.addr");
    return builder.CreateLoad(llvm::Type::getInt64Ty(context), address, "dynarray.capacity");
}

bool ClythLLVMCodegen::emit_dynamic_array_initializer(const std::string& name, const ast::NodePtr& list_node, const semantic::SemanticResult& semantics) {
    auto info = lookup_local_dynamic_array(name);
    if (!info || info->alloca == nullptr || info->element_type == nullptr || info->array_type == nullptr) {
        add_codegen_error(list_node, fmt::format("Internal codegen error: '{}' is not a dynamic array.", name));
        return false;
    }

    const auto elements = list_literal_elements(list_node);
    llvm::Type* i64 = llvm::Type::getInt64Ty(context);
    llvm::Value* zero32 = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* data_index = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* length_index = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 1);
    const std::uint64_t allocation_count = elements.empty() ? 0 : static_cast<std::uint64_t>(elements.size());
    llvm::Value* data_ptr = llvm::ConstantPointerNull::get(llvm::PointerType::get(context, 0));

    if (allocation_count > 0) {
        const std::uint64_t element_bytes = conservative_element_size_bytes(info->element_type);
        llvm::Value* byte_count = llvm::ConstantInt::get(i64, element_bytes * allocation_count);
        data_ptr = builder.CreateCall(declare_libc_malloc(), {byte_count}, name + ".data");
    }

    llvm::Value* data_address = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, data_index}, name + ".data.addr");
    llvm::Value* length_address = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, length_index}, name + ".length.addr");
    builder.CreateStore(data_ptr, data_address);
    builder.CreateStore(llvm::ConstantInt::get(i64, elements.size()), length_address);
    if (info->has_capacity) {
        llvm::Value* capacity_index = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 2);
        llvm::Value* capacity_address = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, capacity_index}, name + ".capacity.addr");
        builder.CreateStore(llvm::ConstantInt::get(i64, allocation_count), capacity_address);
    }

    for (std::size_t index = 0; index < elements.size(); ++index) {
        llvm::Value* index_value = llvm::ConstantInt::get(i64, index);
        llvm::Value* slot = builder.CreateGEP(info->element_type, data_ptr, index_value, fmt::format("{}.init{}", name, index));
        llvm::Value* value = emit_expression(elements[index], semantics);
        if (value == nullptr) {
            return false;
        }
        if (value->getType() != info->element_type && value->getType()->isIntegerTy() && info->element_type->isIntegerTy()) {
            value = builder.CreateIntCast(value, info->element_type, true, "dynarrayinitcast");
        }
        builder.CreateStore(value, slot);
    }

    return true;
}

llvm::Value* ClythLLVMCodegen::emit_dynamic_array_element_address(
    const std::string& name,
    const ast::NodePtr& index_node,
    const semantic::SemanticResult& semantics,
    llvm::Type** out_type
) {
    auto info = lookup_local_dynamic_array(name);
    if (!info || info->alloca == nullptr || info->element_type == nullptr || info->array_type == nullptr) {
        return nullptr;
    }

    llvm::Value* index_value = emit_expression(first_expression_like_child(index_node), semantics);
    if (index_value == nullptr) {
        return nullptr;
    }
    if (!index_value->getType()->isIntegerTy(64)) {
        index_value = builder.CreateIntCast(index_value, llvm::Type::getInt64Ty(context), true, "dynarrayidxcast");
    }

    llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* data_index = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* data_address = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero, data_index}, name + ".data.addr");
    llvm::Value* data_ptr = builder.CreateLoad(llvm::PointerType::get(context, 0), data_address, name + ".data");

    if (out_type != nullptr) {
        *out_type = info->element_type;
    }
    return builder.CreateGEP(info->element_type, data_ptr, index_value, name + ".element.addr");
}

bool ClythLLVMCodegen::emit_dynamic_array_push(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics) {
    auto info = lookup_local_dynamic_array(name);
    if (!info || info->alloca == nullptr || info->element_type == nullptr || info->array_type == nullptr || !info->has_capacity) {
        add_codegen_error(call_node, fmt::format("'{}' is not a growable List/Set container.", name));
        return false;
    }

    const auto args = call_arguments(call_node);
    if (args.size() != 1) {
        add_codegen_error(call_node, "dynamic array push(value) expects exactly one argument.");
        return false;
    }

    llvm::Type* i64 = llvm::Type::getInt64Ty(context);
    llvm::Type* ptr_type = llvm::PointerType::get(context, 0);
    llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* data_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* len_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 1);
    llvm::Value* cap_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 2);

    llvm::Value* data_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero, data_idx}, name + ".data.addr");
    llvm::Value* len_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero, len_idx}, name + ".length.addr");
    llvm::Value* cap_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero, cap_idx}, name + ".capacity.addr");
    llvm::Value* data_ptr = builder.CreateLoad(ptr_type, data_addr, name + ".data");
    llvm::Value* length = builder.CreateLoad(i64, len_addr, name + ".length");
    llvm::Value* capacity = builder.CreateLoad(i64, cap_addr, name + ".capacity");

    llvm::Function* function = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* grow_block = llvm::BasicBlock::Create(context, name + ".grow", function);
    llvm::BasicBlock* store_block = llvm::BasicBlock::Create(context, name + ".store", function);
    llvm::BasicBlock* pre_grow_block = builder.GetInsertBlock();
    llvm::Value* needs_grow = builder.CreateICmpUGE(length, capacity, name + ".needs_grow");
    builder.CreateCondBr(needs_grow, grow_block, store_block);

    builder.SetInsertPoint(grow_block);
    llvm::Value* is_zero = builder.CreateICmpEQ(capacity, llvm::ConstantInt::get(i64, 0), name + ".cap_zero");
    llvm::Value* doubled = builder.CreateMul(capacity, llvm::ConstantInt::get(i64, 2), name + ".cap_double");
    llvm::Value* new_capacity = builder.CreateSelect(is_zero, llvm::ConstantInt::get(i64, 4), doubled, name + ".new_capacity");
    llvm::Value* byte_count = builder.CreateMul(new_capacity, llvm::ConstantInt::get(i64, conservative_element_size_bytes(info->element_type)), name + ".new_bytes");
    llvm::Value* new_data = builder.CreateCall(declare_libc_realloc(), {data_ptr, byte_count}, name + ".new_data");
    builder.CreateStore(new_data, data_addr);
    builder.CreateStore(new_capacity, cap_addr);
    builder.CreateBr(store_block);

    builder.SetInsertPoint(store_block);
    llvm::PHINode* active_data = builder.CreatePHI(ptr_type, 2, name + ".active_data");
    active_data->addIncoming(data_ptr, pre_grow_block);
    active_data->addIncoming(new_data, grow_block);

    llvm::Value* slot = builder.CreateGEP(info->element_type, active_data, length, name + ".push.slot");
    llvm::Value* value = emit_expression(args.front(), semantics);
    if (value == nullptr) {
        return false;
    }
    if (value->getType() != info->element_type && value->getType()->isIntegerTy() && info->element_type->isIntegerTy()) {
        value = builder.CreateIntCast(value, info->element_type, true, "dynarraypushcast");
    }
    builder.CreateStore(value, slot);
    builder.CreateStore(builder.CreateAdd(length, llvm::ConstantInt::get(i64, 1), name + ".new_length"), len_addr);
    return true;
}

llvm::Value* ClythLLVMCodegen::emit_dynamic_array_pop(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics) {
    (void)semantics;
    auto info = lookup_local_dynamic_array(name);
    if (!info || info->alloca == nullptr || info->element_type == nullptr || info->array_type == nullptr || !info->has_capacity) {
        add_codegen_error(call_node, fmt::format("'{}' is not a growable List/Set container.", name));
        return nullptr;
    }
    llvm::Type* i64 = llvm::Type::getInt64Ty(context);
    llvm::Value* zero32 = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* data_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* len_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 1);
    llvm::Value* data_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, data_idx}, name + ".data.addr");
    llvm::Value* len_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, len_idx}, name + ".length.addr");
    llvm::Value* data_ptr = builder.CreateLoad(llvm::PointerType::get(context, 0), data_addr, name + ".data");
    llvm::Value* length = builder.CreateLoad(i64, len_addr, name + ".length");
    llvm::Value* new_length = builder.CreateSub(length, llvm::ConstantInt::get(i64, 1), name + ".pop_length");
    builder.CreateStore(new_length, len_addr);
    llvm::Value* slot = builder.CreateGEP(info->element_type, data_ptr, new_length, name + ".pop.slot");
    return builder.CreateLoad(info->element_type, slot, name + ".pop.value");
}

llvm::Value* ClythLLVMCodegen::emit_dynamic_array_contains(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics) {
    auto info = lookup_local_dynamic_array(name);
    if (!info || info->alloca == nullptr || info->element_type == nullptr || info->array_type == nullptr) {
        add_codegen_error(call_node, fmt::format("'{}' is not a dynamic array/List/Set.", name));
        return nullptr;
    }

    const auto args = call_arguments(call_node);
    if (args.size() != 1) {
        add_codegen_error(call_node, "contains(value) expects exactly one argument.");
        return nullptr;
    }

    llvm::Value* needle_value = emit_expression(args.front(), semantics);
    if (needle_value == nullptr) {
        return nullptr;
    }
    if (needle_value->getType() != info->element_type && needle_value->getType()->isIntegerTy() && info->element_type->isIntegerTy()) {
        needle_value = builder.CreateIntCast(needle_value, info->element_type, true, "containscast");
    }
    if (needle_value->getType() != info->element_type) {
        add_codegen_error(call_node, "contains(value) currently requires a value matching the container element type.");
        return nullptr;
    }

    llvm::Function* function = builder.GetInsertBlock()->getParent();
    llvm::Type* i1 = llvm::Type::getInt1Ty(context);
    llvm::Type* i64 = llvm::Type::getInt64Ty(context);
    llvm::Value* zero32 = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* data_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* len_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 1);

    llvm::AllocaInst* result_slot = create_entry_alloca(function, i1, name + ".contains.result");
    builder.CreateStore(llvm::ConstantInt::getFalse(context), result_slot);

    llvm::Value* data_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, data_idx}, name + ".data.addr");
    llvm::Value* len_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, len_idx}, name + ".length.addr");
    llvm::Value* data_ptr = builder.CreateLoad(llvm::PointerType::get(context, 0), data_addr, name + ".data");
    llvm::Value* length = builder.CreateLoad(i64, len_addr, name + ".length");

    llvm::BasicBlock* preheader = builder.GetInsertBlock();
    llvm::BasicBlock* loop_block = llvm::BasicBlock::Create(context, name + ".contains.loop", function);
    llvm::BasicBlock* body_block = llvm::BasicBlock::Create(context, name + ".contains.body", function);
    llvm::BasicBlock* next_block = llvm::BasicBlock::Create(context, name + ".contains.next", function);
    llvm::BasicBlock* found_block = llvm::BasicBlock::Create(context, name + ".contains.found", function);
    llvm::BasicBlock* exit_block = llvm::BasicBlock::Create(context, name + ".contains.exit", function);

    builder.CreateBr(loop_block);

    builder.SetInsertPoint(loop_block);
    llvm::PHINode* index = builder.CreatePHI(i64, 2, name + ".contains.i");
    index->addIncoming(llvm::ConstantInt::get(i64, 0), preheader);
    llvm::Value* in_bounds = builder.CreateICmpULT(index, length, name + ".contains.in_bounds");
    builder.CreateCondBr(in_bounds, body_block, exit_block);

    builder.SetInsertPoint(body_block);
    llvm::Value* slot = builder.CreateGEP(info->element_type, data_ptr, index, name + ".contains.slot");
    llvm::Value* item = builder.CreateLoad(info->element_type, slot, name + ".contains.item");
    llvm::Value* matches = nullptr;
    if (info->element_type->isIntegerTy()) {
        matches = builder.CreateICmpEQ(item, needle_value, name + ".contains.match");
    } else if (info->element_type->isFloatingPointTy()) {
        matches = builder.CreateFCmpOEQ(item, needle_value, name + ".contains.match");
    } else if (info->element_type->isPointerTy()) {
        matches = builder.CreateICmpEQ(item, needle_value, name + ".contains.match");
    } else {
        add_codegen_error(call_node, "contains(value) currently supports integer, float, and pointer-like element types.");
        return nullptr;
    }
    builder.CreateCondBr(matches, found_block, next_block);

    builder.SetInsertPoint(found_block);
    builder.CreateStore(llvm::ConstantInt::getTrue(context), result_slot);
    builder.CreateBr(exit_block);

    builder.SetInsertPoint(next_block);
    llvm::Value* next_index = builder.CreateAdd(index, llvm::ConstantInt::get(i64, 1), name + ".contains.next_i");
    index->addIncoming(next_index, next_block);
    builder.CreateBr(loop_block);

    builder.SetInsertPoint(exit_block);
    return builder.CreateLoad(i1, result_slot, name + ".contains.value");
}

bool ClythLLVMCodegen::emit_set_insert(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics) {
    llvm::Value* already_present = emit_dynamic_array_contains(name, call_node, semantics);
    if (already_present == nullptr) {
        return false;
    }

    llvm::Function* function = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* insert_block = llvm::BasicBlock::Create(context, name + ".set.insert", function);
    llvm::BasicBlock* done_block = llvm::BasicBlock::Create(context, name + ".set.done", function);
    builder.CreateCondBr(already_present, done_block, insert_block);

    builder.SetInsertPoint(insert_block);
    if (!emit_dynamic_array_push(name, call_node, semantics)) {
        return false;
    }
    builder.CreateBr(done_block);

    builder.SetInsertPoint(done_block);
    return true;
}

llvm::Value* emit_key_match(llvm::IRBuilder<>& builder, llvm::Value* lhs, llvm::Value* rhs, const std::string& name_hint) {
    if (lhs == nullptr || rhs == nullptr || lhs->getType() != rhs->getType()) {
        return nullptr;
    }
    llvm::Type* type = lhs->getType();
    if (type->isIntegerTy() || type->isPointerTy()) {
        return builder.CreateICmpEQ(lhs, rhs, name_hint + ".match");
    }
    if (type->isFloatingPointTy()) {
        return builder.CreateFCmpOEQ(lhs, rhs, name_hint + ".match");
    }
    return nullptr;
}

bool ClythLLVMCodegen::emit_map_initializer(const std::string& name, const ast::NodePtr& keyed_literal_node, const semantic::SemanticResult& semantics) {
    const auto entries = keyed_literal_entries(keyed_literal_node);
    if (entries.empty()) {
        return true;
    }

    for (const auto& entry : entries) {
        if (attr(entry, "entry_kind") != "pair") {
            add_codegen_error(entry, "Map keyed-array initializer requires key: value entries only.");
            return false;
        }
        auto [key_node, value_node] = keyed_entry_expressions(entry);
        if (!key_node || !value_node) {
            add_codegen_error(entry, "Malformed keyed-array entry for Map initializer.");
            return false;
        }
        if (!emit_map_put_values(name, entry, key_node, value_node, semantics)) {
            return false;
        }
    }

    return true;
}

bool ClythLLVMCodegen::emit_map_put(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics) {
    const auto args = call_arguments(call_node);
    if (args.size() != 2) {
        add_codegen_error(call_node, "put(key, value) expects exactly two arguments.");
        return false;
    }
    return emit_map_put_values(name, call_node, args[0], args[1], semantics);
}

bool ClythLLVMCodegen::emit_map_put_values(
    const std::string& name,
    const ast::NodePtr& diagnostic_node,
    const ast::NodePtr& key_node,
    const ast::NodePtr& value_node,
    const semantic::SemanticResult& semantics
) {
    auto info = lookup_local_map(name);
    if (!info || info->alloca == nullptr || info->entry_type == nullptr || info->array_type == nullptr) {
        add_codegen_error(diagnostic_node, fmt::format("'{}' is not a Map<K,V>.", name));
        return false;
    }

    llvm::Value* key_value = emit_expression(key_node, semantics);
    llvm::Value* stored_value = emit_expression(value_node, semantics);
    if (key_value == nullptr || stored_value == nullptr) {
        return false;
    }
    if (key_value->getType() != info->key_type && key_value->getType()->isIntegerTy() && info->key_type->isIntegerTy()) {
        key_value = builder.CreateIntCast(key_value, info->key_type, true, "mapkeycast");
    }
    if (stored_value->getType() != info->value_type && stored_value->getType()->isIntegerTy() && info->value_type->isIntegerTy()) {
        stored_value = builder.CreateIntCast(stored_value, info->value_type, true, "mapvaluecast");
    }
    if (key_value->getType() != info->key_type || stored_value->getType() != info->value_type) {
        add_codegen_error(diagnostic_node, "Map key/value entries must match the declared Map<K,V> types in this backend pass.");
        return false;
    }

    llvm::Function* function = builder.GetInsertBlock()->getParent();
    llvm::Type* i64 = llvm::Type::getInt64Ty(context);
    llvm::Type* ptr_type = llvm::PointerType::get(context, 0);
    llvm::Value* zero32 = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* data_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* len_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 1);
    llvm::Value* cap_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 2);

    llvm::Value* data_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, data_idx}, name + ".data.addr");
    llvm::Value* len_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, len_idx}, name + ".length.addr");
    llvm::Value* cap_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, cap_idx}, name + ".capacity.addr");
    llvm::Value* data_ptr = builder.CreateLoad(ptr_type, data_addr, name + ".data");
    llvm::Value* length = builder.CreateLoad(i64, len_addr, name + ".length");
    llvm::Value* capacity = builder.CreateLoad(i64, cap_addr, name + ".capacity");

    llvm::BasicBlock* preheader = builder.GetInsertBlock();
    llvm::BasicBlock* loop_block = llvm::BasicBlock::Create(context, name + ".map.put.loop", function);
    llvm::BasicBlock* body_block = llvm::BasicBlock::Create(context, name + ".map.put.body", function);
    llvm::BasicBlock* next_block = llvm::BasicBlock::Create(context, name + ".map.put.next", function);
    llvm::BasicBlock* update_block = llvm::BasicBlock::Create(context, name + ".map.put.update", function);
    llvm::BasicBlock* append_check_block = llvm::BasicBlock::Create(context, name + ".map.put.append_check", function);
    llvm::BasicBlock* grow_block = llvm::BasicBlock::Create(context, name + ".map.put.grow", function);
    llvm::BasicBlock* append_block = llvm::BasicBlock::Create(context, name + ".map.put.append", function);
    llvm::BasicBlock* done_block = llvm::BasicBlock::Create(context, name + ".map.put.done", function);

    builder.CreateBr(loop_block);

    builder.SetInsertPoint(loop_block);
    llvm::PHINode* index = builder.CreatePHI(i64, 2, name + ".map.put.i");
    index->addIncoming(llvm::ConstantInt::get(i64, 0), preheader);
    llvm::Value* in_bounds = builder.CreateICmpULT(index, length, name + ".map.put.in_bounds");
    builder.CreateCondBr(in_bounds, body_block, append_check_block);

    builder.SetInsertPoint(body_block);
    llvm::Value* entry_ptr = builder.CreateGEP(info->entry_type, data_ptr, index, name + ".map.put.entry");
    llvm::Value* key_addr = builder.CreateStructGEP(info->entry_type, entry_ptr, 0, name + ".map.put.key.addr");
    llvm::Value* existing_key = builder.CreateLoad(info->key_type, key_addr, name + ".map.put.key");
    llvm::Value* matches = emit_key_match(builder, existing_key, key_value, name + ".map.put");
    if (matches == nullptr) {
        add_codegen_error(diagnostic_node, "Map key comparison currently supports integer, float, and pointer-like keys.");
        return false;
    }
    builder.CreateCondBr(matches, update_block, next_block);

    builder.SetInsertPoint(update_block);
    llvm::Value* update_value_addr = builder.CreateStructGEP(info->entry_type, entry_ptr, 1, name + ".map.put.value.addr");
    builder.CreateStore(stored_value, update_value_addr);
    builder.CreateBr(done_block);

    builder.SetInsertPoint(next_block);
    llvm::Value* next_index = builder.CreateAdd(index, llvm::ConstantInt::get(i64, 1), name + ".map.put.next_i");
    index->addIncoming(next_index, next_block);
    builder.CreateBr(loop_block);

    builder.SetInsertPoint(append_check_block);
    llvm::Value* needs_grow = builder.CreateICmpUGE(length, capacity, name + ".map.put.needs_grow");
    builder.CreateCondBr(needs_grow, grow_block, append_block);

    builder.SetInsertPoint(grow_block);
    llvm::Value* is_zero = builder.CreateICmpEQ(capacity, llvm::ConstantInt::get(i64, 0), name + ".map.put.cap_zero");
    llvm::Value* doubled = builder.CreateMul(capacity, llvm::ConstantInt::get(i64, 2), name + ".map.put.cap_double");
    llvm::Value* new_capacity = builder.CreateSelect(is_zero, llvm::ConstantInt::get(i64, 4), doubled, name + ".map.put.new_capacity");
    llvm::Value* byte_count = builder.CreateMul(new_capacity, llvm::ConstantInt::get(i64, conservative_element_size_bytes(info->entry_type)), name + ".map.put.new_bytes");
    llvm::Value* new_data = builder.CreateCall(declare_libc_realloc(), {data_ptr, byte_count}, name + ".map.put.new_data");
    builder.CreateStore(new_data, data_addr);
    builder.CreateStore(new_capacity, cap_addr);
    builder.CreateBr(append_block);

    builder.SetInsertPoint(append_block);
    llvm::PHINode* active_data = builder.CreatePHI(ptr_type, 2, name + ".map.put.active_data");
    active_data->addIncoming(data_ptr, append_check_block);
    active_data->addIncoming(new_data, grow_block);
    llvm::Value* append_entry_ptr = builder.CreateGEP(info->entry_type, active_data, length, name + ".map.put.append.entry");
    llvm::Value* append_key_addr = builder.CreateStructGEP(info->entry_type, append_entry_ptr, 0, name + ".map.put.append.key.addr");
    llvm::Value* append_value_addr = builder.CreateStructGEP(info->entry_type, append_entry_ptr, 1, name + ".map.put.append.value.addr");
    builder.CreateStore(key_value, append_key_addr);
    builder.CreateStore(stored_value, append_value_addr);
    builder.CreateStore(builder.CreateAdd(length, llvm::ConstantInt::get(i64, 1), name + ".map.put.new_length"), len_addr);
    builder.CreateBr(done_block);

    builder.SetInsertPoint(done_block);
    return true;
}

llvm::Value* ClythLLVMCodegen::emit_map_get(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics) {
    auto info = lookup_local_map(name);
    if (!info || info->alloca == nullptr || info->entry_type == nullptr || info->array_type == nullptr) {
        add_codegen_error(call_node, fmt::format("'{}' is not a Map<K,V>.", name));
        return nullptr;
    }

    const auto args = call_arguments(call_node);
    if (args.size() != 1) {
        add_codegen_error(call_node, "get(key) expects exactly one argument.");
        return nullptr;
    }

    llvm::Value* key_value = emit_expression(args.front(), semantics);
    if (key_value == nullptr) {
        return nullptr;
    }
    if (key_value->getType() != info->key_type && key_value->getType()->isIntegerTy() && info->key_type->isIntegerTy()) {
        key_value = builder.CreateIntCast(key_value, info->key_type, true, "mapgetkeycast");
    }
    if (key_value->getType() != info->key_type) {
        add_codegen_error(call_node, "get(key) requires a key matching the Map<K,V> key type in this backend pass.");
        return nullptr;
    }

    llvm::Function* function = builder.GetInsertBlock()->getParent();
    llvm::Type* i64 = llvm::Type::getInt64Ty(context);
    llvm::Value* zero32 = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* data_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* len_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 1);

    llvm::AllocaInst* result_slot = create_entry_alloca(function, info->value_type, name + ".map.get.result");
    builder.CreateStore(llvm::Constant::getNullValue(info->value_type), result_slot);

    llvm::Value* data_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, data_idx}, name + ".data.addr");
    llvm::Value* len_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, len_idx}, name + ".length.addr");
    llvm::Value* data_ptr = builder.CreateLoad(llvm::PointerType::get(context, 0), data_addr, name + ".data");
    llvm::Value* length = builder.CreateLoad(i64, len_addr, name + ".length");

    llvm::BasicBlock* preheader = builder.GetInsertBlock();
    llvm::BasicBlock* loop_block = llvm::BasicBlock::Create(context, name + ".map.get.loop", function);
    llvm::BasicBlock* body_block = llvm::BasicBlock::Create(context, name + ".map.get.body", function);
    llvm::BasicBlock* next_block = llvm::BasicBlock::Create(context, name + ".map.get.next", function);
    llvm::BasicBlock* found_block = llvm::BasicBlock::Create(context, name + ".map.get.found", function);
    llvm::BasicBlock* exit_block = llvm::BasicBlock::Create(context, name + ".map.get.exit", function);

    builder.CreateBr(loop_block);
    builder.SetInsertPoint(loop_block);
    llvm::PHINode* index = builder.CreatePHI(i64, 2, name + ".map.get.i");
    index->addIncoming(llvm::ConstantInt::get(i64, 0), preheader);
    llvm::Value* in_bounds = builder.CreateICmpULT(index, length, name + ".map.get.in_bounds");
    builder.CreateCondBr(in_bounds, body_block, exit_block);

    builder.SetInsertPoint(body_block);
    llvm::Value* entry_ptr = builder.CreateGEP(info->entry_type, data_ptr, index, name + ".map.get.entry");
    llvm::Value* key_addr = builder.CreateStructGEP(info->entry_type, entry_ptr, 0, name + ".map.get.key.addr");
    llvm::Value* existing_key = builder.CreateLoad(info->key_type, key_addr, name + ".map.get.key");
    llvm::Value* matches = emit_key_match(builder, existing_key, key_value, name + ".map.get");
    if (matches == nullptr) {
        add_codegen_error(call_node, "Map key comparison currently supports integer, float, and pointer-like keys.");
        return nullptr;
    }
    builder.CreateCondBr(matches, found_block, next_block);

    builder.SetInsertPoint(found_block);
    llvm::Value* value_addr = builder.CreateStructGEP(info->entry_type, entry_ptr, 1, name + ".map.get.value.addr");
    llvm::Value* found_value = builder.CreateLoad(info->value_type, value_addr, name + ".map.get.value");
    builder.CreateStore(found_value, result_slot);
    builder.CreateBr(exit_block);

    builder.SetInsertPoint(next_block);
    llvm::Value* next_index = builder.CreateAdd(index, llvm::ConstantInt::get(i64, 1), name + ".map.get.next_i");
    index->addIncoming(next_index, next_block);
    builder.CreateBr(loop_block);

    builder.SetInsertPoint(exit_block);
    return builder.CreateLoad(info->value_type, result_slot, name + ".map.get.result.value");
}

llvm::Value* ClythLLVMCodegen::emit_map_contains_key(const std::string& name, const ast::NodePtr& call_node, const semantic::SemanticResult& semantics) {
    auto info = lookup_local_map(name);
    if (!info || info->alloca == nullptr || info->entry_type == nullptr || info->array_type == nullptr) {
        add_codegen_error(call_node, fmt::format("'{}' is not a Map<K,V>.", name));
        return nullptr;
    }

    const auto args = call_arguments(call_node);
    if (args.size() != 1) {
        add_codegen_error(call_node, "contains_key(key) expects exactly one argument.");
        return nullptr;
    }

    llvm::Value* key_value = emit_expression(args.front(), semantics);
    if (key_value == nullptr) {
        return nullptr;
    }
    if (key_value->getType() != info->key_type && key_value->getType()->isIntegerTy() && info->key_type->isIntegerTy()) {
        key_value = builder.CreateIntCast(key_value, info->key_type, true, "mapcontainskeycast");
    }
    if (key_value->getType() != info->key_type) {
        add_codegen_error(call_node, "contains_key(key) requires a key matching the Map<K,V> key type in this backend pass.");
        return nullptr;
    }

    llvm::Function* function = builder.GetInsertBlock()->getParent();
    llvm::Type* i1 = llvm::Type::getInt1Ty(context);
    llvm::Type* i64 = llvm::Type::getInt64Ty(context);
    llvm::Value* zero32 = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* data_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    llvm::Value* len_idx = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 1);

    llvm::AllocaInst* result_slot = create_entry_alloca(function, i1, name + ".map.contains_key.result");
    builder.CreateStore(llvm::ConstantInt::getFalse(context), result_slot);

    llvm::Value* data_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, data_idx}, name + ".data.addr");
    llvm::Value* len_addr = builder.CreateInBoundsGEP(info->array_type, info->alloca, {zero32, len_idx}, name + ".length.addr");
    llvm::Value* data_ptr = builder.CreateLoad(llvm::PointerType::get(context, 0), data_addr, name + ".data");
    llvm::Value* length = builder.CreateLoad(i64, len_addr, name + ".length");

    llvm::BasicBlock* preheader = builder.GetInsertBlock();
    llvm::BasicBlock* loop_block = llvm::BasicBlock::Create(context, name + ".map.contains_key.loop", function);
    llvm::BasicBlock* body_block = llvm::BasicBlock::Create(context, name + ".map.contains_key.body", function);
    llvm::BasicBlock* next_block = llvm::BasicBlock::Create(context, name + ".map.contains_key.next", function);
    llvm::BasicBlock* found_block = llvm::BasicBlock::Create(context, name + ".map.contains_key.found", function);
    llvm::BasicBlock* exit_block = llvm::BasicBlock::Create(context, name + ".map.contains_key.exit", function);

    builder.CreateBr(loop_block);
    builder.SetInsertPoint(loop_block);
    llvm::PHINode* index = builder.CreatePHI(i64, 2, name + ".map.contains_key.i");
    index->addIncoming(llvm::ConstantInt::get(i64, 0), preheader);
    llvm::Value* in_bounds = builder.CreateICmpULT(index, length, name + ".map.contains_key.in_bounds");
    builder.CreateCondBr(in_bounds, body_block, exit_block);

    builder.SetInsertPoint(body_block);
    llvm::Value* entry_ptr = builder.CreateGEP(info->entry_type, data_ptr, index, name + ".map.contains_key.entry");
    llvm::Value* key_addr = builder.CreateStructGEP(info->entry_type, entry_ptr, 0, name + ".map.contains_key.key.addr");
    llvm::Value* existing_key = builder.CreateLoad(info->key_type, key_addr, name + ".map.contains_key.key");
    llvm::Value* matches = emit_key_match(builder, existing_key, key_value, name + ".map.contains_key");
    if (matches == nullptr) {
        add_codegen_error(call_node, "Map key comparison currently supports integer, float, and pointer-like keys.");
        return nullptr;
    }
    builder.CreateCondBr(matches, found_block, next_block);

    builder.SetInsertPoint(found_block);
    builder.CreateStore(llvm::ConstantInt::getTrue(context), result_slot);
    builder.CreateBr(exit_block);

    builder.SetInsertPoint(next_block);
    llvm::Value* next_index = builder.CreateAdd(index, llvm::ConstantInt::get(i64, 1), name + ".map.contains_key.next_i");
    index->addIncoming(next_index, next_block);
    builder.CreateBr(loop_block);

    builder.SetInsertPoint(exit_block);
    return builder.CreateLoad(i1, result_slot, name + ".map.contains_key.value");
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
    current_scope->local_type_scopes.emplace_back();
    current_scope->local_array_scopes.emplace_back();
    current_scope->local_dynamic_array_scopes.emplace_back();
    current_scope->local_string_scopes.emplace_back();
    current_scope->local_map_scopes.emplace_back();
}

void ClythLLVMCodegen::pop_local_scope() {
    if (!current_scope || current_scope->local_scopes.empty()) {
        return;
    }

    current_scope->local_scopes.pop_back();
    if (!current_scope->local_type_scopes.empty()) {
        current_scope->local_type_scopes.pop_back();
    }
    if (!current_scope->local_array_scopes.empty()) {
        current_scope->local_array_scopes.pop_back();
    }
    if (!current_scope->local_dynamic_array_scopes.empty()) {
        current_scope->local_dynamic_array_scopes.pop_back();
    }
    if (!current_scope->local_string_scopes.empty()) {
        current_scope->local_string_scopes.pop_back();
    }
    if (!current_scope->local_map_scopes.empty()) {
        current_scope->local_map_scopes.pop_back();
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



bool ClythLLVMCodegen::register_local_type(const std::string& name, const std::string& type_name) {
    if (!current_scope || current_scope->local_type_scopes.empty()) {
        diagnostics.add_error(SourceLocation{}, fmt::format("Internal codegen error: no local type scope exists while registering '{}'.", name));
        return false;
    }

    current_scope->local_type_scopes.back()[name] = type_name;
    return true;
}

std::optional<std::string> ClythLLVMCodegen::lookup_symbol_type_name(const std::string& name) const {
    if (current_scope) {
        for (auto it = current_scope->local_type_scopes.rbegin(); it != current_scope->local_type_scopes.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return found->second;
            }
        }
    }

    auto global_it = global_type_names.find(name);
    if (global_it != global_type_names.end()) {
        return global_it->second;
    }

    return std::nullopt;
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

bool ClythLLVMCodegen::register_local_dynamic_array(const std::string& name, const LocalDynamicArrayInfo& info) {
    if (!current_scope || current_scope->local_dynamic_array_scopes.empty()) {
        diagnostics.add_error(SourceLocation{}, fmt::format("Internal codegen error: no dynamic array scope exists while registering '{}'.", name));
        return false;
    }

    current_scope->local_dynamic_array_scopes.back()[name] = info;
    return true;
}

std::optional<ClythLLVMCodegen::LocalDynamicArrayInfo> ClythLLVMCodegen::lookup_local_dynamic_array(const std::string& name) const {
    if (!current_scope) {
        return std::nullopt;
    }

    for (auto it = current_scope->local_dynamic_array_scopes.rbegin(); it != current_scope->local_dynamic_array_scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second;
        }
    }

    return std::nullopt;
}

bool ClythLLVMCodegen::register_local_string(const std::string& name, const LocalStringInfo& info) {
    if (!current_scope || current_scope->local_string_scopes.empty()) {
        diagnostics.add_error(SourceLocation{}, fmt::format("Internal codegen error: no string scope exists while registering '{}'.", name));
        return false;
    }

    current_scope->local_string_scopes.back()[name] = info;
    return true;
}

std::optional<ClythLLVMCodegen::LocalStringInfo> ClythLLVMCodegen::lookup_local_string(const std::string& name) const {
    if (!current_scope) {
        return std::nullopt;
    }

    for (auto it = current_scope->local_string_scopes.rbegin(); it != current_scope->local_string_scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second;
        }
    }

    return std::nullopt;
}

bool ClythLLVMCodegen::register_local_map(const std::string& name, const LocalMapInfo& info) {
    if (!current_scope || current_scope->local_map_scopes.empty()) {
        diagnostics.add_error(SourceLocation{}, fmt::format("Internal codegen error: no map scope exists while registering '{}'.", name));
        return false;
    }

    current_scope->local_map_scopes.back()[name] = info;
    return true;
}

std::optional<ClythLLVMCodegen::LocalMapInfo> ClythLLVMCodegen::lookup_local_map(const std::string& name) const {
    if (!current_scope) {
        return std::nullopt;
    }

    for (auto it = current_scope->local_map_scopes.rbegin(); it != current_scope->local_map_scopes.rend(); ++it) {
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
    for (const auto& link_input : config.extra_link_inputs) {
        command << " " << shell_quote(link_input.string());
    }
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
