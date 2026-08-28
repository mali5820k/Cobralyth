package parser

import (
	"os"
	"path/filepath"
	"strings"
	"testing"
)

func TestRepositoryClythCorpusParses(t *testing.T) {
	root := filepath.Clean(filepath.Join("..", "..", "..", "..", ".."))
	paths := []string{filepath.Join(root, "sample-clyth-programs"), filepath.Join(root, "compiler", "clyth", "bootstrap-v1")}
	for _, base := range paths {
		err := filepath.WalkDir(base, func(path string, d os.DirEntry, err error) error {
			if err != nil {
				return err
			}
			if d.IsDir() || !strings.HasSuffix(path, ".clyth") {
				return nil
			}
			data, err := os.ReadFile(path)
			if err != nil {
				return err
			}
			_, parseErrs, lexErrs := Parse(string(data))
			if len(lexErrs) != 0 {
				t.Errorf("%s lexer errors: %v", path, lexErrs)
			}
			if len(parseErrs) != 0 {
				t.Errorf("%s parser errors: %v", path, parseErrs)
			}
			return nil
		})
		if err != nil {
			t.Fatal(err)
		}
	}
}
