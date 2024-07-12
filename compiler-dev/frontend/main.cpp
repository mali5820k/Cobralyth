#include "common.hpp"
#include "cxxopts.hpp"

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
        std::string program_entrypoint;
        

        if (result.count("help")) {
            std::printf("%s\n", options.help().c_str());
        }

        // Debug-mode check:
        if (result.count("debug")) {
            debug_mode = true;
        }

        if (result.count("output")) {
            output_binary_name = result["output"].as<std::string>();
        }

        // Get the path to the file being compiled:
        if (result.count("compile")) {
            program_entrypoint = result["compile"].as<std::string>();
        }

        // Version flag check:
        if (result.count("version")) {
            std::printf("%s\n", compiler_version.c_str());
        }

        // Get the main-file provided by the user:
        main_file = program_entrypoint;
    } catch(std::exception& e) {
        std::printf("ERROR occurred: %s\nExiting compilation\n", e.what());
        return 1;
    }

    if (std::filesystem::exists(main_file)) {
        if (main_file.extension() != std::string(".clyth")) {
            std::printf("ERROR: Ensure the main program file contains the '.clyth' file extension.\nExiting...");
            return 1;
        }

        std::printf("Compiling %s\n", main_file.c_str());
    }

    return 0;
}