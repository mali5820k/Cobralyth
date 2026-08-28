package compiler

import (
	"os"
	"path/filepath"
	"testing"

	"github.com/mali5820k/clyth-bootstrap/internal/frontend/parser"
)

// Test_semantic_repository_corpus protects the bootstrap contract: every sample
// program and every self-hosted compiler source file must remain semantically
// analyzable by the Go compiler as the Clyth implementation evolves.
func Test_semantic_repository_corpus(t *testing.T) {
	repository_root := filepath.Clean(filepath.Join("..", "..", "..", ".."))
	patterns := []string{
		filepath.Join(repository_root, "sample-clyth-programs", "*.clyth"),
		filepath.Join(repository_root, "compiler", "clyth", "src", "*.clyth"),
	}
	var source_paths []string
	for _, pattern := range patterns {
		matches, err := filepath.Glob(pattern)
		if err != nil {
			t.Fatal(err)
		}
		source_paths = append(source_paths, matches...)
	}
	if len(source_paths) == 0 {
		t.Fatal("semantic corpus did not find any Clyth source files")
	}

	for _, source_path := range source_paths {
		source_path := source_path
		t.Run(filepath.Base(source_path), func(t *testing.T) {
			source, err := os.ReadFile(source_path)
			if err != nil {
				t.Fatal(err)
			}
			program, parse_errors, lex_errors := parser.Parse(string(source))
			if len(parse_errors) != 0 || len(lex_errors) != 0 {
				t.Fatalf("frontend errors: parse=%v lex=%v", parse_errors, lex_errors)
			}
			_, semantic_errors, err := analyze_file(source_path, program)
			if err != nil {
				t.Fatal(err)
			}
			if len(semantic_errors) != 0 {
				t.Fatalf("semantic errors: %v", semantic_errors)
			}
		})
	}
}
