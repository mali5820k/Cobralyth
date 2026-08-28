package lexer

import "github.com/mali5820k/clyth-bootstrap/internal/frontend/token"

type trieNode struct {
	children map[byte]*trieNode
	kind     token.Kind
	terminal bool
}

type trie struct{ root *trieNode }

func newTrie(entries map[string]token.Kind) *trie {
	t := &trie{root: &trieNode{children: map[byte]*trieNode{}}}
	for text, kind := range entries {
		t.insert(text, kind)
	}
	return t
}

func (t *trie) insert(text string, kind token.Kind) {
	n := t.root
	for i := 0; i < len(text); i++ {
		c := text[i]
		if n.children[c] == nil {
			n.children[c] = &trieNode{children: map[byte]*trieNode{}}
		}
		n = n.children[c]
	}
	n.kind, n.terminal = kind, true
}

func (t *trie) exact(text string) (token.Kind, bool) {
	n := t.root
	for i := 0; i < len(text); i++ {
		n = n.children[text[i]]
		if n == nil {
			return token.Illegal, false
		}
	}
	return n.kind, n.terminal
}

func (t *trie) longest(source string, start int) (token.Kind, int, bool) {
	n := t.root
	kind, end, ok := token.Illegal, start, false
	for i := start; i < len(source); i++ {
		n = n.children[source[i]]
		if n == nil {
			break
		}
		if n.terminal {
			kind, end, ok = n.kind, i+1, true
		}
	}
	return kind, end, ok
}
