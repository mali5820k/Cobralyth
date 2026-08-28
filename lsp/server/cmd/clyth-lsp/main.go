package main

import (
	"fmt"
	"os"

	"github.com/mali5820k/clyth-lsp/internal/lsp"
)

func main() {
	if err := lsp.Serve(os.Stdin, os.Stdout); err != nil {
		fmt.Fprintln(os.Stderr, "clyth-lsp:", err)
		os.Exit(1)
	}
}
