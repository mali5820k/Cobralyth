import { parseArgs } from "util";
import path from "node:path";
import Lexer from "./Lexer"
import KeywordTrie from "./KeywordTrie"

const VALID_FILE_EXTENSION = ".clyth"
const KEYWORDS_HEAD = new KeywordTrie("")
/**
 * Supported commandline flags and arguments
 */
const { values, positionals } = parseArgs({
    args: Bun.argv,
    options: {
        debug: {
            type: "boolean"
        },
        compile: {
            type: "string"
        },
        output: {
            type: "string"
        },
        help: {
            type: "boolean"
        },
        version: {
            type: "boolean"
        }
    },
    strict: true,
    allowPositionals: true,
})

/**
 * Main function definition
 */
function main() {
    if (values.help) {
        let help_text = ""
        help_text += "\n--help:   \t\tDisplays help menu with all supported options\n"
        help_text += "\n--version:\t\tShows current compiler version\n"
        help_text += "\n--debug:  \t\tStarts debug-mode for specified file entrypoint\n"
        help_text += `\n--compile:\t\tSets the entrypoint to compile\n\t\t\t(ie) --compile <path-to-file>.${VALID_FILE_EXTENSION}\n`
        help_text += `\n--output: \t\tSets the output binary-name and location for compiled program.\n\t\t\t(ie) --output ./<file-name>\n`
        console.log(help_text)
        process.exit(0)
    } else if (values.version) {
        console.log("\nCobralyth Compiler (gen1) Version Alpha:1.0\n")
        process.exit(0)
    } else if (values.debug) {
        console.log("\nDebugging interpreter coming at a later date.\n")
        process.exit(0)
    } else if (values.compile) {
        if (path.extname(values.compile) != VALID_FILE_EXTENSION) {
            console.log(`\nERROR: Not a valid '${VALID_FILE_EXTENSION}' file-type.\nExiting...\n`)
            process.exit(-1)
        }
        if (values.output) {
            console.log(`Output defined as: ${values.output}\n`)
        }
        console.log(`\nCompiling provided entrypoint: ${values.compile}\n`)

        // Insert logic here to actually compile program...
        let lexer_instance = new Lexer()
        lexer_instance.scanFile(values.compile)
        
        console.log("\nCompile complete\n")
        process.exit(0)
    } else {
        console.log("No supported option was provided.\n")
        process.exit(-1)
    }
}

/**
 * Generates a new KeywordsTrie 
 */
function generateKeywordsTrie() {
    // keywords list: []
    let keywords_list: string[] = []
    for (let keyword of keywords_list) {
        // For each keyword, we traverse it's characters and create a new KeywordTrie and store it in the current
        // active-keyword-trie.
        let active_keyword_trie: KeywordTrie = KEYWORDS_HEAD
        for (let letter of keyword) {
            // If this letter already is defined as a neighbor in the active_trie's neighbors list
            // then we simply hop to that trie and continue traversing.
            if (active_keyword_trie.getNeighbors().has(letter)) {
                let letter_trie = active_keyword_trie.getNeighbors().get(letter)
                if (letter_trie != undefined) {
                    active_keyword_trie = letter_trie
                }
                continue;
            }

            // If no entry for the current letter exists in the neighbors list,
            // we create a new neighbor entry and hop to that Trie and keep appending.
            active_keyword_trie.addNeighbor(letter)
            let letter_trie = active_keyword_trie.getNeighbors().get(letter)
            if (letter_trie != undefined) {
                active_keyword_trie = letter_trie
            }
        }
    }
}

/**
 * Calls the main function
 */
main()