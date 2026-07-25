#ifndef CLYTH_BOOTSTRAPPING_STAGE1_H
#define CLYTH_BOOTSTRAPPING_STAGE1_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t bootstrap_string_equals(const char* left, const char* right);
int32_t bootstrap_string_not_equals(const char* left, const char* right);
int32_t bootstrap_string_is_empty(const char* value);
int64_t bootstrap_string_length(const char* value);
int32_t bootstrap_string_starts_with(const char* value, const char* prefix);
int32_t bootstrap_string_ends_with(const char* value, const char* suffix);
int32_t bootstrap_string_contains(const char* value, const char* needle);
int64_t bootstrap_string_index_of(const char* value, const char* needle);

const char* bootstrap_string_copy(const char* value);
const char* bootstrap_string_concat(const char* left, const char* right);
const char* bootstrap_string_concat3(const char* first, const char* second, const char* third);
const char* bootstrap_string_slice(const char* value, int64_t start, int64_t length);
const char* bootstrap_string_trim(const char* value);
const char* bootstrap_string_split_get(const char* value, const char* delimiter, int64_t index);
int64_t bootstrap_string_split_count(const char* value, const char* delimiter);

const char* bootstrap_read_file(const char* path);
int32_t bootstrap_string_byte_at(const char* value, int64_t index);

#ifdef __cplusplus
}
#endif

#endif
