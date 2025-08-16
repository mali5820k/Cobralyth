# Cobralyth, Clyth for short
Clyth is an ahead-of-time (AOT) compiled language that aims to be the next iteration of C. It takes large inspirations from C++ runtime performance and syntax inspirations from scripting languages to reduce cognitive load on developers with improved ergonomics. Clyth offers an optional opt-in memory management system [MECC](#mecc-overview), which is designed to match or exceed C++ RAII smart-pointers performance for systems-level programming by compiling to LLVM IR.

In short Clyth is:
  * C++ RAII performance
  * Optional MECC (no stop-the-world collector, no tracing overhead)
  * Pass-by-reference
  * High-level language ergonomics
  * LLVM IR

## Project Goals:
### Primary Goals:
- Solving the loss of information of length and size with arrays when passed as function arguments.
- Default data-structures (dynamic lists, maps, and sets).
- Simplifying dereferencing symbols (right-arrow vs dot operator) to just the dot operator.
- [MECC](#mecc-overview), an OPT-IN non-blocking memory-management model, that aims to offer RAII C++ shared-pointer level performance with automatically managed ownership to prevent cyclic references, with no tracing or marking overhead.
- Package manager for Clyth
  - An out-of-the-box solution that can work offline or online.
  - Assisting license management on compilation - any licenses used by imported packages (MIT or BSD for instance) will be compiled into the binary and a separate licenses file to aid developers to automate compliance with those licenses. Note - license compliance ultimately falls onto the developer and any automation failures or mistakes must be checked by the developer for correct and proper compliance.
  
- Documentation and design visualizations will be made at a later date as a solid release of Clyth is made.

### Secondary (Future) Goals:
- JSON support
- GUI support
- Websockets and Webserver backend support
- Package and build system that can work locally instead of always requiring an online connection.
- Support for IoT devices and other embedded architectures, such as running on a raspberry pi
- OpenCL and CUDA support
- MLIR support for GPUs (this needs further research)
- Cross platform compilation to WASM using a non-browser runtime (ie wasmer)

## Runtime Specification:
  - Pass-by-reference is used by default.
    This is done to reduce calling complexity by allowing the programmer to simply call functions and make pass-by-value an explicitly opt-in operation - with the exception being primitive values like ints, floats, doubles, and chars being pass-by-value by default.

  - No function overloading is supported. This is to prevent Java-like code-expansion, which often results in more complexity in large codebases. If you need function-overloading, leverage structs or generics (in the future) and update the functions accordingly.
  For example, functions can receive a struct that resembles JavaScript-like objects with a variable corresponding to a value. 
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
    struct Simple_struct : MECC { // MECC headers are inherited and inlined by the compiler at compile-time.
      // Overriding an inherited function or method is as simple as redeclaring that function or method.
      // Return types are excluded for the below functions as they're implied MECC interface methods.
      // All other functions enclosed in struct definitions must declare their return types.
      constructor() {}
      destructor() {}
      clone() {}
      deep_clone() {}
    }
    ```
  - Functions will be first-class citizens, supported like any other custom or primitive type via the 'function' type. The main goal is to remove the verbosity involved with C-style function-pointers entirely and allow dynamic arguments and return-types leveraging structs and arrays - ultimately feeling more like Typescript or Javascript in this regard while maintaining the high-readability and predictability of systems-programming.
  Type annotations for function references are being considered and may be added at a future date.
    ```cpp
    struct Callback_registry : MECC {
      function last_callback_function,
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
  - The libc++ library will be leveraged to provide features to achieve general-purpose programming support -
  (this is subject to change if linking against libc++ becomes cumbersome enough to warrant a C runtime instead).
  - Memory management via [MECC](#mecc) - Clyth will offer a manually-managed runtime by default with an opt-in automated, managed memory model.
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
  - Managed Entanglement for Corrective Counting (MECC) Architecture for opt-in non-pausing ownership-assisted memory collection. MECC is a garbage collection algorithm/scheme that I have designed using inspiration from Rust's borrow-checker system and RAII smart-pointers from C++ (specifically shared pointers).
    - Eliminates any tracing, marking, and pausing (stop-the-world) that typical tracing garbage collectors suffer from.
    - Eliminates cyclic references by preventing reference-count changes from objects owned by a common root-address.
    - Inheritable MECC header, consisting of fixed 16-bytes on a 64-bit machine.
    ```mermaid
    ---
    title: MECC header in struct
    ---
    classDiagram
      direction LR
      MECC_header -- Struct_inheriting_MECC: Header-inlined

      note for Struct_root1_MECC "Reference Chain Sample"
      Struct_root1_MECC --> Struct_ref1_MECC:root-owns
      Struct_ref1_MECC --> Struct_ref2_MECC
      Struct_ref1_MECC ..> Struct_root1_MECC:points-to-root-address
      Struct_ref2_MECC ..> Struct_root1_MECC:points-to-root-address
      Struct_ref2_MECC .. Struct_ref1_MECC:no-reference-count
      Struct_root2_MECC ..|> Struct_ref1_MECC:"Increments reference-counter"
      Struct_root2_MECC --> Struct_refa1_MECC:root-owns
      Struct_refa1_MECC ..> Struct_root2_MECC:points-to-root-address
      
      class MECC_header["MECC Header"] {
        +int64* 8-byte-memory-address
        +uint64 8-byte-reference-counter
      }
      class Struct_inheriting_MECC["Sample struct inheriting MECC Header"] {
        +MECC_header header
        +string data_name
        Data_struct child_struct = new Data_struct() // Internally initialized members only contain 8-byte pointer-MECC field to parent struct
        Data_struct struct_reference // References point to other objects not initialized internally (full MECC header)
      }
      class Struct_root1_MECC["Root - reference chain 1"] {
        +MECC_header header
        +string data_name
      }
      class Struct_ref1_MECC["Ref 1"] {
        +MECC_header header
        +string data_name
      }
      class Struct_ref2_MECC["Ref 2"] {
        +MECC_header header
        +string data_name
      }
      class Struct_root2_MECC["Root - reference chain 2"] {
        +MECC_header header
        +string data_name
      }
      class Struct_refa1_MECC["Ref 3"] {
        +MECC_header header
        +string data_name
      }
    ```

    - For more MECC details, you can find them [here](./MemoryManagement.md/#mecc-additional-details).

  - Multiple options have been explored in great planning and are listed here to showcase how the memory-management of Clyth has evolved. 
  However - the Clyth language will adopt the MECC model as an opt-in memory-management model to allow system level programmers the flexibility of picking between manual memory management and an opt-in non-blocking, high-performance, and a fixed minimal-overhead memory management model.
  If you're interested in reading into the other options that were explored, you can find them [here](./MemoryMangement.md/#additional-memory-management-options-explored).
    
## Author's Note, Acknowledgements, and Legal comments:
- Upon completing the C-based Lox Programming language implementation from Bob (Robert) Nystrom's "Crafting Interpreters" book, I set out to construct my own programming language from what I had learned then and have learned thus far in my own free time, doing research into programming language design and construction of AOT, JIT compilers, and interpreters. For reference, this project is not a continuation of Nystrom's work, nor is it a continuation of my past work that was derived from his book - this project is my own from-scratch implementation for a language that I am designing from the ground up.
- After considerable thought, I decided to steer away from my previous design decisions to use Go, CIL bytecode, Java bytecode, C, and C++ for my compiler's output language targets. As this is a passion project of mine, the main reason to develop Clyth is to simplify developer-experience for those who love C programming for it's simplicity but need a modernized implementation to fix the pitfalls of C related to it being too barebones, yet to not compromise on performance when garbage collection is used while providing features that are found in higher-level languages.

- This project is free from AI generated code - this is my own work as it's a passion-project of mine.
- The project has undergone redesigns and this is the approach that I've landed on to stick with until Clyth itself is bootstrappable.
- Additionally, any open-source libraries included in this project are mentioned in the 'EXTERNAL_LIBRARIES_LICENSES.md" file for due-diligence to the best of my ability.
- I had to delete the old repository for the sake of cleaning up the entire project. This is now the latest project with the files carrying over from the previous repo version that was deleted. The license was originally from 2022, so that was the only adjustment I made in this repo's license to revert it back to 2022 from 2023.

- I'm in no way assuming trademark of the name "Cobralyth" as it's an open-source project of mine and personally, I think trademarking programming language names is ridiculous as it's a language name, but I'm not a lawyer and this is my statement to anyone it may pertain to that my intention for using "Cobralyth" as a name is strictly for the sake of naming my project and not to infringe on trademarks or cause legal conflicts, and I assume no legal responsibility in the case this name is used elsewhere.
- LEGAL DISCLAIMER - I am not legally responsible for ensuring compiled programs are in compliance with licenses. Reach out to your own legal department or contacts to ensure you are compliant as I am not a lawyer.