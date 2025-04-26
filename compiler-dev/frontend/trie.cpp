#include "trie.hpp"


TrieEntry::TrieEntry(char character, TokenType token_type) {
    this->character = character;
    this->token_type = token_type;
}
TrieEntry::~TrieEntry() {}

bool TrieEntry::addNeighbor(char character, TrieEntry *neighbor_entry) {
    this->neighbors_map.emplace(character, std::unique_ptr<TrieEntry> neighbor(neighbor_entry));
    return true;
}

std::string TrieEntry::toString() {
    return std::string self(this.character) 
}

TrieHead::TrieHead() {}
TrieHead::~TrieHead() {}

bool TrieHead::createTrie(std::vector<std::string, TokenType> reserved_words) {
    for (const auto &[str, value] : reserved_words) {
        if (!str[0]) {
            return false;
        }

        TrieEntry *last_entry;
        last_entry = this->trie_structure.find(str[0]);

        if (last_entry == this->trie_structure.end()) {
            // Then loop through the remainder of the string and
            // create a new Trie Node;
            this->trie_structure.emplace(str[0], TrieEntry(str[0], UNDEFINED));
            last_entry = this->tri_structure.get(str[0]);
            for (int i = 1; i < str.length(); i++) {
                last_entry->neighbors_map.emplace(str[i], TokenEntry(str[i], UNDEFINED));
                if (last_entry->getNeighbor(str[i] != last_entry->neibors_map.end())) {
                    last_entry = last_entry->getNeighbor(str[i]);
                }
            }
            if (last_entry->token_type != value) {
                last_entry->token_type = value;
            }
            return true;
        }
        for (int i = 0; i < str.length(); i++) {
            
            TrieEntry *new_entry = new TrieEntry(str[i])

            if (i == str.length() -1) {
                // add the TokenType to this TriEntry:
                new_entry->token_type = value;
            }
        }
    }
}

std::string TrieHead::toString() {
    std::string token_string = "";
    for (auto &token : this->tokens_list) {
        token_string += token.value;
    }
    return token_string;
}