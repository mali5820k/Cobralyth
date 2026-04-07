#ifndef SCANNER_HPP
#define SCANNER_HPP
#include "common.hpp"

// Utility class for reading files for tokenization:
class Scanner {
public:
    Scanner();
    ~Scanner();
    bool read_file(std::string);
    std::vector<std::string> get_contents();
    std::string to_string();
    void print_output();
private:
    std::vector<std::string> lines;
    std::string assembled_string;
};

#endif