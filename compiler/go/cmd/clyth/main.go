package main

import (
	"fmt"
	"os"

	"github.com/mali5820k/clyth-bootstrap/internal/compiler"
	"github.com/mali5820k/clyth-bootstrap/internal/packagecmd"
	"github.com/mali5820k/clyth-bootstrap/internal/version"
)

func usage() {
	fmt.Print(`Clyth V1.0.0 bootstrap toolchain

Usage: clyth <command> [arguments]

Compiler:
  build [file|project-dir] [--emit-ast-json|--emit-symbols-json|--emit-llvm-ir] [-o output]
        [--target T] [--zig P] [--llc P] [--lld P] [--musl-sysroot P]
        [--mecc=true|false] [--verbose]
  debug

Projects and packages:
  init <project-name>
  install [module] [--build-from-source] [--from-registry=name]
  install -f <module-tarball>
  update [module] [--build-from-source]
  remove <module>
  list
  info <module>
  refresh

Registries:
  add-registry <name> (--path <dir> | --url <url>)
  remove-registry <name>
  list-registries
  registry-info <name>

General:
  -v, --version
  -h, --help
`)
}

func main() {
	args := os.Args[1:]
	if len(args) == 0 || args[0] == "-h" || args[0] == "--help" {
		usage()
		return
	}
	if args[0] == "-v" || args[0] == "--version" {
		fmt.Printf("Clyth bootstrap %s\n", version.Version)
		return
	}

	switch args[0] {
	case "package", "install", "update", "remove", "list", "info", "refresh", "init", "debug", "add-registry", "update-registry", "remove-registry", "list-registries", "registry-info":
		if err := packagecmd.Run(args[0], args[1:], os.Stdout, os.Stderr); err != nil {
			fmt.Fprintln(os.Stderr, "error:", err)
			os.Exit(1)
		}
	case "build":
		if err := compiler.RunBuild(args[1:], os.Stdout, os.Stderr); err != nil {
			fmt.Fprintln(os.Stderr, "error:", err)
			os.Exit(2)
		}
	default:
		fmt.Fprintf(os.Stderr, "error: unknown command %q\n\n", args[0])
		usage()
		os.Exit(2)
	}
}
