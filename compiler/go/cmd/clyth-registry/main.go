package main

import (
	"fmt"
	"github.com/mali5820k/clyth-bootstrap/internal/packagecmd"
	"os"
)

func main() {
	if len(os.Args) != 2 {
		fmt.Fprintln(os.Stderr, "usage: clyth-registry <directory>")
		os.Exit(2)
	}
	if err := packagecmd.WriteRegistry(os.Args[1]); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}
