#include "file.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <uv.h>

static uv_loop_t* file_loop(void) {
    return uv_default_loop();
}

static int32_t uv_result_to_i32(ssize_t result) {
    return result < 0 ? (int32_t)result : 0;
}

static int32_t errno_result(void) {
    return errno == 0 ? -1 : -errno;
}

static int32_t clyth_uv_close_file(uv_file file) {
    uv_fs_t req;
    int result = uv_fs_close(file_loop(), &req, file, NULL);
    uv_fs_req_cleanup(&req);
    return uv_result_to_i32(result);
}

static int32_t clyth_uv_write_all(uv_file file, const char* data, size_t length, int64_t offset) {
    size_t written_total = 0;
    while (written_total < length) {
        uv_buf_t buffer = uv_buf_init((char*)data + written_total, (unsigned int)(length - written_total));
        uv_fs_t req;
        int result = uv_fs_write(file_loop(), &req, file, &buffer, 1, offset < 0 ? -1 : offset + (int64_t)written_total, NULL);
        uv_fs_req_cleanup(&req);
        if (result < 0) {
            return result;
        }
        if (result == 0) {
            return -EIO;
        }
        written_total += (size_t)result;
    }
    return 0;
}

static int32_t clyth_uv_write_fd(uv_file file, const char* data, size_t length) {
    if (data == NULL) {
        return -EINVAL;
    }
    return clyth_uv_write_all(file, data, length, -1);
}

static int32_t clyth_uv_write_cstr_fd(uv_file file, const char* text, int append_newline) {
    if (text == NULL) {
        return -EINVAL;
    }
    int32_t result = clyth_uv_write_fd(file, text, strlen(text));
    if (result != 0 || !append_newline) {
        return result;
    }
    return clyth_uv_write_fd(file, "\n", 1);
}

static int32_t write_text_flags(const char* path, const char* content, int flags, int append_newline) {
    if (path == NULL || content == NULL) {
        return -EINVAL;
    }

    uv_fs_t open_req;
    int open_result = uv_fs_open(file_loop(), &open_req, path, flags, 0664, NULL);
    uv_fs_req_cleanup(&open_req);
    if (open_result < 0) {
        return open_result;
    }

    uv_file file = (uv_file)open_result;
    const size_t length = strlen(content);
    int32_t write_result = clyth_uv_write_all(file, content, length, -1);
    if (write_result == 0 && append_newline) {
        write_result = clyth_uv_write_all(file, "\n", 1, -1);
    }

    int32_t close_result = clyth_uv_close_file(file);
    return write_result != 0 ? write_result : close_result;
}

static int32_t uv_stat_path(const char* path, uv_stat_t* out, int follow_symlink) {
    if (path == NULL || out == NULL) {
        return -EINVAL;
    }
    uv_fs_t req;
    int result = follow_symlink
        ? uv_fs_stat(file_loop(), &req, path, NULL)
        : uv_fs_lstat(file_loop(), &req, path, NULL);
    if (result == 0) {
        *out = req.statbuf;
    }
    uv_fs_req_cleanup(&req);
    return result;
}

static int32_t stat_mode(const char* path, int mask) {
    uv_stat_t info;
    int32_t result = uv_stat_path(path, &info, 1);
    if (result != 0) {
        return 0;
    }
    return (info.st_mode & S_IFMT) == mask ? 1 : 0;
}

int32_t clyth_path_exists(const char* path) {
    uv_stat_t info;
    return uv_stat_path(path, &info, 1) == 0 ? 1 : 0;
}

int32_t clyth_file_exists(const char* path) {
    return clyth_is_file(path);
}

int32_t clyth_directory_exists(const char* path) {
    return clyth_is_directory(path);
}

int32_t clyth_is_file(const char* path) {
    return stat_mode(path, S_IFREG);
}

int32_t clyth_is_directory(const char* path) {
    return stat_mode(path, S_IFDIR);
}

int32_t clyth_is_symlink(const char* path) {
    uv_stat_t info;
    int32_t result = uv_stat_path(path, &info, 0);
    if (result != 0) {
        return 0;
    }
    return S_ISLNK(info.st_mode) ? 1 : 0;
}

int32_t clyth_create_file(const char* path) {
    return write_text_flags(path, "", O_CREAT | O_WRONLY | O_APPEND, 0);
}

int32_t clyth_create_directory(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    uv_fs_t req;
    int result = uv_fs_mkdir(file_loop(), &req, path, 0775, NULL);
    uv_fs_req_cleanup(&req);
    return result == UV_EEXIST ? 0 : result;
}

int32_t clyth_delete_file(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    uv_fs_t req;
    int result = uv_fs_unlink(file_loop(), &req, path, NULL);
    uv_fs_req_cleanup(&req);
    return result;
}

int32_t clyth_delete_directory(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    uv_fs_t req;
    int result = uv_fs_rmdir(file_loop(), &req, path, NULL);
    uv_fs_req_cleanup(&req);
    return result;
}

int32_t clyth_delete_symlink(const char* path) {
    return clyth_delete_file(path);
}

static int32_t read_entire_file(const char* path, char** out_data, size_t* out_length) {
    if (path == NULL || out_data == NULL || out_length == NULL) {
        return -EINVAL;
    }
    *out_data = NULL;
    *out_length = 0;

    uv_fs_t open_req;
    int open_result = uv_fs_open(file_loop(), &open_req, path, O_RDONLY, 0, NULL);
    uv_fs_req_cleanup(&open_req);
    if (open_result < 0) {
        return open_result;
    }
    uv_file file = (uv_file)open_result;

    uv_fs_t stat_req;
    int stat_result = uv_fs_fstat(file_loop(), &stat_req, file, NULL);
    if (stat_result < 0) {
        uv_fs_req_cleanup(&stat_req);
        clyth_uv_close_file(file);
        return stat_result;
    }
    size_t length = stat_req.statbuf.st_size < 0 ? 0 : (size_t)stat_req.statbuf.st_size;
    uv_fs_req_cleanup(&stat_req);

    char* data = (char*)malloc(length + 1);
    if (data == NULL) {
        clyth_uv_close_file(file);
        return -ENOMEM;
    }

    size_t total = 0;
    while (total < length) {
        uv_buf_t buffer = uv_buf_init(data + total, (unsigned int)(length - total));
        uv_fs_t read_req;
        int read_result = uv_fs_read(file_loop(), &read_req, file, &buffer, 1, (int64_t)total, NULL);
        uv_fs_req_cleanup(&read_req);
        if (read_result < 0) {
            free(data);
            clyth_uv_close_file(file);
            return read_result;
        }
        if (read_result == 0) {
            break;
        }
        total += (size_t)read_result;
    }
    data[total] = '\0';
    clyth_uv_close_file(file);
    *out_data = data;
    *out_length = total;
    return 0;
}

int32_t clyth_copy_file(const char* source_path, const char* destination_path) {
    char* data = NULL;
    size_t length = 0;
    int32_t read_result = read_entire_file(source_path, &data, &length);
    if (read_result != 0) {
        return read_result;
    }

    uv_fs_t open_req;
    int open_result = uv_fs_open(file_loop(), &open_req, destination_path, O_CREAT | O_WRONLY | O_TRUNC, 0664, NULL);
    uv_fs_req_cleanup(&open_req);
    if (open_result < 0) {
        free(data);
        return open_result;
    }
    uv_file destination = (uv_file)open_result;
    int32_t write_result = clyth_uv_write_all(destination, data, length, 0);
    int32_t close_result = clyth_uv_close_file(destination);
    free(data);
    return write_result != 0 ? write_result : close_result;
}

int32_t clyth_copy_directory(const char* source_path, const char* destination_path) {
    if (source_path == NULL || destination_path == NULL) {
        return -EINVAL;
    }
    char command[4096];
    int written = snprintf(command, sizeof(command), "cp -R '%s' '%s'", source_path, destination_path);
    if (written < 0 || (size_t)written >= sizeof(command)) {
        return -ENAMETOOLONG;
    }
    return clyth_bash(command);
}

int32_t clyth_rename_path(const char* old_path, const char* new_path) {
    if (old_path == NULL || new_path == NULL) {
        return -EINVAL;
    }
    uv_fs_t req;
    int result = uv_fs_rename(file_loop(), &req, old_path, new_path, NULL);
    uv_fs_req_cleanup(&req);
    return result;
}

int32_t clyth_move_path(const char* source_path, const char* destination_path) {
    return clyth_rename_path(source_path, destination_path);
}

int32_t clyth_create_symlink(const char* target_path, const char* link_path) {
    if (target_path == NULL || link_path == NULL) {
        return -EINVAL;
    }
    uv_fs_t req;
    int result = uv_fs_symlink(file_loop(), &req, target_path, link_path, 0, NULL);
    uv_fs_req_cleanup(&req);
    return result;
}

int32_t clyth_create_hardlink(const char* target_path, const char* link_path) {
    if (target_path == NULL || link_path == NULL) {
        return -EINVAL;
    }
    uv_fs_t req;
    int result = uv_fs_link(file_loop(), &req, target_path, link_path, NULL);
    uv_fs_req_cleanup(&req);
    return result;
}

int64_t clyth_file_size(const char* path) {
    uv_stat_t info;
    int32_t result = uv_stat_path(path, &info, 1);
    if (result != 0) {
        return result;
    }
    return (int64_t)info.st_size;
}

static int64_t directory_walk_size(const char* path, int count_files) {
    if (path == NULL) {
        return -EINVAL;
    }
    uv_fs_t scan_req;
    int scan_result = uv_fs_scandir(file_loop(), &scan_req, path, 0, NULL);
    if (scan_result < 0) {
        uv_fs_req_cleanup(&scan_req);
        return scan_result;
    }

    int64_t total = 0;
    uv_dirent_t entry;
    while (uv_fs_scandir_next(&scan_req, &entry) != UV_EOF) {
        if (strcmp(entry.name, ".") == 0 || strcmp(entry.name, "..") == 0) {
            continue;
        }
        char child[4096];
        int written = snprintf(child, sizeof(child), "%s/%s", path, entry.name);
        if (written < 0 || (size_t)written >= sizeof(child)) {
            uv_fs_req_cleanup(&scan_req);
            return -ENAMETOOLONG;
        }
        uv_stat_t info;
        int32_t stat_result = uv_stat_path(child, &info, 0);
        if (stat_result != 0) {
            uv_fs_req_cleanup(&scan_req);
            return stat_result;
        }
        if (S_ISDIR(info.st_mode)) {
            int64_t nested = directory_walk_size(child, count_files);
            if (nested < 0) {
                uv_fs_req_cleanup(&scan_req);
                return nested;
            }
            total += nested;
        } else if (count_files) {
            total += 1;
        } else {
            total += (int64_t)info.st_size;
        }
    }
    uv_fs_req_cleanup(&scan_req);
    return total;
}

int64_t clyth_directory_size(const char* path) {
    return directory_walk_size(path, 0);
}

int64_t clyth_directory_num_files(const char* path) {
    return directory_walk_size(path, 1);
}

int32_t clyth_file_permissions(const char* path) {
    uv_stat_t info;
    int32_t result = uv_stat_path(path, &info, 1);
    if (result != 0) {
        return result;
    }
    return (int32_t)(info.st_mode & 07777);
}

int32_t clyth_directory_permissions(const char* path) {
    return clyth_file_permissions(path);
}

int32_t clyth_chmod_path(const char* path, int32_t mode) {
    if (path == NULL) {
        return -EINVAL;
    }
    uv_fs_t req;
    int result = uv_fs_chmod(file_loop(), &req, path, (int)mode, NULL);
    uv_fs_req_cleanup(&req);
    return result;
}

int32_t clyth_chown_path(const char* path, int32_t owner_uid, int32_t group_gid) {
    if (path == NULL) {
        return -EINVAL;
    }
    uv_fs_t req;
    int result = uv_fs_chown(file_loop(), &req, path, (uv_uid_t)owner_uid, (uv_gid_t)group_gid, NULL);
    uv_fs_req_cleanup(&req);
    return result;
}

int32_t clyth_file_write_text(const char* path, const char* content) {
    return write_text_flags(path, content, O_CREAT | O_WRONLY | O_TRUNC, 0);
}

int32_t clyth_file_append_text(const char* path, const char* content) {
    return write_text_flags(path, content, O_CREAT | O_WRONLY | O_APPEND, 0);
}

int32_t clyth_file_write_line(const char* path, const char* content) {
    return write_text_flags(path, content, O_CREAT | O_WRONLY | O_TRUNC, 1);
}

int32_t clyth_file_append_line(const char* path, const char* content) {
    return write_text_flags(path, content, O_CREAT | O_WRONLY | O_APPEND, 1);
}

int32_t clyth_file_dos2unix(const char* path) {
    char* input = NULL;
    size_t length = 0;
    int32_t read_result = read_entire_file(path, &input, &length);
    if (read_result != 0) {
        return read_result;
    }

    char* output = (char*)malloc(length + 1);
    if (output == NULL) {
        free(input);
        return -ENOMEM;
    }
    size_t out_len = 0;
    for (size_t i = 0; i < length; ++i) {
        if (input[i] != '\r') {
            output[out_len++] = input[i];
        }
    }
    output[out_len] = '\0';

    uv_fs_t open_req;
    int open_result = uv_fs_open(file_loop(), &open_req, path, O_CREAT | O_WRONLY | O_TRUNC, 0664, NULL);
    uv_fs_req_cleanup(&open_req);
    if (open_result < 0) {
        free(input);
        free(output);
        return open_result;
    }
    uv_file file = (uv_file)open_result;
    int32_t write_result = clyth_uv_write_all(file, output, out_len, 0);
    int32_t close_result = clyth_uv_close_file(file);
    free(input);
    free(output);
    return write_result != 0 ? write_result : close_result;
}

int32_t clyth_file_read_to_stdout(const char* path) {
    char* data = NULL;
    size_t length = 0;
    int32_t result = read_entire_file(path, &data, &length);
    if (result != 0) {
        return result;
    }
    int32_t write_result = clyth_uv_write_fd(1, data, length);
    free(data);
    return write_result;
}

int32_t clyth_file_read_line_to_stdout(const char* path, int64_t line_index) {
    if (line_index < 0) {
        return -EINVAL;
    }
    char* data = NULL;
    size_t length = 0;
    int32_t result = read_entire_file(path, &data, &length);
    if (result != 0) {
        return result;
    }
    int64_t current = 0;
    size_t start = 0;
    for (size_t i = 0; i <= length; ++i) {
        if (i == length || data[i] == '\n') {
            if (current == line_index) {
                int32_t write_result = clyth_uv_write_fd(1, data + start, i - start);
                if (write_result == 0 && i < length && data[i] == '\n') {
                    write_result = clyth_uv_write_fd(1, "\n", 1);
                }
                if (write_result != 0) {
                    free(data);
                    return write_result;
                }
                free(data);
                return 0;
            }
            current += 1;
            start = i + 1;
        }
    }
    free(data);
    return -ENOENT;
}

int32_t clyth_list_files_to_stdout(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    uv_fs_t scan_req;
    int scan_result = uv_fs_scandir(file_loop(), &scan_req, path, 0, NULL);
    if (scan_result < 0) {
        uv_fs_req_cleanup(&scan_req);
        return scan_result;
    }
    uv_dirent_t entry;
    while (uv_fs_scandir_next(&scan_req, &entry) != UV_EOF) {
        if (strcmp(entry.name, ".") != 0 && strcmp(entry.name, "..") != 0) {
            printf("%s\n", entry.name);
        }
    }
    uv_fs_req_cleanup(&scan_req);
    return 0;
}

int32_t clyth_file_info_to_stdout(const char* path) {
    uv_stat_t info;
    int32_t result = uv_stat_path(path, &info, 1);
    if (result != 0) {
        return result;
    }
    printf("size=%lld owner=%d group=%d permissions=%o\n", (long long)info.st_size, (int)info.st_uid, (int)info.st_gid, (unsigned)(info.st_mode & 07777));
    return 0;
}

int32_t clyth_directory_info_to_stdout(const char* path) {
    int64_t size = clyth_directory_size(path);
    int64_t files = clyth_directory_num_files(path);
    int32_t perms = clyth_directory_permissions(path);
    if (size < 0) return (int32_t)size;
    if (files < 0) return (int32_t)files;
    if (perms < 0) return perms;
    printf("size=%lld num_files=%lld permissions=%o\n", (long long)size, (long long)files, (unsigned)perms);
    return 0;
}

int32_t clyth_has_extension(const char* path, const char* extension) {
    if (path == NULL || extension == NULL) {
        return 0;
    }
    const char* dot = strrchr(path, '.');
    if (dot == NULL) {
        return 0;
    }
    if (extension[0] != '.') {
        return strcmp(dot + 1, extension) == 0 ? 1 : 0;
    }
    return strcmp(dot, extension) == 0 ? 1 : 0;
}

int32_t clyth_bash(const char* command) {
    if (command == NULL) {
        return -EINVAL;
    }
    int result = system(command);
    if (result == -1) {
        return errno_result();
    }
    return result;
}

static int32_t run_command3(const char* command, const char* a, const char* b, const char* c) {
    if (command == NULL || a == NULL || b == NULL) {
        return -EINVAL;
    }
    char buffer[4096];
    int written = c == NULL
        ? snprintf(buffer, sizeof(buffer), command, a, b)
        : snprintf(buffer, sizeof(buffer), command, a, b, c);
    if (written < 0 || (size_t)written >= sizeof(buffer)) {
        return -ENAMETOOLONG;
    }
    return clyth_bash(buffer);
}

int32_t clyth_make_tarball(const char* source_path, const char* output_path, const char* compression, int32_t level) {
    (void)level;
    const char* flag = "-czf";
    if (compression != NULL && strcmp(compression, "xz") == 0) {
        flag = "-cJf";
    } else if (compression != NULL && strcmp(compression, "none") == 0) {
        flag = "-cf";
    }
    char command[4096];
    int written = snprintf(command, sizeof(command), "tar %s '%s' '%s'", flag, output_path, source_path);
    if (written < 0 || (size_t)written >= sizeof(command)) {
        return -ENAMETOOLONG;
    }
    return clyth_bash(command);
}

int32_t clyth_extract_tarball(const char* archive_path, const char* output_directory) {
    return run_command3("mkdir -p '%s' && tar -xf '%s' -C '%s'", output_directory, archive_path, output_directory);
}

int32_t clyth_make_zip(const char* source_path, const char* output_path, int32_t level) {
    (void)level;
    return run_command3("zip -r '%s' '%s'", output_path, source_path, NULL);
}

int32_t clyth_extract_zip(const char* archive_path, const char* output_directory) {
    return run_command3("mkdir -p '%s' && unzip -o '%s' -d '%s'", output_directory, archive_path, output_directory);
}

int32_t clyth_scp(const char* source_path, const char* destination) {
    return run_command3("scp -r '%s' '%s'", source_path, destination, NULL);
}

int32_t clyth_ssh(const char* server, const char* command) {
    return run_command3("ssh '%s' '%s'", server, command, NULL);
}

int32_t clyth_file_remove(const char* path) {
    return clyth_delete_file(path);
}

int32_t clyth_print(const char* text) {
    return clyth_uv_write_cstr_fd(1, text, 0);
}

int32_t clyth_println(const char* text) {
    return clyth_uv_write_cstr_fd(1, text, 1);
}

int32_t clyth_eprint(const char* text) {
    return clyth_uv_write_cstr_fd(2, text, 0);
}

int32_t clyth_eprintln(const char* text) {
    return clyth_uv_write_cstr_fd(2, text, 1);
}

/* Public Clyth-facing aliases. These keep the module API ergonomic while the
 * implementation stays clyth_* namespaced internally. */
int32_t path_exists(const char* path) { return clyth_path_exists(path); }
int32_t file_exists(const char* path) { return clyth_file_exists(path); }
int32_t directory_exists(const char* path) { return clyth_directory_exists(path); }
int32_t is_file(const char* path) { return clyth_is_file(path); }
int32_t is_directory(const char* path) { return clyth_is_directory(path); }
int32_t is_symlink(const char* path) { return clyth_is_symlink(path); }

int32_t create_file(const char* path) { return clyth_create_file(path); }
int32_t create_directory(const char* path) { return clyth_create_directory(path); }
int32_t delete_file(const char* path) { return clyth_delete_file(path); }
int32_t delete_directory(const char* path) { return clyth_delete_directory(path); }
int32_t delete_symlink(const char* path) { return clyth_delete_symlink(path); }

int32_t copy_file(const char* source_path, const char* destination_path) { return clyth_copy_file(source_path, destination_path); }
int32_t copy_directory(const char* source_path, const char* destination_path) { return clyth_copy_directory(source_path, destination_path); }
int32_t rename_file(const char* old_path, const char* new_path) { return clyth_rename_path(old_path, new_path); }
int32_t rename_directory(const char* old_path, const char* new_path) { return clyth_rename_path(old_path, new_path); }
int32_t move_file(const char* source_path, const char* destination_path) { return clyth_move_path(source_path, destination_path); }
int32_t move_directory(const char* source_path, const char* destination_path) { return clyth_move_path(source_path, destination_path); }
int32_t create_symlink(const char* target_path, const char* link_path) { return clyth_create_symlink(target_path, link_path); }
int32_t create_hardlink(const char* target_path, const char* link_path) { return clyth_create_hardlink(target_path, link_path); }

int64_t file_size(const char* path) { return clyth_file_size(path); }
int64_t directory_size(const char* path) { return clyth_directory_size(path); }
int64_t directory_num_files(const char* path) { return clyth_directory_num_files(path); }
int32_t permissions(const char* path) { return clyth_file_permissions(path); }
int32_t directory_permissions(const char* path) { return clyth_directory_permissions(path); }
int32_t chmod_path(const char* path, int32_t mode) { return clyth_chmod_path(path, mode); }
int32_t chown_path(const char* path, int32_t owner_uid, int32_t group_gid) { return clyth_chown_path(path, owner_uid, group_gid); }

int32_t write_text(const char* path, const char* content) { return clyth_file_write_text(path, content); }
int32_t append_text(const char* path, const char* content) { return clyth_file_append_text(path, content); }
int32_t write_line(const char* path, const char* content) { return clyth_file_write_line(path, content); }
int32_t append_line(const char* path, const char* content) { return clyth_file_append_line(path, content); }
int32_t dos2unix(const char* path) { return clyth_file_dos2unix(path); }
int32_t read_to_stdout(const char* path) { return clyth_file_read_to_stdout(path); }
int32_t read_line_to_stdout(const char* path, int64_t line_index) { return clyth_file_read_line_to_stdout(path, line_index); }
int32_t list_files_to_stdout(const char* path) { return clyth_list_files_to_stdout(path); }
int32_t file_info_to_stdout(const char* path) { return clyth_file_info_to_stdout(path); }
int32_t directory_info_to_stdout(const char* path) { return clyth_directory_info_to_stdout(path); }

int32_t has_extension(const char* path, const char* extension) { return clyth_has_extension(path, extension); }
int32_t bash(const char* command) { return clyth_bash(command); }
int32_t make_tarball(const char* source_path, const char* output_path, const char* compression, int32_t level) { return clyth_make_tarball(source_path, output_path, compression, level); }
int32_t extract_tarball(const char* archive_path, const char* output_directory) { return clyth_extract_tarball(archive_path, output_directory); }
int32_t make_zip(const char* source_path, const char* output_path, int32_t level) { return clyth_make_zip(source_path, output_path, level); }
int32_t extract_zip(const char* archive_path, const char* output_directory) { return clyth_extract_zip(archive_path, output_directory); }
int32_t scp(const char* source_path, const char* destination) { return clyth_scp(source_path, destination); }
int32_t ssh(const char* server, const char* command) { return clyth_ssh(server, command); }

int32_t print(const char* text) { return clyth_print(text); }
int32_t println(const char* text) { return clyth_println(text); }
int32_t eprint(const char* text) { return clyth_eprint(text); }
int32_t eprintln(const char* text) { return clyth_eprintln(text); }
