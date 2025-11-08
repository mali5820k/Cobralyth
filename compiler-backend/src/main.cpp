#include "common.hpp"
#include "cxxopts/cxxopts.hpp"
#include "scanner.hpp"
#include "json.hpp"

//#include "parser.hpp"

int main(int argc, char** argv) {
    // Metadata and generate commandline flags:
    std::string compiler_version("Cobralyth Compiler CPP: Version 1.0.0");
    cxxopts::Options options("clythcpp", compiler_version.c_str());
    std::filesystem::path main_file;
    std::string output_binary_name = std::string("main.bin");
    std::string default_program_json_file_name = std::string("main.json");
    options.add_options()
    ("d,debug", "Debug Mode enabled flag", cxxopts::value<bool>()->default_value("false"))
    ("c,compile", "Program JSON Instructions file", cxxopts::value<std::string>()->default_value(default_program_json_file_name))
    ("o,output", "Output Binary Name", cxxopts::value<std::string>()->default_value(output_binary_name))
    ("v,version", "Version of compiler", cxxopts::value<bool>()->default_value("false"))
    ("h,help", "Display supported commands", cxxopts::value<bool>()->default_value("false"))
    ;
    try {
        auto result = options.parse(argc, argv);
        bool debug_mode = false;
        bool flag_provided = false;
        std::string program_entrypoint;

        if (result.count("help")) {
            printf("%s\n", options.help().c_str());
            flag_provided = true;
            return 0;
        }

        // Debug-mode check:
        if (result.count("debug")) {
            debug_mode = true;
            flag_provided = true;
        }

        // Output binary name check:
        if (result.count("output")) {
            output_binary_name = result["output"].as<std::string>();
            flag_provided = true;
        }

        // Get the path to the file being compiled:
        if (result.count("compile")) {
            program_entrypoint = result["compile"].as<std::string>();
            flag_provided = true;
        }

        // Version flag check:
        if (result.count("version")) {
            printf("%s\n", compiler_version.c_str());
            flag_provided = true;
        }

        if (!flag_provided) {
            throw std::runtime_error("Invalid program arguments, please run with -h flag for sample usage.");
        }

        // Get the main-file provided by the user:
        main_file = program_entrypoint;
    } catch(std::exception& e) {
        printf("ERROR occurred: %s\nExiting compilation\n", e.what());
        return 1;
    }

    if (!std::filesystem::exists(main_file)) {
        printf("ERROR: Ensure the main program json file\nExiting...");
        return 1;
    }

    printf("Proceeding with compile for: %s\n", main_file.c_str());
    
    // The AST from the main.json file is expected to be linearized - if it's not, the codegen will have unintended or unknown behavior once it's mapped to LLVM IR.


    return 0;
}