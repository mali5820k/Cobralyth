#include "json_bindings.h"

#include <stddef.h>
#include <string.h>

/*
 * yyjson is vendored at build time under vendor/yyjson.
 * It is intentionally not reimplemented here. This file is only Clyth's
 * stable C ABI shim over the upstream C implementation.
 */
#include "vendor/yyjson/src/yyjson.c"

static yyjson_doc* clyth_json_read_doc(const char* text) {
    if (text == NULL) {
        return NULL;
    }
    return yyjson_read(text, strlen(text), 0);
}

int32_t clyth_json_is_valid(const char* text) {
    yyjson_doc* doc = clyth_json_read_doc(text);
    if (doc == NULL) {
        return 0;
    }
    yyjson_doc_free(doc);
    return 1;
}

static int32_t root_matches(const char* text, int expected_kind) {
    yyjson_doc* doc = clyth_json_read_doc(text);
    if (doc == NULL) {
        return 0;
    }

    yyjson_val* root = yyjson_doc_get_root(doc);
    int32_t result = 0;
    if (root != NULL) {
        switch (expected_kind) {
            case 1: result = yyjson_is_obj(root) ? 1 : 0; break;
            case 2: result = yyjson_is_arr(root) ? 1 : 0; break;
            case 3: result = yyjson_is_str(root) ? 1 : 0; break;
            case 4: result = yyjson_is_num(root) ? 1 : 0; break;
            case 5: result = yyjson_is_bool(root) ? 1 : 0; break;
            case 6: result = yyjson_is_null(root) ? 1 : 0; break;
            default: result = 0; break;
        }
    }

    yyjson_doc_free(doc);
    return result;
}

int32_t clyth_json_is_object(const char* text) { return root_matches(text, 1); }
int32_t clyth_json_is_array(const char* text) { return root_matches(text, 2); }
int32_t clyth_json_is_string(const char* text) { return root_matches(text, 3); }
int32_t clyth_json_is_number(const char* text) { return root_matches(text, 4); }
int32_t clyth_json_is_bool(const char* text) { return root_matches(text, 5); }
int32_t clyth_json_is_null(const char* text) { return root_matches(text, 6); }

int32_t json_is_valid(const char* text) { return clyth_json_is_valid(text); }
int32_t json_is_object(const char* text) { return clyth_json_is_object(text); }
int32_t json_is_array(const char* text) { return clyth_json_is_array(text); }
int32_t json_is_string(const char* text) { return clyth_json_is_string(text); }
int32_t json_is_number(const char* text) { return clyth_json_is_number(text); }
int32_t json_is_bool(const char* text) { return clyth_json_is_bool(text); }
int32_t json_is_null(const char* text) { return clyth_json_is_null(text); }
