#include "file.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>

static int32_t errno_result(void) {
    return errno == 0 ? -1 : -errno;
}

static int32_t write_text_mode(const char* path, const char* content, const char* mode, int append_newline) {
    if (path == NULL || content == NULL || mode == NULL) {
        return -EINVAL;
    }

    FILE* file = fopen(path, mode);
    if (file == NULL) {
        return errno_result();
    }

    const size_t length = strlen(content);
    if (length > 0 && fwrite(content, 1, length, file) != length) {
        const int saved_errno = errno == 0 ? EIO : errno;
        fclose(file);
        return -saved_errno;
    }

    if (append_newline && fputc('\n', file) == EOF) {
        const int saved_errno = errno == 0 ? EIO : errno;
        fclose(file);
        return -saved_errno;
    }

    if (fclose(file) != 0) {
        return errno_result();
    }

    return 0;
}

static int32_t stat_mode(const char* path, mode_t mask) {
    if (path == NULL) {
        return 0;
    }
    struct stat info;
    if (stat(path, &info) != 0) {
        return 0;
    }
    return (info.st_mode & S_IFMT) == mask ? 1 : 0;
}

int32_t clyth_path_exists(const char* path) {
    return path != NULL && access(path, F_OK) == 0 ? 1 : 0;
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
    if (path == NULL) {
        return 0;
    }
    struct stat info;
    if (lstat(path, &info) != 0) {
        return 0;
    }
    return S_ISLNK(info.st_mode) ? 1 : 0;
}

int32_t clyth_create_file(const char* path) {
    return write_text_mode(path, "", "ab", 0);
}

int32_t clyth_create_directory(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    if (mkdir(path, 0775) == 0 || errno == EEXIST) {
        return 0;
    }
    return errno_result();
}

int32_t clyth_delete_file(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    if (unlink(path) == 0) {
        return 0;
    }
    return errno_result();
}

int32_t clyth_delete_directory(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    if (rmdir(path) == 0) {
        return 0;
    }
    return errno_result();
}

int32_t clyth_delete_symlink(const char* path) {
    return clyth_delete_file(path);
}

int32_t clyth_copy_file(const char* source_path, const char* destination_path) {
    if (source_path == NULL || destination_path == NULL) {
        return -EINVAL;
    }

    FILE* source = fopen(source_path, "rb");
    if (source == NULL) {
        return errno_result();
    }
    FILE* destination = fopen(destination_path, "wb");
    if (destination == NULL) {
        const int saved_errno = errno_result();
        fclose(source);
        return saved_errno;
    }

    char buffer[8192];
    size_t read_count;
    while ((read_count = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        if (fwrite(buffer, 1, read_count, destination) != read_count) {
            const int saved_errno = errno == 0 ? EIO : errno;
            fclose(source);
            fclose(destination);
            return -saved_errno;
        }
    }

    if (ferror(source)) {
        const int saved_errno = errno == 0 ? EIO : errno;
        fclose(source);
        fclose(destination);
        return -saved_errno;
    }

    if (fclose(source) != 0) {
        fclose(destination);
        return errno_result();
    }
    if (fclose(destination) != 0) {
        return errno_result();
    }
    return 0;
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
    return rename(old_path, new_path) == 0 ? 0 : errno_result();
}

int32_t clyth_move_path(const char* source_path, const char* destination_path) {
    return clyth_rename_path(source_path, destination_path);
}

int32_t clyth_create_symlink(const char* target_path, const char* link_path) {
    if (target_path == NULL || link_path == NULL) {
        return -EINVAL;
    }
    return symlink(target_path, link_path) == 0 ? 0 : errno_result();
}

int32_t clyth_create_hardlink(const char* target_path, const char* link_path) {
    if (target_path == NULL || link_path == NULL) {
        return -EINVAL;
    }
    return link(target_path, link_path) == 0 ? 0 : errno_result();
}

int64_t clyth_file_size(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    struct stat info;
    if (stat(path, &info) != 0) {
        return errno_result();
    }
    return (int64_t)info.st_size;
}

static int64_t directory_walk_size(const char* path, int count_files) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        return errno_result();
    }
    int64_t total = 0;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        char child[PATH_MAX];
        int written = snprintf(child, sizeof(child), "%s/%s", path, entry->d_name);
        if (written < 0 || (size_t)written >= sizeof(child)) {
            closedir(dir);
            return -ENAMETOOLONG;
        }
        struct stat info;
        if (lstat(child, &info) != 0) {
            closedir(dir);
            return errno_result();
        }
        if (S_ISDIR(info.st_mode)) {
            int64_t nested = directory_walk_size(child, count_files);
            if (nested < 0) {
                closedir(dir);
                return nested;
            }
            total += nested;
        } else if (count_files) {
            total += 1;
        } else {
            total += (int64_t)info.st_size;
        }
    }
    closedir(dir);
    return total;
}

int64_t clyth_directory_size(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    return directory_walk_size(path, 0);
}

int64_t clyth_directory_num_files(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    return directory_walk_size(path, 1);
}

int32_t clyth_file_permissions(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    struct stat info;
    if (stat(path, &info) != 0) {
        return errno_result();
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
    return chmod(path, (mode_t)mode) == 0 ? 0 : errno_result();
}

int32_t clyth_chown_path(const char* path, int32_t owner_uid, int32_t group_gid) {
    if (path == NULL) {
        return -EINVAL;
    }
    return chown(path, (uid_t)owner_uid, (gid_t)group_gid) == 0 ? 0 : errno_result();
}

int32_t clyth_file_write_text(const char* path, const char* content) {
    return write_text_mode(path, content, "wb", 0);
}

int32_t clyth_file_append_text(const char* path, const char* content) {
    return write_text_mode(path, content, "ab", 0);
}

int32_t clyth_file_write_line(const char* path, const char* content) {
    return write_text_mode(path, content, "wb", 1);
}

int32_t clyth_file_append_line(const char* path, const char* content) {
    return write_text_mode(path, content, "ab", 1);
}

int32_t clyth_file_dos2unix(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        return errno_result();
    }
    if (fseek(file, 0, SEEK_END) != 0) {
        const int saved_errno = errno_result();
        fclose(file);
        return saved_errno;
    }
    long length = ftell(file);
    if (length < 0) {
        const int saved_errno = errno_result();
        fclose(file);
        return saved_errno;
    }
    rewind(file);
    char* input = (char*)malloc((size_t)length + 1);
    if (input == NULL) {
        fclose(file);
        return -ENOMEM;
    }
    size_t read_count = fread(input, 1, (size_t)length, file);
    fclose(file);
    if (read_count != (size_t)length) {
        free(input);
        return -EIO;
    }

    file = fopen(path, "wb");
    if (file == NULL) {
        const int saved_errno = errno_result();
        free(input);
        return saved_errno;
    }
    for (size_t i = 0; i < read_count; ++i) {
        if (input[i] != '\r') {
            fputc(input[i], file);
        }
    }
    free(input);
    return fclose(file) == 0 ? 0 : errno_result();
}

int32_t clyth_file_read_to_stdout(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        return errno_result();
    }
    char buffer[4096];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (fwrite(buffer, 1, n, stdout) != n) {
            fclose(file);
            return -EIO;
        }
    }
    if (ferror(file)) {
        fclose(file);
        return -EIO;
    }
    return fclose(file) == 0 ? 0 : errno_result();
}

int32_t clyth_file_read_line_to_stdout(const char* path, int64_t line_index) {
    if (path == NULL || line_index < 0) {
        return -EINVAL;
    }
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        return errno_result();
    }
    char buffer[4096];
    int64_t current = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (current == line_index) {
            fputs(buffer, stdout);
            fclose(file);
            return 0;
        }
        current += 1;
    }
    fclose(file);
    return -ENOENT;
}

int32_t clyth_list_files_to_stdout(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    DIR* dir = opendir(path);
    if (dir == NULL) {
        return errno_result();
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("%s\n", entry->d_name);
        }
    }
    closedir(dir);
    return 0;
}

int32_t clyth_file_info_to_stdout(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
    struct stat info;
    if (stat(path, &info) != 0) {
        return errno_result();
    }
    printf("size=%lld owner=%d group=%d permissions=%o\n", (long long)info.st_size, (int)info.st_uid, (int)info.st_gid, (unsigned)(info.st_mode & 07777));
    return 0;
}

int32_t clyth_directory_info_to_stdout(const char* path) {
    if (path == NULL) {
        return -EINVAL;
    }
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
    if (text == NULL) {
        return -EINVAL;
    }
    return fputs(text, stdout) >= 0 ? 0 : -EIO;
}

int32_t clyth_println(const char* text) {
    if (text == NULL) {
        return -EINVAL;
    }
    if (fputs(text, stdout) < 0) {
        return -EIO;
    }
    return fputc('\n', stdout) == EOF ? -EIO : 0;
}

int32_t clyth_eprint(const char* text) {
    if (text == NULL) {
        return -EINVAL;
    }
    return fputs(text, stderr) >= 0 ? 0 : -EIO;
}

int32_t clyth_eprintln(const char* text) {
    if (text == NULL) {
        return -EINVAL;
    }
    if (fputs(text, stderr) < 0) {
        return -EIO;
    }
    return fputc('\n', stderr) == EOF ? -EIO : 0;
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
