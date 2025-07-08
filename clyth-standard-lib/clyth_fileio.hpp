#ifndef CLYTH_FILEIO_HPP
#define CLYTH_FILEIO_HPP

#include "clyth_common.hpp"
#include <string>
#include <vector>

/**
 * Supported file operation modes
 */
enum FILE_MODE {
    APPEND, WRITE, READ
};

/**
 * FileHandler allows for a scoped-file access where a file can be securely operated on with read
 * and write operations.
 */
class FileHandler {
public:
    FileHandler();
    FileHandler(std::string file_name);
    ~FileHandler();

    bool exists();
    bool exists(std::string file_path);
    bool isFile();
    bool isFile(std::string file_path);
    bool isDir();
    bool isDir(std::string file_path);
    bool isSymlink();
    bool isSymlink(std::string file_path);
    bool createFile();
    bool createFile(std::string file_path);
    bool deleteFile();
    bool deleteFile(std::string file_path);

    std::string readLine();
    std::string readLine(std::string file_name);
    std::vector<std::string> readLines();
    std::vector<std::string> readLines(std::string file_name);
    std::vector<std::string> split(std::string, std::string);
    bool write(std::string data, FILE_MODE mode);
    bool write(std::string file_name, std::string content, FILE_MODE mode);
    bool writeLines(std::vector<std::string> content_vector, FILE_MODE mode);
    bool writeLines(std::string file_name, std::vector<std::string> content_vector, FILE_MODE mode);

private:
    int64_t cur_line = 0;
    std::string file_name;
};

#endif
