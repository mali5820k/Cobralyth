#include "web.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#if defined(__unix__) || defined(__APPLE__)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif


typedef void* clyth_route_handler_fn;

static int64_t next_handle = 1;

static int64_t allocate_handle(void) {
    return next_handle++;
}

int64_t clyth_router_create(void) {
    return allocate_handle();
}

int32_t clyth_router_post(int64_t router_handle, const char* path, clyth_route_handler_fn handler) {
    if (router_handle <= 0 || path == NULL || handler == NULL) {
        return -EINVAL;
    }
    return 0;
}

int32_t clyth_router_get(int64_t router_handle, const char* path, clyth_route_handler_fn handler) {
    if (router_handle <= 0 || path == NULL || handler == NULL) {
        return -EINVAL;
    }
    return 0;
}

int64_t clyth_http_configuration_create(int32_t port, int64_t router_handle) {
    if (port <= 0 || port > 65535 || router_handle <= 0) {
        return -EINVAL;
    }
    return allocate_handle();
}

int64_t clyth_https_configuration_create(const char* key_pem, const char* cert_pem, int32_t port, int64_t router_handle) {
    if (key_pem == NULL || cert_pem == NULL || port <= 0 || port > 65535 || router_handle <= 0) {
        return -EINVAL;
    }
    return allocate_handle();
}

int64_t clyth_http_server_create(int64_t configuration_handle) {
    if (configuration_handle <= 0) {
        return -EINVAL;
    }
    return allocate_handle();
}

int64_t clyth_https_server_create(int64_t configuration_handle) {
    if (configuration_handle <= 0) {
        return -EINVAL;
    }
    return allocate_handle();
}

int32_t clyth_http_server_start(int64_t server_handle) {
    if (server_handle <= 0) {
        return -EINVAL;
    }
    return 0;
}

int32_t clyth_https_server_start(int64_t server_handle) {
    (void)server_handle;
    return -ENOSYS;
}

int32_t clyth_response_send_text(int64_t response_handle, int32_t code, const char* payload) {
    if (response_handle < 0 || code < 100 || payload == NULL) {
        return -EINVAL;
    }
    return 0;
}

int32_t clyth_response_send_json(int64_t response_handle, int32_t code, const char* payload) {
    if (response_handle < 0 || code < 100 || payload == NULL) {
        return -EINVAL;
    }
    return 0;
}

int32_t clyth_http_serve_text_once(int32_t port, const char* body) {
#if defined(__unix__) || defined(__APPLE__)
    if (body == NULL || port <= 0 || port > 65535) {
        return -EINVAL;
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        return -errno;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons((uint16_t)port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) != 0) {
        int saved_errno = errno;
        close(server_fd);
        return -saved_errno;
    }

    if (listen(server_fd, 1) != 0) {
        int saved_errno = errno;
        close(server_fd);
        return -saved_errno;
    }

    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        int saved_errno = errno;
        close(server_fd);
        return -saved_errno;
    }

    char request_buffer[1024];
    (void)recv(client_fd, request_buffer, sizeof(request_buffer), 0);

    char header[256];
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n",
        strlen(body));
    if (header_len > 0) {
        send(client_fd, header, (size_t)header_len, 0);
    }
    send(client_fd, body, strlen(body), 0);

    close(client_fd);
    close(server_fd);
    return 0;
#else
    (void)port;
    (void)body;
    return -ENOSYS;
#endif
}

int32_t clyth_https_serve_text_once(int32_t port, const char* cert_path, const char* key_path, const char* body) {
    (void)port;
    (void)cert_path;
    (void)key_path;
    (void)body;
    return -ENOSYS;
}
