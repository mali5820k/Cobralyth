#include "lexer.hpp"

// Lexer Class methods:
Lexer::Lexer() {}
Lexer::~Lexer() {}

bool Lexer::lexFile(std::string file_name) {
    if (!this->file_scanner->readFile(file_name)) {
        this->lexer_error_string.push_back(std::string("ERROR: Could not open file for lexing... " + file_name));
        return false;
    }
    if (!this->lexInput(this->file_scanner->getContents())) {
        this->lexer_error_string.push_back("ERROR: Could not lex scanned input.");
        return false;
    }

    return true;
}

bool Lexer::lexInput(std::vector<std::string> file_contents) {
    return true;
}

std::unique_ptr<LexerTreeHead> createLexerTreeFromMap(std::map<std::string, TokenType> tokens_map) {
    std::unique_ptr<LexerTreeHead> newLexerTreeHead;
    return newLexerTreeHead;
}

bool setKeywordsTree(std::unique_ptr<LexerTreeHead> tree_head) {
    return true;
}

bool setSymbolsTree(std::unique_ptr<LexerTreeHead> tree_head) {
    return true;
}

std::string Lexer::toString() {
    std::string token_string = "";
    for (auto &token : this->tokens_list) {
        token_string += token.value;
    }
    return token_string;
}

// LexerTreeNode Class methods:
LexerTreeNode::LexerTreeNode() {}

LexerTreeNode::~LexerTreeNode() {}

bool LexerTreeNode::hasNeighbor(std::string target_letter) {
    return true;
}

LexerTreeNode* LexerTreeNode::getNext(std::string target_letter) {
    return NULL; // Null pointer for now
}

std::string LexerTreeNode::toString() {
    return std::string("Sample string");
}

// LexerTreeHead
LexerTreeHead::LexerTreeHead() {}
LexerTreeHead::~LexerTreeHead() {}
bool LexerTreeHead::hasNeighbor(std::string target_letter) {
    return true;
}
bool LexerTreeHead::insertLexerTreeNode(LexerTreeNode new_node_entry) {
    return true;
}