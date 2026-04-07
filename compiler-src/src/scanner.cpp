#include "scanner.hpp"
#include "utilFunctions.hpp"

Scanner::Scanner() {}

Scanner::~Scanner() {}

bool Scanner::read_file(std::string file_name) {
    if (file_exists(file_name)) {
        this->lines = read_lines(file_name);
        return true;
    }
    this->assembled_string = "";
    return false;
}

std::vector<std::string> Scanner::get_contents() {
    return this->lines;
}

std::string Scanner::to_string() {
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

void Scanner::print_output() {
    printf("%s", this->to_string().c_str());
}

