# Cobralyth, Clyth for short
A Compiled language that is inspired from C++ runtime performance and Python's scripting properties, as well as various syntax inspirations from C#, JavaScript, TypeScript, Java, and Kotlin.

## Project Goals:
### Primary Goals:
- Concurrency-first base-language design
- Simple syntax
- Providing features usually found in scripting languages
- Python-like white-space formatting for scopes
- File-IO

**An important note:** I may utilize Antlr4 for accelerating the progress of this project to get the first-gen compiler. Pratt Parsers, custom lexers and parsers are magnificent, but it's also subject to rigorous unit-testing to match a high-quality lexer/parser, in which case Antlr4 already hits that mark right from the get-go. What this also means is that the 'trie'-based lexer in the custom implementation prior to switching to Antlr will still be referrable to in a separate branch and simply may be reimplemented in Cobralyth itself for the second-gen compiler.

### Secondary Goals:
- GUI support
- OpenCL and CUDA support
- IoT devices such as running on a raspberry pi
- Websockets and Webserver backend support
- Package system
- Cross platform compilation (either native or wasm)
- Porting components to LLVM IR/MLIR for further optimizations (as applicable)
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
    
    func val1(int a, int b) int {
      if (a > 5) {
        return 0;
      }
      
      return val2(a, b);
    }
    
    func val2(int a, int b) float32 {
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
   - Pass-by-reference is used by prefixing a variable with an "&", pass-by-value is used otherwise.
     For example:
     ```ts
     function sampleFunction(int x, int y) {
         x += y;
         y *= x;
     }

     // "let" is equivalent of "auto" in C++ for variable declarations
     let x = 10;
     let y = 20;
     
      // Pass-by-value:
     sampleFunction(x, y); // x remains 10 and y remains 20

     // Pass-by-reference:
     sampleFunction(&x, &y); // x is now 30 and y is now 600
     ```
     All the primitives of the language will be compiled to wrapper-classes that are built around the primitives of C++ with extra functionality and very lean metadata. This allows for a simple-wrapper object to be passed to functions that allows for switching in between passing-by-reference and passing-by-value.
     For example:
     ```cpp
     template <typename T>
     class PrimitiveWrapper {
        public:
         T *p_ref;
         bool is_ref;
         TypeInfo value_type;
         PrimitiveWrapper(T value, bool is_ref) {
            if(is_ref) { this->p_value = value; }
            else { this->p_ref = &value; }
         }
         void updateValue(T value) {
            if (is_ref) { this->p_ref; }
            else { this->p_value = value; }
         }
         T getValue(T value) {
            return this->p_ref
         }
     };
     ```
- Memory Management Module (Included as a Runtime Module for modularity):
    - Option 1 `No longer considered for implmentation`: Scope-based Inheritance Borrow Checker Reference Table Scheme (SIBCRT)
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
    - Option 2 (`Prioritized and Preferred implementation`): EC Architecture (Entanglement Component(s))
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
    - Option 3: Tracing garbage collector:
      - Not a high-priority to implement since most languages have or have had some form of this type of garbage collector. This collector type can be implemented at a later date, but implementing this design for the main language would introduce cycles and memory overhead that generally slows performance down considerably and introduces challenges to parallelize across multiple threads.

- Concurrency:
   -  Threads are synonymous to Jobs. Jobs abstract away the complexity of managing threads and allow for programming logic in a single-threaded fashion, allowing the language's runtime to do the heavy-lifting of managing atomic accesses and thread scheduling.
   -  Threads will be utilized to implement the underlying behavior of Jobs for scheduling them on multiple CPU cores.
   -  Language variable primitives will be updated to be atomic when declared and used outside of a Jobs' scope, ensuring that global variables are atomically accessible. When declared inside of a Jobs' scope, language variable primitives will not be atomic by default.
   -  Atomic datastructures will be provided for out of the box usage, included in the standard library. Non-atomic variants will also be included as they'll be wrapped in logic to form their atomic variants.
   -  Jobs orchestration is handled by the programmer to a certain degree, such as the order of execution can be defined for a series of jobs by declaring them as dependencies of other Jobs, similar to an "await" statement.
   -  An example of Jobs in action:
     ```js
     function testJob(int a, string filename) {
        /// write a-lines to this file of random characters...
        return 0
     }
     let x = new Job(testJob())
     let y = x.runJob().getExitCode() /// blocking variant
     x.runJob().getExitCode(&y) /// async-variant, better to check for in loops
     let z = x.jobStatus() /// stores the job status of x in z; ie. starting, in-progress, failed, finished
     
     if y == 0 {
         print("\nAll good! Success!\n")
     }
     else {
         print("\nJob failed!\n")
     }
     ```
   - An example where you can utilize Job-chaining/Job-orchestration:
     ```js
     let x = new Job(testJob(20, "filename-here.txt"))
     let y = new Job(testOnJobCompletion())
     x.runJob().onCompletion([ y ]) // Array of jobs to run on completion of x's job
     ```
   - An example where you can utilize notify-events to run Jobs at different stages of a particular Job:
     ```js
     let x = new Job(testJob(20, "filename-here.txt"))
     let y = new Job(testOnJobCompletion)
     let z = new Job(testOnJobCompletion2, "function-parameter-for-test-on-job-completion-2") /// function parameters compile to array of PrimitiveWrapper classes (as an example implementation)
     let w1 = new Job(() => {
         print("JobStart notification")
     })
     let w2 = new Job(() => {
         print("JobInProgress notification")
     })
     let w3 = new Job(() => {
         print("JobEnd notification")
     })
     x.runJob().onCompletion([y, z]).onNotify(["jobStart", [w1]], ["jobInProgress", [w2]] ,["jobEnd", [w3]]) // onNotify runs on job x, not y and z.
     ```
  - Syntax and feature expansion of jobs will be coming in the future as functionality is implemented.
  - Consideration of job-to-job communication is a topic of interest, as well as allowing parameters to be sent to jobs to allow for greater reusability.
    
# Author's Note:
- Upon completing the C-based Lox Programming language implementation from Bob (Robert) Nystrom's "Crafting Interpreters" book, I set out to construct my own programming language from what I had learned then and have learned thus far in my own free time doing research into programming language design and construction of AOT, JIT compilers and interpreters. For reference, this project is not a continuation of Nystrom's work, nor is it a continuation of my past work that was derived from his book, hence this project is my own from-scratch implementation for a language that I am designing from the ground up and isn't affiliated with Robert Nystrom nor with "Crafting Interpreters". 

- After considerable thought, I decided to steer away from my previous decision to use Go for my compiler. My revised goal for Cobralyth is to invest in creating a borrow-checker system that is more flexible than Rust's while also being concurrency-first. Meaning out of the box developers won't have to fight with the language to code multithreaded/multicore programs for modern-day machines while also maintaining the high-level features Rust provides. This entails the base-language's API to include locking and other synchronizing mechanisms to prevent data-races and resource contention from occuring. As a short summary of what this would look like on a macro-scale, everything from base datastructures and variables will be atomic and customizable in how the atomicity of those structures are handled via extension of base-classes and custom classes. There will be a borrow-checker system that utilizes reference counting but with upgradeable-owners and root-owners in a reference-chain graph, which ultimately will update like a virtual-DOM found in ReactJS. The garbage collector will be event-driven and activated on the deallocation of strong-reference chains, ignoring weak-reference chains from any objects that may cause cyclical reference chains. Testing will be conducted to achieve low-latency garbage collection in the most concurrent way possible.
As the language develops a more concrete documentation of the desired runtime will be written and linked to this page.

- I'm in no way assuming trademark of the name "Cobralyth" as it's an open-source project of mine that I am not intending on making commercial/monetary gains from. Personally, I think trademarking progamming language names is ridiculous as it's a language name, but I'm not a lawyer and this is my statement to anyone it may pertain to that my intention for using "Cobralyth" as a name is strictly for the sake of naming my project and not to infringe on trademarks or cause legal conflicts, and I assume no legal responsibility in the case this name is used elsewhere.

- Other informative update(s) notes:
   - I had to delete the old repository for the sake of cleaning up the entire project. This is now the latest project with the files carrying over from the previous repo version that was deleted. The license was originally from 2022, so that was the only adjustment I made in this repo's license to revert it back to 2022 from 2023.
   -  Lastly, the Go-approach that I started is archived as a deprecated branch if anyone is interested in taking a look at it.

# Acknowledgements:
  - The design and implementation of the language, SIBCRT, and the EC architecture are fully my own, developed independently and adhering to FOSS principles. Conversations with ChatGPT served as a valuable sounding board, helping to explore, clarify, and polish concepts and potential approaches to implementation without directly shaping the underlying design.
  - With that said, I am thankful for ChatGPT and feel it was an invaluable experience to bounce ideas off of and discuss many aspects of compiler and programming language design with, essentially like a research partner who helped me learn how to discover what design and planning a complex compiler project looks like and ultimately motivated me to create a project that is my own work.
  As a matter of fact, the first bullet-point of this Acknowledgements section was generated by ChatGPT itself. And to further clarify, there is no AI generated code used in this project as it is my own original work and original concepts, ideas, and documentation, and the first paragraph in this Acknowledgements section is the only one generated by ChatGPT so I could say more in a concise fashion.