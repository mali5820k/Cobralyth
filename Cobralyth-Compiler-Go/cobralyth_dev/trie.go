package cobralyth_dev

import (
	"fmt"
)

type TrieNode struct {
	character        rune
	tokenType        TokenType
	neighbors        map[rune]TrieNode
	indentationLevel int
}

func addStringToTree(keywordString string, rootTriNode *TrieNode) bool {

	if rootTriNode == nil {
		return false
	}

	// For each rune in keywordString, check if the current TrieNode has a neighbor containing that rune
	// if so, increment to the next rune and repeat.
	// If no neighbor is found, then hold the reference to the last trienode and exit the loop with it.
	// Create a new TrieNode and add the current rune to that node's character value
	// and add that node as a neighbor to the node that you held the reference to from the loop.
	// Create remaining trieNodes for the remainder of the string in keywordString and add them to be neighbors of one another
	// in a linear fashion and then add as a neighbor to the trienode added first since the loop exited.

	var i int = 0
	var neighbor *TrieNode = rootTriNode
	fmt.Printf("\n\n" + keywordString + "\n")
	for {
		if i < len(keywordString) {
			if value, ok := neighbor.neighbors[rune(keywordString[i])]; ok {
				i++
				neighbor = &value
				PrintTrie(&value)
				continue
			} else {
				// Didn't find a matching key so will break to add it in now.
				break
			}
		} else {
			// Break to create the alternate nodes and mark reachedEnd;
			break
		}
	}
	// Create the individual trienodes for each character and append them as a neighbor in the
	// previous node's map of neighbors.
	for i < len(keywordString) {
		newTrieNode := NewTrieNode(rune(keywordString[i]), TOKEN_UNDEFINED, neighbor.indentationLevel+1)
		neighbor.neighbors[newTrieNode.character] = *newTrieNode
		neighbor = newTrieNode
		PrintTrie(neighbor)
		i++
		if i == len(keywordString) {
			neighbor.tokenType = languageDictionary[keywordString]
		}
	}
	return true
}

func PrintTrie(rootTriNode *TrieNode) {
	trieString := rootTriNode.String()
	fmt.Print(trieString)
}

func (tri TrieNode) String() string {

	// Likely want a pre-order traversal on this one so all elements are printed out row-by-row.
	allKeys := ""
	for key := range tri.neighbors {
		allKeys += ", " + string(key)
	}

	// Sets the appropriate indentation level of this specific Trie
	indentationString := ""
	for i := tri.indentationLevel; i > 0; i-- {
		indentationString += "\t"
	}
	repr := fmt.Sprintf("%sTrieNode:\tCharacter: %s\tTokenType: %d\tNeighbors: %s\tIndentationLevel: %d\n", indentationString, string(tri.character), tri.tokenType, allKeys, tri.indentationLevel)
	return repr
}

func NewTrieNode(character rune, tokenType TokenType, indentationLevel int) *TrieNode {
	neighbors := map[rune]TrieNode{}
	newTrieNode := TrieNode{character, tokenType, neighbors, indentationLevel}
	return &newTrieNode
}

func (tri *TrieNode) addNeighbor(neighborNode *TrieNode) bool {
	if neighborNode == nil {
		return false
	}
	tri.neighbors[neighborNode.character] = *neighborNode
	return true
}

func (tri TrieNode) getNeighborWith(character rune) (*TrieNode, bool) {
	nextNeighbor, found := tri.neighbors[character]
	if found {
		return &nextNeighbor, true
	}
	return nil, false
}

func (tri TrieNode) getTokenType() TokenType {
	return tri.tokenType
}
