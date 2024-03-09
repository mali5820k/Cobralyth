class KeywordTrie {
    private character: string = ""
    private neighbors = new Map<string, KeywordTrie>([])
    
    constructor(letter: string) {
        this.character = letter
    }

    /**
     * Returns the character that this trie corresponds to.
     */
    getData() {
        return this.character
    }

    /**
     * Returns the map of neighbor-characters that follow this character
     * in the specific keyword-character-chain.
     * (ie) "&&"": '&' -> ['&': KeywordTrie('&')]
     *      The current character in this case is '&', neighbors list is ['&']
     *      and the neighbors list entries each map to their own KeywordTrie instance.
     */
    getNeighbors() {
        return this.neighbors
    }

    addNeighbor(letter: string) {
        this.neighbors.set(letter, new KeywordTrie(letter))
    }
}

export default KeywordTrie;