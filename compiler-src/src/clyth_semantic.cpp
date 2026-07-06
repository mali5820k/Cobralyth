#include "clyth_semantic.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace clyth::semantic {

namespace {

const std::unordered_set<std::string>& builtin_type_names() {
    static const std::unordered_set<std::string> types {
        "uint8", "uint16", "uint32", "uint64", "uint",
        "uintptr", "usize", "size_t",
        "int8", "int16", "int32", "int64", "int",
        "numeric",
        "float32", "float64", "float", "double",
        "string", "char", "bool", "void", "auto"
    };

    return types;
}

bool is_builtin_type_name(const std::string& name) {
    return builtin_type_names().count(name) > 0;
}


std::vector<std::string> split_function_type_parameters(const std::string& text) {
    std::vector<std::string> result;
    std::string current;
    int angle_depth = 0;
    int paren_depth = 0;

    for (char ch : text) {
        if (ch == '<') {
            ++angle_depth;
            current.push_back(ch);
            continue;
        }
        if (ch == '>') {
            --angle_depth;
            current.push_back(ch);
            continue;
        }
        if (ch == '(') {
            ++paren_depth;
            current.push_back(ch);
            continue;
        }
        if (ch == ')') {
            --paren_depth;
            current.push_back(ch);
            continue;
        }
        if ((ch == ',' || ch == ':') && angle_depth == 0 && paren_depth == 0) {
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

struct FunctionTypeParts {
    std::string return_type_name;
    std::vector<std::string> parameter_type_names;
};

std::optional<FunctionTypeParts> parse_function_type_text(const std::string& type_name) {
    const std::string prefix = "function<";
    if (type_name.rfind(prefix, 0) != 0 || type_name.empty() || type_name.back() != '>') {
        return std::nullopt;
    }

    const std::string inner = type_name.substr(prefix.size(), type_name.size() - prefix.size() - 1);
    if (inner.empty()) {
        return std::nullopt;
    }

    auto trim = [](std::string value) {
        auto not_space = [](unsigned char ch) { return !std::isspace(ch); };
        value.erase(value.begin(), std::find_if(value.begin(), value.end(), not_space));
        value.erase(std::find_if(value.rbegin(), value.rend(), not_space).base(), value.end());
        return value;
    };

    int angle_depth = 0;
    std::size_t open_paren = std::string::npos;
    for (std::size_t i = 0; i < inner.size(); ++i) {
        const char ch = inner[i];
        if (ch == '<') {
            ++angle_depth;
            continue;
        }
        if (ch == '>') {
            --angle_depth;
            continue;
        }
        if (ch == '(' && angle_depth == 0) {
            open_paren = i;
            break;
        }
    }

    if (open_paren == std::string::npos || inner.back() != ')') {
        return std::nullopt;
    }

    FunctionTypeParts result;
    result.return_type_name = trim(inner.substr(0, open_paren));
    const std::string params = inner.substr(open_paren + 1, inner.size() - open_paren - 2);

    if (result.return_type_name.empty()) {
        return std::nullopt;
    }

    if (!trim(params).empty()) {
        result.parameter_type_names = split_function_type_parameters(params);
    }

    return result;
}


bool looks_like_keyword(const std::string& text) {
    static const std::unordered_set<std::string> keywords {
        "include", "extern", "C", "struct", "mecc",
        "if", "else", "for", "while", "return", "break", "continue",
        "is", "in", "not",
        "true", "false", "null", "constructor", "destructor"
    };

    return keywords.count(text) > 0;
}

bool looks_like_identifier(const std::string& text) {
    if (text.empty()) {
        return false;
    }

    const char first = text.front();

    if (!(std::isalpha(static_cast<unsigned char>(first)) || first == '_')) {
        return false;
    }

    for (char ch : text) {
        if (!(std::isalnum(static_cast<unsigned char>(ch)) || ch == '_')) {
            return false;
        }
    }

    return !looks_like_keyword(text) && !is_builtin_type_name(text);
}

std::optional<std::string> first_identifier_token_after_type(const ast::NodePtr& node) {
    bool passed_type = false;

    for (const auto& child : node->children) {
        if (!child) {
            continue;
        }

        if (child->kind == ast::NodeKind::Type) {
            passed_type = true;
            continue;
        }

        if (passed_type && child->kind == ast::NodeKind::Token && looks_like_identifier(child->text)) {
            return child->text;
        }
    }

    return std::nullopt;
}

std::optional<std::string> first_identifier_token(const ast::NodePtr& node) {
    if (!node) {
        return std::nullopt;
    }

    for (const auto& child : node->children) {
        if (child && child->kind == ast::NodeKind::Token && looks_like_identifier(child->text)) {
            return child->text;
        }
    }

    return std::nullopt;
}

void collect_nodes_by_kind(const ast::NodePtr& node, ast::NodeKind kind, std::vector<ast::NodePtr>& out) {
    if (!node) {
        return;
    }

    if (node->kind == kind) {
        out.push_back(node);
    }

    for (const auto& child : node->children) {
        collect_nodes_by_kind(child, kind, out);
    }
}

void collect_signature_params(const ast::NodePtr& node, std::vector<ast::NodePtr>& out) {
    if (!node) {
        return;
    }

    if (node->label == "paramList" || node->label == "externParamList") {
        collect_nodes_by_kind(node, ast::NodeKind::Param, out);
        return;
    }

    if (node->kind == ast::NodeKind::BlockStmt ||
        node->kind == ast::NodeKind::MeccBlockStmt ||
        node->kind == ast::NodeKind::LambdaExpr) {
        return;
    }

    for (const auto& child : node->children) {
        collect_signature_params(child, out);
    }
}

} // namespace

// ============================================================
// TypeInfo / Symbol / Scope
// ============================================================

std::string symbol_kind_name(SymbolKind kind) {
    switch (kind) {
        case SymbolKind::Include: return "include";
        case SymbolKind::Struct: return "struct";
        case SymbolKind::Field: return "field";
        case SymbolKind::Method: return "method";
        case SymbolKind::Function: return "function";
        case SymbolKind::ExternFunction: return "extern function";
        case SymbolKind::GlobalVariable: return "global variable";
        case SymbolKind::LocalVariable: return "local variable";
        case SymbolKind::Parameter: return "parameter";
    }

    return "unknown";
}

TypeInfo TypeInfo::unknown() {
    return TypeInfo{};
}

TypeInfo TypeInfo::error(std::string name) {
    TypeInfo type;
    type.name = std::move(name);
    type.is_error = true;
    return type;
}

bool TypeInfo::operator==(const TypeInfo& other) const {
    return name == other.name &&
           is_builtin == other.is_builtin &&
           is_void == other.is_void &&
           is_auto == other.is_auto &&
           is_error == other.is_error;
}

bool TypeInfo::operator!=(const TypeInfo& other) const {
    return !(*this == other);
}

Scope::Scope(std::string name, Scope* parent)
    : name(std::move(name)),
      parent(parent) {}

bool Scope::declare(const Symbol& symbol, DiagnosticBag& diagnostics) {
    if (symbol.name.empty()) {
        diagnostics.add_error(symbol.location, "Cannot declare a symbol with an empty name.");
        return false;
    }

    if (symbols.find(symbol.name) != symbols.end()) {
        diagnostics.add_error(
            symbol.location,
            fmt::format("Duplicate {} declaration '{}'.", symbol_kind_name(symbol.kind), symbol.name)
        );

        return false;
    }

    symbols.emplace(symbol.name, symbol);
    return true;
}

Symbol* Scope::find_local(const std::string& symbol_name) {
    auto it = symbols.find(symbol_name);

    if (it == symbols.end()) {
        return nullptr;
    }

    return &it->second;
}

Symbol* Scope::find_recursive(const std::string& symbol_name) {
    for (Scope* scope = this; scope != nullptr; scope = scope->parent) {
        if (Symbol* symbol = scope->find_local(symbol_name)) {
            return symbol;
        }
    }

    return nullptr;
}

// ============================================================
// SemanticContext
// ============================================================

SemanticContext::SemanticContext(DiagnosticBag& diagnostics)
    : diagnostics(diagnostics),
      global_scope("global", nullptr),
      current_scope(&global_scope) {}

Scope& SemanticContext::push_scope(const std::string& name) {
    owned_scopes.push_back(std::make_unique<Scope>(name, current_scope));
    current_scope = owned_scopes.back().get();
    return *current_scope;
}

void SemanticContext::pop_scope() {
    if (current_scope == nullptr || current_scope == &global_scope) {
        current_scope = &global_scope;
        return;
    }

    current_scope = current_scope->parent;
}

bool SemanticContext::declare_global(const Symbol& symbol) {
    const bool ok = global_scope.declare(symbol, diagnostics);

    if (ok) {
        if (Symbol* stored = global_scope.find_local(symbol.name)) {
            result.global_symbols.push_back(*stored);
        }
    }

    return ok;
}

bool SemanticContext::declare_local(const Symbol& symbol) {
    if (current_scope == nullptr) {
        diagnostics.add_error(symbol.location, "Internal semantic error: no current scope exists.");
        return false;
    }

    return current_scope->declare(symbol, diagnostics);
}

Symbol* SemanticContext::find_symbol(const std::string& name) {
    if (current_scope == nullptr) {
        return global_scope.find_local(name);
    }

    return current_scope->find_recursive(name);
}

bool SemanticContext::is_known_type(const TypeInfo& type) const {
    if (type.is_error || type.name.empty() || type.name == "unknown") {
        return false;
    }

    if (type.is_builtin || type.is_auto) {
        return true;
    }

    const std::string& name = type.name;

    // First-class function callback syntax: function<Return(Param1, Param2)>.
    // The outer `function` marker is structural, while the return type and
    // parameter types still need to be known Clyth types.
    if (auto function_type = parse_function_type_text(name)) {
        TypeInfo return_type = parse_type_text(function_type->return_type_name);
        if (!is_known_type(return_type)) {
            return false;
        }

        for (const std::string& parameter_type_name : function_type->parameter_type_names) {
            TypeInfo parameter_type = parse_type_text(parameter_type_name);
            if (!is_known_type(parameter_type)) {
                return false;
            }
        }

        return true;
    }

    // Array syntax: T[N] and T[]. The element type must be known; lengths
    // and dynamic array layout are validated/lowered in later passes.
    const std::size_t bracket_pos = name.find('[');
    if (bracket_pos != std::string::npos && name.back() == ']') {
        const std::string element_name = name.substr(0, bracket_pos);
        TypeInfo element = parse_type_text(element_name);
        return is_known_type(element);
    }

    // Generic runtime/library container syntax: List<T>, Map<K:V>, Set<T>.
    // Generic containers are allowed as known V1 types even before their full
    // runtime implementation exists, because semantic/codegen passes lower them
    // into runtime/library calls later.
    const std::size_t generic_pos = name.find('<');
    if (generic_pos != std::string::npos && name.back() == '>') {
        const std::string base = name.substr(0, generic_pos);
        if (base == "List" || base == "list" ||
            base == "Map" || base == "map" ||
            base == "Set" || base == "set" ||
            base == "pointer") {
            return true;
        }

        return global_scope.symbols.find(base) != global_scope.symbols.end();
    }

    return global_scope.symbols.find(type.name) != global_scope.symbols.end();
}

TypeInfo SemanticContext::parse_type_text(const std::string& text) const {
    if (text.empty()) {
        return TypeInfo::unknown();
    }

    TypeInfo type;
    type.name = text;
    type.is_builtin = is_builtin_type_name(text);
    type.is_void = text == "void";
    type.is_auto = text == "auto";

    // Collection and map types are accepted as "known" at this phase when their
    // surface spelling starts with a known or user-defined base type. Deep
    // collection type validation can be added as a later semantic pass.
    if (!type.is_builtin) {
        const std::size_t collection_pos = text.find_first_of("[:(<");
        const std::string base = collection_pos == std::string::npos
            ? text
            : text.substr(0, collection_pos);

        if (is_builtin_type_name(base) || global_scope.symbols.find(base) != global_scope.symbols.end()) {
            type.is_builtin = is_builtin_type_name(base);
        }
    }

    return type;
}

TypeInfo SemanticContext::type_of_node(const ast::NodePtr& node) const {
    if (!node) {
        return TypeInfo::unknown();
    }

    auto it = result.node_types.find(node.get());

    if (it == result.node_types.end()) {
        return TypeInfo::unknown();
    }

    return it->second;
}

void SemanticContext::set_node_type(const ast::NodePtr& node, const TypeInfo& type) {
    if (node) {
        result.node_types[node.get()] = type;
    }
}

bool SemanticContext::in_function() const {
    return !function_stack.empty();
}

const FunctionFrame* SemanticContext::current_function() const {
    if (function_stack.empty()) {
        return nullptr;
    }

    return &function_stack.back();
}

void SemanticContext::push_function(FunctionFrame frame) {
    function_stack.push_back(std::move(frame));
}

void SemanticContext::pop_function() {
    if (!function_stack.empty()) {
        function_stack.pop_back();
    }
}

// ============================================================
// Query Helpers
// ============================================================

namespace query {

bool has_label(const ast::NodePtr& node, const std::string& label) {
    return node && node->label == label;
}

bool text_is(const ast::NodePtr& node, const std::string& text) {
    return node && node->text == text;
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

std::vector<ast::NodePtr> token_children(const ast::NodePtr& node) {
    return children_of_kind(node, ast::NodeKind::Token);
}

std::vector<std::string> token_texts(const ast::NodePtr& node) {
    std::vector<std::string> result;

    for (const auto& token : token_children(node)) {
        result.push_back(token->text);
    }

    return result;
}

bool contains_token_text(const ast::NodePtr& node, const std::string& token_text) {
    const auto tokens = token_texts(node);

    return std::find(tokens.begin(), tokens.end(), token_text) != tokens.end();
}

std::optional<std::string> declared_name(const ast::NodePtr& node) {
    if (!node) {
        return std::nullopt;
    }

    if (auto explicit_name = attr(node, "name")) {
        return *explicit_name;
    }

    switch (node->kind) {
        case ast::NodeKind::FunctionDecl:
        case ast::NodeKind::VarDeclStmt:
        case ast::NodeKind::StructField:
        case ast::NodeKind::Param:
            return first_identifier_token_after_type(node);

        case ast::NodeKind::StructDecl:
            return first_identifier_token(node);

        case ast::NodeKind::ExternDecl:
            return first_identifier_token_after_type(node);

        default:
            return first_identifier_token(node);
    }
}

std::optional<TypeInfo> declared_type(const ast::NodePtr& node, const SemanticContext& context) {
    const ast::NodePtr type_node = first_child_of_kind(node, ast::NodeKind::Type);

    if (!type_node) {
        return std::nullopt;
    }

    if (auto name = attr(type_node, "name")) {
        return context.parse_type_text(*name);
    }

    if (!type_node->text.empty()) {
        return context.parse_type_text(type_node->text);
    }

    return std::nullopt;
}

bool is_expression_node(const ast::NodePtr& node) {
    if (!node) {
        return false;
    }

    switch (node->kind) {
        case ast::NodeKind::LiteralExpr:
        case ast::NodeKind::IdentifierExpr:
        case ast::NodeKind::BinaryExpr:
        case ast::NodeKind::UnaryExpr:
        case ast::NodeKind::CallExpr:
        case ast::NodeKind::MemberAccessExpr:
        case ast::NodeKind::IndexExpr:
        case ast::NodeKind::PostfixExpr:
        case ast::NodeKind::AllocationExpr:
        case ast::NodeKind::LambdaExpr:
        case ast::NodeKind::ListLiteralExpr:
        case ast::NodeKind::CurlyLiteralExpr:
            return true;

        default:
            return false;
    }
}

bool is_statement_node(const ast::NodePtr& node) {
    if (!node) {
        return false;
    }

    switch (node->kind) {
        case ast::NodeKind::BlockStmt:
        case ast::NodeKind::VarDeclStmt:
        case ast::NodeKind::AssignmentStmt:
        case ast::NodeKind::ExprStmt:
        case ast::NodeKind::ReturnStmt:
        case ast::NodeKind::IfStmt:
        case ast::NodeKind::WhileStmt:
        case ast::NodeKind::ForStmt:
        case ast::NodeKind::BreakStmt:
        case ast::NodeKind::ContinueStmt:
        case ast::NodeKind::MeccBlockStmt:
            return true;

        default:
            return false;
    }
}

bool is_declaration_node(const ast::NodePtr& node) {
    if (!node) {
        return false;
    }

    switch (node->kind) {
        case ast::NodeKind::IncludeDecl:
        case ast::NodeKind::ExternDecl:
        case ast::NodeKind::StructDecl:
        case ast::NodeKind::FunctionDecl:
        case ast::NodeKind::StructField:
        case ast::NodeKind::Param:
        case ast::NodeKind::VarDeclStmt:
            return true;

        default:
            return false;
    }
}

} // namespace query

// ============================================================
// Passes
// ============================================================

std::string TopLevelDeclarationPass::name() const {
    return "TopLevelDeclarationPass";
}

void TopLevelDeclarationPass::run(SemanticContext& context, const ast::ProgramPtr& program) {
    if (!program) {
        context.diagnostics.add_error(SourceLocation{}, "Top-level pass received null program.");
        return;
    }

    for (const auto& child : program->children) {
        if (!child) {
            continue;
        }

        switch (child->kind) {
            case ast::NodeKind::IncludeDecl: {
                Symbol symbol;
                symbol.kind = SymbolKind::Include;
                symbol.name = child->text;
                symbol.location = child->location;
                symbol.declaration = child;
                context.declare_global(symbol);
                break;
            }

            case ast::NodeKind::StructDecl: {
                const auto name = query::declared_name(child);

                if (!name) {
                    context.diagnostics.add_error(child->location, "Struct declaration is missing a name.");
                    break;
                }

                Symbol symbol;
                symbol.name = *name;
                symbol.kind = SymbolKind::Struct;
                symbol.type = context.parse_type_text(*name);
                symbol.location = child->location;
                symbol.declaration = child;
                context.declare_global(symbol);
                break;
            }

            case ast::NodeKind::FunctionDecl:
            case ast::NodeKind::MethodDecl:
            case ast::NodeKind::ExternDecl: {
                const auto name = query::declared_name(child);
                const auto type = query::declared_type(child, context);

                if (!name) {
                    context.diagnostics.add_error(child->location, "Function declaration is missing a name.");
                    break;
                }

                if (!type) {
                    context.diagnostics.add_error(child->location, fmt::format("Function '{}' is missing a return type.", *name));
                    break;
                }

                Symbol symbol;
                symbol.name = *name;
                if (child->kind == ast::NodeKind::ExternDecl) {
                    symbol.kind = SymbolKind::ExternFunction;
                } else if (child->kind == ast::NodeKind::MethodDecl) {
                    symbol.kind = SymbolKind::Method;
                } else {
                    symbol.kind = SymbolKind::Function;
                }
                symbol.type = *type;
                symbol.location = child->location;
                symbol.declaration = child;

                context.declare_global(symbol);
                context.set_node_type(child, *type);
                break;
            }

            case ast::NodeKind::VarDeclStmt: {
                const auto name = query::declared_name(child);
                const auto type = query::declared_type(child, context);

                if (!name || !type) {
                    context.diagnostics.add_error(child->location, "Top-level variable declaration is malformed.");
                    break;
                }

                Symbol symbol;
                symbol.name = *name;
                symbol.kind = SymbolKind::GlobalVariable;
                symbol.type = *type;
                symbol.location = child->location;
                symbol.declaration = child;

                context.declare_global(symbol);
                context.set_node_type(child, *type);
                break;
            }

            default:
                // Top-level statements are allowed by the grammar for testing,
                // but you can turn this into an error later.
                context.diagnostics.add_warning(
                    child->location,
                    fmt::format(
                        "Top-level '{}' is accepted for now but may be restricted later.",
                        ast::node_kind_name(child->kind)
                    )
                );
                break;
        }
    }
}

std::string StructValidationPass::name() const {
    return "StructValidationPass";
}

void StructValidationPass::run(SemanticContext& context, const ast::ProgramPtr& program) {
    std::vector<ast::NodePtr> structs;
    collect_nodes_by_kind(program, ast::NodeKind::StructDecl, structs);

    for (const auto& struct_node : structs) {
        std::unordered_set<std::string> field_names;

        for (const auto& field : struct_node->children) {
            if (!field || field->kind != ast::NodeKind::StructField) {
                continue;
            }

            const auto name = query::declared_name(field);
            const auto type = query::declared_type(field, context);

            if (!name) {
                context.diagnostics.add_error(field->location, "Struct field is missing a name.");
                continue;
            }

            if (field_names.count(*name) > 0) {
                context.diagnostics.add_error(field->location, fmt::format("Duplicate struct field '{}'.", *name));
            }

            field_names.insert(*name);

            if (!type) {
                context.diagnostics.add_error(field->location, fmt::format("Struct field '{}' is missing a type.", *name));
                continue;
            }

            if (!context.is_known_type(*type)) {
                context.diagnostics.add_error(field->location, fmt::format("Unknown field type '{}' for field '{}'.", type->name, *name));
            }

            context.set_node_type(field, *type);
        }
    }
}

std::string FunctionSignaturePass::name() const {
    return "FunctionSignaturePass";
}

void FunctionSignaturePass::run(SemanticContext& context, const ast::ProgramPtr& program) {
    std::vector<ast::NodePtr> functions;
    collect_nodes_by_kind(program, ast::NodeKind::FunctionDecl, functions);
    collect_nodes_by_kind(program, ast::NodeKind::MethodDecl, functions);
    collect_nodes_by_kind(program, ast::NodeKind::ExternDecl, functions);

    for (const auto& fn : functions) {
        const auto name = query::declared_name(fn);
        const auto return_type = query::declared_type(fn, context);

        if (!name || !return_type) {
            continue;
        }

        if (!context.is_known_type(*return_type)) {
            context.diagnostics.add_error(fn->location, fmt::format("Unknown return type '{}' for function '{}'.", return_type->name, *name));
        }

        std::unordered_set<std::string> param_names;
        std::vector<ast::NodePtr> params;
        collect_signature_params(fn, params);

        for (const auto& param : params) {
            const auto param_name = query::declared_name(param);
            const auto param_type = query::declared_type(param, context);

            if (!param_name) {
                // extern C varargs or unnamed extern params may exist.
                if (param->label == "externParam") {
                    continue;
                }

                context.diagnostics.add_error(param->location, fmt::format("Parameter in function '{}' is missing a name.", *name));
                continue;
            }

            if (param_names.count(*param_name) > 0) {
                context.diagnostics.add_error(param->location, fmt::format("Duplicate parameter '{}' in function '{}'.", *param_name, *name));
            }

            param_names.insert(*param_name);

            if (!param_type) {
                context.diagnostics.add_error(param->location, fmt::format("Parameter '{}' is missing a type.", *param_name));
                continue;
            }

            if (!context.is_known_type(*param_type)) {
                context.diagnostics.add_error(param->location, fmt::format("Unknown parameter type '{}' for parameter '{}'.", param_type->name, *param_name));
            }

            context.set_node_type(param, *param_type);
        }
    }
}

std::string ScopeAndSymbolPass::name() const {
    return "ScopeAndSymbolPass";
}

void ScopeAndSymbolPass::run(SemanticContext& context, const ast::ProgramPtr& program) {
    visit_node(context, program);
}

void ScopeAndSymbolPass::visit_node(SemanticContext& context, const ast::NodePtr& node) {
    if (!node) {
        return;
    }

    switch (node->kind) {
        case ast::NodeKind::FunctionDecl:
        case ast::NodeKind::MethodDecl:
            visit_function(context, node);
            return;

        case ast::NodeKind::BlockStmt:
        case ast::NodeKind::MeccBlockStmt:
            visit_block(context, node, node->label.empty() ? "block" : node->label);
            return;

        case ast::NodeKind::VarDeclStmt:
            visit_var_decl(context, node);
            return;

        case ast::NodeKind::AssignmentStmt:
            visit_assignment(context, node);
            return;

        default:
            break;
    }

    for (const auto& child : node->children) {
        visit_node(context, child);
    }
}

void ScopeAndSymbolPass::visit_function(SemanticContext& context, const ast::NodePtr& node) {
    const auto name = query::declared_name(node).value_or("<anonymous_function>");
    const auto return_type = query::declared_type(node, context).value_or(TypeInfo::unknown());

    context.push_function(FunctionFrame{name, return_type, node->location});
    context.push_scope(fmt::format("function:{}", name));

    std::vector<ast::NodePtr> params;
    collect_signature_params(node, params);

    for (const auto& param : params) {
        const auto param_name = query::declared_name(param);
        const auto param_type = query::declared_type(param, context);

        if (!param_name || !param_type) {
            continue;
        }

        Symbol symbol;
        symbol.name = *param_name;
        symbol.kind = SymbolKind::Parameter;
        symbol.type = *param_type;
        symbol.location = param->location;
        symbol.declaration = param;

        context.declare_local(symbol);
        context.set_node_type(param, *param_type);
    }

    // Only walk the direct body block here; params were already processed.
    for (const auto& child : node->children) {
        if (child && (child->kind == ast::NodeKind::BlockStmt || child->kind == ast::NodeKind::MeccBlockStmt)) {
            visit_block(context, child, fmt::format("function_body:{}", name));
        }
    }

    context.pop_scope();
    context.pop_function();
}

void ScopeAndSymbolPass::visit_block(SemanticContext& context, const ast::NodePtr& node, const std::string& scope_name) {
    context.push_scope(scope_name);

    if (node->kind == ast::NodeKind::MeccBlockStmt) {
        ++context.mecc_depth;
    }

    for (const auto& child : node->children) {
        visit_node(context, child);
    }

    if (node->kind == ast::NodeKind::MeccBlockStmt && context.mecc_depth > 0) {
        --context.mecc_depth;
    }

    context.pop_scope();
}

void ScopeAndSymbolPass::visit_var_decl(SemanticContext& context, const ast::NodePtr& node) {
    const auto name = query::declared_name(node);
    const auto type = query::declared_type(node, context);

    if (!name) {
        context.diagnostics.add_error(node->location, "Variable declaration is missing a name.");
        return;
    }

    if (!type) {
        context.diagnostics.add_error(node->location, fmt::format("Variable '{}' is missing a type.", *name));
        return;
    }

    if (!context.is_known_type(*type)) {
        context.diagnostics.add_error(node->location, fmt::format("Unknown variable type '{}' for '{}'.", type->name, *name));
    }

    Symbol symbol;
    symbol.name = *name;
    symbol.kind = context.current_scope == &context.global_scope
        ? SymbolKind::GlobalVariable
        : SymbolKind::LocalVariable;
    symbol.type = *type;
    symbol.location = node->location;
    symbol.declaration = node;

    if (context.current_scope == &context.global_scope) {
        context.declare_global(symbol);
    } else {
        context.declare_local(symbol);
    }

    context.set_node_type(node, *type);

    // Walk initializer/children after declaration so recursive expressions can
    // resolve the current variable name if the language later allows it.
    for (const auto& child : node->children) {
        if (child->kind != ast::NodeKind::Type && child->kind != ast::NodeKind::Token) {
            visit_node(context, child);
        }
    }
}

void ScopeAndSymbolPass::visit_assignment(SemanticContext& context, const ast::NodePtr& node) {
    const auto maybe_name = first_identifier_token(node);

    if (maybe_name) {
        if (Symbol* symbol = context.find_symbol(*maybe_name)) {
            context.result.resolved_symbols[node.get()] = symbol;
        } else {
            context.diagnostics.add_error(node->location, fmt::format("Assignment target '{}' is not declared.", *maybe_name));
        }
    }

    for (const auto& child : node->children) {
        visit_node(context, child);
    }
}

std::string ControlFlowPass::name() const {
    return "ControlFlowPass";
}

void ControlFlowPass::run(SemanticContext& context, const ast::ProgramPtr& program) {
    visit_node(context, program);
}

void ControlFlowPass::visit_node(SemanticContext& context, const ast::NodePtr& node) {
    if (!node) {
        return;
    }

    switch (node->kind) {
        case ast::NodeKind::FunctionDecl:
        case ast::NodeKind::MethodDecl:
            visit_function(context, node);
            return;

        case ast::NodeKind::ForStmt:
        case ast::NodeKind::WhileStmt:
            ++context.loop_depth;
            for (const auto& child : node->children) {
                visit_node(context, child);
            }
            if (context.loop_depth > 0) {
                --context.loop_depth;
            }
            return;

        case ast::NodeKind::BreakStmt:
        case ast::NodeKind::ContinueStmt:
            if (context.loop_depth == 0) {
                context.diagnostics.add_error(
                    node->location,
                    fmt::format("'{}' used outside of a loop.", node->label)
                );
            }
            return;

        case ast::NodeKind::ReturnStmt:
            if (!context.in_function()) {
                context.diagnostics.add_error(node->location, "'return' used outside of a function.");
            }
            return;

        default:
            break;
    }

    for (const auto& child : node->children) {
        visit_node(context, child);
    }
}

void ControlFlowPass::visit_function(SemanticContext& context, const ast::NodePtr& node) {
    const auto name = query::declared_name(node).value_or("<anonymous_function>");
    const auto return_type = query::declared_type(node, context).value_or(TypeInfo::unknown());

    context.push_function(FunctionFrame{name, return_type, node->location});

    for (const auto& child : node->children) {
        visit_node(context, child);
    }

    context.pop_function();
}



std::string CollectionLiteralSemanticPass::name() const {
    return "CollectionLiteralSemanticPass";
}

void CollectionLiteralSemanticPass::run(SemanticContext& context, const ast::ProgramPtr& program) {
    visit_node(context, program);
}

void CollectionLiteralSemanticPass::visit_node(SemanticContext& context, const ast::NodePtr& node) {
    if (!node) {
        return;
    }

    if (node->kind == ast::NodeKind::ListLiteralExpr) {
        node->attributes["semantic_collection"] = "list_or_array_initializer";
        node->attributes["lowering_hint"] = "target_type_decides_list_vs_fixed_array";
    }

    if (node->kind == ast::NodeKind::CurlyLiteralExpr) {
        bool has_pair = false;
        bool has_element = false;

        std::vector<ast::NodePtr> entries;
        collect_nodes_by_kind(node, ast::NodeKind::CurlyEntry, entries);

        for (const auto& entry : entries) {
            const auto kind = query::attr(entry, "entry_kind").value_or("element");
            if (kind == "pair") {
                has_pair = true;
            } else {
                has_element = true;
            }
        }

        if (has_pair && has_element) {
            context.diagnostics.add_error(
                node->location,
                "Curly collection literal cannot mix map key-value entries and set elements."
            );
            node->attributes["semantic_collection"] = "invalid_mixed_curly_literal";
        } else if (has_pair) {
            node->attributes["semantic_collection"] = "map_initializer";
        } else {
            node->attributes["semantic_collection"] = "set_initializer";
        }
    }

    for (const auto& child : node->children) {
        visit_node(context, child);
    }
}

std::string MethodValidationPass::name() const {
    return "MethodValidationPass";
}

void MethodValidationPass::run(SemanticContext& context, const ast::ProgramPtr& program) {
    std::vector<ast::NodePtr> method_blocks;
    collect_nodes_by_kind(program, ast::NodeKind::MethodBlock, method_blocks);

    for (const auto& block : method_blocks) {
        const auto owner_attr = query::attr(block, "owner");
        const std::string owner = owner_attr.value_or("");

        if (owner.empty()) {
            context.diagnostics.add_error(block->location, "Method block is missing a struct owner name.");
            continue;
        }

        Symbol* owner_symbol = context.global_scope.find_local(owner);

        if (owner_symbol == nullptr || owner_symbol->kind != SymbolKind::Struct) {
            context.diagnostics.add_error(block->location, fmt::format("Method block references unknown struct '{}'.", owner));
            continue;
        }

        std::unordered_set<std::string> local_method_names;

        for (const auto& child : block->children) {
            if (!child || child->kind != ast::NodeKind::MethodDecl) {
                continue;
            }

            const auto method_name = query::declared_name(child);

            if (!method_name) {
                context.diagnostics.add_error(child->location, fmt::format("Method declared for '{}' is missing a name.", owner));
                continue;
            }

            if (local_method_names.count(*method_name) > 0) {
                context.diagnostics.add_error(child->location, fmt::format("Duplicate method '{}.{}' in method block.", owner, *method_name));
            }

            local_method_names.insert(*method_name);

            child->attributes["owner"] = owner;

            if (!method_name->empty() && method_name->front() == '_') {
                child->attributes["visibility"] = "private";
            } else {
                child->attributes["visibility"] = "public";
            }

            Symbol symbol;
            symbol.name = owner + "." + *method_name;
            symbol.kind = SymbolKind::Method;
            symbol.type = query::declared_type(child, context).value_or(TypeInfo::unknown());
            symbol.location = child->location;
            symbol.declaration = child;

            context.declare_global(symbol);
        }
    }

    std::vector<ast::NodePtr> methods;
    collect_nodes_by_kind(program, ast::NodeKind::MethodDecl, methods);

    for (const auto& method : methods) {
        if (query::attr(method, "owner")) {
            continue;
        }

        // Qualified method syntax: ReturnType StructName.method_name(...)
        // Current generic AST stores raw text, so this pass annotates conservatively.
        if (method->text.find('.') != std::string::npos) {
            method->attributes["qualified_method"] = "true";
        }
    }
}


std::string MeccSemanticPass::name() const {
    return "MeccSemanticPass";
}

void MeccSemanticPass::run(SemanticContext& context, const ast::ProgramPtr& program) {
    visit_node(context, program);
}

void MeccSemanticPass::visit_node(SemanticContext& context, const ast::NodePtr& node) {
    if (!node) {
        return;
    }

    const bool entering_mecc =
        node->kind == ast::NodeKind::MeccBlockStmt ||
        (node->kind == ast::NodeKind::FunctionDecl && query::contains_token_text(node, "mecc")) ||
        (node->kind == ast::NodeKind::MethodDecl && query::contains_token_text(node, "mecc"));

    if (entering_mecc) {
        ++context.mecc_depth;
        node->attributes["mecc"] = "true";
    }

    if (node->kind == ast::NodeKind::AllocationExpr) {
        MeccAllocationInfo info;
        info.node = node.get();
        info.location = node->location;

        if (node->text.find("iso_malloc") != std::string::npos) {
            node->attributes["allocation"] = "iso_malloc";
            node->attributes["estate"] = "independent";
            node->attributes["mecc_counter"] = "estate_refcount";
            node->attributes["shared_handle"] = "required";
            node->attributes["escape_policy"] = "independent_estate";

            info.allocation_kind = "iso_malloc";
            info.estate = "independent";
            info.requires_estate_counter = true;
            info.requires_shared_handle = true;
            info.escapes_current_scope = true;
        } else if (node->text.find("malloc") != std::string::npos) {
            node->attributes["allocation"] = "malloc";

            if (context.mecc_depth > 0) {
                node->attributes["estate"] = "current";
                node->attributes["mecc_counter"] = "estate_refcount";
                node->attributes["shared_handle"] = "contextual";
                node->attributes["escape_policy"] = "current_estate";

                info.allocation_kind = "malloc";
                info.estate = "current";
                info.requires_estate_counter = true;
                info.requires_shared_handle = true;
                info.escapes_current_scope = false;
            } else {
                node->attributes["estate"] = "manual";
                node->attributes["mecc_counter"] = "none";
                node->attributes["shared_handle"] = "none";
                node->attributes["escape_policy"] = "manual_free";

                info.allocation_kind = "malloc";
                info.estate = "manual";
                info.requires_estate_counter = false;
                info.requires_shared_handle = false;
                info.escapes_current_scope = false;
            }
        }

        if (!info.allocation_kind.empty()) {
            context.result.mecc_allocations.push_back(info);
        }
    }

    if (query::contains_token_text(node, "free") && context.mecc_depth > 0) {
        context.diagnostics.add_error(
            node->location,
            "MECC-managed scope cannot manually free estate-owned memory. Use MECC estate lifetime rules instead."
        );
    }

    for (const auto& child : node->children) {
        visit_node(context, child);
    }

    if (entering_mecc && context.mecc_depth > 0) {
        --context.mecc_depth;
    }
}

// ============================================================
// Pipeline
// ============================================================

ClythSemanticPipeline::ClythSemanticPipeline(DiagnosticBag& diagnostics)
    : diagnostics(diagnostics) {
    register_default_passes();
}

SemanticResult ClythSemanticPipeline::analyze(const ast::ProgramPtr& program) {
    SemanticContext context(diagnostics);

    if (!program) {
        diagnostics.add_error(SourceLocation{}, "Semantic pipeline received a null AST program.");
        context.result.ok = false;
        return context.result;
    }

    for (const auto& pass : passes) {
        if (!pass) {
            continue;
        }

        const std::size_t before = diagnostics.error_count();
        pass->run(context, program);
        const std::size_t after = diagnostics.error_count();

        if (after > before) {
            // Keep running for now to collect more diagnostics.
            // If you want fail-fast, return here instead.
        }
    }

    context.result.ok = !diagnostics.has_errors();
    return context.result;
}

void ClythSemanticPipeline::add_pass(std::unique_ptr<ISemanticPass> pass) {
    passes.push_back(std::move(pass));
}

void ClythSemanticPipeline::register_default_passes() {
    passes.clear();

    add_pass(std::make_unique<TopLevelDeclarationPass>());
    add_pass(std::make_unique<StructValidationPass>());
    add_pass(std::make_unique<FunctionSignaturePass>());
    add_pass(std::make_unique<MethodValidationPass>());
    add_pass(std::make_unique<CollectionLiteralSemanticPass>());
    add_pass(std::make_unique<ScopeAndSymbolPass>());
    add_pass(std::make_unique<ControlFlowPass>());
    add_pass(std::make_unique<MeccSemanticPass>());
}

void SemanticResult::print_summary() const {
    fmt::print("Semantic summary:\n");
    fmt::print("  global symbols: {}\n", global_symbols.size());
    fmt::print("  typed nodes: {}\n", node_types.size());
    fmt::print("  resolved symbols: {}\n", resolved_symbols.size());
    fmt::print("  MECC allocations: {}\n", mecc_allocations.size());

    for (const auto& symbol : global_symbols) {
        fmt::print(
            "    - {} '{}' : {}\n",
            symbol_kind_name(symbol.kind),
            symbol.name,
            symbol.type.name
        );
    }
}

} // namespace clyth::semantic
