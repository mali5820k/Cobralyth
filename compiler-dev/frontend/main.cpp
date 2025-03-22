#include "common.hpp"
#include "cxxopts/cxxopts.hpp"
#include "scanner.hpp"
#include "lexer.hpp"
//#include "parser.hpp"

int main(int argc, char** argv) {
    
    // Metadata and generate commandline flags:
    std::string compiler_version("Cobralyth Compiler CPP: Version 1.0.0");
    cxxopts::Options options("clythcpp", compiler_version.c_str());
    std::filesystem::path main_file;
    std::string output_binary_name = std::string("clyth_program.bin");
    options.add_options()
    ("d,debug", "Debug Mode enabled flag", cxxopts::value<bool>()->default_value("false"))
    ("c,compile", "Program Entrypoint", cxxopts::value<std::string>()->default_value("main.clyth"))
    ("o,output", "Output Binary Name", cxxopts::value<std::string>()->default_value("clyth_program.bin"))
    ("v,version", "Version of compiler", cxxopts::value<bool>()->default_value("false"))
    ("h,help", "Display supported commands", cxxopts::value<bool>()->default_value("false"))
    ;
    try {
        auto result = options.parse(argc, argv);
        bool debug_mode = false;
        bool flag_provided = false;
        std::string program_entrypoint;

        if (result.count("help")) {
            std::println("{}\n", options.help().c_str());
            flag_provided = true;
        }

        // Debug-mode check:
        if (result.count("debug")) {
            debug_mode = true;
            flag_provided = true;
        }

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
            std::println("{}\n", compiler_version);
            flag_provided = true;
        }

        if (!flag_provided) {
            throw std::runtime_error("Invalid program arguments, please run with -h flag for sample usage.");
        }

        // Get the main-file provided by the user:
        main_file = program_entrypoint;
    } catch(std::exception& e) {
        std::println("ERROR occurred: {}\nExiting compilation\n", e.what());
        return 1;
    }

    if (std::filesystem::exists(main_file)) {
        if (main_file.extension() != std::string(".clyth")) {
            std::println("ERROR: Ensure the main program file contains the '.clyth' file extension.\nExiting...");
            return 1;
        }

        std::println("Compiling {}\n", main_file.c_str());
        

        // Now to lex this result:
        Lexer lexer = Lexer();
        lexer.lexFile((std::string)main_file);
    }

    return 0;
}