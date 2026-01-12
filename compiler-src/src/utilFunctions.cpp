#include "common.hpp"

/**
 * Returns true or false if file path exists.
 * @param file_name path containing name of file
 * @returns bool
 */
bool fileExists(std::string file_name) {
    std::filesystem::path file_path(file_name);
    return std::filesystem::exists(file_path);
}

/**
 * Reads all lines from withing a given file's name if it exists.
 * @param file_name path containing name of file
 * @returns std::vector<std::string> with scanned lines or an empty vector if invalid.
 */
std::vector<std::string> readLines(std::string file_name) {
    std::vector<std::string> scanned_lines;
    if (!fileExists(file_name)) {
        printf("\nERROR: File path %s does NOT exist\n", file_name.c_str());
        return scanned_lines;
    }
    std::ifstream file(file_name);
    std::string current_line;
    while (std::getline(file, current_line)) {
        scanned_lines.push_back(current_line);
    }
    return scanned_lines;
}

/**
 * Splits a given string via delimitter.
 * @param input_string The string to split via delimitter
 * @param delimitter The delimitter string pattern to split by
 * @return std::vector<std::string> vectors of strings
 */
std::vector<std::string> split(std::string input_string, std::string delimitter) {
    bool search_exhausted = false;
    std::vector<std::string> split_vector;
    std::size_t substring_found;
    std::size_t last_start = 0;
    std::string substring_value(input_string);
    while (!search_exhausted) {
        printf("Now searching at index %zu\n", last_start);
        substring_found = input_string.find(delimitter, last_start);
        printf("Substring found at: %zu", substring_found);
        if (substring_found == std::string::npos) {
            search_exhausted = true;
            substring_value = input_string.substr(last_start, input_string.length()-last_start);
        } else {
            substring_value = input_string.substr(last_start, substring_found-last_start);
        }
        printf("Substring length should be: %zu", substring_found - last_start);
        printf("Adding substring: %s", substring_value.c_str());
        split_vector.emplace_back(substring_value);
        last_start = substring_found + delimitter.length();
        if (last_start >= input_string.length()) {
            return split_vector;
        }
    }

    return split_vector;
}