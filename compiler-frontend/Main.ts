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
    }

    // console.log("Current flags used:", compiler_flags)
    if (!compiler_flags.c) {
        console.log("ERROR: Must provide a -c flag and Clyth code-file to begin compilation!")
        Deno.exit(2)
    }
    const file_contents = await readFile(compiler_flags.c)
    console.log(file_contents)
    
    // Read in a file and lex it line-by-line:
    const lexer = new Lexer(file_contents)
    console.log(`Lexer tokens: ${lexer.tokenize_string()}`)

}

main(Deno.args)