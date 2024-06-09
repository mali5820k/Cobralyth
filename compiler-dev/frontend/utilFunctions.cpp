#include "common.hpp"

bool file_exists(std::string file_name) {
    std::filesystem::path file_path(file_name);
    return std::filesystem::exists(file_path);
}

std::vector<std::string> read_lines(std::string file_name) {
    if (!file_exists(file_name)) {
        std::println("\nERROR: File path {} does NOT exist\n", file_name);
        std::exit(-1);
    }
    std::ifstream file(file_name);
    std::vector<std::string> scanned_lines;
    return scanned_lines;
}

std::vector<std::string> split(std::string input_string, std::string delimitter) {
    bool search_exhausted = false;
    std::vector<std::string> split_vector;
    std::size_t substring_found;
    std::size_t last_start = 0;
    std::string substring_value(input_string);
    while (!search_exhausted) {
        std::println("Now searching at index {}\n", last_start);
        substring_found = input_string.find(delimitter, last_start);
        std::println("Substring found at: {}", substring_found);
        if (substring_found == std::string::npos) {
            search_exhausted = true;
            substring_value = input_string.substr(last_start, input_string.length()-last_start);
        } else {
            substring_value = input_string.substr(last_start, substring_found-last_start);
        }
        std::println("Substring length should be: {}", substring_found - last_start);
        std::println("Adding substring: {}", substring_value);
        split_vector.emplace_back(substring_value);
        last_start = substring_found + delimitter.length();
        if (last_start >= input_string.length()) {
            return split_vector;
        }
    }

    return split_vector;
}