package main

import (
	"flag"
	"fmt"

	. "cobralyth_dev/cobralyth_dev"
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
	lexer := NewLexer("D:\\Github Projects\\Cobralyth\\Sample-Cobralyth-Programs\\Hello-World\\helloworld.clyth")
	fmt.Println("Main Program Start")
	fmt.Printf("This is the lexer to string before lexing: %s", lexer)

	lexer.ScanFile()
	fmt.Printf("This is the lexer to string after scanning: %s", lexer)
	fmt.Println("Main Program End")
}
