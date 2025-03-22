#include<vector>
#include "token.hpp"
#include "scanner.hpp"

// Tree/Trie - same purpose of branching keywords.
class LexerTreeNode {
public:
    std::string current_letter;
    std::map<std::string, LexerTreeNode> neighbors;
    LexerTreeNode();
    ~LexerTreeNode();
    std::unique_ptr<LexerTreeNode> createNewNode(std::string character);
    bool hasNeighbor(std::string target_letter);
    LexerTreeNode* getNext(std::string target_letter);
    
    std::string toString();
};
class LexerTreeHead {
public:
    bool hasNeighbor(std::string target_letter);
    std::unique_ptr<LexerTreeNode> getNext(std::string target_letter);
    bool insertLexerTreeNode(LexerTreeNode new_node_entry);
    std::string toString();
};

class Lexer {
public:
    Lexer();
    ~Lexer();
    bool lexFile(std::string file_name);
    bool lexInput(std::vector<std::string> file_contents);
    std::string toString();
    std::unique_ptr<LexerTreeHead> createLexerTreeFromMap(std::map<std::string, TokenType> tokens_map);
    bool setKeywordsTree(std::unique_ptr<LexerTreeHead> tree_head);
    bool setSymbolsTree(std::unique_ptr<LexerTreeHead> tree_head);
private:
    std::vector<Token> lexImportedFiles();
    std::vector<Token> lexImportedFile(std::string file_name); // Threadable;

    std::vector<Token> tokens_list;
    std::unique_ptr<LexerTreeNode> keywords_tree;
    std::unique_ptr<LexerTreeNode> symbols_tree;
    std::unique_ptr<Scanner> file_scanner;
    std::map<std::string, bool> import_files_tracker;
    std::vector<std::string> lexer_error_string;
};