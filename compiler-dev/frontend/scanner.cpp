#include "scanner.hpp"
#include "utilFunctions.hpp"

Scanner::Scanner() {}

Scanner::~Scanner() {}

bool Scanner::readFile(std::string file_name) {
    if (file_exists(file_name)) {
        this->lines = read_lines(file_name);
        return true;
    }
    return false;
}

std::vector<std::string> Scanner::getContents() {
    return this->lines;
}

