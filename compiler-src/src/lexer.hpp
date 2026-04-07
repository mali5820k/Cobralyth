#include <vector>
#include <string>
#include "token.hpp"
#include "scanner.hpp"

class Lexer {
public:
    Lexer();
    ~Lexer();
    bool lex_file(std::string file_name);
    bool lex_input(std::vector<std::string> file_contents);
    std::string to_string();
private:

    std::string look_ahead();
    std::string current_character();
    std::string consume_character();

    void lex_string();
    void lex_backtick_string();
    void lex_char();
    void lex_numeric();
    void lex_variable();
    void lex_identifier();
    void lex_expression();
    void lex_statement();

    std::string current_file_name = "";
    int64_t file_contents_char_idx = 0;
    int64_t file_contents_line_idx = 0;
    int64_t current_char_idx = 0;
    int64_t current_line_idx = 0;
    std::vector<Token> tokens_list;
    Scanner file_scanner;
    std::vector<std::string> lexer_error_string;
};
