#ifndef CLYTH_RUNTIME_JSON_BINDINGS_H
#define CLYTH_RUNTIME_JSON_BINDINGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t clyth_json_is_valid(const char* text);
int32_t clyth_json_is_object(const char* text);
int32_t clyth_json_is_array(const char* text);
int32_t clyth_json_is_string(const char* text);
int32_t clyth_json_is_number(const char* text);
int32_t clyth_json_is_bool(const char* text);
int32_t clyth_json_is_null(const char* text);

/* Clyth-facing aliases. */
int32_t json_is_valid(const char* text);
int32_t json_is_object(const char* text);
int32_t json_is_array(const char* text);
int32_t json_is_string(const char* text);
int32_t json_is_number(const char* text);
int32_t json_is_bool(const char* text);
int32_t json_is_null(const char* text);

#ifdef __cplusplus
}
#endif

#endif
