package compiler

import (
	"encoding/json"
	"fmt"
	"os"

	"github.com/mali5820k/clyth-bootstrap/internal/frontend/ast"
	"github.com/mali5820k/clyth-bootstrap/internal/frontend/parser"
)

type FrontendError struct {
	Path    string
	Line    int
	Column  int
	Message string
}

func (e FrontendError) Error() string {
	if e.Path == "" {
		return fmt.Sprintf("%d:%d: %s", e.Line, e.Column, e.Message)
	}
	return fmt.Sprintf("%s:%d:%d: %s", e.Path, e.Line, e.Column, e.Message)
}

func ParseFile(path string) (*ast.Program, []FrontendError, error) {
	data, err := os.ReadFile(path)
	if err != nil {
		return nil, nil, err
	}
	program, parseErrs, lexErrs := parser.Parse(string(data))
	frontErrs := make([]FrontendError, 0, len(lexErrs)+len(parseErrs))
	for _, e := range lexErrs {
		frontErrs = append(frontErrs, FrontendError{Path: path, Line: e.Span.Start.Line, Column: e.Span.Start.Column, Message: e.Message})
	}
	for _, e := range parseErrs {
		frontErrs = append(frontErrs, FrontendError{Path: path, Line: e.Span.Start.Line, Column: e.Span.Start.Column, Message: e.Message})
	}
	return program, frontErrs, nil
}

func ASTJSON(program *ast.Program) ([]byte, error) {
	return json.MarshalIndent(program, "", "  ")
}
