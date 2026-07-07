#ifndef CLYTH_RUNTIME_WEB_H
#define CLYTH_RUNTIME_WEB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct clyth_runtime_request {
    int64_t native_handle;
} clyth_runtime_request;

typedef struct clyth_runtime_response {
    int64_t native_handle;
} clyth_runtime_response;

typedef void (*clyth_route_handler_fn)(clyth_runtime_request request, clyth_runtime_response response);

int64_t clyth_router_create(void);
int32_t clyth_router_post(int64_t router_handle, const char* path, clyth_route_handler_fn handler);
int32_t clyth_router_get(int64_t router_handle, const char* path, clyth_route_handler_fn handler);

int64_t clyth_server_configuration_create(int32_t port, int64_t router_handle, const char* certificate, const char* key);
int64_t clyth_server_create(int64_t configuration_handle);
int32_t clyth_server_start(int64_t server_handle);

int64_t clyth_http_configuration_create(int32_t port, int64_t router_handle);
int64_t clyth_https_configuration_create(const char* key_pem, const char* cert_pem, int32_t port, int64_t router_handle);
int64_t clyth_http_server_create(int64_t configuration_handle);
int64_t clyth_https_server_create(int64_t configuration_handle);
int32_t clyth_http_server_start(int64_t server_handle);
int32_t clyth_https_server_start(int64_t server_handle);

const char* clyth_request_method(int64_t request_handle);
const char* clyth_request_path(int64_t request_handle);
const char* clyth_request_query(int64_t request_handle);
const char* clyth_request_body(int64_t request_handle);

int32_t clyth_response_send(int64_t response_handle, int32_t code, const char* payload);

#ifdef __cplusplus
}
#endif

#endif
