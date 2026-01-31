# Clyth Runtime

## Project files:
- Clyth runtime consists of a C stack of libraries to provide common language runtime features, such as file-io, HTTP/HTTPS web-server, storage containers (ie vectors, maps, and sets), Clyth types, and MECC support.
- The runtime hasn't been implemented yet at the time of writing this readme - however these are the expected libraries that will be incorporated into the implementation:
    1. https://github.com/kraj/musl?tab=readme-ov-file (github mirror of original site https://git.musl-libc.org/cgit/musl/tree/README) - Musl-libc for standalone binaries via Zig - this is the backing of the C standard library used in this implementation due to it's permissive licensing and toolchain support for building lean-static binaries via the Zig compiler(s) toolchain. This effectively allows all Clyth programs to be standalone binaries once compiled for a target architecture and operating system.
    The added benefits of a C-based runtime is name mangling won't interfere when calling functions in LLVM IR.

    2. https://github.com/DaveGamble/cJSON?tab=readme-ov-file - for JSON support.
