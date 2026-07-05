# Clyth Backend HTTP Demo

This preview demonstrates a browser page served by a Clyth program. The page includes a JSON textarea and JavaScript that POSTs to `/api/echo`. The C runtime binding responds with a JSON confirmation payload containing the submitted body.

This is intentionally a 0.5.0 proof-of-capability sample. The longer-term HTTP/HTTPS runtime stack is Clyth-owned at the API layer and backed by libuv, llhttp, OpenSSL, wslay, yyjson, and rapidhash modules where appropriate.

Third-party project names are used only to describe runtime architecture and dependencies. Clyth is not affiliated with, endorsed by, or sponsored by those projects or their maintainers.

## Run

From the repository root:

```bash
./build_compiler.sh --skip-dist
mkdir -p sample-clyth-programs/.sample-bin
./build-compiler/clyth_compiler_bin \
  -c sample-clyth-programs/web-app-preview/clyth_backend_welcome.clyth \
  -o sample-clyth-programs/.sample-bin/clyth_backend_welcome
./sample-clyth-programs/.sample-bin/clyth_backend_welcome
```

Then open:

```text
http://127.0.0.1:9090
```

The current one-shot demo server handles one browser request per process. Refresh or rerun the sample when needed.
