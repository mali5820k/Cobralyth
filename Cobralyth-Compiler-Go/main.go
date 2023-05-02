package main

import (
	"fmt"
	"cobralyth_dev"
)

func main() {
	// Initialize lexer and parsers:
	lexer := Lexer {
		File:         "nothing"
		lineNumber: 1
		scannedTokens: []{"if", "{","}"}
		currentChar   'i'
		nextChar      'f'
	}
	fmt.Println("Main Program Start")
	fmt.Printf("This is a sample printf\n%d", 20)
}
