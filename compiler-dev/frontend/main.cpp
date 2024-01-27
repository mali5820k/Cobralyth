#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "astgen.hpp"
#include <vector>
#include <map>
#include <filesystem>
#include <format>

/**
 * Package-based imports instead of direct file-based imports for the sake of linker-tracing simplification.
 * Packages are simply folders with unique names.
 * All files within a package are able to access one another's public contents
 * One package-header file exists in each package that will be imported when package is imported.
*/
std::string packagesDirPath;
std::map<std::string, bool> nestedPackages;

int main (int argc, char** argv) {
    // Check if any arguments are provided to the compiler before continuing:
    if (argc < 2) {
        // error out:
        std::cout << "\nERROR: No arguments were provided for compilation\n";
        return 1;
    }

    // Only open .clyth files
    // So check if from the end of the fileName "7 characters" the .clyth characters exist
    std::string programFile(argv[1]);
    if (extensionLength <= 0 || std::filesystem::path(programFile).extension() != ".clyth") {
        std::cout << std::format("\nERROR: Starting file: {} doesn't have valid extension\n", programFile);
        return 2;
    }

    // Scan starting the provided file.
    

    return 0;
}
