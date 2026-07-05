# Clyth JSON Runtime Module

JSON belongs in the runtime, not the compiler.

0.5.0 establishes the official JSON module boundary and keeps HTTP/HTTPS JSON responses Clyth-owned. Full parsing/stringifying support is planned as Clyth runtime work, built from native strings, arrays, maps, lists, keyed arrays, and struct/object formatting.

Current practical support:

- JSON payload strings can be sent through HTTP/HTTPS response APIs.
- `JsonValue` establishes the tagged runtime value direction.
- `json_raw`, `json_string`, and `json_to_string` provide a minimal module surface for examples and future expansion.

Near-term work:

- `JSON.stringify(value)` style object formatting.
- `JSON.parse(string)` to produce `JsonValue`.
- Object/array builders.
- Integration with `Map<string, T>` and struct `to_string` / formatting hooks.
