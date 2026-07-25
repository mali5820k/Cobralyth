#include "bootstrapping-stage1.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* safe_string(const char* value) {
    return value == NULL ? "" : value;
}

static char* copy_range(const char* value, size_t length) {
    char* result = (char*)malloc(length + 1);
    if (result == NULL) {
        return NULL;
    }
    if (length != 0) {
        memcpy(result, value, length);
    }
    result[length] = '\0';
    return result;
}

int32_t bootstrap_string_equals(const char* left, const char* right) {
    return strcmp(safe_string(left), safe_string(right)) == 0 ? 1 : 0;
}

int32_t bootstrap_string_not_equals(const char* left, const char* right) {
    return bootstrap_string_equals(left, right) == 1 ? 0 : 1;
}

int32_t bootstrap_string_is_empty(const char* value) {
    return safe_string(value)[0] == '\0' ? 1 : 0;
}

int64_t bootstrap_string_length(const char* value) {
    return (int64_t)strlen(safe_string(value));
}

int32_t bootstrap_string_starts_with(const char* value, const char* prefix) {
    const char* text = safe_string(value);
    const char* wanted = safe_string(prefix);
    size_t wanted_length = strlen(wanted);
    return strncmp(text, wanted, wanted_length) == 0 ? 1 : 0;
}

int32_t bootstrap_string_ends_with(const char* value, const char* suffix) {
    const char* text = safe_string(value);
    const char* wanted = safe_string(suffix);
    size_t text_length = strlen(text);
    size_t wanted_length = strlen(wanted);
    if (wanted_length > text_length) {
        return 0;
    }
    return memcmp(text + text_length - wanted_length, wanted, wanted_length) == 0 ? 1 : 0;
}

int32_t bootstrap_string_contains(const char* value, const char* needle) {
    return strstr(safe_string(value), safe_string(needle)) == NULL ? 0 : 1;
}

int64_t bootstrap_string_index_of(const char* value, const char* needle) {
    const char* text = safe_string(value);
    const char* match = strstr(text, safe_string(needle));
    return match == NULL ? -1 : (int64_t)(match - text);
}

const char* bootstrap_string_copy(const char* value) {
    const char* text = safe_string(value);
    return copy_range(text, strlen(text));
}

const char* bootstrap_string_concat(const char* left, const char* right) {
    const char* first = safe_string(left);
    const char* second = safe_string(right);
    size_t first_length = strlen(first);
    size_t second_length = strlen(second);
    char* result = (char*)malloc(first_length + second_length + 1);
    if (result == NULL) {
        return NULL;
    }
    memcpy(result, first, first_length);
    memcpy(result + first_length, second, second_length + 1);
    return result;
}

const char* bootstrap_string_concat3(const char* first, const char* second, const char* third) {
    const char* a = safe_string(first);
    const char* b = safe_string(second);
    const char* c = safe_string(third);
    size_t a_length = strlen(a);
    size_t b_length = strlen(b);
    size_t c_length = strlen(c);
    char* result = (char*)malloc(a_length + b_length + c_length + 1);
    if (result == NULL) {
        return NULL;
    }
    memcpy(result, a, a_length);
    memcpy(result + a_length, b, b_length);
    memcpy(result + a_length + b_length, c, c_length + 1);
    return result;
}

const char* bootstrap_string_slice(const char* value, int64_t start, int64_t length) {
    const char* text = safe_string(value);
    size_t text_length = strlen(text);
    if (start < 0 || length <= 0 || (uint64_t)start >= (uint64_t)text_length) {
        return copy_range("", 0);
    }
    size_t offset = (size_t)start;
    size_t requested = (size_t)length;
    size_t available = text_length - offset;
    if (requested > available) {
        requested = available;
    }
    return copy_range(text + offset, requested);
}

const char* bootstrap_string_trim(const char* value) {
    const char* text = safe_string(value);
    const char* begin = text;
    while (*begin == ' ' || *begin == '\t' || *begin == '\r' || *begin == '\n') {
        ++begin;
    }
    const char* end = text + strlen(text);
    while (end > begin && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\r' || end[-1] == '\n')) {
        --end;
    }
    return copy_range(begin, (size_t)(end - begin));
}

int64_t bootstrap_string_split_count(const char* value, const char* delimiter) {
    const char* text = safe_string(value);
    const char* separator = safe_string(delimiter);
    size_t separator_length = strlen(separator);
    if (separator_length == 0) {
        return 1;
    }
    int64_t count = 1;
    const char* cursor = text;
    while ((cursor = strstr(cursor, separator)) != NULL) {
        ++count;
        cursor += separator_length;
    }
    return count;
}

const char* bootstrap_string_split_get(const char* value, const char* delimiter, int64_t index) {
    const char* text = safe_string(value);
    const char* separator = safe_string(delimiter);
    size_t separator_length = strlen(separator);
    if (index < 0) {
        return copy_range("", 0);
    }
    if (separator_length == 0) {
        return index == 0 ? bootstrap_string_copy(text) : copy_range("", 0);
    }

    const char* field_begin = text;
    int64_t field_index = 0;
    for (;;) {
        const char* match = strstr(field_begin, separator);
        if (field_index == index) {
            const char* field_end = match == NULL ? text + strlen(text) : match;
            return copy_range(field_begin, (size_t)(field_end - field_begin));
        }
        if (match == NULL) {
            return copy_range("", 0);
        }
        field_begin = match + separator_length;
        ++field_index;
    }
}


const char* bootstrap_read_file(const char* path) {
    const char* source_path = safe_string(path);
    FILE* file = fopen(source_path, "rb");
    if (file == NULL) {
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }

    long end = ftell(file);
    if (end < 0 || fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return NULL;
    }

    size_t length = (size_t)end;
    char* result = (char*)malloc(length + 1);
    if (result == NULL) {
        fclose(file);
        return NULL;
    }

    size_t read_length = fread(result, 1, length, file);
    if (read_length != length || ferror(file)) {
        free(result);
        fclose(file);
        return NULL;
    }

    result[length] = '\0';
    fclose(file);
    return result;
}

int32_t bootstrap_string_byte_at(const char* value, int64_t index) {
    const unsigned char* text = (const unsigned char*)safe_string(value);
    size_t length = strlen((const char*)text);
    if (index < 0 || (uint64_t)index >= (uint64_t)length) {
        return -1;
    }
    return (int32_t)text[(size_t)index];
}
