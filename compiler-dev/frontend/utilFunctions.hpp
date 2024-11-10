#ifndef UTIL_FUNCTIONS_HPP
#define UTIL_FUNCTIONS_HPP
#include <string>
#include <vector>

bool file_exists(std::string file_path);
std::vector<std::string> read_lines(std::string file_name);
std::vector<std::string> split(std::string, std::string);

#endif