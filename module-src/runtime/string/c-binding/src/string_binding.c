#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char *copy_text(const char *value) { const char *source = value ? value : ""; size_t size = strlen(source); char *result = (char *)malloc(size + 1); if (!result) return NULL; memcpy(result, source, size + 1); return result; }

int64_t bootstrap_string_length(const char *value) { return value ? (int64_t)strlen(value) : 0; }
int32_t bootstrap_string_equals(const char *left, const char *right) {
    if (!left || !right) return left == right;
    return strcmp(left, right) == 0;
}
int32_t bootstrap_string_contains(const char *value, const char *needle) {
    return value && needle && strstr(value, needle) != NULL;
}
int32_t bootstrap_string_starts_with(const char *value, const char *prefix) {
    if (!value || !prefix) return 0;
    const size_t length = strlen(prefix);
    return strncmp(value, prefix, length) == 0;
}
int32_t bootstrap_string_ends_with(const char *value, const char *suffix) {
    if (!value || !suffix) return 0;
    const size_t value_length = strlen(value);
    const size_t suffix_length = strlen(suffix);
    return suffix_length <= value_length && strcmp(value + value_length - suffix_length, suffix) == 0;
}
int32_t bootstrap_string_is_empty(const char *value) { return !value || value[0] == '\0'; }
char *bootstrap_string_slice(const char *value, int64_t offset, int64_t length) {
    if (!value || offset < 0 || length < 0) return copy_text("");
    const size_t size = strlen(value);
    if ((uint64_t)offset > size) return copy_text("");
    size_t count = (size_t)length;
    if ((size_t)offset + count > size) count = size - (size_t)offset;
    char *result = (char *)malloc(count + 1);
    if (!result) return NULL;
    memcpy(result, value + offset, count);
    result[count] = '\0';
    return result;
}
char *bootstrap_string_copy(const char *value) { return copy_text(value); }


/** Concatenates two nullable Clyth strings into newly allocated storage. */
char *bootstrap_string_concat(const char *left, const char *right) {
    const char *a = left ? left : "";
    const char *b = right ? right : "";
    size_t an = strlen(a), bn = strlen(b);
    char *result = (char *)malloc(an + bn + 1);
    if (!result) return NULL;
    memcpy(result, a, an);
    memcpy(result + an, b, bn + 1);
    return result;
}

/** Concatenates three nullable Clyth strings into newly allocated storage. */
char *bootstrap_string_concat3(const char *first, const char *second, const char *third) {
    const char *a = first ? first : "";
    const char *b = second ? second : "";
    const char *c = third ? third : "";
    size_t an = strlen(a), bn = strlen(b), cn = strlen(c);
    char *result = (char *)malloc(an + bn + cn + 1);
    if (!result) return NULL;
    memcpy(result, a, an);
    memcpy(result + an, b, bn);
    memcpy(result + an + bn, c, cn + 1);
    return result;
}
