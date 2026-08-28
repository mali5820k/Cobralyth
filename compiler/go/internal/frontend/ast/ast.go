package ast

import "github.com/mali5820k/clyth-bootstrap/internal/frontend/token"

type Node interface{ Span() token.Span }
type Decl interface {
	Node
	declNode()
}
type Stmt interface {
	Node
	stmtNode()
}
type Expr interface {
	Node
	exprNode()
}

type Program struct {
	Declarations []Decl     `json:"declarations"`
	SourceSpan   token.Span `json:"span"`
}

func (n *Program) Span() token.Span { return n.SourceSpan }

// MeccConfigDirectiveDecl is a file-scope compiler directive.  It is parsed as
// structured compiler metadata rather than textual macro substitution.
type MeccConfigDirectiveDecl struct {
	Property   string     `json:"property"`
	Value      bool       `json:"value"`
	SourceSpan token.Span `json:"span"`
}

func (*MeccConfigDirectiveDecl) declNode()          {}
func (n *MeccConfigDirectiveDecl) Span() token.Span { return n.SourceSpan }

// ErrorTypeDirectiveDecl registers a compile-time ErrorType identity. It is
// compiler metadata, not a runtime variable or textual macro expansion.
type ErrorTypeDirectiveDecl struct {
	Name       string     `json:"name"`
	SourceSpan token.Span `json:"span"`
}

func (*ErrorTypeDirectiveDecl) declNode()          {}
func (n *ErrorTypeDirectiveDecl) Span() token.Span { return n.SourceSpan }

type ImportKind string

const (
	ImportModule ImportKind = "module"
	ImportFile   ImportKind = "file"
)

type ImportDecl struct {
	Kind       ImportKind `json:"kind"`
	Target     string     `json:"target"`
	Alias      string     `json:"alias,omitempty"`
	SourceSpan token.Span `json:"span"`
}

func (*ImportDecl) declNode()          {}
func (n *ImportDecl) Span() token.Span { return n.SourceSpan }

type Visibility string

const (
	VisibilityDefault Visibility = "default"
	VisibilityPrivate Visibility = "private"
	VisibilityPublic  Visibility = "public"
)

type ArrayDimension struct {
	Dynamic    bool       `json:"dynamic"`
	Size       Expr       `json:"size,omitempty"`
	SourceSpan token.Span `json:"span"`
}

type TypeRef struct {
	Name         string           `json:"name,omitempty"`
	GenericArgs  []*TypeRef       `json:"generic_args,omitempty"`
	Const        bool             `json:"const,omitempty"`
	PointerDepth int              `json:"pointer_depth,omitempty"`
	Arrays       []ArrayDimension `json:"arrays,omitempty"`
	Callable     *CallableType    `json:"callable,omitempty"`
	SourceSpan   token.Span       `json:"span"`
}

func (n *TypeRef) Span() token.Span { return n.SourceSpan }

type CallableType struct {
	Return     *TypeRef   `json:"return"`
	Parameters []*TypeRef `json:"parameters"`
	Variadic   bool       `json:"variadic,omitempty"`
}

type GenericParameter struct {
	Name       string     `json:"name"`
	SourceSpan token.Span `json:"span"`
}

type Parameter struct {
	Name         string     `json:"name,omitempty"`
	Type         *TypeRef   `json:"type"`
	DefaultValue Expr       `json:"default_value,omitempty"`
	Variadic     bool       `json:"variadic,omitempty"`
	SourceSpan   token.Span `json:"span"`
}

func (n *Parameter) Span() token.Span { return n.SourceSpan }

type FunctionDecl struct {
	Name       string       `json:"name"`
	Receiver   string       `json:"receiver,omitempty"`
	ReturnType *TypeRef     `json:"return_type"`
	Parameters []*Parameter `json:"parameters"`
	Body       *BlockStmt   `json:"body,omitempty"`
	Extern     bool         `json:"extern,omitempty"`
	ABI        string       `json:"abi,omitempty"`
	Visibility Visibility   `json:"visibility,omitempty"`
	SourceSpan token.Span   `json:"span"`
}

func (*FunctionDecl) declNode()          {}
func (n *FunctionDecl) Span() token.Span { return n.SourceSpan }

type ConstructorDecl struct {
	Parameters []*Parameter `json:"parameters"`
	Body       *BlockStmt   `json:"body"`
	Visibility Visibility   `json:"visibility,omitempty"`
	SourceSpan token.Span   `json:"span"`
}

func (n *ConstructorDecl) Span() token.Span { return n.SourceSpan }

type DestructorDecl struct {
	Body       *BlockStmt `json:"body"`
	Visibility Visibility `json:"visibility,omitempty"`
	SourceSpan token.Span `json:"span"`
}

func (n *DestructorDecl) Span() token.Span { return n.SourceSpan }

type StructMember interface {
	Node
	structMemberNode()
}
type FieldDecl struct {
	Name        string     `json:"name"`
	Type        *TypeRef   `json:"type"`
	Visibility  Visibility `json:"visibility,omitempty"`
	Initializer Expr       `json:"initializer,omitempty"`
	SourceSpan  token.Span `json:"span"`
}

func (n *FieldDecl) Span() token.Span { return n.SourceSpan }
func (*FieldDecl) structMemberNode()  {}

type MethodDecl struct{ FunctionDecl }

func (*MethodDecl) structMemberNode() {}

type ConstructorMember struct{ ConstructorDecl }

func (*ConstructorMember) structMemberNode() {}

type DestructorMember struct{ DestructorDecl }

func (*DestructorMember) structMemberNode() {}

type StructDecl struct {
	Name              string             `json:"name"`
	Intrinsic         bool               `json:"intrinsic,omitempty"`
	GenericParameters []GenericParameter `json:"generic_parameters,omitempty"`
	Bases             []*TypeRef         `json:"bases,omitempty"`
	Members           []StructMember     `json:"members"`
	SourceSpan        token.Span         `json:"span"`
}

func (*StructDecl) declNode()          {}
func (n *StructDecl) Span() token.Span { return n.SourceSpan }

type EnumMember struct {
	Name       string     `json:"name"`
	Value      Expr       `json:"value,omitempty"`
	SourceSpan token.Span `json:"span"`
}

func (n *EnumMember) Span() token.Span { return n.SourceSpan }

type EnumDecl struct {
	Name       string        `json:"name"`
	Members    []*EnumMember `json:"members"`
	SourceSpan token.Span    `json:"span"`
}

func (*EnumDecl) declNode()          {}
func (n *EnumDecl) Span() token.Span { return n.SourceSpan }

type MethodBlockDecl struct {
	Receiver   *TypeRef       `json:"receiver"`
	Members    []StructMember `json:"members"`
	SourceSpan token.Span     `json:"span"`
}

func (*MethodBlockDecl) declNode()          {}
func (n *MethodBlockDecl) Span() token.Span { return n.SourceSpan }

type ProtocolMember struct {
	Name       string       `json:"name"`
	ReturnType *TypeRef     `json:"return_type"`
	Parameters []*Parameter `json:"parameters"`
	SourceSpan token.Span   `json:"span"`
}

func (n *ProtocolMember) Span() token.Span { return n.SourceSpan }

type ProtocolDecl struct {
	SelfType          string             `json:"self_type,omitempty"`
	Name              string             `json:"name"`
	GenericParameters []GenericParameter `json:"generic_parameters,omitempty"`
	Members           []*ProtocolMember  `json:"members"`
	SourceSpan        token.Span         `json:"span"`
}

func (*ProtocolDecl) declNode()          {}
func (n *ProtocolDecl) Span() token.Span { return n.SourceSpan }

type GlobalVarDecl struct {
	Name        string     `json:"name"`
	Type        *TypeRef   `json:"type"`
	Initializer Expr       `json:"initializer,omitempty"`
	SourceSpan  token.Span `json:"span"`
}

func (*GlobalVarDecl) declNode()          {}
func (n *GlobalVarDecl) Span() token.Span { return n.SourceSpan }

type BlockStmt struct {
	Statements []Stmt     `json:"statements"`
	SourceSpan token.Span `json:"span"`
}

func (*BlockStmt) stmtNode()          {}
func (n *BlockStmt) Span() token.Span { return n.SourceSpan }

// MeccConfigDirectiveStmt applies a lexically-scoped MECC policy override.
// Nested blocks inherit their parent policy and restore it when the block ends.
type MeccConfigDirectiveStmt struct {
	Property   string     `json:"property"`
	Value      bool       `json:"value"`
	SourceSpan token.Span `json:"span"`
}

func (*MeccConfigDirectiveStmt) stmtNode()          {}
func (n *MeccConfigDirectiveStmt) Span() token.Span { return n.SourceSpan }

type VarDeclStmt struct {
	Name        string     `json:"name"`
	Type        *TypeRef   `json:"type"`
	Initializer Expr       `json:"initializer,omitempty"`
	SourceSpan  token.Span `json:"span"`
}

func (*VarDeclStmt) stmtNode()          {}
func (n *VarDeclStmt) Span() token.Span { return n.SourceSpan }

type ReturnStmt struct {
	Value      Expr       `json:"value,omitempty"`
	SourceSpan token.Span `json:"span"`
}

func (*ReturnStmt) stmtNode()          {}
func (n *ReturnStmt) Span() token.Span { return n.SourceSpan }

type IfStmt struct {
	Condition  Expr       `json:"condition"`
	Then       Stmt       `json:"then"`
	Else       Stmt       `json:"else,omitempty"`
	SourceSpan token.Span `json:"span"`
}

func (*IfStmt) stmtNode()          {}
func (n *IfStmt) Span() token.Span { return n.SourceSpan }

type WhileStmt struct {
	Condition  Expr       `json:"condition"`
	Body       Stmt       `json:"body"`
	SourceSpan token.Span `json:"span"`
}

func (*WhileStmt) stmtNode()          {}
func (n *WhileStmt) Span() token.Span { return n.SourceSpan }

type ForStmt struct {
	Init       Stmt       `json:"init,omitempty"`
	Condition  Expr       `json:"condition,omitempty"`
	Post       Expr       `json:"post,omitempty"`
	Body       Stmt       `json:"body"`
	SourceSpan token.Span `json:"span"`
}

func (*ForStmt) stmtNode()          {}
func (n *ForStmt) Span() token.Span { return n.SourceSpan }

type SwitchCase struct {
	Value      Expr       `json:"value,omitempty"`
	Statements []Stmt     `json:"statements"`
	Default    bool       `json:"default,omitempty"`
	SourceSpan token.Span `json:"span"`
}

func (n *SwitchCase) Span() token.Span { return n.SourceSpan }

type SwitchStmt struct {
	Value      Expr          `json:"value"`
	Cases      []*SwitchCase `json:"cases"`
	SourceSpan token.Span    `json:"span"`
}

func (*SwitchStmt) stmtNode()          {}
func (n *SwitchStmt) Span() token.Span { return n.SourceSpan }

type BreakStmt struct {
	SourceSpan token.Span `json:"span"`
}

func (*BreakStmt) stmtNode()          {}
func (n *BreakStmt) Span() token.Span { return n.SourceSpan }

type ContinueStmt struct {
	SourceSpan token.Span `json:"span"`
}

func (*ContinueStmt) stmtNode()          {}
func (n *ContinueStmt) Span() token.Span { return n.SourceSpan }

type ExprStmt struct {
	Expression Expr       `json:"expression"`
	SourceSpan token.Span `json:"span"`
}

func (*ExprStmt) stmtNode()          {}
func (n *ExprStmt) Span() token.Span { return n.SourceSpan }

type IdentifierExpr struct {
	Name       string     `json:"name"`
	SourceSpan token.Span `json:"span"`
}

func (*IdentifierExpr) exprNode()          {}
func (n *IdentifierExpr) Span() token.Span { return n.SourceSpan }

type LiteralKind string

const (
	LiteralInteger            LiteralKind = "integer"
	LiteralFloat              LiteralKind = "float"
	LiteralString             LiteralKind = "string"
	LiteralInterpolatedString LiteralKind = "interpolated_string"
	LiteralCharacter          LiteralKind = "character"
	LiteralBool               LiteralKind = "bool"
	LiteralNull               LiteralKind = "null"
)

type LiteralExpr struct {
	Kind       LiteralKind `json:"kind"`
	Raw        string      `json:"raw"`
	SourceSpan token.Span  `json:"span"`
}

func (*LiteralExpr) exprNode()          {}
func (n *LiteralExpr) Span() token.Span { return n.SourceSpan }

type UnaryExpr struct {
	Operator   string     `json:"operator"`
	Operand    Expr       `json:"operand"`
	Prefix     bool       `json:"prefix"`
	SourceSpan token.Span `json:"span"`
}

func (*UnaryExpr) exprNode()          {}
func (n *UnaryExpr) Span() token.Span { return n.SourceSpan }

type BinaryExpr struct {
	Operator   string     `json:"operator"`
	Left       Expr       `json:"left"`
	Right      Expr       `json:"right"`
	SourceSpan token.Span `json:"span"`
}

func (*BinaryExpr) exprNode()          {}
func (n *BinaryExpr) Span() token.Span { return n.SourceSpan }

type AssignExpr struct {
	Operator   string     `json:"operator"`
	Target     Expr       `json:"target"`
	Value      Expr       `json:"value"`
	SourceSpan token.Span `json:"span"`
}

func (*AssignExpr) exprNode()          {}
func (n *AssignExpr) Span() token.Span { return n.SourceSpan }

type CallExpr struct {
	Callee     Expr       `json:"callee"`
	Arguments  []Expr     `json:"arguments"`
	SourceSpan token.Span `json:"span"`
}

func (*CallExpr) exprNode()          {}
func (n *CallExpr) Span() token.Span { return n.SourceSpan }

type GenericSpecializationExpr struct {
	Target        Expr       `json:"target"`
	TypeArguments []*TypeRef `json:"type_arguments"`
	SourceSpan    token.Span `json:"span"`
}

func (*GenericSpecializationExpr) exprNode()          {}
func (n *GenericSpecializationExpr) Span() token.Span { return n.SourceSpan }

type MemberExpr struct {
	Object     Expr       `json:"object"`
	Name       string     `json:"name"`
	SourceSpan token.Span `json:"span"`
}

func (*MemberExpr) exprNode()          {}
func (n *MemberExpr) Span() token.Span { return n.SourceSpan }

type IndexExpr struct {
	Object     Expr       `json:"object"`
	Index      Expr       `json:"index"`
	SourceSpan token.Span `json:"span"`
}

func (*IndexExpr) exprNode()          {}
func (n *IndexExpr) Span() token.Span { return n.SourceSpan }

type ArrayExpr struct {
	Elements   []Expr     `json:"elements"`
	SourceSpan token.Span `json:"span"`
}

func (*ArrayExpr) exprNode()          {}
func (n *ArrayExpr) Span() token.Span { return n.SourceSpan }

type KeyValueExpr struct {
	Key        Expr       `json:"key"`
	Value      Expr       `json:"value"`
	SourceSpan token.Span `json:"span"`
}

func (*KeyValueExpr) exprNode()          {}
func (n *KeyValueExpr) Span() token.Span { return n.SourceSpan }

type LambdaExpr struct {
	ReturnType *TypeRef     `json:"return_type,omitempty"`
	Parameters []*Parameter `json:"parameters"`
	Body       *BlockStmt   `json:"body"`
	SourceSpan token.Span   `json:"span"`
}

func (*LambdaExpr) exprNode()          {}
func (n *LambdaExpr) Span() token.Span { return n.SourceSpan }

type NewExpr struct {
	Value      Expr       `json:"value"`
	SourceSpan token.Span `json:"span"`
}

func (*NewExpr) exprNode()          {}
func (n *NewExpr) Span() token.Span { return n.SourceSpan }

type CastExpr struct {
	Type       *TypeRef   `json:"type"`
	Value      Expr       `json:"value"`
	SourceSpan token.Span `json:"span"`
}

func (*CastExpr) exprNode()          {}
func (n *CastExpr) Span() token.Span { return n.SourceSpan }
