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
- Deep Learning based optimization passes; ie auto-parallelization of serial code, dead-code removal, auto GPU kernel optimization for target hardware, and more as time allows.
- GUI support
- OpenCL and CUDA support
- IoT devices such as running on a raspberry pi
- Websockets and Webserver backend support
- Package system
- Cross platform compilation (either native or wasm)
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
    - Aim is to allow for swapping of runtime modules for testing different runtimes.
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
   - Scope-based Inheritance Borrow Checker Reference Table Scheme (SIBCRT):
      - Each scope will maintain its own SIBCRT which tracks strong and weak references amongst objects to prevent cyclic references from occurring in mark-and-sweep-like garbage-collected languages, theoretically reducing garbage collection overhead if done in a minimal fashion.
      - Strong references can be made by any object to another, as long as the object storing the reference doesn't already have the object it's referring to stored as a strong reference in one of its parents. IE objA objB and objC exist, objA stores a strong reference to objC, objC stores a strong reference to objB, but objB can't store a strong reference to objA nor objC if it needs to, therefore objB will resort to weak-references to both objA and objC to prevent a cycle of references from occurring.
      - SIBCRT lookups will be performed whenever a reference is assigned by an object to another on invocation of the "=" operator. The SIBCRT for that scope will be referenced and the parents of the object being pointed to will be checked for existing strong references before assigning the type of reference to use.
      - Owners of an object can be multiple in number, they will simply be appended to a queue of owners that will "inherit" the ownership of the object that all the other owners are currently referencing as well. In the case an owner goes out of scope or is no longer referring to an object with multiple owners, then only that owner is removed from the queue of owners. Once all owners no longer maintain the reference to the shared object, that object will be garbage-collected as no strong references will remain after the last owner loses the reference to the shared object.
      - When exiting a scope, the SIBCRT will:
         - upgrade all weak-references of an object that is being returned to strong-references before returning from the scope.
         - transfer the "owner" of the object being returned to the parent-scope's SIBCRT
   - Async Atomic Tracing Garbage Collector:
      - Garbage Collector will run atomically and asynchronously to the main program, preventing GC overhead induced from common stop-the-world collectors at runtime.
      - Will only be invoked whenever a strong-reference goes out of scope or an owner of an object from the queue of owners is freed or no longer maintains the strong reference it had on its referenced object.
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
