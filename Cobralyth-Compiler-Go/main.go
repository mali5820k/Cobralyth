package main

import (
	. "cobralyth/cobralyth_dev" // I know this isn't best practice but I like the brevity for my codebase :p
	"flag"
	"fmt"
)

// Global-scope
var dbgMode bool

func setupFlags() {
	dbgMode = false
	//gcEnabled := true // Not sure about this one yet, potentially once the compiler moves to other targets that don't rely on Go's infrastructure and GC.
	flag.BoolVar(&dbgMode, "d", false, "Debug mode enabled (true if used)")
	flag.Parse()

	if dbgMode {
		fmt.Printf("\nDebug-mode is activated\n")
	} else {
		fmt.Printf("\nDebug-mode is NOT active\n")
	}
}

func main() {
	// Check if the user has provided a sample cobralyth program file:
	setupFlags()

	// Initialize lexer and parsers:
	lexer := NewLexer()
	fmt.Println("Main Program Start")
	fmt.Printf("This is the lexer to string: %s", lexer)
}
