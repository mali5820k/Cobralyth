#ifndef CLYTH_RUNTIME_WEB_H
#define CLYTH_RUNTIME_WEB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* clyth_route_handler_fn;

int64_t clyth_router_create(void);
int32_t clyth_router_post(int64_t router_handle, const char* path, clyth_route_handler_fn handler);
int32_t clyth_router_get(int64_t router_handle, const char* path, clyth_route_handler_fn handler);

int64_t clyth_http_configuration_create(int32_t port, int64_t router_handle);
int64_t clyth_https_configuration_create(const char* key_pem, const char* cert_pem, int32_t port, int64_t router_handle);
int64_t clyth_http_server_create(int64_t configuration_handle);
int64_t clyth_https_server_create(int64_t configuration_handle);
int32_t clyth_http_server_start(int64_t server_handle);
int32_t clyth_https_server_start(int64_t server_handle);

int32_t clyth_response_send_text(int64_t response_handle, int32_t code, const char* payload);
int32_t clyth_response_send_json(int64_t response_handle, int32_t code, const char* payload);
int32_t clyth_http_serve_text_once(int32_t port, const char* body);
int32_t clyth_http_serve_html_once(int32_t port, const char* body);
int32_t clyth_https_serve_text_once(int32_t port, const char* cert_path, const char* key_path, const char* body);

#ifdef __cplusplus
}
#endif

#endif
