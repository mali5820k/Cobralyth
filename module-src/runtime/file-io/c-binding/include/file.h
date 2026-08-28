#ifndef CLYTH_RUNTIME_FILE_H
#define CLYTH_RUNTIME_FILE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

char* absolute_path(const char* path);
char* path_dirname_copy(const char* path);
char* glob_text(const char* pattern);

int32_t path_exists(const char* path);
int32_t file_exists(const char* path);
int32_t directory_exists(const char* path);
int32_t is_file(const char* path);
int32_t is_directory(const char* path);
int32_t is_symlink(const char* path);

int32_t create_file(const char* path);
int32_t create_directory(const char* path);
int32_t create_directories(const char* path);
int32_t delete_file(const char* path);
int32_t delete_directory(const char* path);
int32_t delete_path_recursive(const char* path);
int32_t delete_symlink(const char* path);
int32_t copy_file(const char* source_path, const char* destination_path);
int32_t copy_directory(const char* source_path, const char* destination_path);
int32_t copy_path(const char* source_path, const char* destination_path);
int32_t rename_file(const char* old_path, const char* new_path);
int32_t rename_directory(const char* old_path, const char* new_path);
int32_t move_file(const char* source_path, const char* destination_path);
int32_t move_directory(const char* source_path, const char* destination_path);
int32_t create_symlink(const char* target_path, const char* link_path);
int32_t create_hardlink(const char* target_path, const char* link_path);

int64_t file_size(const char* path);
int64_t directory_size(const char* path);
int64_t directory_num_files(const char* path);
int32_t permissions(const char* path);
int32_t directory_permissions(const char* path);
int8_t permission_owner(const char* path);
int8_t permission_group(const char* path);
int8_t permission_other(const char* path);
int32_t set_permissions(const char* path, int8_t owner, int8_t group, int8_t other);
int32_t chmod_path(const char* path, int32_t mode);
int32_t chown_path(const char* path, int32_t owner_uid, int32_t group_gid);

int32_t write_text(const char* path, const char* content);
int32_t overwrite_file(const char* path, const char* content);
int32_t append_text(const char* path, const char* content);
int32_t append_file(const char* path, const char* content);
int32_t append_at_end(const char* path, const char* content);
int32_t clear_file(const char* path);
int32_t write_line(const char* path, const char* content);
int32_t write_line_at(const char* path, int64_t line_index, const char* content);
int32_t append_line(const char* path, const char* content);
int32_t dos2unix(const char* path);
char* read_text(const char* path);
char* read_line(const char* path, int64_t line_index);
int32_t read_to_stdout(const char* path);
int32_t read_line_to_stdout(const char* path, int64_t line_index);
int32_t list_files_to_stdout(const char* path);
int32_t file_info_to_stdout(const char* path);
int32_t directory_info_to_stdout(const char* path);

int32_t has_extension(const char* path, const char* extension);
int32_t make_tarball(const char* source_path, const char* output_path, const char* compression, int32_t level);
int32_t extract_tarball(const char* archive_path, const char* output_directory);
int32_t make_zip(const char* source_path, const char* output_path, int32_t level);
int32_t extract_zip(const char* archive_path, const char* output_directory);
int32_t scp(const char* source_path, const char* destination);
int32_t ssh(const char* server, const char* command);
int32_t print(const char* text);
int32_t println(const char* text);

#ifdef __cplusplus
}
#endif
#endif
