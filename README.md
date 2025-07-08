# Cobralyth, Clyth for short
A Compiled language that is inspired from C++ runtime performance and Python's scripting properties, as well as various syntax inspirations from C#, JavaScript, TypeScript, Java, and Kotlin.

## Current state:
This C++ frontend approach is being put on hold/archived.
I simply want to prototype this frontend more rapidly without concern of the project collapsing inwards whenever an implementation update is made and get onto bootstrapping Clyth with itself and I keep finding myself re-writing the compiler's frontend and designing utility functions that I very well could just spend the same amount of time prototyping more with a higher-level programming language.
If at any point I want to come back to this, I will and update accordingly. For now, I'm leaving this here to show efforts were made and why I left this branch as it is now.

- Frontend is being constructed with C++ and a Pratt parser implementation.
- Backend will be C++ for runtime features, statically linking with libc++ from LLVM.
- Additional backend option targetted is MLIR for GPU agnostic programming.

## Project Goals:
### Primary Goals:
- An opinionated C implementation that addresses the loss of information of length and size with arrays, default data-structures such as dynamic lists, maps, and sets, and syntactical revision of dereferencing symbol (right-arrow vs dot operator)
  - This is crucially the first implementation step as it will allow the base language of Clyth to start supporting itself with Clyth-written libraries.
  - Leveraging a C++ backend for specific runtime and library implementations - such as threads, file-io, and etc...
  - Linking with libc++ provided by LLVM for specific runtime features, ie vectors, maps, sets, and strings.
- Simplifying syntax for virtual thread creation and creating schedulers for virtual thread orchestration.
- Providing features usually found in scripting languages (ie dynamic lists, pre-included data-structures, generics, type-inference as a later feature)
- File-IO

### Secondary Goals:
- GUI support
- Websockets and Webserver backend support
- Package and build system that can work locally instead of always requiring an online connection.
- IoT devices such as running on a raspberry pi

### Optional goal(s):
- OpenCL and CUDA support 
- Cross platform compilation to wasm using a non-browser runtime (ie wasmer)
- Function-leaping/hopping or Function-zipping (still trying to decide on a good name for this feature if it becomes a sensible/practical feature):
   - Where the function-control-flow can be altered by returning directly to the root-caller of a function-call-stack instead of linear return-flow from function to function. 
   - This is different from gotos since it's limiting the hop to the root-caller and not freely allowing jumps to any other block of code.
   - Example for what it would look like if an existing language supported such a feature out of the box (using Go for displaying the concept) 
      - (Go doesn't have this feature): 
    ```go
    func main() {
      myVal := val1(1, 2)
      fmt.Printf("%f", myVal) // This should print out 100
    }
    
    func val1(a int, b int) int {
      if (a > 5) {
        return 0;
      }
      
      return val2(a, b);
    }
    
    func val2(a int, b int) float32 {
      if (a == 2) {
        return (math.pow(a, b)* 0.75)
      }
      val2.returnToRoot(math.pow(a*10, b))
    }
    ```

    Regular function call logic-flow for the above block looks like this:
    ```mermaid
    graph TD;
      main-->val1;
      val1-->main;
      val1-->val2;
      val2-->val1;
    ```

    Function hopping looks like this:
    ```mermaid
    graph TD;
      main-->val1;
      val1-->val2;
      val2-->main;
    ```
    
# Project Structure:
   The project is divided into three main parts:
  - Frontend:
    - Lexer
    - Parser
    - Main-program entrypoint
  - Backend:
    - Codegen module to compile to a specific target language (currently C++)
       - Also variants of codegen modules for utilizing different runtime module API calls.
    - Debug module for debugger 
  - Runtime module(s):
    - Files that provide a C++ API exposing the language's runtime functionality (Not a VM or Interpreter)
    - Aim is to allow for swapping of runtime modules for testing different runtimes and features.
## Runtime Specification:
- Pass-by-reference and Pass-by-value handling:
   - Pass-by-reference is used by prefixing a function's parameter with a 'ref<type>', pass-by-value is used otherwise.
     This is done to reduce calling complexity by allowing the programmer to simply call functions and let the compiler do the heavy-lifting for packaging the parameters in a ref depending on the function's signature.
     For example:
     ```c
     // Pass-by-value:
     void sampleFunction_byValue(int x, int y) {
         x += y;
         y *= x;
     }

     int x = 10;
     int y = 20;
     
     sampleFunction_byValue(x, y); // x remains 10 and y remains 20

     // Pass-by-reference:
     void sampleFunction2_byRef(ref<int> x, ref<int> y) {
         x += y;
         y *= x;
     }
     sampleFunction2_byRef(x, y); // x is now 30 and y is now 600
     ```
     All Clyth code constructs are aimed to map to equivalent 1-2 lines of C++ to provide predictable mappings when debugging.
     Additionally, all Clyth objects are pass-by-reference by default. Any pass-by-value is to be handled via explicit copy operations.
     Furthermore, in favor of readability - no function overloading is supported.

- ### Memory Management:
  - Multiple options have been explored in great planning, however the base or core Clyth language will implement Option 4.

    - Option 1: Scope-based Inheritance Borrow Checker Reference Table Scheme (SIBCRT)
      - Each scope will maintain its own SIBCRT which tracks strong and weak references amongst objects to prevent cyclic references from occurring in mark-and-sweep-like garbage-collected languages, theoretically reducing garbage collection overhead if done in a minimal fashion.
      - Strong references can be made by any object to another, as long as the object storing the reference doesn't already have the object it's referring to stored as a strong reference in one of its parents. IE objA objB and objC exist, objA stores a strong reference to objC, objC stores a strong reference to objB, but objB can't store a strong reference to objA nor objC if it needs to, therefore objB will resort to weak-references to both objA and objC to prevent a cycle of references from occurring.
      - SIBCRT lookups will be performed whenever a reference is assigned by an object to another on invocation of the "=" operator. The SIBCRT for that scope will be referenced and the parents of the object being pointed to will be checked for existing strong references before assigning the type of reference to use.
      - Owners of an object can be multiple in number, they will simply be appended to a queue of owners that will "inherit" the ownership of the object that all the other owners are currently referencing as well. In the case an owner goes out of scope or is no longer referring to an object with multiple owners, then only that owner is removed from the queue of owners. Once all owners no longer maintain the reference to the shared object, that object will be garbage-collected as no strong references will remain after the last owner loses the reference to the shared object.
      - When exiting a scope, the SIBCRT will:
         - upgrade all weak-references of an object that is being returned to strong-references before returning from the scope.
         - transfer the "owner" of the object being returned to the parent-scope's SIBCRT
      - Async Atomic Tracing Garbage Collector Component
        - Garbage Collector will run atomically and asynchronously to the main program, preventing GC overhead induced from common stop-the-world collectors at runtime.
        - Will only be invoked whenever a strong-reference goes out of scope or an owner of an object from the queue of owners is freed or no longer maintains the strong reference it had on its referenced object.

    - Option 2: Tracing garbage collector:
      - Not a high-priority to implement since most languages have or have had some form of this type of garbage collector. This collector type can be implemented at a later date, but implementing this design for the main language would introduce cycles and memory overhead that generally slows performance down considerably and introduces challenges to parallelize across multiple threads.

    - Option 3: EC Architecture (Entanglement Component(s))
      - EC Architecture consists of EC entities and EC sockets. Inspired by exploration of Websockets and Smart-Pointers. The entire purpose of this architecture is to have a lean garbage collector that mimicks manual memory management but is automated via a borrow-checker-like 'entanglement' system. Entanglement can be best described as a subscription of components (known as EC entities) and sockets (known as EC sockets) that manage them. This allows for a runtime-oriented implementation that allows for greater flexibility than a typical reference-counting or ahead of time (AOT) borrow-checker implementation, all while stripping the usual Garbage-Collection 'helper' cycles to determine cyclic refrences,
        - EC Entities:
          - Wrappers around data or objects that 
        - EC Sockets:
          - Maintain references to EC entities where ownership of entities is managed through transfer operations.
          - Difference between smart-pointers in C++ and EC sockets is this: EC sockets manage transfer operations for reference passing, copying of pointers or objects, being thread-safe (atomic), and NOT relying on reference counting while allowing for a flexible borrow-checker-like Garbage collection scheme at runtime.
          - EC sockets manage memory/garbage collection of the EC entities they have ownership over. As the name implies, each EC socket is allowed to manage an EC entity if the EC socket has ownership over that particular This action of entities being managed by sockets is called 'entanglement', which is what makes the 'Entanglement-Component' acronym in the EC Architecture. As a result of a single-owner, sockets have method that can be invoked to perform various null-safe or non-null-safe operations for transferring reference-ownership, non-ownership-references, copy of entity that's currently entangled. This greatly simplifies memory management to where GC tracing for cyclic cycles is no longer a concern and memory deallocations can occur asynchronously in a 'chain-like' effect, similar to how manual memory-management in C and C++ languages is conducted but more-inline with C++ destructors being automatically called.
        - Transfer operations:
          - transfer: Transfer of ownership over to another EC socket, performing null-safety-checks to ensure the EC entity object itself isn't null.
          - ftransfer: Fast-transfer of ownership over to another EC Socket, not performing any null-safety-checks.
          - ctransfer: Copy-transfer, which copies the original object's memory and transfers ownership of that copied entity to the new EC socket.
        - Detail Specification:
          - This requires a EC-centric standard library implementation that covers all primitives of the language to allow the EC system to have first-class support. An additional standard library implementation for non-EC primitives will be made for the sake of advanced systems programming for those who require it, especially in embedded system environments/platforms. These standard libraries will contain mostly primitives and datastructures that are the building-blocks to creating seemless objects and constructs with for developers. In the case of the EC-standard library, its main focus is to provide EC-friendly datastructures, primitives, and types, allowing developers to not concern themselves with managing the memory, instead focusing on the implementation details of their programs with the expectation of automated memory management.
            - Standard library variants would allow each library variant a unique modularity that allows any type of GC to be supported. The intended design is for each standard library type to be namespaced under the ```stdlib.{variant name}``` nomenclature for separation of concerns.
            - Cross-compatibility between EC and non-EC components will be managed through a 'wrapper' class that any non-EC component can be wrapped into to allow cross-compatibility.
            - Datastructures for the EC-centric standard library will primarily consist of EC sockets, and optimized with integration for EC entities.
              - Developers can specify if the specific datastructure or EC socket being assigned an EC entity is receiving direct ownership or a reference. All references are considered weak-references unless an ownership transfer is used for that particular reference EC entity that contains that pointer value.
              - Nested EC sockets are possible, which will allow for multi-dimensional data-structures like 3D+ arrays/lists.

    - Option 4: (Prioritized and Preferred implementation): Simplified Option 3 by orchestrating smart-pointers:
      - Option 3's EC Architecture can be mapped to a much simpler smart-pointer reference - where at any-point in time the smart-pointer holds onto it's owned data as a strong-reference, and only when transferring ownership does it transition to a weak-pointer in behavior where it no longer affects the deallocation of the previously owned data.
      - Implementation can be as straightforward as a struct with a generic pointer to the target type/storage type, and a flag/bit-toggle to alter the deallocation behavior for the object on strong to weak reference transition - and vice-versa as well.
      ```Java
      {
        SampleClass myClass = new SampleClass();
        SampleClass myClass2 = myClass; // ownership transfer (if in the same scope as the original object's creation).
        SampleClass myClass3ref = &myClass2; // This is strictly a reference to myClass2, not an ownership transfer;
        SampleClass myClass3 = myClass2; // Ownership transfer - myClass2 is now a reference and myClass3 the owner.

        // At this point, myClass, myClass2, and myClass3ref are all references to the first instance of SampleClass.
        // Though, unlike Rust, all the variables that're now references can still operate on the same instance and will not throw an error.
        // You may still operate on myClass3 as the owner moving forwards.
      } // At this point, if myClass3 hasn't been returned out of this scope, it's responsible for cleaning up the SampleClass instance.
      ```
      - Function calls are NOT able to pass ownership along as Rust does. The only exception to this is if you pass a container object which stores the instance/object - ie an array/list.

- Concurrency:
   - Threads will be implemented in a similar manner to how C does, however, the final implmentation will favor intuitiveness and take inspiration from Go's goroutines - yet different in the execution and scheduling of threads.
   The original plan was to map threads to Jobs - where jobs were an abstraction to reduce the complexity of managing threads and allow for programming logic in a more Nodejs-async-style single-threaded fashion, allowing the language's runtime to do the heavy-lifting of managing atomic accesses and thread scheduling.
    
# Author's Note:
- Upon completing the C-based Lox Programming language implementation from Bob (Robert) Nystrom's "Crafting Interpreters" book, I set out to construct my own programming language from what I had learned then and have learned thus far in my own free time doing research into programming language design and construction of AOT, JIT compilers and interpreters. For reference, this project is not a continuation of Nystrom's work, nor is it a continuation of my past work that was derived from his book, hence this project is my own from-scratch implementation for a language that I am designing from the ground up and isn't affiliated with Robert Nystrom nor with "Crafting Interpreters". 

- After considerable thought, I decided to steer away from my previous design decisions to use Go, CIL bytecode, Java bytecode, C, and C++ for my compiler's output language targets. As this is a passion project of mine, the main reason to develop Clyth has been to simplify developer experience for someone who loves C programming, yet not compromise on performance when any form of garbage collection is used. For this, the language must sport a flexible borrow-checker implementation that is similar to that of Rust's but more deliberate in the actions of transferring ownership than just an assignment operator or function call.
To accomplish this, Clyth must have a base-language with manual memory management - which would enable the creation of more language features in Clyth itself to orchestrate smart-pointers in a transactional way to achieve ownership-driven garbage collection (such as Option 4 from the Memory Management section above), as well as allowing usage of custom garbage collectors in specific scopes (like a mark and sweep collector) as a bonus feature to allow for a multi-garbage collection paradigm language.

- I'm in no way assuming trademark of the name "Cobralyth" as it's an open-source project of mine that I am not intending on making commercial/monetary gains from. Personally, I think trademarking progamming language names is ridiculous as it's a language name, but I'm not a lawyer and this is my statement to anyone it may pertain to that my intention for using "Cobralyth" as a name is strictly for the sake of naming my project and not to infringe on trademarks or cause legal conflicts, and I assume no legal responsibility in the case this name is used elsewhere.

- Other informative update(s) notes:
   - I had to delete the old repository for the sake of cleaning up the entire project. This is now the latest project with the files carrying over from the previous repo version that was deleted. The license was originally from 2022, so that was the only adjustment I made in this repo's license to revert it back to 2022 from 2023.

# Acknowledgements:
  - This project is free from AI generated code - this is my own work as it's a passion-project of mine.
  - The project has undergone redesigns, and as much as I'd like to implement things from scratch using LLVM IR, it's not worth re-inventing the wheel at that level - unless I see meaningful benefit otherwise. For the purpose of my project, I want to leverage as much of the C++ infrastructure and features as possible, limiting MLIR usage to GPU support (for now) as I see fit.
  - Additionally, any open-source libraries included in this project are mentioned in the 'EXTERNAL_LIBRARIES_LICENSES.md" file to do my due-dilligence to the best of my ability.