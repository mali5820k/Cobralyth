package cobralyth_dev

type TrieNode struct {
	character rune
	tokenType TokenType
	neighbors map[rune]TrieNode
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
	for true {
		if value, ok := neighbor.neighbors[rune(keywordString[i])]; ok {
			i++
			neighbor = &value
			continue
		} else if i >= len(keywordString) {
			return false
		} else {
			// Break to create the alternate nodes and mark reachedEnd;
			break
		}
	}
	// Create the individual trienodes for each character and append them as a neighbor in the
	// previous node's map of neighbors.
	for i < len(keywordString) {
		newTrieNode := NewTrieNode(rune(keywordString[i]), TOKEN_UNDEFINED)
		neighbor.neighbors[newTrieNode.character] = *newTrieNode
		neighbor = newTrieNode
		i++
		if i == len(keywordString) {
			neighbor.tokenType = languageDictionary[keywordString]
		}
	}
	return true
}

func PrintOutTree(rootTriNode *TrieNode) {

	// Likely want a pre-order traversal on this one so all elements are printed out row-by-row.
	for true {
		return
	}
}

func NewTrieNode(character rune, tokenType TokenType) *TrieNode {
	neighbors := map[rune]TrieNode{}
	newTrieNode := TrieNode{character, tokenType, neighbors}
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
