#include "common.hpp"
#include "cxxopts/cxxopts.hpp"
#include "scanner.hpp"

// ANTLR4 runtime includes.
#include "antlr4-runtime.h"

// Generated from ClythV1.g4.
// These headers are produced by the ANTLR4 C++ target.
#include "clyth_antlr_files/ClythV1Lexer.h"
#include "clyth_antlr_files/ClythV1Parser.h"

class ClythErrorListener : public antlr4::BaseErrorListener {
public:
    std::size_t error_count = 0;

    void syntaxError(
        antlr4::Recognizer *recognizer,
        antlr4::Token *offendingSymbol,
        std::size_t line,
        std::size_t charPositionInLine,
        const std::string &msg,
        std::exception_ptr e
    ) override {
        (void)recognizer;
        (void)offendingSymbol;
        (void)e;

        ++error_count;
        fmt::print(stderr, "Syntax error at {}:{} -> {}\n", line, charPositionInLine, msg);
    }
};

static int parse_clyth_file(const std::filesystem::path &main_file, bool debug_mode) {
    Scanner scanner;

    if (!scanner.read_file(main_file.string())) {
        fmt::print(stderr, "ERROR: Failed to read source file: {}\n", main_file.string());
        return 1;
    }

    const std::string source_code = scanner.to_string();

    if (source_code.empty()) {
        fmt::print(stderr, "ERROR: Source file is empty: {}\n", main_file.string());
        return 1;
    }

    antlr4::ANTLRInputStream input(source_code);
    ClythV1Lexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    ClythV1Parser parser(&tokens);

    ClythErrorListener lexer_errors;
    ClythErrorListener parser_errors;

    lexer.removeErrorListeners();
    parser.removeErrorListeners();

    lexer.addErrorListener(&lexer_errors);
    parser.addErrorListener(&parser_errors);

    // Parse from the grammar entrypoint.
    ClythV1Parser::ProgramContext *tree = parser.program();

    const std::size_t total_errors = lexer_errors.error_count + parser_errors.error_count;

    if (total_errors > 0 || parser.getNumberOfSyntaxErrors() > 0) {
        fmt::print(stderr, "ERROR: Parsing failed with {} syntax error(s).\n", total_errors);
        return 1;
    }

    fmt::print("Parse succeeded for {}\n", main_file.string());

    if (debug_mode) {
        fmt::print("\nParse tree:\n{}\n", tree->toStringTree(&parser));
    }

    // TODO: Replace parse-tree dump with:
    //   1. ASTBuilder visitor
    //   2. semantic analysis
    //   3. LLVM IR generation
    //   4. object/binary emission

    return 0;
}

int main(int argc, char** argv) {
    std::string compiler_version("Cobralyth Compiler CPP: Version 1.0.0");
    cxxopts::Options options("clythcpp", compiler_version.c_str());

    std::filesystem::path main_file;
    std::string output_binary_name = std::string("clyth_program.bin");

    options.add_options()
        ("d,debug", "Debug Mode enabled flag", cxxopts::value<bool>()->default_value("false"))
        ("c,compile", "Program Entrypoint", cxxopts::value<std::string>()->default_value("main.clyth"))
        ("o,output", "Output Binary Name", cxxopts::value<std::string>()->default_value("clyth_program.bin"))
        ("v,version", "Version of compiler", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "Display supported commands", cxxopts::value<bool>()->default_value("false"));

    bool debug_mode = false;

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

        debug_mode = result["debug"].as<bool>();
        output_binary_name = result["output"].as<std::string>();
        main_file = result["compile"].as<std::string>();

    } catch (const std::exception& e) {
        fmt::print(stderr, "ERROR occurred: {}\nExiting compilation\n", e.what());
        return 1;
    }

    if (!std::filesystem::exists(main_file)) {
        fmt::print(stderr, "ERROR: Source file does not exist: {}\n", main_file.string());
        return 1;
    }

    if (main_file.extension() != ".clyth") {
        fmt::print(stderr, "ERROR: Ensure the main program file contains the '.clyth' file extension.\n");
        return 1;
    }

    fmt::print("Compiling {} -> {}\n", main_file.string(), output_binary_name);

    return parse_clyth_file(main_file, debug_mode);
}
