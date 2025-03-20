#ifndef SCANNER_HPP
#define SCANNER_HPP
#include <cstdbool>
#include <cstdint>
#include <vector>

// Utility class for reading files for tokenization:
class Scanner {

public:
    Scanner();
    ~Scanner();
    bool readFile(std::string);
    std::vector<std::string> getContents();
private:
    std::vector<std::string> lines;
};

#endif