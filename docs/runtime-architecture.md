# Runtime architecture

Clyth keeps the native C layer deliberately thin. C bindings expose operating-system or ABI primitives; scheduling policy and higher-level composition belong in Clyth modules.

## Dependency direction

```text
musl-libc
    |
    +-- concurrency  -> pthread primitives and shared worker pools
    +-- file-io      -> direct POSIX filesystem calls
    +-- network      -> direct socket/epoll primitives (planned Clyth-facing layer)

concurrency + file-io + network
    -> HTTP/router/HTTPS modules written primarily in Clyth
       +-- llhttp: HTTP/1 parser only
       +-- wslay: WebSocket framing/protocol only
```

`llhttp` and `wslay` are leaf protocol dependencies. They must not own Clyth's event loop, worker scheduling, filesystem layer, or thread policy. Their source is fetched on demand from the exact commits in `third-party.lock`; upstream source trees are never checked into this repository.

The intended server model is one readiness/event-loop thread plus work queues backed by the shared `concurrency` module. DNS, filesystem, and other potentially blocking tasks can use separate logical queues without requiring duplicated thread pools. Higher-level networking code chooses dispatch and load-balancing policy.

The current native router transport is a transitional direct Linux `epoll` implementation replacing the previous event-loop dependency. As the self-hosted runtime matures, policy should move upward into Clyth while the native layer shrinks toward socket/epoll ABI calls only.
