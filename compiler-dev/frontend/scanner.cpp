#include "scanner.hpp"
#include "utilFunctions.hpp"

Scanner::Scanner() {}

Scanner::~Scanner() {}

bool Scanner::readFile(std::string file_name) {
    if (fileExists(file_name)) {
        this->lines = readLines(file_name);
        return true;
    }
    this->assembled_string = "";
    return false;
}

std::vector<std::string> Scanner::getContents() {
    return this->lines;
}

std::string Scanner::toString() {
    // If there's already contents stored in the last
    // assembled string, then we just return that string.
    if (this->assembled_string.length() > 0) {
        return this->assembled_string;
    }

    // Otherwise we will re-assemble the string for all other cases.
    for (auto &line : this->lines) {
        this->assembled_string.append(line);
    }
    return assembled_string;
}

void Scanner::printOutput() {
    std::println("{}", this->toString());
}

