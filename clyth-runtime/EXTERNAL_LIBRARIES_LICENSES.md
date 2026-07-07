# Clyth Runtime External Library Notes

Clyth runtime modules prefer C, Clyth, and musl-compatible C ABI bindings. Official runtime modules avoid C++ runtime dependencies.

## Active / Targeted Runtime Backends for 0.5.0

### rapidhash

Purpose: hashing backend for runtime collection support.

Clyth uses a thin C ABI shim around upstream rapidhash. The Clyth-facing API controls the seed value explicitly from Clyth code.

### libuv

Purpose: event loop, async filesystem, TCP, timers, and worker-thread/runtime scheduling foundation.

libuv is the planned shared lower layer for file I/O, networking, and future `async`/`await` runtime scheduling.

### llhttp

Purpose: HTTP/1.1 request parsing.

llhttp is parser-only. Clyth owns the router/server API and routes parsed requests into Clyth `Request` and `Response` values.

### yyjson

Purpose: JSON parsing, validation, and writing backend under the official Clyth JSON module.

yyjson is used behind a thin C ABI shim. The Clyth-facing JSON API remains owned by the Clyth runtime module.

### OpenSSL

Purpose: TLS/HTTPS backend.

Clyth uses OpenSSL as the HTTPS/TLS direction. wolfSSL is intentionally excluded because its licensing options do not fit Clyth's current distribution goals.

### wslay

Purpose: WebSocket frame/state handling after an HTTP Upgrade request is accepted by the Clyth HTTP/router layer.

wslay does not replace Clyth routing or HTTP parsing. It is the WebSocket protocol component under the Clyth-owned WebSocket API surface.

Project names above are used only to describe architecture and third-party dependency direction. Clyth is not affiliated with, endorsed by, or sponsored by those projects or their maintainers.
