#include "common.hpp"
#include "cxxopts/cxxopts.hpp"
#include "scanner.hpp"
#include "clyth_ast.hpp"
#include "clyth_semantic.hpp"
#include "clyth_lowering_plan.hpp"
#include "clyth_llvm_stub.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <iterator>
#include <regex>
#include <set>
#include <sstream>
#include <vector>

// ANTLR4 runtime includes.
#include "antlr4-runtime.h"

// Generated from ClythV1.g4.
#include "clyth_antlr_files/ClythV1Lexer.h"
#include "clyth_antlr_files/ClythV1Parser.h"

struct CompilerOptions {
    bool debug_mode = false;
    bool dump_ast_json_stdout = false;
    bool dump_ast_bytecode_stdout = false;
    bool dump_codegen_plan = false;
    bool emit_ir_only = false;
    bool print_ir = false;

    std::filesystem::path main_file;
    std::filesystem::path ast_json_file;
    std::filesystem::path ast_bytecode_file;
    std::string output_binary_name = "clyth_program.bin";
};


static std::string strip_quotes(std::string value) {
    if (value.size() >= 2 &&
        ((value.front() == '"' && value.back() == '"') ||
         (value.front() == '`' && value.back() == '`'))) {
        return value.substr(1, value.size() - 2);
    }
    return value;
}

static bool read_clyth_source_with_includes_impl(
    const std::filesystem::path& file,
    std::set<std::filesystem::path>& active_stack,
    std::set<std::filesystem::path>& already_included,
    std::ostringstream& output
) {
    std::error_code ec;
    const std::filesystem::path canonical = std::filesystem::weakly_canonical(file, ec);
    const std::filesystem::path normalized = ec ? file.lexically_normal() : canonical;

    if (active_stack.count(normalized) > 0) {
        fmt::print(stderr, "ERROR: include cycle detected at {}\n", normalized.string());
        return false;
    }

    if (already_included.count(normalized) > 0) {
        return true;
    }

    Scanner scanner;
    if (!scanner.read_file(normalized.string())) {
        fmt::print(stderr, "ERROR: Failed to read source file: {}\n", normalized.string());
        return false;
    }

    active_stack.insert(normalized);
    already_included.insert(normalized);

    const std::filesystem::path base_dir = normalized.parent_path();
    const std::regex include_pattern(R"(^\s*include\s+([^;\s]+)\s*;?\s*$)");
    std::istringstream input(scanner.to_string());
    std::string line;

    output << "// begin include unit: " << normalized.string() << "\n";

    while (std::getline(input, line)) {
        std::smatch match;
        if (std::regex_match(line, match, include_pattern)) {
            std::string include_target = strip_quotes(match[1].str());
            std::filesystem::path include_path(include_target);

            if (include_path.extension().empty()) {
                include_path += ".clyth";
            }

            if (include_path.is_relative()) {
                include_path = base_dir / include_path;
            }

            if (!read_clyth_source_with_includes_impl(include_path, active_stack, already_included, output)) {
                return false;
            }
            continue;
        }

        output << line << "\n";
    }

    output << "// end include unit: " << normalized.string() << "\n";
    active_stack.erase(normalized);
    return true;
}


struct StructInlineDesugarResult {
    bool ok = true;
    std::string source;
};

static bool is_ident_start(char ch) {
    return std::isalpha(static_cast<unsigned char>(ch)) || ch == '_';
}

static bool is_ident_continue(char ch) {
    return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_';
}

static std::size_t skip_ws(const std::string& source, std::size_t pos) {
    while (pos < source.size() && std::isspace(static_cast<unsigned char>(source[pos]))) {
        ++pos;
    }
    return pos;
}

static bool starts_keyword_at(const std::string& source, std::size_t pos, const std::string& keyword) {
    if (pos + keyword.size() > source.size() || source.compare(pos, keyword.size(), keyword) != 0) {
        return false;
    }
    const bool left_ok = pos == 0 || !is_ident_continue(source[pos - 1]);
    const bool right_ok = pos + keyword.size() >= source.size() || !is_ident_continue(source[pos + keyword.size()]);
    return left_ok && right_ok;
}

static std::size_t find_matching_brace(const std::string& source, std::size_t open_pos) {
    std::size_t depth = 0;
    bool in_string = false;
    bool escaped = false;

    for (std::size_t i = open_pos; i < source.size(); ++i) {
        const char ch = source[i];
        if (in_string) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (ch == '"') {
                in_string = false;
            }
            continue;
        }
        if (ch == '"') {
            in_string = true;
            continue;
        }
        if (ch == '{') {
            ++depth;
        } else if (ch == '}') {
            if (depth == 0) {
                return std::string::npos;
            }
            --depth;
            if (depth == 0) {
                return i;
            }
        }
    }

    return std::string::npos;
}

static std::string trim_copy(std::string text) {
    auto not_space = [](unsigned char ch) { return !std::isspace(ch); };
    text.erase(text.begin(), std::find_if(text.begin(), text.end(), not_space));
    text.erase(std::find_if(text.rbegin(), text.rend(), not_space).base(), text.end());
    return text;
}

static std::vector<std::string> split_struct_body_items(const std::string& body) {
    std::vector<std::string> items;
    std::size_t start = 0;
    std::size_t depth_paren = 0;
    std::size_t depth_brace = 0;
    std::size_t depth_bracket = 0;
    bool in_string = false;
    bool escaped = false;

    auto push_item_until = [&](std::size_t end_exclusive) {
        std::string item = trim_copy(body.substr(start, end_exclusive - start));
        if (!item.empty()) {
            items.push_back(item);
        }
        start = end_exclusive;
    };

    for (std::size_t i = 0; i < body.size(); ++i) {
        const char ch = body[i];
        if (in_string) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (ch == '"') {
                in_string = false;
            }
            continue;
        }
        if (ch == '"') {
            in_string = true;
            continue;
        }
        if (ch == '(') { ++depth_paren; continue; }
        if (ch == ')' && depth_paren > 0) { --depth_paren; continue; }
        if (ch == '[') { ++depth_bracket; continue; }
        if (ch == ']' && depth_bracket > 0) { --depth_bracket; continue; }
        if (ch == '{') { ++depth_brace; continue; }
        if (ch == '}' && depth_brace > 0) {
            --depth_brace;
            if (depth_brace == 0 && depth_paren == 0 && depth_bracket == 0) {
                push_item_until(i + 1);
            }
            continue;
        }

        if (depth_paren == 0 && depth_brace == 0 && depth_bracket == 0) {
            if (ch == ';' || ch == ',') {
                push_item_until(i + 1);
                continue;
            }

            if (ch == '\n') {
                // Clyth permits semicolon-less field declarations. A field inside
                // a struct is usually a single top-level line like `int32 value` or
                // `private int32 value`. Method signatures include `(` and are
                // completed when their body-closing brace is seen above.
                std::string candidate = trim_copy(body.substr(start, i - start));
                if (!candidate.empty() && candidate.find('(') == std::string::npos) {
                    items.push_back(candidate);
                    start = i + 1;
                }
            }
        }
    }

    std::string tail = trim_copy(body.substr(start));
    if (!tail.empty()) {
        items.push_back(tail);
    }

    return items;
}

static bool looks_like_inline_method(const std::string& item) {
    const auto paren = item.find('(');
    const auto brace = item.find('{');
    if (paren == std::string::npos || brace == std::string::npos || paren > brace) {
        return false;
    }
    return item.find('}') != std::string::npos;
}

static std::string strip_visibility_prefix(std::string item) {
    std::string trimmed = trim_copy(item);
    if (starts_keyword_at(trimmed, 0, "public")) {
        return trim_copy(trimmed.substr(6));
    }
    if (starts_keyword_at(trimmed, 0, "private")) {
        // Alpha 0.2 accepts the spelling so code can be written in the intended
        // style. Visibility enforcement is intentionally a semantic pass later.
        return trim_copy(trimmed.substr(7));
    }
    return item;
}

static std::string normalize_constructor_item(std::string item) {
    std::string trimmed = trim_copy(item);
    if (starts_keyword_at(trimmed, 0, "constructor")) {
        return "void " + trimmed;
    }
    if (starts_keyword_at(trimmed, 0, "destructor")) {
        return "void " + trimmed;
    }
    return item;
}

static StructInlineDesugarResult desugar_inline_struct_methods(const std::string& source) {
    StructInlineDesugarResult result;
    std::ostringstream out;
    std::size_t pos = 0;

    while (pos < source.size()) {
        if (!starts_keyword_at(source, pos, "struct")) {
            out << source[pos++];
            continue;
        }

        const std::size_t struct_start = pos;
        std::size_t cursor = skip_ws(source, pos + 6);
        if (cursor >= source.size() || !is_ident_start(source[cursor])) {
            out << source[pos++];
            continue;
        }

        const std::size_t name_start = cursor;
        while (cursor < source.size() && is_ident_continue(source[cursor])) {
            ++cursor;
        }
        const std::string struct_name = source.substr(name_start, cursor - name_start);
        cursor = skip_ws(source, cursor);
        if (cursor >= source.size() || source[cursor] != '{') {
            out << source[pos++];
            continue;
        }

        const std::size_t open_brace = cursor;
        const std::size_t close_brace = find_matching_brace(source, open_brace);
        if (close_brace == std::string::npos) {
            result.ok = false;
            result.source = source;
            return result;
        }

        std::size_t after = close_brace + 1;
        if (after < source.size() && source[after] == ';') {
            ++after;
        }

        const std::string body = source.substr(open_brace + 1, close_brace - open_brace - 1);
        const std::vector<std::string> items = split_struct_body_items(body);
        std::vector<std::string> fields;
        std::vector<std::string> methods;

        for (std::string item : items) {
            item = strip_visibility_prefix(item);
            item = normalize_constructor_item(item);
            if (looks_like_inline_method(item)) {
                methods.push_back(item);
            } else if (!trim_copy(item).empty()) {
                fields.push_back(trim_copy(item));
            }
        }

        out << "struct " << struct_name << " {\n";
        for (const auto& field : fields) {
            out << "    " << field;
            const std::string trimmed = trim_copy(field);
            if (!trimmed.empty() && trimmed.back() != ';' && trimmed.back() != ',') {
                out << ";";
            }
            out << "\n";
        }
        out << "}\n";

        if (!methods.empty()) {
            out << "\n" << struct_name << " {\n";
            for (const auto& method : methods) {
                out << method << "\n";
            }
            out << "}\n";
        }

        pos = after;
        (void)struct_start;
    }

    result.source = out.str();
    return result;
}

static bool read_clyth_source_with_includes(
    const std::filesystem::path& main_file,
    std::string& source_code
) {
    std::set<std::filesystem::path> active_stack;
    std::set<std::filesystem::path> already_included;
    std::ostringstream output;

    if (!read_clyth_source_with_includes_impl(main_file, active_stack, already_included, output)) {
        return false;
    }

    source_code = output.str();
    return true;
}

static int parse_clyth_file(const CompilerOptions& opts) {
    std::string source_code;

    if (!read_clyth_source_with_includes(opts.main_file, source_code)) {
        return 1;
    }

    if (source_code.empty()) {
        fmt::print(stderr, "ERROR: Source file is empty: {}\n", opts.main_file.string());
        return 1;
    }

    StructInlineDesugarResult desugared = desugar_inline_struct_methods(source_code);
    if (!desugared.ok) {
        fmt::print(stderr, "ERROR: Failed to desugar inline struct methods/constructors.\n");
        return 1;
    }
    source_code = desugared.source;

    antlr4::ANTLRInputStream input(source_code);
    ClythV1Lexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    ClythV1Parser parser(&tokens);

    clyth::DiagnosticBag diagnostics;
    clyth::ClythSyntaxErrorListener lexer_errors(diagnostics);
    clyth::ClythSyntaxErrorListener parser_errors(diagnostics);

    lexer.removeErrorListeners();
    parser.removeErrorListeners();

    lexer.addErrorListener(&lexer_errors);
    parser.addErrorListener(&parser_errors);

    ClythV1Parser::ProgramContext *tree = parser.program();

    if (diagnostics.has_errors() || parser.getNumberOfSyntaxErrors() > 0) {
        diagnostics.print_to_stderr(opts.main_file);
        fmt::print(stderr, "ERROR: Parsing failed with {} syntax error(s).\n", diagnostics.error_count());
        return 1;
    }

    fmt::print("Parse succeeded for {}\n", opts.main_file.string());

    if (opts.debug_mode) {
        fmt::print("\nParse tree:\n{}\n", tree->toStringTree(&parser));
    }

    clyth::ClythAST ast_builder(diagnostics);
    clyth::ast::ProgramPtr program_ast = ast_builder.build(tree);

    if (diagnostics.has_errors() || program_ast == nullptr) {
        diagnostics.print_to_stderr(opts.main_file);
        fmt::print(stderr, "ERROR: AST generation failed with {} error(s).\n", diagnostics.error_count());
        return 1;
    }

    if (opts.debug_mode) {
        fmt::print("\nClyth AST:\n");
        clyth::ast::print_ast(program_ast);
    }

    if (opts.dump_ast_json_stdout) {
        fmt::print("\n{}\n", clyth::ast::to_json(program_ast));
    }

    if (!opts.ast_json_file.empty()) {
        if (!clyth::ast::write_json_file(program_ast, opts.ast_json_file)) {
            fmt::print(stderr, "ERROR: Failed to write AST JSON file: {}\n", opts.ast_json_file.string());
            return 1;
        }

        fmt::print("Wrote AST JSON: {}\n", opts.ast_json_file.string());
    }

    if (opts.dump_ast_bytecode_stdout) {
        fmt::print("\n{}\n", clyth::ast::to_clyth_bytecode(program_ast));
    }

    if (!opts.ast_bytecode_file.empty()) {
        if (!clyth::ast::write_bytecode_file(program_ast, opts.ast_bytecode_file)) {
            fmt::print(stderr, "ERROR: Failed to write AST bytecode file: {}\n", opts.ast_bytecode_file.string());
            return 1;
        }

        fmt::print("Wrote AST bytecode: {}\n", opts.ast_bytecode_file.string());
    }

    clyth::semantic::ClythSemanticPipeline semantic_pipeline(diagnostics);
    clyth::semantic::SemanticResult semantic_result = semantic_pipeline.analyze(program_ast);

    if (!semantic_result.ok || diagnostics.has_errors()) {
        diagnostics.print_to_stderr(opts.main_file);
        fmt::print(stderr, "ERROR: Semantic analysis failed with {} error(s).\n", diagnostics.error_count());
        return 1;
    }

    fmt::print("Semantic analysis completed for {}\n", opts.main_file.string());

    if (opts.debug_mode) {
        fmt::print("\n");
        semantic_result.print_summary();
    }

    clyth::lowering::ClythLoweringPlan lowering_plan =
        clyth::lowering::ClythLoweringPlanBuilder::build(program_ast, semantic_result, diagnostics);

    if (diagnostics.has_errors()) {
        diagnostics.print_to_stderr(opts.main_file);
        fmt::print(stderr, "ERROR: Lowering-plan generation failed with {} error(s).\n", diagnostics.error_count());
        return 1;
    }

    if (opts.debug_mode) {
        fmt::print("\n");
        lowering_plan.print();
    }

    clyth::llvm_stub::ClythLLVMCodegen codegen(diagnostics);
    clyth::llvm_stub::CodegenConfig codegen_config;
    codegen_config.module_name = opts.main_file.stem().string();
    codegen_config.dump_codegen_plan = opts.dump_codegen_plan || opts.debug_mode;
    codegen_config.output_binary_path = opts.output_binary_name;
    std::filesystem::path ir_output_path = opts.output_binary_name;
    ir_output_path.replace_extension(".ll");
    codegen_config.output_ir_path = ir_output_path;
    codegen_config.compile_to_executable = !opts.emit_ir_only;
    codegen_config.print_ir_to_stdout = opts.print_ir || opts.debug_mode;

    if (!codegen.emit(lowering_plan, semantic_result, codegen_config) || diagnostics.has_errors()) {
        diagnostics.print_to_stderr(opts.main_file);
        fmt::print(stderr, "ERROR: LLVM codegen stub failed with {} error(s).\n", diagnostics.error_count());
        return 1;
    }

    fmt::print("Frontend pipeline completed. LLVM IR generation completed.\n");
    return 0;
}

int main(int argc, char** argv) {
    std::string compiler_version("Cobralyth Compiler CPP: Version 1.0.0");
    cxxopts::Options options("clythcpp", compiler_version.c_str());

    CompilerOptions opts;

    options.add_options()
        ("d,debug", "Debug Mode enabled flag", cxxopts::value<bool>()->default_value("false"))
        ("c,compile", "Program Entrypoint", cxxopts::value<std::string>()->default_value("main.clyth"))
        ("o,output", "Output Binary Name", cxxopts::value<std::string>()->default_value("clyth_program.bin"))
        ("ast-json", "Write human-readable AST JSON to file", cxxopts::value<std::string>()->default_value(""))
        ("dump-ast-json", "Print human-readable AST JSON to stdout", cxxopts::value<bool>()->default_value("false"))
        ("ast-bytecode", "Write Clyth AST bytecode/debug format to file", cxxopts::value<std::string>()->default_value(""))
        ("dump-ast-bytecode", "Print Clyth AST bytecode/debug format to stdout", cxxopts::value<bool>()->default_value("false"))
        ("dump-codegen-plan", "Print LLVM codegen activity", cxxopts::value<bool>()->default_value("false"))
        ("emit-ir-only", "Write LLVM IR but do not link an executable", cxxopts::value<bool>()->default_value("false"))
        ("print-ir", "Print generated LLVM IR to stdout", cxxopts::value<bool>()->default_value("false"))
        ("show-licenses", "Print Clyth and bundled third-party license text", cxxopts::value<bool>()->default_value("false"))
        ("v,version", "Version of compiler", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "Display supported commands", cxxopts::value<bool>()->default_value("false"));

    try {
        auto result = options.parse(argc, argv);

        if (result["help"].as<bool>()) {
            fmt::print("{}\n", options.help());
            return 0;
        }

        if (result["version"].as<bool>()) {
            fmt::print("{}\n", compiler_version);
            return 0;
        }

        if (result["show-licenses"].as<bool>()) {
            const std::array<std::filesystem::path, 11> license_candidates {
                "../LICENSE",
                "../compiler-src/EXTERNAL_LIBRARIES_LICENSES.md",
                "../clyth-runtime/EXTERNAL_LIBRARIES_LICENSES.md",
                "LICENSE",
                "EXTERNAL_LIBRARIES_LICENSES.md",
                "licenses/CLYTH_LICENSE",
                "licenses/EXTERNAL_LIBRARIES_LICENSES.md",
                "licenses/RUNTIME_EXTERNAL_LIBRARIES_LICENSES.md",
                "licenses/ANTLR4_RUNTIME_LICENSE.txt",
                "licenses/FMT_LICENSE",
                "licenses/LLVM_LICENSE.TXT"
            };

            bool printed_any = false;
            for (const auto& path : license_candidates) {
                if (!std::filesystem::exists(path)) {
                    continue;
                }

                std::ifstream file(path);
                if (!file) {
                    continue;
                }

                fmt::print("\n===== {} =====\n", path.string());
                fmt::print("{}", std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
                fmt::print("\n");
                printed_any = true;
            }

            if (!printed_any) {
                fmt::print(stderr, "No license files were found near the compiler binary.\n");
                return 1;
            }

            return 0;
        }

        opts.debug_mode = result["debug"].as<bool>();
        opts.output_binary_name = result["output"].as<std::string>();
        opts.main_file = result["compile"].as<std::string>();
        opts.dump_ast_json_stdout = result["dump-ast-json"].as<bool>();
        opts.dump_ast_bytecode_stdout = result["dump-ast-bytecode"].as<bool>();
        opts.dump_codegen_plan = result["dump-codegen-plan"].as<bool>();
        opts.emit_ir_only = result["emit-ir-only"].as<bool>();
        opts.print_ir = result["print-ir"].as<bool>();

        const std::string ast_json = result["ast-json"].as<std::string>();
        const std::string ast_bytecode = result["ast-bytecode"].as<std::string>();

        if (!ast_json.empty()) {
            opts.ast_json_file = ast_json;
        }

        if (!ast_bytecode.empty()) {
            opts.ast_bytecode_file = ast_bytecode;
        }

    } catch (const std::exception& e) {
        fmt::print(stderr, "ERROR occurred: {}\nExiting compilation\n", e.what());
        return 1;
    }

    if (!std::filesystem::exists(opts.main_file)) {
        fmt::print(stderr, "ERROR: Source file does not exist: {}\n", opts.main_file.string());
        return 1;
    }

    if (opts.main_file.extension() != ".clyth") {
        fmt::print(stderr, "ERROR: Ensure the main program file contains the '.clyth' file extension.\n");
        return 1;
    }

    fmt::print("Compiling {} -> {}\n", opts.main_file.string(), opts.output_binary_name);

    return parse_clyth_file(opts);
}
