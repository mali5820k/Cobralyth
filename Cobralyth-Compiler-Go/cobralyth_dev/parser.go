package cobralyth_dev

type ASTNode struct {
	LeftChild  *ASTNode
	RightChild *ASTNode
	Operation  int32
}

type ASTGenerator struct {
	LexStream    []string
	CurrentToken string
	NextToken    string
}
