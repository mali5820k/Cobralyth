# Cobralyth
A Compiled language that is inspired from C++ runtime performance and Python's scripting properties, as well as various syntax inspirations from C#, JavaScript, Java, and Kotlin.

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

# Project Structure:
   The project is divided into two main parts:
  - Frontend:
    - Lexer
    - Parser
    - Main-program entrypoint
  - Backend:
    - Codegen module to transpile to a specific target language (currently C++)
    - Runtime module files that provide a C++ API that provides the language's runtime functionality (Not a VM or Interpreter)
    - Debug module for debugger
    

# Author's Note:
- Upon completing the C-based Lox Programming language implementation from Bob (Robert) Nystrom's "Crafting Interpreters" book, I set out to construct my own programming language from what I had learned then and have learned thus far in my own free time doing research into programming language design and construction of AOT, JIT compilers and interpreters. For reference, this project is not a continuation of Nystrom's work, nor is it a continuation of my past work that was derived from his book, hence this project is my own from-scratch implementation for a language that I am designing from the ground up and isn't affiliated with Robert Nystrom nor with "Crafting Interpreters". 

- After considerable thought, I decided to steer away from my previous decision to use Go for my compiler. My revised goal for Cobralyth is to invest in creating a borrow-checker system that is more flexible than Rust's while also being concurrency-first. Meaning out of the box developers won't have to fight with the language to code multithreaded/multicore programs for modern-day machines while also maintaining the high-level features Rust provides. This entails the base-language's API to include locking and other synchronizing mechanisms to prevent data-races and resource contention from occuring. As a short summary of what this would look like on a macro-scale, everything from base datastructures and variables will be atomic and customizable in how the atomicity of those structures are handled via extension of base-classes and custom classes. There will be a borrow-checker system that utilizes reference counting but with upgradeable-owners and root-owners in a reference-chain graph, which ultimately will update like a virtual-DOM found in ReactJS. The garbage collector will be event-driven and activated on the deallocation of strong-reference chains, ignoring weak-reference chains from any objects that may cause cyclical reference chains. Testing will be conducted to achieve low-latency garbage collection in the most concurrent way possible.
As the language develops a more concrete documentation of the desired runtime will be written and linked to this page.

- I'm in no way assuming trademark of the name "Cobralyth" as it's an open-source project of mine that I am not intending on making commercial/monetary gains from. Personally, I think trademarking progamming language names is ridiculous as it's a language name, but I'm not a lawyer and this is my statement to anyone it may pertain to that my intention for using "Cobralyth" as a name is strictly for the sake of naming my project and not to infringe on trademarks or cause legal conflicts, and I assume no legal responsibility in the case this name is used elsewhere.

- Other informative update(s) notes:
   - I had to delete the old repository for the sake of cleaning up the entire project. This is now the latest project with the files carrying over from the previous repo version that was deleted. The license was originally from 2022, so that was the only adjustment I made in this repo's license to revert it back to 2022 from 2023.
   -  Lastly, the Go-approach that I started is archived as a deprecated branch if anyone is interested in taking a look at it.
