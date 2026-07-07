#include "clyth_ast.hpp"
#include <sstream>

namespace clyth {

// ============================================================
// DiagnosticBag
// ============================================================

void DiagnosticBag::add_error(SourceLocation location, std::string message) {
    diagnostics.push_back(Diagnostic{
        Diagnostic::Severity::Error,
        location,
        std::move(message)
    });
}

void DiagnosticBag::add_warning(SourceLocation location, std::string message) {
    diagnostics.push_back(Diagnostic{
        Diagnostic::Severity::Warning,
        location,
        std::move(message)
    });
}

void DiagnosticBag::add_info(SourceLocation location, std::string message) {
    diagnostics.push_back(Diagnostic{
        Diagnostic::Severity::Info,
        location,
        std::move(message)
    });
}

bool DiagnosticBag::has_errors() const {
    for (const auto& diagnostic : diagnostics) {
        if (diagnostic.severity == Diagnostic::Severity::Error) {
            return true;
        }
    }

    return false;
}

std::size_t DiagnosticBag::error_count() const {
    std::size_t count = 0;

    for (const auto& diagnostic : diagnostics) {
        if (diagnostic.severity == Diagnostic::Severity::Error) {
            ++count;
        }
    }

    return count;
}

const std::vector<Diagnostic>& DiagnosticBag::all() const {
    return diagnostics;
}

void DiagnosticBag::print_to_stderr(const std::filesystem::path& file) const {
    for (const auto& diagnostic : diagnostics) {
        std::string severity = "error";

        if (diagnostic.severity == Diagnostic::Severity::Warning) {
            severity = "warning";
        } else if (diagnostic.severity == Diagnostic::Severity::Info) {
            severity = "info";
        }

        if (!file.empty()) {
            fmt::print(
                stderr,
                "{}:{}:{}: {}: {}\n",
                file.string(),
                diagnostic.location.line,
                diagnostic.location.column,
                severity,
                diagnostic.message
            );
        } else {
            fmt::print(
                stderr,
                "{}:{}: {}: {}\n",
                diagnostic.location.line,
                diagnostic.location.column,
                severity,
                diagnostic.message
            );
        }
    }
}

// ============================================================
// ClythSyntaxErrorListener
// ============================================================

ClythSyntaxErrorListener::ClythSyntaxErrorListener(DiagnosticBag& diagnostics)
    : diagnostics(diagnostics) {}

void ClythSyntaxErrorListener::syntaxError(
    antlr4::Recognizer* recognizer,
    antlr4::Token* offending_symbol,
    std::size_t line,
    std::size_t char_position_in_line,
    const std::string& msg,
    std::exception_ptr e
) {
    (void)recognizer;
    (void)e;

    std::string message = msg;

    if (offending_symbol != nullptr) {
        const std::string offending_text = offending_symbol->getText();

        if (!offending_text.empty()) {
            message = fmt::format("{} near '{}'", msg, offending_text);
        }
    }

    diagnostics.add_error(SourceLocation{line, char_position_in_line}, message);
}

// ============================================================
// AST helpers
// ============================================================

namespace ast {

std::string node_kind_name(NodeKind kind) {
    switch (kind) {
        case NodeKind::Program: return "Program";
        case NodeKind::IncludeDecl: return "IncludeDecl";
        case NodeKind::ExternDecl: return "ExternDecl";
        case NodeKind::StructDecl: return "StructDecl";
        case NodeKind::GenericStructDecl: return "GenericStructDecl";
        case NodeKind::StructField: return "StructField";
        case NodeKind::MethodBlock: return "MethodBlock";
        case NodeKind::MethodDecl: return "MethodDecl";
        case NodeKind::FunctionDecl: return "FunctionDecl";
        case NodeKind::Param: return "Param";
        case NodeKind::BlockStmt: return "BlockStmt";
        case NodeKind::VarDeclStmt: return "VarDeclStmt";
        case NodeKind::AssignmentStmt: return "AssignmentStmt";
        case NodeKind::ExprStmt: return "ExprStmt";
        case NodeKind::ReturnStmt: return "ReturnStmt";
        case NodeKind::IfStmt: return "IfStmt";
        case NodeKind::ElseClause: return "ElseClause";
        case NodeKind::WhileStmt: return "WhileStmt";
        case NodeKind::ForStmt: return "ForStmt";
        case NodeKind::BreakStmt: return "BreakStmt";
        case NodeKind::ContinueStmt: return "ContinueStmt";
        case NodeKind::MeccBlockStmt: return "MeccBlockStmt";
        case NodeKind::Type: return "Type";
        case NodeKind::LiteralExpr: return "LiteralExpr";
        case NodeKind::IdentifierExpr: return "IdentifierExpr";
        case NodeKind::BinaryExpr: return "BinaryExpr";
        case NodeKind::UnaryExpr: return "UnaryExpr";
        case NodeKind::CallExpr: return "CallExpr";
        case NodeKind::MemberAccessExpr: return "MemberAccessExpr";
        case NodeKind::IndexExpr: return "IndexExpr";
        case NodeKind::PostfixExpr: return "PostfixExpr";
        case NodeKind::AllocationExpr: return "AllocationExpr";
        case NodeKind::LambdaExpr: return "LambdaExpr";
        case NodeKind::ListLiteralExpr: return "ListLiteralExpr";
        case NodeKind::CurlyLiteralExpr: return "CurlyLiteralExpr";
        case NodeKind::CurlyEntry: return "CurlyEntry";
        case NodeKind::Generic: return "Generic";
        case NodeKind::Token: return "Token";
    }

    return "Unknown";
}

ASTNode::ASTNode(NodeKind kind, SourceLocation location, std::string label)
    : kind(kind),
      location(location),
      label(std::move(label)) {}

void print_ast(const NodePtr& node, int indent) {
    if (!node) {
        return;
    }

    const std::string padding(static_cast<std::size_t>(indent), ' ');

    fmt::print(
        "{}{}",
        padding,
        node_kind_name(node->kind)
    );

    if (!node->label.empty()) {
        fmt::print(" [{}]", node->label);
    }

    if (!node->text.empty()) {
        fmt::print(" text='{}'", node->text);
    }

    if (!node->attributes.empty()) {
        fmt::print(" attrs={{");

        bool first = true;
        for (const auto& [key, value] : node->attributes) {
            if (!first) {
                fmt::print(", ");
            }

            fmt::print("{}: '{}'", key, value);
            first = false;
        }

        fmt::print("}}");
    }

    fmt::print(" @{}:{}\n", node->location.line, node->location.column);

    for (const auto& child : node->children) {
        print_ast(child, indent + 2);
    }
}


namespace {

std::string json_escape(const std::string& input) {
    std::string out;
    out.reserve(input.size() + 8);

    for (char ch : input) {
        switch (ch) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (static_cast<unsigned char>(ch) < 0x20) {
                    out += fmt::format("\\u{:04x}", static_cast<unsigned int>(static_cast<unsigned char>(ch)));
                } else {
                    out += ch;
                }
                break;
        }
    }

    return out;
}

void to_json_impl(const NodePtr& node, std::ostringstream& out, int indent) {
    const std::string pad(static_cast<std::size_t>(indent), ' ');
    const std::string child_pad(static_cast<std::size_t>(indent + 2), ' ');

    if (!node) {
        out << "null";
        return;
    }

    out << "{\n";
    out << child_pad << "\"kind\": \"" << json_escape(node_kind_name(node->kind)) << "\",\n";
    out << child_pad << "\"label\": \"" << json_escape(node->label) << "\",\n";
    out << child_pad << "\"text\": \"" << json_escape(node->text) << "\",\n";
    out << child_pad << "\"location\": {\"line\": " << node->location.line
        << ", \"column\": " << node->location.column << "},\n";

    out << child_pad << "\"attributes\": {";
    bool first_attr = true;
    for (const auto& [key, value] : node->attributes) {
        if (!first_attr) {
            out << ", ";
        }
        out << "\"" << json_escape(key) << "\": \"" << json_escape(value) << "\"";
        first_attr = false;
    }
    out << "},\n";

    out << child_pad << "\"children\": [";
    if (!node->children.empty()) {
        out << "\n";
        for (std::size_t i = 0; i < node->children.size(); ++i) {
            out << std::string(static_cast<std::size_t>(indent + 4), ' ');
            to_json_impl(node->children[i], out, indent + 4);
            if (i + 1 < node->children.size()) {
                out << ",";
            }
            out << "\n";
        }
        out << child_pad;
    }
    out << "]\n";
    out << pad << "}";
}

void bytecode_walk(const NodePtr& node, std::ostringstream& out, std::size_t& next_id, std::optional<std::size_t> parent_id, std::size_t depth) {
    if (!node) {
        return;
    }

    const std::size_t id = next_id++;

    out << "NODE "
        << id
        << " parent=" << (parent_id ? std::to_string(*parent_id) : "none")
        << " depth=" << depth
        << " kind=" << node_kind_name(node->kind);

    if (!node->label.empty()) {
        out << " label=\"" << json_escape(node->label) << "\"";
    }

    if (!node->text.empty()) {
        out << " text=\"" << json_escape(node->text) << "\"";
    }

    out << " loc=" << node->location.line << ":" << node->location.column;

    for (const auto& [key, value] : node->attributes) {
        out << " attr." << key << "=\"" << json_escape(value) << "\"";
    }

    out << "\n";

    for (const auto& child : node->children) {
        bytecode_walk(child, out, next_id, id, depth + 1);
    }
}

} // namespace

std::string to_json(const NodePtr& node, int indent) {
    std::ostringstream out;
    to_json_impl(node, out, indent);
    out << "\n";
    return out.str();
}

std::string to_clyth_bytecode(const NodePtr& node) {
    std::ostringstream out;
    out << "CLYTH_AST_BYTECODE v1\n";
    out << "# This is a stable, line-oriented AST bytecode/debug format.\n";
    out << "# It is intentionally not executable VM bytecode yet.\n";

    std::size_t next_id = 0;
    bytecode_walk(node, out, next_id, std::nullopt, 0);
    return out.str();
}

bool write_json_file(const NodePtr& node, const std::filesystem::path& path) {
    std::ofstream file(path);

    if (!file) {
        return false;
    }

    file << to_json(node);
    return true;
}

bool write_bytecode_file(const NodePtr& node, const std::filesystem::path& path) {
    std::ofstream file(path);

    if (!file) {
        return false;
    }

    file << to_clyth_bytecode(node);
    return true;
}

} // namespace ast

// ============================================================
// ClythAST
// ============================================================

ClythAST::ClythAST(DiagnosticBag& diagnostics)
    : diagnostics(diagnostics) {}

ast::ProgramPtr ClythAST::build(ClythV1Parser::ProgramContext* ctx) {
    if (ctx == nullptr) {
        diagnostics.add_error(SourceLocation{}, "Cannot build AST from null program context.");
        return nullptr;
    }

    try {
        return any_to_node(visitProgram(ctx), "build(program)");
    } catch (const std::exception& e) {
        diagnostics.add_error(location_from(ctx), fmt::format("AST build failed: {}", e.what()));
        return nullptr;
    }
}

SourceLocation ClythAST::location_from(antlr4::ParserRuleContext* ctx) const {
    if (ctx == nullptr || ctx->getStart() == nullptr) {
        return {};
    }

    return SourceLocation{
        ctx->getStart()->getLine(),
        ctx->getStart()->getCharPositionInLine()
    };
}

ast::NodePtr ClythAST::make_node(
    ast::NodeKind kind,
    antlr4::ParserRuleContext* ctx,
    std::string label
) const {
    auto node = std::make_shared<ast::ASTNode>(
        kind,
        location_from(ctx),
        std::move(label)
    );

    if (ctx != nullptr) {
        node->text = ctx->getText();
    }

    return node;
}

ast::NodePtr ClythAST::make_token_node(antlr4::tree::TerminalNode* terminal) const {
    auto node = std::make_shared<ast::ASTNode>(
        ast::NodeKind::Token,
        SourceLocation{},
        "token"
    );

    if (terminal != nullptr && terminal->getSymbol() != nullptr) {
        antlr4::Token* token = terminal->getSymbol();

        node->location = SourceLocation{
            token->getLine(),
            token->getCharPositionInLine()
        };

        node->text = token->getText();
        node->attributes["token_type"] = std::to_string(token->getType());
    }

    return node;
}

ast::NodePtr ClythAST::any_to_node(const std::any& value, const std::string& caller) {
    if (!value.has_value()) {
        diagnostics.add_error(SourceLocation{}, fmt::format("{} returned an empty AST value.", caller));
        return nullptr;
    }

    try {
        return std::any_cast<ast::NodePtr>(value);
    } catch (const std::bad_any_cast& e) {
        diagnostics.add_error(
            SourceLocation{},
            fmt::format("{} returned an unexpected AST value type: {}", caller, e.what())
        );

        return nullptr;
    }
}

bool ClythAST::is_ignored_terminal(antlr4::tree::TerminalNode* terminal) const {
    if (terminal == nullptr || terminal->getSymbol() == nullptr) {
        return true;
    }

    const int type = terminal->getSymbol()->getType();

    if (type == antlr4::Token::EOF) {
        return true;
    }

    const std::string text = terminal->getText();

    // Punctuation exists in the parse tree but usually does not need to become
    // a semantic AST node.
    return text == "(" || text == ")" ||
           text == "{" || text == "}" ||
           text == "[" || text == "]" ||
           text == "," || text == ";";
}

std::vector<ast::NodePtr> ClythAST::visit_rule_children(antlr4::ParserRuleContext* ctx) {
    std::vector<ast::NodePtr> result;

    if (ctx == nullptr) {
        return result;
    }

    for (antlr4::tree::ParseTree* child : ctx->children) {
        if (child == nullptr) {
            continue;
        }

        if (auto* rule = dynamic_cast<antlr4::ParserRuleContext*>(child)) {
            ast::NodePtr child_node = any_to_node(visit(rule), "visit_rule_children");

            if (child_node != nullptr) {
                result.push_back(child_node);
            }

            continue;
        }

        if (auto* terminal = dynamic_cast<antlr4::tree::TerminalNode*>(child)) {
            if (!is_ignored_terminal(terminal)) {
                result.push_back(make_token_node(terminal));
            }
        }
    }

    return result;
}

ast::NodePtr ClythAST::visit_single_rule_child(antlr4::ParserRuleContext* ctx) {
    if (ctx == nullptr) {
        diagnostics.add_error(SourceLocation{}, "Cannot visit children of a null parser context.");
        return nullptr;
    }

    for (antlr4::tree::ParseTree* child : ctx->children) {
        if (auto* rule = dynamic_cast<antlr4::ParserRuleContext*>(child)) {
            return any_to_node(visit(rule), "visit_single_rule_child");
        }
    }

    diagnostics.add_error(location_from(ctx), "Expected a parser-rule child but found none.");
    return nullptr;
}

ast::NodePtr ClythAST::build_generic(
    ast::NodeKind kind,
    antlr4::ParserRuleContext* ctx,
    std::string label
) {
    auto node = make_node(kind, ctx, std::move(label));
    node->children = visit_rule_children(ctx);
    return node;
}

// Top-level wrappers.

std::any ClythAST::visitProgram(ClythV1Parser::ProgramContext* ctx) {
    return build_generic(ast::NodeKind::Program, ctx, "program");
}

std::any ClythAST::visitTopLevelItem(ClythV1Parser::TopLevelItemContext* ctx) {
    // topLevelItem is a grammar wrapper. Forward to the one matched child.
    return visit_single_rule_child(ctx);
}

std::any ClythAST::visitIncludeDecl(ClythV1Parser::IncludeDeclContext* ctx) {
    auto node = build_generic(ast::NodeKind::IncludeDecl, ctx, "include");
    return node;
}

std::any ClythAST::visitIncludeTarget(ClythV1Parser::IncludeTargetContext* ctx) {
    auto node = make_node(ast::NodeKind::Generic, ctx, "includeTarget");
    node->attributes["target"] = ctx->getText();
    return node;
}

std::any ClythAST::visitExternDecl(ClythV1Parser::ExternDeclContext* ctx) {
    auto node = build_generic(ast::NodeKind::ExternDecl, ctx, "extern");
    return node;
}

std::any ClythAST::visitExternAbi(ClythV1Parser::ExternAbiContext* ctx) {
    auto node = make_node(ast::NodeKind::Generic, ctx, "externAbi");
    node->attributes["abi"] = ctx->getText();
    return node;
}

std::any ClythAST::visitExternParamList(ClythV1Parser::ExternParamListContext* ctx) {
    return build_generic(ast::NodeKind::Generic, ctx, "externParamList");
}

std::any ClythAST::visitExternParam(ClythV1Parser::ExternParamContext* ctx) {
    return build_generic(ast::NodeKind::Param, ctx, "externParam");
}

std::any ClythAST::visitStructDecl(ClythV1Parser::StructDeclContext* ctx) {
    const std::string text = ctx->getText();
    const bool is_generic_template = text.find('<') != std::string::npos
        && text.find('<') < text.find('{');
    auto node = build_generic(is_generic_template ? ast::NodeKind::GenericStructDecl : ast::NodeKind::StructDecl, ctx, "struct");
    if (is_generic_template) {
        node->attributes["generic_template"] = "true";
    }
    return node;
}

std::any ClythAST::visitGenericParamList(ClythV1Parser::GenericParamListContext* ctx) {
    auto node = make_node(ast::NodeKind::Generic, ctx, "genericParamList");

    if (ctx != nullptr) {
        for (auto* identifier : ctx->IDENTIFIER()) {
            if (identifier == nullptr) {
                continue;
            }

            auto param = make_token_node(identifier);
            param->label = "genericParam";
            param->attributes["name"] = identifier->getText();
            node->children.push_back(param);
        }
    }

    return node;
}

std::any ClythAST::visitStructField(ClythV1Parser::StructFieldContext* ctx) {
    return build_generic(ast::NodeKind::StructField, ctx, "field");
}

std::any ClythAST::visitVisibilityModifier(ClythV1Parser::VisibilityModifierContext* ctx) {
    auto node = make_node(ast::NodeKind::Generic, ctx, "visibilityModifier");
    node->attributes["visibility"] = ctx != nullptr ? ctx->getText() : "";
    node->text = ctx != nullptr ? ctx->getText() : "";
    return node;
}

std::any ClythAST::visitMethodBlock(ClythV1Parser::MethodBlockContext* ctx) {
    auto node = build_generic(ast::NodeKind::MethodBlock, ctx, "methodBlock");
    node->attributes["owner"] = ctx->IDENTIFIER() != nullptr ? ctx->IDENTIFIER()->getText() : "";
    return node;
}

std::any ClythAST::visitMethodDecl(ClythV1Parser::MethodDeclContext* ctx) {
    auto node = build_generic(ast::NodeKind::MethodDecl, ctx, "method");
    const bool qualified = ctx != nullptr && ctx->DOT() != nullptr && ctx->IDENTIFIER() != nullptr;
    node->attributes["qualified"] = qualified ? "true" : "false";
    if (qualified) {
        node->attributes["owner"] = ctx->IDENTIFIER()->getText();
    }
    if (ctx != nullptr && ctx->methodSimpleName() != nullptr) {
        node->attributes["name"] = ctx->methodSimpleName()->getText();
    }
    return node;
}

std::any ClythAST::visitMethodSimpleName(ClythV1Parser::MethodSimpleNameContext* ctx) {
    auto node = make_node(ast::NodeKind::Generic, ctx, "methodSimpleName");
    node->attributes["name"] = ctx != nullptr ? ctx->getText() : "";
    return node;
}

std::any ClythAST::visitFunctionDecl(ClythV1Parser::FunctionDeclContext* ctx) {
    return build_generic(ast::NodeKind::FunctionDecl, ctx, "function");
}

std::any ClythAST::visitParamList(ClythV1Parser::ParamListContext* ctx) {
    return build_generic(ast::NodeKind::Generic, ctx, "paramList");
}

std::any ClythAST::visitParam(ClythV1Parser::ParamContext* ctx) {
    return build_generic(ast::NodeKind::Param, ctx, "param");
}

// Statements.

std::any ClythAST::visitBlock(ClythV1Parser::BlockContext* ctx) {
    return build_generic(ast::NodeKind::BlockStmt, ctx, "block");
}

std::any ClythAST::visitStatement(ClythV1Parser::StatementContext* ctx) {
    // statement is a grammar wrapper. Forward to the concrete statement.
    return visit_single_rule_child(ctx);
}

std::any ClythAST::visitVarDecl(ClythV1Parser::VarDeclContext* ctx) {
    return build_generic(ast::NodeKind::VarDeclStmt, ctx, "varDecl");
}

std::any ClythAST::visitAssignmentStmt(ClythV1Parser::AssignmentStmtContext* ctx) {
    return build_generic(ast::NodeKind::AssignmentStmt, ctx, "assignment");
}

std::any ClythAST::visitAssignable(ClythV1Parser::AssignableContext* ctx) {
    return build_generic(ast::NodeKind::Generic, ctx, "assignable");
}

std::any ClythAST::visitAssignmentOp(ClythV1Parser::AssignmentOpContext* ctx) {
    auto node = make_node(ast::NodeKind::Token, ctx, "assignmentOp");
    node->attributes["operator"] = ctx->getText();
    return node;
}

std::any ClythAST::visitExprStmt(ClythV1Parser::ExprStmtContext* ctx) {
    return build_generic(ast::NodeKind::ExprStmt, ctx, "exprStmt");
}

std::any ClythAST::visitReturnStmt(ClythV1Parser::ReturnStmtContext* ctx) {
    return build_generic(ast::NodeKind::ReturnStmt, ctx, "return");
}

std::any ClythAST::visitIfStmt(ClythV1Parser::IfStmtContext* ctx) {
    return build_generic(ast::NodeKind::IfStmt, ctx, "if");
}

std::any ClythAST::visitElseClause(ClythV1Parser::ElseClauseContext* ctx) {
    return build_generic(ast::NodeKind::ElseClause, ctx, "else");
}

std::any ClythAST::visitWhileStmt(ClythV1Parser::WhileStmtContext* ctx) {
    return build_generic(ast::NodeKind::WhileStmt, ctx, "while");
}

std::any ClythAST::visitForStmt(ClythV1Parser::ForStmtContext* ctx) {
    return build_generic(ast::NodeKind::ForStmt, ctx, "for");
}

std::any ClythAST::visitForEachHeader(ClythV1Parser::ForEachHeaderContext* ctx) {
    return build_generic(ast::NodeKind::Generic, ctx, "forEachHeader");
}

std::any ClythAST::visitForEachBinding(ClythV1Parser::ForEachBindingContext* ctx) {
    return build_generic(ast::NodeKind::Generic, ctx, "forEachBinding");
}

std::any ClythAST::visitForEachVar(ClythV1Parser::ForEachVarContext* ctx) {
    return build_generic(ast::NodeKind::Generic, ctx, "forEachVar");
}

std::any ClythAST::visitForClassicHeader(ClythV1Parser::ForClassicHeaderContext* ctx) {
    return build_generic(ast::NodeKind::Generic, ctx, "forClassicHeader");
}

std::any ClythAST::visitForInit(ClythV1Parser::ForInitContext* ctx) {
    return build_generic(ast::NodeKind::Generic, ctx, "forInit");
}

std::any ClythAST::visitVarDeclNoSemi(ClythV1Parser::VarDeclNoSemiContext* ctx) {
    return build_generic(ast::NodeKind::VarDeclStmt, ctx, "varDeclNoSemi");
}

std::any ClythAST::visitAssignmentNoSemi(ClythV1Parser::AssignmentNoSemiContext* ctx) {
    return build_generic(ast::NodeKind::AssignmentStmt, ctx, "assignmentNoSemi");
}

std::any ClythAST::visitBreakStmt(ClythV1Parser::BreakStmtContext* ctx) {
    return make_node(ast::NodeKind::BreakStmt, ctx, "break");
}

std::any ClythAST::visitContinueStmt(ClythV1Parser::ContinueStmtContext* ctx) {
    return make_node(ast::NodeKind::ContinueStmt, ctx, "continue");
}

std::any ClythAST::visitMeccBlock(ClythV1Parser::MeccBlockContext* ctx) {
    return build_generic(ast::NodeKind::MeccBlockStmt, ctx, "mecc");
}

// Expressions.

std::any ClythAST::visitParenExpr(ClythV1Parser::ParenExprContext* ctx) {
    return visit_single_rule_child(ctx);
}

std::any ClythAST::visitExpression(ClythV1Parser::ExpressionContext* ctx) {
    return visit_single_rule_child(ctx);
}

std::any ClythAST::visitLogicalOr(ClythV1Parser::LogicalOrContext* ctx) {
    return build_generic(ast::NodeKind::BinaryExpr, ctx, "logicalOr");
}

std::any ClythAST::visitLogicalAnd(ClythV1Parser::LogicalAndContext* ctx) {
    return build_generic(ast::NodeKind::BinaryExpr, ctx, "logicalAnd");
}

std::any ClythAST::visitEquality(ClythV1Parser::EqualityContext* ctx) {
    return build_generic(ast::NodeKind::BinaryExpr, ctx, "equality");
}

std::any ClythAST::visitRelation(ClythV1Parser::RelationContext* ctx) {
    return build_generic(ast::NodeKind::BinaryExpr, ctx, "relation");
}

std::any ClythAST::visitComparison(ClythV1Parser::ComparisonContext* ctx) {
    return build_generic(ast::NodeKind::BinaryExpr, ctx, "comparison");
}

std::any ClythAST::visitAdditive(ClythV1Parser::AdditiveContext* ctx) {
    return build_generic(ast::NodeKind::BinaryExpr, ctx, "additive");
}

std::any ClythAST::visitMultiplicative(ClythV1Parser::MultiplicativeContext* ctx) {
    return build_generic(ast::NodeKind::BinaryExpr, ctx, "multiplicative");
}

std::any ClythAST::visitUnary(ClythV1Parser::UnaryContext* ctx) {
    return build_generic(ast::NodeKind::UnaryExpr, ctx, "unary");
}

std::any ClythAST::visitPostfix(ClythV1Parser::PostfixContext* ctx) {
    return build_generic(ast::NodeKind::PostfixExpr, ctx, "postfix");
}

std::any ClythAST::visitPostfixSuffix(ClythV1Parser::PostfixSuffixContext* ctx) {
    const std::string text = ctx->getText();

    if (!text.empty() && text[0] == '.') {
        return build_generic(ast::NodeKind::MemberAccessExpr, ctx, "memberAccess");
    }

    if (!text.empty() && text[0] == '[') {
        return build_generic(ast::NodeKind::IndexExpr, ctx, "index");
    }

    if (!text.empty() && text[0] == '(') {
        return build_generic(ast::NodeKind::CallExpr, ctx, "call");
    }

    return build_generic(ast::NodeKind::PostfixExpr, ctx, "postfixSuffix");
}

std::any ClythAST::visitPrimary(ClythV1Parser::PrimaryContext* ctx) {
    // Primary is usually a wrapper around literal, collection literal, identifier,
    // or parenthesized expression.
    return build_generic(ast::NodeKind::Generic, ctx, "primary");
}

std::any ClythAST::visitLambdaExpression(ClythV1Parser::LambdaExpressionContext* ctx) {
    auto node = build_generic(ast::NodeKind::LambdaExpr, ctx, "lambda");
    // Anonymous functions no longer carry an explicit return type in the lambda
    // expression grammar. The semantic pass infers void when there are no
    // return statements, or validates a consistent inferred return type later.
    node->attributes["return_type"] = "void";
    node->attributes["type"] = ctx->getText();
    return node;
}

std::any ClythAST::visitLambdaParamList(ClythV1Parser::LambdaParamListContext* ctx) {
    return build_generic(ast::NodeKind::Generic, ctx, "lambdaParamList");
}

std::any ClythAST::visitLambdaParam(ClythV1Parser::LambdaParamContext* ctx) {
    return build_generic(ast::NodeKind::Param, ctx, "lambdaParam");
}

std::any ClythAST::visitArgumentList(ClythV1Parser::ArgumentListContext* ctx) {
    return build_generic(ast::NodeKind::Generic, ctx, "argumentList");
}

std::any ClythAST::visitLiteral(ClythV1Parser::LiteralContext* ctx) {
    auto node = make_node(ast::NodeKind::LiteralExpr, ctx, "literal");
    node->attributes["literal"] = ctx->getText();
    return node;
}

// Collections.

std::any ClythAST::visitCollectionLiteral(ClythV1Parser::CollectionLiteralContext* ctx) {
    return visit_single_rule_child(ctx);
}

std::any ClythAST::visitListLiteral(ClythV1Parser::ListLiteralContext* ctx) {
    auto node = build_generic(ast::NodeKind::ListLiteralExpr, ctx, "listLiteral");
    node->attributes["literal_kind"] = "list";
    return node;
}

std::any ClythAST::visitCurlyLiteral(ClythV1Parser::CurlyLiteralContext* ctx) {
    auto node = build_generic(ast::NodeKind::CurlyLiteralExpr, ctx, "curlyLiteral");
    node->attributes["literal_kind"] = "curly";
    node->attributes["semantic_kind"] = "map_or_set";
    return node;
}

std::any ClythAST::visitCurlyEntryList(ClythV1Parser::CurlyEntryListContext* ctx) {
    return build_generic(ast::NodeKind::Generic, ctx, "curlyEntryList");
}

std::any ClythAST::visitCurlyEntry(ClythV1Parser::CurlyEntryContext* ctx) {
    auto node = build_generic(ast::NodeKind::CurlyEntry, ctx, "curlyEntry");
    node->attributes["entry_kind"] = ctx->COLON() != nullptr ? "pair" : "element";
    return node;
}

std::any ClythAST::visitExpressionList(ClythV1Parser::ExpressionListContext* ctx) {
    return build_generic(ast::NodeKind::Generic, ctx, "expressionList");
}

// Types.

std::any ClythAST::visitType(ClythV1Parser::TypeContext* ctx) {
    auto node = build_generic(ast::NodeKind::Type, ctx, "type");
    node->attributes["name"] = ctx->getText();
    return node;
}

std::any ClythAST::visitFixedArrayType(ClythV1Parser::FixedArrayTypeContext* ctx) {
    auto node = build_generic(ast::NodeKind::Type, ctx, "fixedArrayType");
    node->attributes["name"] = ctx->getText();
    node->attributes["container"] = "array";
    node->attributes["array_kind"] = "fixed";
    return node;
}


std::any ClythAST::visitDynamicArrayType(ClythV1Parser::DynamicArrayTypeContext* ctx) {
    auto node = build_generic(ast::NodeKind::Type, ctx, "dynamicArrayType");
    node->attributes["name"] = ctx->getText();
    node->attributes["container"] = "array";
    node->attributes["array_kind"] = "dynamic";
    return node;
}

std::any ClythAST::visitFunctionType(ClythV1Parser::FunctionTypeContext* ctx) {
    auto node = build_generic(ast::NodeKind::Type, ctx, "functionType");
    node->attributes["name"] = ctx->getText();
    node->attributes["container"] = "function";
    return node;
}

std::any ClythAST::visitFunctionParamTypeList(ClythV1Parser::FunctionParamTypeListContext* ctx) {
    auto node = build_generic(ast::NodeKind::Generic, ctx, "functionParamTypeList");
    node->attributes["text"] = ctx->getText();
    return node;
}

std::any ClythAST::visitGenericType(ClythV1Parser::GenericTypeContext* ctx) {
    auto node = build_generic(ast::NodeKind::Type, ctx, "genericType");
    node->attributes["name"] = ctx->getText();
    node->attributes["container"] = "generic";
    return node;
}

std::any ClythAST::visitGenericArgList(ClythV1Parser::GenericArgListContext* ctx) {
    auto node = build_generic(ast::NodeKind::Generic, ctx, "genericArgList");
    node->attributes["text"] = ctx->getText();
    return node;
}

std::any ClythAST::visitTypeAtom(ClythV1Parser::TypeAtomContext* ctx) {
    return visit_single_rule_child(ctx);
}

std::any ClythAST::visitBaseType(ClythV1Parser::BaseTypeContext* ctx) {
    auto node = make_node(ast::NodeKind::Type, ctx, "baseType");
    node->attributes["name"] = ctx->getText();
    return node;
}

std::any ClythAST::visitCommaOrSemi(ClythV1Parser::CommaOrSemiContext* ctx) {
    auto node = make_node(ast::NodeKind::Token, ctx, "separator");
    node->attributes["separator"] = ctx->getText();
    return node;
}

// ============================================================
// Semantic Analysis Stub
// ============================================================

ClythSemanticAnalyzer::ClythSemanticAnalyzer(DiagnosticBag& diagnostics)
    : diagnostics(diagnostics) {}

bool ClythSemanticAnalyzer::analyze(const ast::ProgramPtr& program) {
    if (program == nullptr) {
        diagnostics.add_error(SourceLocation{}, "Semantic analysis received a null AST program.");
        return false;
    }

    analyze_program(program);

    return !diagnostics.has_errors();
}

void ClythSemanticAnalyzer::analyze_node(const ast::NodePtr& node) {
    if (node == nullptr) {
        return;
    }

    switch (node->kind) {
        case ast::NodeKind::Program:
            analyze_program(node);
            return;

        case ast::NodeKind::FunctionDecl:
            analyze_function_decl(node);
            return;

        case ast::NodeKind::StructDecl:
            analyze_struct_decl(node);
            return;

        case ast::NodeKind::VarDeclStmt:
            analyze_var_decl(node);
            return;

        default:
            break;
    }

    for (const auto& child : node->children) {
        analyze_node(child);
    }
}

void ClythSemanticAnalyzer::analyze_program(const ast::NodePtr& node) {
    // TODO:
    // - collect top-level symbols
    // - reject invalid top-level statements if desired
    // - check duplicate includes/externs/functions/structs

    for (const auto& child : node->children) {
        analyze_node(child);
    }
}

void ClythSemanticAnalyzer::analyze_function_decl(const ast::NodePtr& node) {
    // TODO:
    // - enter function scope
    // - register parameters
    // - validate return statements
    // - type-check body

    for (const auto& child : node->children) {
        analyze_node(child);
    }
}

void ClythSemanticAnalyzer::analyze_struct_decl(const ast::NodePtr& node) {
    // TODO:
    // - collect fields
    // - reject duplicate fields
    // - resolve field types

    for (const auto& child : node->children) {
        analyze_node(child);
    }
}

void ClythSemanticAnalyzer::analyze_var_decl(const ast::NodePtr& node) {
    // TODO:
    // - resolve declared type
    // - type-check initializer
    // - register local symbol in current scope

    for (const auto& child : node->children) {
        analyze_node(child);
    }
}

} // namespace clyth
