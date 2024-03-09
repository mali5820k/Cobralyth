// @bun
// src/main.ts
import {parseArgs} from "util";
import path from "path";

// src/KeywordTrie.ts
class KeywordTrie {
  character = "";
  neighbors = new Map([]);
  constructor(letter) {
    this.character = letter;
  }
  getData() {
    return this.character;
  }
  getNeighbors() {
    return this.neighbors;
  }
  addNeighbor(letter) {
    this.neighbors.set(letter, new KeywordTrie(letter));
  }
}
var KeywordTrie_default = KeywordTrie;

// src/main.ts
var main = function() {
  if (values.help) {
    let help_text = "";
    help_text += "\n--help:   \t\tDisplays help menu with all supported options\n";
    help_text += "\n--version:\t\tShows current compiler version\n";
    help_text += "\n--debug:  \t\tStarts debug-mode for specified file entrypoint\n";
    help_text += `\n--compile:\t\tSets the entrypoint to compile\n\t\t\t(ie) --compile <path-to-file>.${VALID_FILE_EXTENSION}\n`;
    help_text += `\n--output: \t\tSets the output binary-name and location for compiled program.\n\t\t\t(ie) --output ./<file-name>\n`;
    console.log(help_text);
    process.exit(0);
  } else if (values.version) {
    console.log("\nCobralyth Compiler (gen1) Version Alpha:1.0\n");
    process.exit(0);
  } else if (values.debug) {
    console.log("\nDebugging interpreter coming at a later date.\n");
    process.exit(0);
  } else if (values.compile) {
    if (path.extname(values.compile) != VALID_FILE_EXTENSION) {
      console.log(`\nERROR: Not a valid '${VALID_FILE_EXTENSION}' file-type.\nExiting...\n`);
      process.exit(-1);
    }
    if (values.output) {
      console.log(`Output defined as: ${values.output}\n`);
    }
    console.log(`\nCompiling provided entrypoint: ${values.compile}\n`);
    console.log("\nCompile complete\n");
    process.exit(0);
  } else {
    console.log("No supported option was provided.\n");
    process.exit(-1);
  }
};
var VALID_FILE_EXTENSION = ".clyth";
var KEYWORDS_HEAD = new KeywordTrie_default("");
var { values, positionals } = parseArgs({
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
  allowPositionals: true
});
main();
