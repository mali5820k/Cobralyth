#ifndef SCANNER_HPP
#define SCANNER_HPP
#include "common.hpp"

// Utility class for reading files for tokenization:
class Scanner {

public:
    Scanner();
    ~Scanner();
    bool readFile(std::string);
    std::vector<std::string> getContents();
    std::string toString();
    void printOutput();
private:
    std::vector<std::string> lines;
    std::string assembled_string;
};

#endif