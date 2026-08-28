# Clyth syntax decisions

## Imports

Module/package imports are unquoted and may contain hyphens:

```clyth
import file-io
import concurrency
```

Direct source-file imports are double quoted:

```clyth
import "support/helpers.clyth"
import "../shared/types.clyth" as types
```

The distinction is lexical and intentional: unquoted names resolve through the Clyth module/package system, while quoted names resolve as source paths.

## Callable types

Callable values use the function signature itself as the type, with the callable variable name following the type:

```clyth
void() callback
int32(string) parser
Response(Request, Response) handler
bool(Request, Response)[] filters
```

A callable type is parsed only in a type/declaration context. Calls and constructors remain expressions:

```clyth
callback()
parser(text)
Point(10, 20)
```

Explicit casts use a reserved cast expression and therefore do not compete with callable-type syntax:

```clyth
int8 tiny = cast<int8>(large)
```

Editor integrations provide lexical callable-type highlighting immediately. `clyth-lsp` additionally exposes semantic tokens for types, type parameters, callable variables, functions, module namespaces, and quoted source imports.

## Lambdas

Clyth V1.0.0 keeps lambda syntax deliberately narrow. A self-typed lambda uses `ReturnType(typed parameters) => { ... }` and can infer an `auto` variable:

```clyth
auto custom_lambda = void(int32 value) => {
    print(value);
};
```

An explicitly typed callable can receive either a fully typed lambda or a contextually typed lambda:

```clyth
void(int32) explicit_lambda = void(int32 value) => { print(value); };
void(int32) typed_context = (int32 value) => { print(value); };
void(int32) inferred_context = (value) => { print(value); };
```

The last form obtains `value: int32` from the receiving callable signature. `auto f = (value) => { ... }` is invalid because no parameter type is available. Anonymous lambdas are still valid when consumed immediately by an API, such as `for_each(void(Item *item) => { ... })` or `managed.pin(void() => { ... })`.

The `=>` token is the canonical boundary between a callable signature/parameter list and a lambda body; identifier-less function declaration syntax is not a lambda form.

## V1.0.0 enums and switch

Enums are scoped nominal types. Members are referenced through the enum name, and explicit integer values are permitted.

```clyth
enum Color { red, green = 3, blue }
Color value = Color.green
```

`switch` falls through by default. `break` exits the nearest switch or loop; `continue` remains loop-only.

```clyth
switch (value) {
case 1:
    first()
case 2:
    second()
    break
default:
    other()
}
```

## Struct member visibility

Struct members are public by default. `public` may be written explicitly, while `private` restricts access to methods, constructors, and `on_free` logic of the declaring struct.

```clyth
struct List<T> {
    private T[] data
    private int64 head

    private void ensure_capacity(int64 required) {
        // internal implementation detail
    }

    void append(T value) {
        ensure_capacity(length + 1)
    }
}
```

Private members remain usable through `this` or implicit receiver lookup inside the declaring struct, but member access such as `value.ensure_capacity(...)` from unrelated code is a semantic error.
