#ifndef TRIE_HPP
#define TRIE_HPP
#include "token.hpp"

class TrieEntry {
public:
    TrieEntry();
    ~TrieEntry();
    bool addNeighbor(char character, TrieEntry *neighbor_entry);
    std::vector<char> getNeighbors();
    TrieEntry* getNeighbor(char character);
    std::string toString();
    TokenType token_type;
    char character;
private:
    std::map<char, std::unique_ptr<TrieEntry>> neighbors_map;
};

class TrieHead {
public:
    TrieHead();
    ~TrieHead();
    bool createTrie(std::vector<std::string, TokenType> reserved_words);
    std::map<char, TrieEntry> trie_structure;
    std::string toString();
private:

};

#endif