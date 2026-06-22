#include "common.hpp"
#include "cxxopts/cxxopts.hpp"
#include "scanner.hpp"
#include "clyth_ast.hpp"
#include "clyth_semantic.hpp"
#include "clyth_lowering_plan.hpp"
#include "clyth_llvm_stub.hpp"

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

    std::filesystem::path main_file;
    std::filesystem::path ast_json_file;
    std::filesystem::path ast_bytecode_file;
    std::string output_binary_name = "clyth_program.bin";
};

static int parse_clyth_file(const CompilerOptions& opts) {
    Scanner scanner;

    if (!scanner.read_file(opts.main_file.string())) {
        fmt::print(stderr, "ERROR: Failed to read source file: {}\n", opts.main_file.string());
        return 1;
    }

    const std::string source_code = scanner.to_string();

    if (source_code.empty()) {
        fmt::print(stderr, "ERROR: Source file is empty: {}\n", opts.main_file.string());
        return 1;
    }

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
        ("dump-codegen-plan", "Print LLVM codegen stub activity", cxxopts::value<bool>()->default_value("false"))
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

        opts.debug_mode = result["debug"].as<bool>();
        opts.output_binary_name = result["output"].as<std::string>();
        opts.main_file = result["compile"].as<std::string>();
        opts.dump_ast_json_stdout = result["dump-ast-json"].as<bool>();
        opts.dump_ast_bytecode_stdout = result["dump-ast-bytecode"].as<bool>();
        opts.dump_codegen_plan = result["dump-codegen-plan"].as<bool>();

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
