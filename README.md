# Cobralyth, Clyth for short

## Current Status:
The Clyth frontend is currently being explored between an Antlr4 frontend implementation and a C++ Pratt Parser implementation. This decision may swing in favor of Antlr4 if project time and effort exceeds the benefits of maintaining a custom parser.

The project is evolving and the Readme may be updated to reflect the latest design decisions.

## About Clyth:
Clyth is an ahead-of-time (AOT) compiled language that aims to be an opinionated iteration of C that improves memory-management experience for developers without the runtime overhead associated with common garbage collection implementations. It takes major inspiration from C++ runtime performance and syntax inspired by scripting languages to reduce cognitive load on developers with improved ergonomics. The Managed Entanglement with Container-based Collections (MECC) system is designed to automatically transform developer code into single-ownership compliant code with pre-determined lifetimes for memory collection at compile-time, enabling performance comparable to Rust's borrow-checker and C++ RAII smart-pointers or arena based schemas in terms of runtime CPU performance without the cognitive burden on developers for adapting to a new paradigm of programming and without the tracing overhead common in garbage collected languages like Golang.

In short Clyth is:
  * Ahead Of Time (AOT) compiled
  * [MECC](#mecc-overview)
  * Pass-by-reference
  * High-level language ergonomics
  * LLVM IR

You will find the project split into sub-projects that all fall under the main MIT license specified in the repo:
  * compiler-src
    - A C++ project, specifically responsible for generating and consuming an AST and producing a binary from LLVM IR to statically link against the clyth-runtime
  * clyth-runtime
    - The language's standard library to provide file-io, multi-threading, common datastructures, and more as the project grows in maturity.
  As with all projects - the directory structure may change as the project evolves.

## Goals and Aspirations:
### Primary Goals:
- Solving the loss of information of length and size with arrays when passed as function arguments.
- Default data-structures (dynamic lists, maps, and sets).
- Simplifying dereferencing symbols (right-arrow vs dot operator) to just the dot operator.
- [MECC](#mecc-overview), a non-blocking memory-management model with automatic single-ownership enforcement for memory reclamation to prevent cyclic references.

### Future Aspirations:
- Documentation and design visualizations will be made at a later date as a solid release of Clyth is made.
- Package manager for Clyth
  - An out-of-the-box solution that can work offline or online.
  - Assisting license management on compilation - any licenses used by imported packages (MIT or BSD for instance) will be compiled into the binary and a separate licenses file to aid developers to automate compliance with those licenses. Note - license compliance ultimately falls onto the developer and any automation failures or mistakes must be checked by the developer for correct and proper compliance.
- JSON support
- GUI support
- Websockets and Webserver backend support
- Package and build system that can work locally instead of always requiring an online connection.
- Support for IoT devices and other embedded architectures, such as running on a raspberry pi
- OpenCL and CUDA support
- MLIR support for GPUs (this needs further research)
- Cross platform compilation to WASM using a non-browser runtime (i.e., wasmer)

## The Current Runtime Specification (subject to change as project evolves):
  - Pass-by-reference is used by default.
    This is done to reduce calling complexity by allowing the programmer to simply call functions and make pass-by-value an explicitly opt-in operation - with the exception being primitive values like ints, floats, doubles, and chars being pass-by-value by default.

  - No function overloading is supported. This is to prevent Java-like code-expansion, which often results in more complexity in large codebases. If you need function-overloading, leverage structs or generics (in the future) and update the functions accordingly.
  For example, functions can receive a struct that resembles JavaScript-like objects with a variable corresponding to a value. This is subject to change if the ergonomics for writing generic code are hindered greatly.
    ```cpp
    int test_function (Obj1 object1, int64 value2, bool optional=false) {
      // ... more logic here
      return 0
    }

    int main(string[] args) {
      printf(test_function({ object1: new Obj1(), value2: 100 }))
      printf(test_function({ value2: 100 }))

      return 0
    }
    ```
  - Structural inheritance will be used to inline the inherited struct-members into the inheriting struct.
  This avoids unnecessary nesting and an interface-like approach for a leaner object-oriented programming approach.
    ```cpp
    struct Simple_struct : ParentStruct {
      // Overriding an inherited function or method is as simple as redeclaring that function or method.
      // All other functions enclosed in struct definitions must declare their return types.
      constructor() {}
      destructor() {}
      clone() {}
      deep_clone() {}
    }
    ```
  - Functions will be first-class citizens, supported like any other custom or primitive type via the 'function' type. The main goal is to remove the verbosity involved with C-style function-pointers entirely and allow dynamic arguments and return-types leveraging structs and arrays - ultimately feeling more like Typescript or Javascript in this regard while maintaining the high-readability and predictability of systems-programming.
  The final syntax for 'args {}' may evolve as the design is implemented.
  Type annotations for function references are being considered and may be added at a future date.
    ```cpp
    struct Callback_registry {
      function last_callback_function
      function[] callback_functions

      constructor() {}
      destructor() {}
      clone() {}
      deep_clone() {}

      void register_callback_function(function myFunc, args {}=null) {
        this.last_callback_function = myFunc
        this.last_callback_function.args = args
        this.callback_functions.push(myFunc, args) // alternatively .append - also args is optional if args is null/empty
      }

      void call_registered_functions() {
        this.last_callback_function.call(this.last_callback_function.args)
        this.last_callback_function.call({100, "myName"}) // can change args to call function with as well if you know the args-signature
        for auto callback_function in this.callback_functions {
          callback_function.call({12003, "different args name"}) // different args
        }
      }
    }

    bool sample_function(int64 myArg, string name) {
      printf(myArg, name)
      return true
    }

    int main(string[] args) {
      Callback_registry local_registry = new Callback_registry()
      local_registry.register_callback_function(sample_function, {100, "sample_function is my name!"})
      local_registry.register_callback_function(sample_function, {20009234, "another sample function!"})
      local_registry.call_registered_functions()

      return 0
    }
    ```
  - Memory management via [MECC](#mecc-overview)
  - Concurrency:
    - Threads will be implemented in a similar manner to how C does, however, the final implementation will favor intuitiveness and take inspiration from Go's goroutines - yet different in the execution and scheduling of threads.
    ```cpp
      // Thread signature:
      Thread (function thread_instructions, args {})

      bool sort_data(string[] data) {
        // ... sorting-logic ... //
        return true
      }

      int main(string[] args) {
          Thread background_task = new Thread(sort_data, {data: ["1", "2", "3", "4", "apple", "box", "pear", "BaNANa"]})
          background_task.run()
          background_task.join() // will attempt to join thread - will wait until thread is finished.
          printf(background_task.value()) // returns the value returned by the function running in the thread
      }
    ```

- ### Memory Management:
  #### MECC Overview:
  - Managed Entanglement with Container-based Collections (MECC) Architecture for non-pausing ownership-based memory collection.
  - Conceptually consider MECC as a compile time static-analysis and structuring memory management for runtime to ensure resources are freed in a deterministic fashion without burdening the user with borrow-checker semantics and learning curve. The compiler aims to do the heavy-lifting and keep the experience as close as possible to writing C-like code.
    - Eliminates tracing, marking + sweeping, and pausing (stop-the-world) that typical tracing garbage collectors suffer from.
    - Eliminates cyclic references by enforcing single-ownership semantics and weak-pointer references for object references and containers (lists/vectors, maps, etc) by default.
    - The cognitive load of memory-management is lifted away from the programmer and managed by the compiler - therefore semantics for strong versus weak references are managed by the compiler at compilation.
    - No naked pointers - each reference is wrapped in a smart-pointer-esque wrapper where a bit signifies if the wrapper owns the reference being pointed to or not. These are what get passed around and NOT raw references.
    All container objects hold references to these wrappers but only on a move/assignment operation - by themselves, they're empty, and no duplicate wrappers will exist for a referenced object as move operations will be done by the compiler to ensure single references after unnecessary aliases are removed (see below for alias cleanup example).
    ```cpp
    // NOTE: This is not exposed in Clyth, this is an implicit set of metadata built into the language itself.
    struct ExampleWrapperVisualization<T> {
      bool owns_reference,
      generic<T> reference_to_obj
    }
    ```
    - All objects and containers (lists, arrays, linked-lists, maps, and sets) ownership semantics follows this simple set of rules:
    1. Containers will have priority over individual variable reference objects when ownership is being established - unless the individual variable reference is global alongside the collection it's competing with.
    ```cpp
    struct TestObj {
      int32 value;
    }
    TestObj[] a_global_obj_ref = []
    TestObj my_global_obj

    void testFunc() {
      TestObj my_local_obj = new TestObj();
      a_global_obj_ref.push(my_local_obj) // Ownership of the TestObj reference now goes to a_global_obj_ref, even though my_local_obj was assigned it first.
    }

    void testFunc2() {
      my_global_obj = new TestObj();
      a_global_obj_ref.push(my_global_obj) // Ownership of the TestObj reference DOES NOT CHANGE since my_global_obj is global and there's no point in transferring ownership between two identical lifetime containers/variables.
    }

    int32 main(string[] args) {
      testFunc() // Global container competing with local variable
      testFunc2() // Global variable competing with global container
    }
    ```
    2. Globally declared variables and containers have the highest precedence for obtaining ownership of objects when assigned to them.
    ```cpp
    struct TestObj {
      int32 value;
    }
    let a_global_obj_ref = []
    void testFunc() {
      TestObj my_local_obj = new TestObj();
      a_global_obj_ref.push(my_local_obj) // Ownership of the TestObj reference now goes to a_global_obj_ref, even though my_local_obj was assigned it first.
    }

    int32 main(string[] args) {
      testFunc()
    }
    ```
    3. If no global variable or container is in the current scope, the function/method parameter variables and containers will take precedence for ownership on a first-come first-serve basis.
    ```cpp
    struct TestObj {
      int32 value;
    }
    void testFunc(TestObj[] param_container) {
      TestObj my_local_obj = new TestObj();
      param_container.push(my_local_obj) // Ownership of the TestObj reference now goes to a_global_obj_ref, even though my_local_obj was assigned it first.
    }

    int32 main(string[] args) {
      testFunc()
    }
    ```
    4. Lastly, non-parameter variables and containers have the lowest precedence for obtaining ownership of objects, so if none of their assignments are re-assigned to higher-priority variables or containers, they are owners of their assigned data references.
    ```cpp
      struct TestObj {
        int32 value;
      }
      void testFunc() {
        TestObj my_local_obj = new TestObj();
      }

      int32 main(string[] args) {
        testFunc()
      }
    ```
    - Cleanup of containers and variables is done at the end of a scope - if lifetimes persist beyond a scope, they will continue to exist.
    - Any aliases in the same scope to the same reference will be cleaned up by the compiler at compile-time. This removes unnecessary aliases and clutter in the final produced code.
    ie: source-code:
    ```cpp
    int32 main(string[] args) {
      let alias1 = args;
      let alias2 = alias1;
      let alias3 = alias2;

      printf(`Alias cleanup for args: ${alias3}`)
    }
    ```
    transforms into:
    ```cpp
    int32 main(string[] args) {
      printf(`Alias cleanup for args: ${args}`)
    }
    ```
## Acknowledgements and Legal comments:
- Zig compiler(s) toolchain is leveraged by the project to have cross-platform standalone binaries, specifically used for linking against musl-libc and llvm's libc++ libraries without any glibc or gnu libstdc++ implementations being used. This is primarily done for license preferences and avoiding GPL or LGPL licensing from impacting the distribution of binaries when statically linked. This is mentioned in the [EXTERNAL_LIBRARIES_LICENSES.md](./compiler-src/EXTERNAL_LIBRARIES_LICENSES.md) file for the compiler-backend and clyth-runtime projects which leverage the static-linkage capabilities.
- This project is free from AI generated code - this is my own work as it's a passion-project of mine.
- LEGAL DISCLAIMER - I am not legally responsible for ensuring license(s) compliance for programs written and compiled with Clyth. Reach out to your own legal department or contacts to ensure you are compliant with any licenses you end up using in your final executable as I am not a lawyer.
