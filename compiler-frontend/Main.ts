import { Lexer, Token, TokenType } from "./Lexer.ts"
import { parseArgs } from "jsr:@std/cli/parse-args"
import { exists } from "jsr:@std/fs/exists";
import release_info_file from "./release_info.json" with { type: "json" }

async function readFile(file_path: string) {
    try {
        // Check if the file exists first:
        const file_exists = await exists(file_path)
        if (!file_exists) {
            throw new Error(`File "${file_path}" does not exist!`)
        }

        const file_contents = await Deno.readTextFile(file_path)
        return file_contents
    } catch (error: any) {
        console.log(`ERROR: Failed with error:`, error)
        Deno.exit(1)
    }
}

async function main(args: string[]) {
    // Parse arguments:
    const compiler_flags = parseArgs(args, {
        boolean: ["h","help", "v", "version"],
        string: ["c", "o"]
    })

    if (compiler_flags.v) {
        console.log(`${release_info_file.Name} ${release_info_file.Version}`)
        Deno.exit(0)
    }

    if (compiler_flags.h || compiler_flags.help) {
        console.log("Sample usage:\n\tdeno run Main.ts -c main.clyth -o output.bin\n\n\t-h or --help for printing supported flags (this)\n\t-v or --version for vompiler version information\n\t-c for specifying main clyth program source code file (file containing main function)\n\t-o for specifying the output binary name\n")
        Deno.exit(0)
    }
    if (!compiler_flags.c) {
        console.log("ERROR: Must provide a -c flag and Clyth code-file to begin compilation!\nUsage: deno run Main.ts -c main.clyth -o output.bin\n\nFor help use the '-h' or '--help' flags.\n")
        Deno.exit(2)
    }
    
    // Read in a file and lex it line-by-line:
    const lexer = new Lexer(compiler_flags.c)
    console.log(`Lexer tokens: ${await lexer.tokenize_file()}`)
    console.log(`Completed Lexing of file - ending program...\n`)
}

main(Deno.args)