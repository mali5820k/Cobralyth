# Cobralyth, Clyth for short

## Current Status:
Clyth is currently being transitioned from an Antlr4 frontend implementation to a C++ and Pratt Parser implementation for future development flexibility and eventually performance.

The project is evolving and the Readme may be updated to reflect the latest design decisions.

## About Clyth:
Clyth is an ahead-of-time (AOT) compiled language that aims to be an opinionated iteration of C that improves memory-management experience for developers without the runtime overhead associated with common garbage collection implementations. It takes major inspiration from C++ runtime performance and syntax inspired by scripting languages to reduce cognitive load on developers with improved ergonomics. The Managed Entanglement for Collapsable Collection [MECC](#mecc-overview) memory management system is designed to embed arena-oriented-programming (AOP) into the base language, enabling performance comparable to C++ RAII smart-pointers or arena based schemas in terms of runtime CPU performance without the tracing overhead common in garbage collected languages like Golang.

In short Clyth is:
  * Ahead Of Time (AOT) compiled
  * Arena Oriented Programming (AOP)
  * MECC (no stop-the-world collector, no tracing overhead)
  * Pass-by-reference
  * High-level language ergonomics
  * LLVM IR

You will find the project split into three sub-projects - All sub-projects fall under the MIT license specified in the repo:
  * compiler-src
    - A C++ project, specifically responsible for consuming the linearized AST from the compiler-frontend and producing LLVM IR and final binary statically linked against the clyth-runtime
  * clyth-runtime
    - A C project that provides external libraries leveraging Musl-libc to provide standalone binaries with permissive licenses. A C runtime was chosen to avoid the name-mangling issues encountered with C++ (LLVM libc++) when calling functions from within generated LLVM IR.
  As with all projects - the directory structure may change as the project evolves.

## Goals and Aspirations:
### Primary Goals:
- Solving the loss of information of length and size with arrays when passed as function arguments.
- Default data-structures (dynamic lists, maps, and sets).
- Simplifying dereferencing symbols (right-arrow vs dot operator) to just the dot operator.
- [MECC](#mecc-overview), a non-blocking memory-management model with automatic single-ownership enforcement for memory reclamation to prevent cyclic references in memory-arenas.

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
  - The Musl-libc library (in conjunction with external C libraries mentioned in the external libraries readme file under the clyth-runtime project folder) will be leveraged to provide features to achieve general-purpose programming support.
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
  - Managed Entanglement for Collapsable Collection (MECC) Architecture for non-pausing ownership-based memory collection.
  - Conceptually consider MECC as compile time static-analysis to ensure resources are freed in a deterministic fashion without burdening the user with borrow-checker semantics and learning curve. The compiler aims to do the heavy-lifting and keep the experience as close as possible to writing C-like code.
    - Eliminates tracing, marking, and pausing (stop-the-world) that typical tracing garbage collectors suffer from.
    - Eliminates cyclic references by enforcing single-ownership with arena promotions and weak-pointer like behavior for containers (lists/vectors, maps, etc) by default.
    - A scope is a memory-arena by default, which ensures all heap allocations are made to a default location if an alternative arena isn't defined for allocations.
    - A memory-arena can be promoted and bound to a parent's memory-arena. This is especially leveraged in Clyth to perform escape analysis on returned elements and promote them to the arena of the calling scope (i.e., main method arena is the parent arena for any function called directly from the main function's scope).
    - A memory-arena may grow in size to accommodate additional allocations. This is managed via arena.add(object) calls and can be optional as the compiler will do this step unless the programmer chooses another arena to promote or allocate values into.
    - Binding is the action of making an arena from a different parent-scope to be bound to the same lifetime. As an example, threads of different arenas may maintain different parent scopes. The inspiration of this is to allow cases where a developer may wish to allocate an arena and preserve it for different stages of a program.
    - Unmounting is the action of 'freeing' memory. The unmount() calls are injected by the compiler after function-call boundaries, end of scopes, and at unmount(object) calls which are entirely optional and are written by the programmer if memory is no longer needed and can be freed earlier than MECC's unmount events. A note - MECC doesn't pause for freeing memory, these instructions are akin to calling 'free' in C or 'delete' in C++.
    - Transitive dependencies occur when a returned object references other heap-allocated objects from the same scope. The compiler (MECC) will ensure those transitive dependencies are within the same memory arena as the returned object by promoting to the parent-arena at compile time. On deallocation, the transitive dependencies are also freed as they share the same memory-arena.
    - Ownership limitation of containers (lists/vectors, maps, etc) due to MECC - CONTAINERS DO NOT own elements unless they're value-types (primitives). Any heap-referenced or allocated objects are not owned by containers, instead the pointers within the container (which point to objects on the heap) are owned and unmounting a container simply frees the reference-pointers and the container itself NOT the elements referenced by those pointers. This preserves ownership of elements by arena and avoids cases where two or more containers refer to the same object and one container gets deallocated, effectively causing dangling pointer references to non-existent memory everywhere else.
    Conceptually, you may consider containers emulating 'weak-pointer references'.
    
## Acknowledgements and Legal comments:
- Zig compiler(s) toolchain is leveraged by the project to have cross-platform standalone binaries, specifically used for linking against musl-libc and llvm's libc++ libraries without any glibc or gnu libstdc++ implementations being used. This is primarily done for license preferences and avoiding GPL or LGPL licensing from impacting the distribution of binaries when statically linked. This is mentioned in the [EXTERNAL_LIBRARIES_LICENSES.md](./compiler-src/EXTERNAL_LIBRARIES_LICENSES.md) file for the compiler-backend and clyth-runtime projects which leverage the static-linkage capabilities.
- This project is free from AI generated code - this is my own work as it's a passion-project of mine.
- LEGAL DISCLAIMER - I am not legally responsible for ensuring license(s) compliance for programs written and compiled with Clyth. Reach out to your own legal department or contacts to ensure you are compliant with any licenses you end up using in your final executable as I am not a lawyer.