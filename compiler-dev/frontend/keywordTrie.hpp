#ifndef keyword_trie_hpp
#define keyword_trie_hpp
#include <string>
#include <vector>

class TrieNode {
    public:
        TrieNode(char data);
        ~TrieNode();

        char getData();
        std::string getNeighbors();
    private:
        char data;
};
class Trie {
    public:
        Trie(std::vector<std::string> symbols);
        ~Trie();

        std::string trieInfo();
};

#endif