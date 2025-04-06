#include <vector>
#include <string>
#include "token.hpp"
#include "scanner.hpp"

class Lexer {
public:
    Lexer();
    ~Lexer();
    bool lexFile(std::string file_name);
    bool lexInput(std::vector<std::string> file_contents);
    std::string toString();

    bool addKeywords();
    bool addSymbols();

private:
    std::vector<Token> lexImportedFiles();
    std::vector<Token> lexImportedFile(std::string file_name); // Threadable;

    std::vector<Token> tokens_list;
    std::map<std::string, TokenType> keywords_lookup_map;
    std::map<std::string, TokenType> symbols_lookup_map; // Includes operators like *, +, -, {, (, ...
    std::unique_ptr<Scanner> file_scanner = std::make_unique<Scanner>();
    std::map<std::string, bool> import_files_tracker;
    std::vector<std::string> lexer_error_string;
};