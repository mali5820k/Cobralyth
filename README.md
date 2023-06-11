# Cobralyth
A Compiled language that is inspired from C++ runtime performance and Python's scripting properties, as well as various syntax inspirations from C#, JavaScript, Java, and Kotlin.

## Project Goals:
### Primary Goals:
- Cross platform compilation (either native or wasm)
- GUI support
- Simple syntax providing scripting langauge like featureset and pythonic white-space formatting for scopes
- Function-leaping/hopping or Function-zipping (still trying to decide on a good name for this feature):
   - Where the function-control-flow can be altered by returning directly to the root-caller of a function-call-stack instead of linear return-flow from function to function. 
   - This is different from gotos since it's limiting the hop to the root-caller and not freely allowing jumps to any other block of code.
   - Example for what it would look like if an existing language supported such a feature out of the box (using Go for displaying the concept; Go doesn't have this feature): 
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
- Websockets and Webserver backend support
- Package system

### Secondary Goals:
- OpenCL and CUDA support
- IoT devices such as running on a raspberry pi

# Author's Note:
- Upon completing the C-based Lox Programming language implementation from Bob (Robert) Nystrom's "Crafting Interpreters" book, I set out to construct my own programming language from what I had learned then and have learned thus far in my own free time doing research into programming language design and construction of AOT, JIT compilers and interpreters. For reference, this project is not a continuation of Nystrom's work, nor is it a continuation of my past work that was derived from his book, hence this project is my own from-scratch implementation for a language that I am designing from the ground up and isn't affiliated with Robert Nystrom nor with "Crafting Interpreters". 

- Furthermore, I'm in no way assuming trademark of the name "Cobralyth" as it's an open-source project of mine that I am not intending on making commercial/monetary gains from. Personally, I think trademarking progamming language names is ridiculous as it's a language name, but I'm not a lawyer and this is my statement to anyone it may pertain to that my intention for using "Cobralyth" as a name is strictly for the sake of naming my project and not to infringe on trademarks or cause legal conflicts, and I assume no legal responsibility in the case this name is used elsewhere.

- As an extension, I may decide to pivot and create a separate version of this language or a new revision that is strictly an AOT compiled, dynamically-typed language featureset. Not trying to jump on that now but something I may consider making with Cobralyth if it's feasible.
- **An important note:** if I find myself running short on time to dedicate to this project, I may pivot and utilize a parser tool like Antlr4 for accelerating the progress of this project. Pratt Parsers, custom lexers and parsers are magnificent, but it's also subject to rigorous unit-testing to match a high-quality lexer/parser, in which case Antlr4 already hits that mark right from the get-go. What this also means is that the 'trie'-based lexer in the custom implementation prior to switching to Antlr will still be referrable to in a separate branch. 

- Other informative update(s) notes:
   - I had to delete the old repository for the sake of cleaning up the entire project. This is now the latest project with the files carrying over from the previous repo version that was deleted. The license was originally from 2022, so that was the only adjustment I made in this repo's license to revert it back to 2022 from 2023.
   - If it makes sense in the long run, I may also create a VM interpreter for this language for being able to both test and debug features.
