#include "web.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libuv/vendor/libuv/include/uv.h"

#define CLYTH_MAX_ROUTERS 256
#define CLYTH_MAX_ROUTES 1024
#define CLYTH_MAX_CONFIGS 256
#define CLYTH_MAX_SERVERS 256
#define CLYTH_MAX_RESPONSES 4096
#define CLYTH_MAX_REQUESTS 4096
#define CLYTH_MAX_REQUEST_BYTES (1024 * 1024)

typedef enum clyth_http_method {
    CLYTH_HTTP_GET,
    CLYTH_HTTP_POST,
    CLYTH_HTTP_OPTIONS,
} clyth_http_method;

typedef struct clyth_route_entry {
    int64_t router_handle;
    clyth_http_method method;
    char* path;
    clyth_route_handler_fn handler;
} clyth_route_entry;

typedef struct clyth_http_configuration {
    int64_t handle;
    int32_t port;
    int64_t router_handle;
    char* certificate;
    char* key;
} clyth_http_configuration;

typedef struct clyth_http_server_state {
    int64_t handle;
    int64_t configuration_handle;
} clyth_http_server_state;

typedef struct clyth_uv_server {
    uv_loop_t loop;
    uv_tcp_t server;
    int32_t port;
    int64_t router_handle;
} clyth_uv_server;

typedef struct clyth_uv_client {
    uv_tcp_t handle;
    clyth_uv_server* server;
    char* request;
    size_t request_length;
    size_t request_capacity;
    int response_started;
} clyth_uv_client;

typedef struct clyth_uv_write {
    uv_write_t request;
    uv_buf_t buffer;
} clyth_uv_write;

typedef struct clyth_response_slot {
    int64_t handle;
    clyth_uv_client* client;
    int sent;
} clyth_response_slot;

typedef struct clyth_request_slot {
    int64_t handle;
    char* method;
    char* path;
    char* query;
    char* body;
} clyth_request_slot;

static int64_t next_handle = 1;
static clyth_route_entry routes[CLYTH_MAX_ROUTES];
static size_t route_count = 0;
static int64_t routers[CLYTH_MAX_ROUTERS];
static size_t router_count = 0;
static clyth_http_configuration configurations[CLYTH_MAX_CONFIGS];
static size_t configuration_count = 0;
static clyth_http_server_state servers[CLYTH_MAX_SERVERS];
static size_t server_count = 0;
static clyth_response_slot responses[CLYTH_MAX_RESPONSES];
static size_t response_count = 0;
static clyth_request_slot requests[CLYTH_MAX_REQUESTS];
static size_t request_count = 0;

static int clyth_http_trace_enabled(void) {
    const char* value = getenv("CLYTH_HTTP_TRACE");
    return value != NULL && value[0] != '\0' && strcmp(value, "0") != 0;
}

static int64_t allocate_handle(void) {
    return next_handle++;
}

static char* clyth_strdup(const char* text) {
    if (text == NULL) {
        return NULL;
    }
    size_t length = strlen(text);
    char* copy = (char*)malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, text, length + 1);
    return copy;
}

static int router_exists(int64_t handle) {
    for (size_t i = 0; i < router_count; ++i) {
        if (routers[i] == handle) {
            return 1;
        }
    }
    return 0;
}

static clyth_http_configuration* find_configuration(int64_t handle) {
    for (size_t i = 0; i < configuration_count; ++i) {
        if (configurations[i].handle == handle) {
            return &configurations[i];
        }
    }
    return NULL;
}

static clyth_http_server_state* find_server(int64_t handle) {
    for (size_t i = 0; i < server_count; ++i) {
        if (servers[i].handle == handle) {
            return &servers[i];
        }
    }
    return NULL;
}


static clyth_route_entry* find_route(int64_t router_handle, clyth_http_method method, const char* path) {
    for (size_t i = 0; i < route_count; ++i) {
        if (routes[i].router_handle == router_handle && routes[i].method == method && strcmp(routes[i].path, path) == 0) {
            return &routes[i];
        }
    }
    return NULL;
}

static clyth_response_slot* find_response(int64_t handle) {
    for (size_t i = 0; i < response_count; ++i) {
        if (responses[i].handle == handle) {
            return &responses[i];
        }
    }
    return NULL;
}

static int64_t register_response(clyth_uv_client* client) {
    if (response_count >= CLYTH_MAX_RESPONSES || client == NULL) {
        return -ENOMEM;
    }
    int64_t handle = allocate_handle();
    responses[response_count].handle = handle;
    responses[response_count].client = client;
    responses[response_count].sent = 0;
    ++response_count;
    return handle;
}

static void unregister_response(int64_t handle) {
    for (size_t i = 0; i < response_count; ++i) {
        if (responses[i].handle == handle) {
            responses[i] = responses[response_count - 1];
            --response_count;
            return;
        }
    }
}

static clyth_request_slot* find_request(int64_t handle) {
    for (size_t i = 0; i < request_count; ++i) {
        if (requests[i].handle == handle) {
            return &requests[i];
        }
    }
    return NULL;
}

static int64_t register_request(const char* method, const char* path, const char* query, const char* body) {
    if (request_count >= CLYTH_MAX_REQUESTS) {
        return -ENOMEM;
    }

    char* method_copy = clyth_strdup(method == NULL ? "" : method);
    char* path_copy = clyth_strdup(path == NULL ? "" : path);
    char* query_copy = clyth_strdup(query == NULL ? "" : query);
    char* body_copy = clyth_strdup(body == NULL ? "" : body);
    if (method_copy == NULL || path_copy == NULL || query_copy == NULL || body_copy == NULL) {
        free(method_copy);
        free(path_copy);
        free(query_copy);
        free(body_copy);
        return -ENOMEM;
    }

    int64_t handle = allocate_handle();
    requests[request_count].handle = handle;
    requests[request_count].method = method_copy;
    requests[request_count].path = path_copy;
    requests[request_count].query = query_copy;
    requests[request_count].body = body_copy;
    ++request_count;
    return handle;
}

static void unregister_request(int64_t handle) {
    for (size_t i = 0; i < request_count; ++i) {
        if (requests[i].handle == handle) {
            free(requests[i].method);
            free(requests[i].path);
            free(requests[i].query);
            free(requests[i].body);
            requests[i] = requests[request_count - 1];
            --request_count;
            return;
        }
    }
}

static const char* request_string_or_empty(int64_t request_handle, int field) {
    clyth_request_slot* slot = find_request(request_handle);
    if (slot == NULL) {
        return "";
    }
    switch (field) {
        case 0: return slot->method == NULL ? "" : slot->method;
        case 1: return slot->path == NULL ? "" : slot->path;
        case 2: return slot->query == NULL ? "" : slot->query;
        case 3: return slot->body == NULL ? "" : slot->body;
        default: return "";
    }
}

const char* clyth_request_method(int64_t request_handle) {
    return request_string_or_empty(request_handle, 0);
}

const char* clyth_request_path(int64_t request_handle) {
    return request_string_or_empty(request_handle, 1);
}

const char* clyth_request_query(int64_t request_handle) {
    return request_string_or_empty(request_handle, 2);
}

const char* clyth_request_body(int64_t request_handle) {
    return request_string_or_empty(request_handle, 3);
}

int64_t clyth_router_create(void) {
    if (router_count >= CLYTH_MAX_ROUTERS) {
        return -ENOMEM;
    }
    int64_t handle = allocate_handle();
    routers[router_count++] = handle;
    return handle;
}

static int32_t register_route(int64_t router_handle, clyth_http_method method, const char* path, clyth_route_handler_fn handler) {
    if (!router_exists(router_handle) || path == NULL || handler == NULL) {
        return -EINVAL;
    }
    if (route_count >= CLYTH_MAX_ROUTES) {
        return -ENOMEM;
    }

    clyth_route_entry* existing = find_route(router_handle, method, path);
    if (existing != NULL) {
        existing->handler = handler;
        return 0;
    }

    char* path_copy = clyth_strdup(path);
    if (path_copy == NULL) {
        return -ENOMEM;
    }

    routes[route_count].router_handle = router_handle;
    routes[route_count].method = method;
    routes[route_count].path = path_copy;
    routes[route_count].handler = handler;
    ++route_count;
    return 0;
}

int32_t clyth_router_post(int64_t router_handle, const char* path, clyth_route_handler_fn handler) {
    return register_route(router_handle, CLYTH_HTTP_POST, path, handler);
}

int32_t clyth_router_get(int64_t router_handle, const char* path, clyth_route_handler_fn handler) {
    return register_route(router_handle, CLYTH_HTTP_GET, path, handler);
}

int64_t clyth_server_configuration_create(int32_t port, int64_t router_handle, const char* certificate, const char* key) {
    if (port <= 0 || port > 65535 || !router_exists(router_handle)) {
        return -EINVAL;
    }
    if (configuration_count >= CLYTH_MAX_CONFIGS) {
        return -ENOMEM;
    }

    char* certificate_copy = clyth_strdup(certificate == NULL ? "" : certificate);
    char* key_copy = clyth_strdup(key == NULL ? "" : key);
    if (certificate_copy == NULL || key_copy == NULL) {
        free(certificate_copy);
        free(key_copy);
        return -ENOMEM;
    }

    int64_t handle = allocate_handle();
    configurations[configuration_count].handle = handle;
    configurations[configuration_count].port = port;
    configurations[configuration_count].router_handle = router_handle;
    configurations[configuration_count].certificate = certificate_copy;
    configurations[configuration_count].key = key_copy;
    ++configuration_count;
    return handle;
}

int64_t clyth_http_configuration_create(int32_t port, int64_t router_handle) {
    return clyth_server_configuration_create(port, router_handle, "", "");
}

int64_t clyth_https_configuration_create(const char* key_pem, const char* cert_pem, int32_t port, int64_t router_handle) {
    return clyth_server_configuration_create(port, router_handle, cert_pem, key_pem);
}

int64_t clyth_server_create(int64_t configuration_handle) {
    if (find_configuration(configuration_handle) == NULL) {
        return -EINVAL;
    }
    if (server_count >= CLYTH_MAX_SERVERS) {
        return -ENOMEM;
    }
    int64_t handle = allocate_handle();
    servers[server_count].handle = handle;
    servers[server_count].configuration_handle = configuration_handle;
    ++server_count;
    return handle;
}

int64_t clyth_http_server_create(int64_t configuration_handle) {
    return clyth_server_create(configuration_handle);
}

int64_t clyth_https_server_create(int64_t configuration_handle) {
    return clyth_server_create(configuration_handle);
}

static const char* find_header_end(const char* data, size_t length) {
    if (data == NULL || length < 4) {
        return NULL;
    }
    for (size_t i = 0; i + 3 < length; ++i) {
        if (data[i] == '\r' && data[i + 1] == '\n' && data[i + 2] == '\r' && data[i + 3] == '\n') {
            return data + i + 4;
        }
    }
    return NULL;
}

static int parse_content_length(const char* data) {
    const char* needle = "Content-Length:";
    const char* found = strstr(data, needle);
    if (found == NULL) {
        needle = "content-length:";
        found = strstr(data, needle);
    }
    if (found == NULL) {
        return 0;
    }
    found += strlen(needle);
    while (*found == ' ' || *found == '\t') {
        ++found;
    }
    return atoi(found);
}

static int parse_request_line(const char* request, char* method, size_t method_size, char* path, size_t path_size, char* query, size_t query_size) {
    if (request == NULL || method == NULL || path == NULL || query == NULL || method_size == 0 || path_size == 0 || query_size == 0) {
        return -EINVAL;
    }

    const char* first_space = strchr(request, ' ');
    if (first_space == NULL) {
        return -EINVAL;
    }
    const char* second_space = strchr(first_space + 1, ' ');
    if (second_space == NULL) {
        return -EINVAL;
    }

    size_t method_length = (size_t)(first_space - request);
    size_t target_length = (size_t)(second_space - first_space - 1);
    if (method_length + 1 > method_size || target_length == 0) {
        return -EINVAL;
    }

    memcpy(method, request, method_length);
    method[method_length] = '\0';

    const char* target = first_space + 1;
    const char* qmark = memchr(target, '?', target_length);
    size_t pure_path_length = qmark == NULL ? target_length : (size_t)(qmark - target);
    size_t query_length = qmark == NULL ? 0 : target_length - pure_path_length - 1;
    if (pure_path_length + 1 > path_size || query_length + 1 > query_size) {
        return -EINVAL;
    }

    memcpy(path, target, pure_path_length);
    path[pure_path_length] = '\0';
    if (query_length > 0) {
        memcpy(query, qmark + 1, query_length);
    }
    query[query_length] = '\0';
    return 0;
}

static const char* content_type_for_payload(const char* payload) {
    if (payload == NULL) {
        return "text/plain; charset=utf-8";
    }
    while (*payload == ' ' || *payload == '\t' || *payload == '\r' || *payload == '\n') {
        ++payload;
    }
    if (*payload == '<') {
        return "text/html; charset=utf-8";
    }
    if (*payload == '{' || *payload == '[') {
        return "application/json; charset=utf-8";
    }
    return "text/plain; charset=utf-8";
}

static void on_client_closed(uv_handle_t* handle);

static void after_write(uv_write_t* request, int status) {
    (void)status;
    clyth_uv_write* write = (clyth_uv_write*)request;
    free(write->buffer.base);
    uv_close((uv_handle_t*)request->handle, on_client_closed);
    free(write);
}

static void write_response(clyth_uv_client* client, int status_code, const char* content_type, const char* body) {
    if (client == NULL || client->response_started) {
        return;
    }
    client->response_started = 1;
    uv_read_stop((uv_stream_t*)&client->handle);

    const char* reason = status_code == 200 ? "OK" : status_code == 404 ? "Not Found" : "Error";
    if (body == NULL) {
        body = "";
    }
    if (content_type == NULL) {
        content_type = content_type_for_payload(body);
    }

    size_t body_length = strlen(body);
    char header[512];
    int header_length = snprintf(
        header,
        sizeof(header),
        "HTTP/1.1 %d %s\r\nContent-Type: %s\r\nContent-Length: %zu\r\nConnection: close\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: Content-Type\r\nAccess-Control-Allow-Methods: GET, POST, OPTIONS\r\n\r\n",
        status_code,
        reason,
        content_type,
        body_length
    );

    if (header_length < 0) {
        return;
    }

    size_t response_length = (size_t)header_length + body_length;
    char* response = (char*)malloc(response_length);
    if (response == NULL) {
        return;
    }
    memcpy(response, header, (size_t)header_length);
    memcpy(response + header_length, body, body_length);

    clyth_uv_write* write = (clyth_uv_write*)calloc(1, sizeof(clyth_uv_write));
    if (write == NULL) {
        free(response);
        return;
    }
    write->buffer = uv_buf_init(response, (unsigned int)response_length);
    uv_write(&write->request, (uv_stream_t*)&client->handle, &write->buffer, 1, after_write);
}

int32_t clyth_response_send(int64_t response_handle, int32_t code, const char* payload) {
    if (response_handle <= 0 || code < 100 || payload == NULL) {
        return -EINVAL;
    }
    clyth_response_slot* slot = find_response(response_handle);
    if (slot == NULL || slot->client == NULL) {
        return -EINVAL;
    }
    write_response(slot->client, code, content_type_for_payload(payload), payload);
    slot->sent = 1;
    return 0;
}

static clyth_http_method method_from_text(const char* method) {
    if (strcmp(method, "POST") == 0) {
        return CLYTH_HTTP_POST;
    }
    if (strcmp(method, "OPTIONS") == 0) {
        return CLYTH_HTTP_OPTIONS;
    }
    return CLYTH_HTTP_GET;
}

static void maybe_handle_request(clyth_uv_client* client) {
    if (client == NULL || client->request == NULL || client->response_started) {
        return;
    }

    const char* header_end = find_header_end(client->request, client->request_length);
    if (header_end == NULL) {
        return;
    }

    int content_length = parse_content_length(client->request);
    if (content_length < 0 || content_length > CLYTH_MAX_REQUEST_BYTES) {
        write_response(client, 413, "text/plain; charset=utf-8", "Request body too large");
        return;
    }

    size_t header_length = (size_t)(header_end - client->request);
    if (content_length > 0 && client->request_length < header_length + (size_t)content_length) {
        return;
    }

    char method[16];
    char path[512];
    char query[512];
    if (parse_request_line(client->request, method, sizeof(method), path, sizeof(path), query, sizeof(query)) != 0) {
        write_response(client, 400, "text/plain; charset=utf-8", "Bad request");
        return;
    }

    if (clyth_http_trace_enabled()) {
        printf("[HTTP] %s %s%s%s (%zu bytes, content-length=%d)\n", method, path, query[0] ? "?" : "", query, client->request_length, content_length);
        fflush(stdout);
    }

    if (strcmp(method, "OPTIONS") == 0) {
        write_response(client, 200, "text/plain; charset=utf-8", "");
        return;
    }

    if (strcmp(method, "GET") == 0 && strncmp(path, "/.well-known/", 13) == 0) {
        write_response(client, 404, "text/plain; charset=utf-8", "Not found");
        return;
    }

    clyth_route_entry* route = find_route(client->server->router_handle, method_from_text(method), path);
    if (route == NULL || route->handler == NULL) {
        if (clyth_http_trace_enabled()) {
            printf("[HTTP] no route for %s %s\n", method, path);
            fflush(stdout);
        }
        write_response(client, 404, "text/plain; charset=utf-8", "Not found");
        return;
    }

    const char* body = header_end;
    char* body_copy = (char*)malloc((size_t)content_length + 1);
    if (body_copy == NULL) {
        write_response(client, 500, "text/plain; charset=utf-8", "allocation failed");
        return;
    }
    memcpy(body_copy, body, (size_t)content_length);
    body_copy[content_length] = '\0';

    int64_t request_handle = register_request(method, path, query, body_copy);
    free(body_copy);
    if (request_handle <= 0) {
        write_response(client, 500, "text/plain; charset=utf-8", "request allocation failed");
        return;
    }

    int64_t response_handle = register_response(client);
    if (response_handle <= 0) {
        unregister_request(request_handle);
        write_response(client, 500, "text/plain; charset=utf-8", "response allocation failed");
        return;
    }

    clyth_runtime_request request;
    request.native_handle = request_handle;

    clyth_runtime_response response;
    response.native_handle = response_handle;

    route->handler(request, response);

    clyth_response_slot* slot = find_response(response_handle);
    const int sent = slot != NULL && slot->sent;
    unregister_response(response_handle);
    unregister_request(request_handle);

    if (!sent && !client->response_started) {
        write_response(client, 500, "text/plain; charset=utf-8", "route handler did not send a response");
    }
}

static void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buffer) {
    (void)handle;
    buffer->base = (char*)malloc(suggested_size);
    buffer->len = suggested_size;
}

static void on_client_closed(uv_handle_t* handle) {
    clyth_uv_client* client = (clyth_uv_client*)handle->data;
    if (client != NULL) {
        free(client->request);
        free(client);
    }
}

static void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buffer) {
    clyth_uv_client* client = (clyth_uv_client*)stream->data;
    if (nread > 0 && client != NULL) {
        size_t required = client->request_length + (size_t)nread + 1;
        if (required > CLYTH_MAX_REQUEST_BYTES) {
            write_response(client, 413, "text/plain; charset=utf-8", "Request too large");
            free(buffer->base);
            return;
        }
        if (required > client->request_capacity) {
            size_t next_capacity = client->request_capacity == 0 ? 4096 : client->request_capacity * 2;
            while (next_capacity < required) {
                next_capacity *= 2;
            }
            char* next = (char*)realloc(client->request, next_capacity);
            if (next == NULL) {
                uv_close((uv_handle_t*)&client->handle, on_client_closed);
                free(buffer->base);
                return;
            }
            client->request = next;
            client->request_capacity = next_capacity;
        }
        memcpy(client->request + client->request_length, buffer->base, (size_t)nread);
        client->request_length += (size_t)nread;
        client->request[client->request_length] = '\0';
        maybe_handle_request(client);
    } else if (nread < 0 && client != NULL && !uv_is_closing((uv_handle_t*)&client->handle)) {
        uv_close((uv_handle_t*)&client->handle, on_client_closed);
    }
    free(buffer->base);
}

static void on_connection(uv_stream_t* server_stream, int status) {
    if (status < 0) {
        return;
    }

    clyth_uv_server* server = (clyth_uv_server*)server_stream->data;
    clyth_uv_client* client = (clyth_uv_client*)calloc(1, sizeof(clyth_uv_client));
    if (client == NULL) {
        return;
    }

    client->server = server;
    client->handle.data = client;
    uv_tcp_init(&server->loop, &client->handle);

    if (uv_accept(server_stream, (uv_stream_t*)&client->handle) == 0) {
        uv_read_start((uv_stream_t*)&client->handle, alloc_buffer, on_read);
    } else {
        uv_close((uv_handle_t*)&client->handle, on_client_closed);
    }
}

static int32_t clyth_http_serve_uv(int32_t port, int64_t router_handle) {
    if (port <= 0 || port > 65535 || !router_exists(router_handle)) {
        return -EINVAL;
    }

    clyth_uv_server server;
    memset(&server, 0, sizeof(server));
    server.port = port;
    server.router_handle = router_handle;

    int rc = uv_loop_init(&server.loop);
    if (rc != 0) {
        return rc;
    }

    server.server.data = &server;
    rc = uv_tcp_init(&server.loop, &server.server);
    if (rc != 0) {
        uv_loop_close(&server.loop);
        return rc;
    }

    int enable = 1;
    uv_tcp_nodelay(&server.server, enable);
    uv_tcp_simultaneous_accepts(&server.server, enable);

    struct sockaddr_in address;
    rc = uv_ip4_addr("0.0.0.0", port, &address);
    if (rc != 0) {
        uv_loop_close(&server.loop);
        return rc;
    }

    rc = uv_tcp_bind(&server.server, (const struct sockaddr*)&address, 0);
    if (rc != 0) {
        uv_loop_close(&server.loop);
        return rc;
    }

    rc = uv_listen((uv_stream_t*)&server.server, 128, on_connection);
    if (rc != 0) {
        uv_loop_close(&server.loop);
        return rc;
    }

    rc = uv_run(&server.loop, UV_RUN_DEFAULT);
    uv_loop_close(&server.loop);
    return rc;
}

int32_t clyth_server_start(int64_t server_handle) {
    clyth_http_server_state* server = find_server(server_handle);
    if (server == NULL) {
        return -EINVAL;
    }
    clyth_http_configuration* configuration = find_configuration(server->configuration_handle);
    if (configuration == NULL) {
        return -EINVAL;
    }
    return clyth_http_serve_uv(configuration->port, configuration->router_handle);
}

int32_t clyth_http_server_start(int64_t server_handle) {
    return clyth_server_start(server_handle);
}

int32_t clyth_https_server_start(int64_t server_handle) {
    return clyth_server_start(server_handle);
}
