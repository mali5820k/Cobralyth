#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "astgen.hpp"
#include <vector>
#include <map>
/**
 * Package-based imports instead of direct file-based imports for the sake of linker-tracing simplification.
 * Packages are simply folders with unique names.
 * All files within a package are able to access one another's public contents
 * One package-header file exists in each package that will be imported when package is imported.
*/
std::string packagesDirPath;
std::map<std::string, bool> nestedPackages;

int main (int argc, char** argv) {
    // Only open .clyth files
    // Lex starting file *usually the file containing the main function*
    // The lexer will auto-populate the nestedPackages list with the appropriate paths and mark if imported
    // parse the lexer's tokens and generate AST
    // Codegen AST to target language

    // Print Status on exit.

    return 0;
}
