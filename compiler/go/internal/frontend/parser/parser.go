package parser

import (
	"fmt"
	"strings"

	"github.com/mali5820k/clyth-bootstrap/internal/frontend/ast"
	"github.com/mali5820k/clyth-bootstrap/internal/frontend/lexer"
	"github.com/mali5820k/clyth-bootstrap/internal/frontend/token"
)

type Error struct {
	Span    token.Span
	Message string
}

func (e Error) Error() string {
	return fmt.Sprintf("%d:%d: %s", e.Span.Start.Line, e.Span.Start.Column, e.Message)
}

type Parser struct {
	tokens []token.Token
	pos    int
	errs   []Error
}

func New(source string) (*Parser, []lexer.Error) {
	tokens, lexErrs := lexer.Lex(source)
	return &Parser{tokens: tokens}, lexErrs
}

func Parse(source string) (*ast.Program, []Error, []lexer.Error) {
	p, lexErrs := New(source)
	program := p.ParseProgram()
	return program, p.Errors(), lexErrs
}

// ParseExpression parses one standalone Clyth expression. It is used by
// compiler-owned surfaces such as interpolated-string bodies so those bodies
// share the canonical Pratt grammar instead of maintaining a second parser.
func ParseExpression(source string) (ast.Expr, []Error, []lexer.Error) {
	p, lexErrs := New(source)
	p.skipNewlines()
	expression := p.parseExpression(1)
	p.skipNewlines()
	if !p.at(token.EOF) {
		p.errorAt(p.current(), "unexpected token after expression")
	}
	return expression, p.Errors(), lexErrs
}

func (p *Parser) Errors() []Error { return append([]Error(nil), p.errs...) }
func (p *Parser) current() token.Token {
	if p.pos >= len(p.tokens) {
		return p.tokens[len(p.tokens)-1]
	}
	return p.tokens[p.pos]
}
func (p *Parser) previous() token.Token {
	if p.pos == 0 {
		return p.current()
	}
	return p.tokens[p.pos-1]
}
func (p *Parser) peek(n int) token.Token {
	i := p.pos + n
	if i >= len(p.tokens) {
		return p.tokens[len(p.tokens)-1]
	}
	return p.tokens[i]
}
func isNameKind(k token.Kind) bool {
	return k == token.Identifier || k == token.Auto
}
func (p *Parser) at(kind token.Kind) bool { return p.current().Kind == kind }
func (p *Parser) advance() token.Token {
	t := p.current()
	if t.Kind != token.EOF {
		p.pos++
	}
	return t
}
func (p *Parser) match(kinds ...token.Kind) bool {
	for _, k := range kinds {
		if p.at(k) {
			p.advance()
			return true
		}
	}
	return false
}
func (p *Parser) expect(kind token.Kind, msg string) token.Token {
	if p.at(kind) {
		return p.advance()
	}
	p.errorAt(p.current(), msg)
	return p.current()
}
func (p *Parser) errorAt(t token.Token, msg string) {
	p.errs = append(p.errs, Error{Span: t.Span, Message: msg})
}
func (p *Parser) skipNewlines() {
	for p.match(token.Newline, token.Semicolon) {
	}
}
func (p *Parser) consumeStmtEnd() {
	for p.match(token.Semicolon, token.Newline) {
	}
}
func (p *Parser) skipInlineNewlines() {
	for p.match(token.Newline) {
	}
}

func (p *Parser) ParseProgram() *ast.Program {
	p.skipNewlines()
	start := p.current().Span
	program := &ast.Program{}
	for !p.at(token.EOF) {
		before := p.pos
		if d := p.parseDeclaration(); d != nil {
			program.Declarations = append(program.Declarations, d)
		}
		if p.pos == before {
			p.errorAt(p.current(), "expected declaration")
			p.advance()
		}
		p.skipNewlines()
	}
	end := p.current().Span
	program.SourceSpan = token.Merge(start, end)
	return program
}

func (p *Parser) parseDeclaration() ast.Decl {
	p.skipNewlines()
	switch p.current().Kind {
	case token.Hash:
		return p.parse_compiler_directive_decl()
	case token.Import:
		return p.parseImport()
	case token.Struct:
		return p.parseStruct(false)
	case token.IntrinsicStruct:
		return p.parseStruct(true)
	case token.Protocol:
		return p.parseProtocol()
	case token.Enum:
		return p.parseEnum()
	case token.Extern:
		return p.parseExtern()
	}
	return p.parseTypedTopLevel()
}

func (p *Parser) parse_mecc_config_fields() (string, bool, token.Span) {
	start := p.expect(token.Hash, "expected '#' for compiler directive").Span
	name := p.expect(token.Identifier, "expected compiler directive name after '#'")
	if name.Lexeme != "MECC_config" {
		p.errorAt(name, "unknown compiler directive '#"+name.Lexeme+"'")
	}
	p.expect(token.Dot, "expected '.' after #MECC_config")
	property := p.expect(token.Identifier, "expected MECC configuration property")
	p.expect(token.Assign, "expected '=' after MECC configuration property")
	value := false
	if p.match(token.True) {
		value = true
	} else if p.match(token.False) {
		value = false
	} else {
		p.errorAt(p.current(), "MECC configuration values must be compile-time bool literals")
		if !p.at(token.Newline) && !p.at(token.Semicolon) && !p.at(token.EOF) && !p.at(token.RBrace) {
			p.advance()
		}
	}
	end := p.previous().Span
	p.consumeStmtEnd()
	return property.Lexeme, value, token.Merge(start, end)
}

func (p *Parser) parse_compiler_directive_decl() ast.Decl {
	if p.peek(1).Kind == token.Identifier && p.peek(1).Lexeme == "ErrorType" {
		start := p.expect(token.Hash, "expected '#' for compiler directive").Span
		p.advance() // ErrorType
		name := p.expect(token.Identifier, "expected error type name after #ErrorType")
		end := name.Span
		p.consumeStmtEnd()
		return &ast.ErrorTypeDirectiveDecl{Name: name.Lexeme, SourceSpan: token.Merge(start, end)}
	}
	return p.parse_mecc_config_decl()
}

func (p *Parser) parse_mecc_config_decl() ast.Decl {
	property, value, span := p.parse_mecc_config_fields()
	return &ast.MeccConfigDirectiveDecl{Property: property, Value: value, SourceSpan: span}
}

func (p *Parser) parse_mecc_config_stmt() ast.Stmt {
	property, value, span := p.parse_mecc_config_fields()
	return &ast.MeccConfigDirectiveStmt{Property: property, Value: value, SourceSpan: span}
}

func (p *Parser) parseImport() ast.Decl {
	start := p.advance().Span
	p.skipInlineNewlines()
	if p.at(token.String) {
		t := p.advance()
		target := unquote(t.Lexeme)
		alias := ""
		if p.match(token.As) {
			alias = p.expect(token.Identifier, "expected import alias").Lexeme
		}
		end := p.previous().Span
		p.consumeStmtEnd()
		return &ast.ImportDecl{Kind: ast.ImportFile, Target: target, Alias: alias, SourceSpan: token.Merge(start, end)}
	}
	if !p.at(token.Identifier) {
		p.errorAt(p.current(), "expected module name or quoted source path after import")
		return nil
	}
	var b strings.Builder
	b.WriteString(p.advance().Lexeme)
	for p.at(token.Minus) {
		b.WriteByte('-')
		p.advance()
		if !p.at(token.Identifier) {
			p.errorAt(p.current(), "expected module-name segment after '-'")
			break
		}
		b.WriteString(p.advance().Lexeme)
	}
	alias := ""
	if p.match(token.As) {
		alias = p.expect(token.Identifier, "expected import alias").Lexeme
	}
	end := p.previous().Span
	p.consumeStmtEnd()
	return &ast.ImportDecl{Kind: ast.ImportModule, Target: b.String(), Alias: alias, SourceSpan: token.Merge(start, end)}
}

func (p *Parser) parseEnum() ast.Decl {
	start := p.advance().Span
	name := p.expect(token.Identifier, "expected enum name")
	p.expect(token.LBrace, "expected '{' after enum name")
	p.skipNewlines()
	var members []*ast.EnumMember
	for !p.at(token.RBrace) && !p.at(token.EOF) {
		member_name := p.expect(token.Identifier, "expected enum member name")
		var value ast.Expr
		if p.match(token.Assign) {
			value = p.parseExpression(precAssignment)
		}
		end := p.previous().Span
		members = append(members, &ast.EnumMember{Name: member_name.Lexeme, Value: value, SourceSpan: token.Merge(member_name.Span, end)})
		if !p.match(token.Comma) {
			p.consumeStmtEnd()
		}
		p.skipNewlines()
	}
	end := p.expect(token.RBrace, "expected '}' after enum").Span
	p.consumeStmtEnd()
	return &ast.EnumDecl{Name: name.Lexeme, Members: members, SourceSpan: token.Merge(start, end)}
}

func (p *Parser) parseExtern() ast.Decl {
	start := p.advance().Span
	abi := ""
	if p.at(token.Identifier) && p.current().Lexeme == "C" {
		abi = p.advance().Lexeme
	}
	t := p.parseTypeRef()
	if t == nil {
		p.errorAt(p.current(), "expected extern return type")
		p.synchronizeDecl()
		return nil
	}
	name := p.expect(token.Identifier, "expected extern function name")
	params := p.parseParameters()
	end := p.previous().Span
	p.consumeStmtEnd()
	return &ast.FunctionDecl{Name: name.Lexeme, ReturnType: t, Parameters: params, Extern: true, ABI: abi, SourceSpan: token.Merge(start, end)}
}

func (p *Parser) parseStruct(intrinsic bool) ast.Decl {
	start := p.advance().Span
	name := p.expect(token.Identifier, "expected struct name")
	gens := p.parseGenericParameters()
	var bases []*ast.TypeRef
	if p.match(token.Colon) {
		for {
			if t := p.parseTypeRef(); t != nil {
				bases = append(bases, t)
			} else {
				break
			}
			if !p.match(token.Comma) {
				break
			}
		}
	}
	p.expect(token.LBrace, "expected '{' after struct declaration")
	members := p.parseStructMembers()
	end := p.expect(token.RBrace, "expected '}' after struct").Span
	p.consumeStmtEnd()
	return &ast.StructDecl{Name: name.Lexeme, Intrinsic: intrinsic, GenericParameters: gens, Bases: bases, Members: members, SourceSpan: token.Merge(start, end)}
}

func (p *Parser) parseProtocol() ast.Decl {
	start := p.advance().Span
	name := p.expect(token.Identifier, "expected protocol name")
	gens := p.parseGenericParameters()
	p.expect(token.LBrace, "expected '{' after protocol name")
	var members []*ast.ProtocolMember
	p.skipNewlines()
	for !p.at(token.RBrace) && !p.at(token.EOF) {
		mstart := p.current().Span
		rt := p.parseTypeRef()
		if rt == nil {
			p.errorAt(p.current(), "expected protocol member return type")
			p.synchronizeStmt()
			continue
		}
		name := p.expect(token.Identifier, "expected protocol member name")
		params := p.parseParameters()
		end := p.previous().Span
		p.consumeStmtEnd()
		members = append(members, &ast.ProtocolMember{Name: name.Lexeme, ReturnType: rt, Parameters: params, SourceSpan: token.Merge(mstart, end)})
		p.skipNewlines()
	}
	end := p.expect(token.RBrace, "expected '}' after protocol").Span
	p.consumeStmtEnd()
	return &ast.ProtocolDecl{Name: name.Lexeme, GenericParameters: gens, Members: members, SourceSpan: token.Merge(start, end)}
}

func (p *Parser) parseTypedTopLevel() ast.Decl {
	startPos := p.pos
	start := p.current().Span
	t := p.parseTypeRef()
	if t == nil {
		return nil
	}
	// Detached method block: Type { ... }
	if p.at(token.LBrace) {
		p.advance()
		members := p.parseStructMembers()
		end := p.expect(token.RBrace, "expected '}' after method block").Span
		p.consumeStmtEnd()
		return &ast.MethodBlockDecl{Receiver: t, Members: members, SourceSpan: token.Merge(start, end)}
	}
	if !isNameKind(p.current().Kind) {
		p.pos = startPos
		return nil
	}
	first := p.advance()
	receiver := ""
	name := first.Lexeme
	if p.match(token.Dot) {
		receiver = first.Lexeme
		name = p.expect(token.Identifier, "expected method name after '.'").Lexeme
	}
	if p.at(token.LParen) {
		params := p.parseParameters()
		var body *ast.BlockStmt
		if p.at(token.LBrace) {
			body = p.parseBlock()
		} else {
			p.consumeStmtEnd()
		}
		end := p.previous().Span
		return &ast.FunctionDecl{Name: name, Receiver: receiver, ReturnType: t, Parameters: params, Body: body, SourceSpan: token.Merge(start, end)}
	}
	if receiver != "" {
		p.errorAt(p.current(), "detached method declaration requires parameter list")
		p.synchronizeDecl()
		return nil
	}
	var init ast.Expr
	if p.match(token.Assign) {
		init = p.parseExpression(precAssignment)
	}
	end := p.previous().Span
	p.consumeStmtEnd()
	return &ast.GlobalVarDecl{Name: name, Type: t, Initializer: init, SourceSpan: token.Merge(start, end)}
}

func (p *Parser) parseStructMembers() []ast.StructMember {
	var out []ast.StructMember
	p.skipNewlines()
	for !p.at(token.RBrace) && !p.at(token.EOF) {
		vis := ast.VisibilityDefault
		if p.match(token.Private) {
			vis = ast.VisibilityPrivate
		} else if p.match(token.Public) {
			vis = ast.VisibilityPublic
		}
		start := p.current().Span
		if p.match(token.Constructor) {
			params := p.parseParameters()
			body := p.parseBlock()
			out = append(out, &ast.ConstructorMember{ConstructorDecl: ast.ConstructorDecl{Parameters: params, Body: body, Visibility: vis, SourceSpan: token.Merge(start, body.Span())}})
			p.skipNewlines()
			continue
		}
		if p.match(token.OnFree) {
			if p.at(token.LParen) {
				_ = p.parseParameters()
			}
			body := p.parseBlock()
			out = append(out, &ast.DestructorMember{DestructorDecl: ast.DestructorDecl{Body: body, Visibility: vis, SourceSpan: token.Merge(start, body.Span())}})
			p.skipNewlines()
			continue
		}
		t := p.parseTypeRef()
		if t == nil {
			p.errorAt(p.current(), "expected field, method, constructor, or on_free declaration")
			p.synchronizeStmt()
			continue
		}
		name := p.expect(token.Identifier, "expected field or method name")
		if p.at(token.LParen) {
			params := p.parseParameters()
			body := p.parseBlock()
			f := ast.FunctionDecl{Name: name.Lexeme, ReturnType: t, Parameters: params, Body: body, Visibility: vis, SourceSpan: token.Merge(start, body.Span())}
			out = append(out, &ast.MethodDecl{FunctionDecl: f})
		} else {
			var init ast.Expr
			if p.match(token.Assign) {
				init = p.parseExpression(precAssignment)
			}
			end := p.previous().Span
			if p.match(token.Comma) {
				end = p.previous().Span
			}
			p.consumeStmtEnd()
			out = append(out, &ast.FieldDecl{Name: name.Lexeme, Type: t, Visibility: vis, Initializer: init, SourceSpan: token.Merge(start, end)})
		}
		p.skipNewlines()
	}
	return out
}

func (p *Parser) parseGenericParameters() []ast.GenericParameter {
	if !p.match(token.Less) {
		return nil
	}
	var out []ast.GenericParameter
	for !p.at(token.Greater) && !p.at(token.EOF) {
		t := p.expect(token.Identifier, "expected generic parameter name")
		out = append(out, ast.GenericParameter{Name: t.Lexeme, SourceSpan: t.Span})
		if !p.match(token.Comma) {
			break
		}
	}
	p.expect(token.Greater, "expected '>' after generic parameters")
	return out
}

func (p *Parser) parseTypeRef() *ast.TypeRef {
	startPos := p.pos
	p.skipInlineNewlines()
	start := p.current().Span
	isConst := false
	if p.match(token.Const) {
		isConst = true
	}
	if !isNameKind(p.current().Kind) {
		p.pos = startPos
		return nil
	}
	nameTok := p.advance()
	t := &ast.TypeRef{Name: nameTok.Lexeme, Const: isConst, SourceSpan: token.Merge(start, nameTok.Span)}
	if p.match(token.Less) {
		for !p.at(token.Greater) && !p.at(token.EOF) {
			arg := p.parseTypeRef()
			if arg == nil {
				p.errorAt(p.current(), "expected generic type argument")
				break
			}
			t.GenericArgs = append(t.GenericArgs, arg)
			if !p.match(token.Comma) {
				break
			}
		}
		end := p.expect(token.Greater, "expected '>' after generic type arguments").Span
		t.SourceSpan = token.Merge(start, end)
	}
	for p.match(token.Star) {
		t.PointerDepth++
		t.SourceSpan = token.Merge(start, p.previous().Span)
	}
	// Callable syntax is legal only here, in a type context.
	if p.at(token.LParen) {
		callStart := p.pos
		p.advance()
		var params []*ast.TypeRef
		variadic := false
		p.skipInlineNewlines()
		valid := true
		if !p.at(token.RParen) {
			for {
				if p.match(token.Ellipsis) {
					variadic = true
					break
				}
				pt := p.parseTypeRef()
				if pt == nil {
					valid = false
					break
				}
				params = append(params, pt)
				p.skipInlineNewlines()
				if p.match(token.Ellipsis) {
					variadic = true
					break
				}
				if !p.match(token.Comma) {
					break
				}
				p.skipInlineNewlines()
			}
		}
		if valid && p.at(token.RParen) {
			end := p.advance().Span
			retCopy := *t
			retCopy.Callable = nil
			t = &ast.TypeRef{Callable: &ast.CallableType{Return: &retCopy, Parameters: params, Variadic: variadic}, SourceSpan: token.Merge(start, end)}
		} else {
			p.pos = callStart
		}
	}
	for p.match(token.LBracket) {
		dimStart := p.previous().Span
		d := ast.ArrayDimension{Dynamic: true, SourceSpan: dimStart}
		if !p.at(token.RBracket) {
			d.Dynamic = false
			d.Size = p.parseExpression(precAssignment)
		}
		end := p.expect(token.RBracket, "expected ']' in array type").Span
		d.SourceSpan = token.Merge(dimStart, end)
		t.Arrays = append(t.Arrays, d)
		t.SourceSpan = token.Merge(start, end)
	}
	return t
}

func (p *Parser) parseParameters() []*ast.Parameter {
	p.expect(token.LParen, "expected '('")
	p.skipInlineNewlines()
	var out []*ast.Parameter
	seen_default := false
	if p.match(token.RParen) {
		return out
	}
	for !p.at(token.EOF) {
		start := p.current().Span
		if p.match(token.Ellipsis) {
			out = append(out, &ast.Parameter{Variadic: true, SourceSpan: p.previous().Span})
			p.skipInlineNewlines()
			break
		}
		t := p.parseTypeRef()
		if t == nil {
			p.errorAt(p.current(), "expected parameter type")
			p.synchronizeTo(token.Comma, token.RParen)
			if p.match(token.Comma) {
				continue
			}
			break
		}
		variadic := p.match(token.Ellipsis)
		name := ""
		if isNameKind(p.current().Kind) {
			name = p.advance().Lexeme
		}
		var default_value ast.Expr
		if p.match(token.Assign) {
			if variadic {
				p.errorAt(p.previous(), "variadic parameters cannot have default values")
			}
			seen_default = true
			default_value = p.parseExpression(precAssignment)
		} else if seen_default && !variadic {
			p.errorAt(p.current(), "required parameters cannot follow defaulted parameters")
		}
		end := p.previous().Span
		out = append(out, &ast.Parameter{Name: name, Type: t, DefaultValue: default_value, Variadic: variadic, SourceSpan: token.Merge(start, end)})
		if variadic {
			p.skipInlineNewlines()
			break
		}
		p.skipInlineNewlines()
		if !p.match(token.Comma) {
			break
		}
		p.skipInlineNewlines()
	}
	p.expect(token.RParen, "expected ')' after parameters")
	return out
}

func (p *Parser) parseBlock() *ast.BlockStmt {
	start := p.expect(token.LBrace, "expected '{'").Span
	p.skipNewlines()
	var stmts []ast.Stmt
	for !p.at(token.RBrace) && !p.at(token.EOF) {
		before := p.pos
		s := p.parseStatement()
		if s != nil {
			stmts = append(stmts, s)
		}
		if p.pos == before {
			p.errorAt(p.current(), "expected statement")
			p.advance()
		}
		p.skipNewlines()
	}
	end := p.expect(token.RBrace, "expected '}'").Span
	return &ast.BlockStmt{Statements: stmts, SourceSpan: token.Merge(start, end)}
}

func (p *Parser) parseStatement() ast.Stmt {
	p.skipNewlines()
	switch p.current().Kind {
	case token.Hash:
		return p.parse_mecc_config_stmt()
	case token.LBrace:
		return p.parseBlock()
	case token.Return:
		return p.parseReturn()
	case token.If:
		return p.parseIf()
	case token.While:
		return p.parseWhile()
	case token.For:
		return p.parseFor()
	case token.Switch:
		return p.parseSwitch()
	case token.Break:
		t := p.advance()
		p.consumeStmtEnd()
		return &ast.BreakStmt{SourceSpan: t.Span}
	case token.Continue:
		t := p.advance()
		p.consumeStmtEnd()
		return &ast.ContinueStmt{SourceSpan: t.Span}
	}
	// Speculatively parse a declaration. We require a following identifier so identifier expressions remain expressions.
	save := p.pos
	start := p.current().Span
	if t := p.parseTypeRef(); t != nil && isNameKind(p.current().Kind) {
		name := p.advance()
		var init ast.Expr
		if p.match(token.Assign) {
			init = p.parseExpression(precAssignment)
		}
		end := p.previous().Span
		p.consumeStmtEnd()
		return &ast.VarDeclStmt{Name: name.Lexeme, Type: t, Initializer: init, SourceSpan: token.Merge(start, end)}
	}
	p.pos = save
	expr := p.parseExpression(precAssignment)
	if expr == nil {
		p.synchronizeStmt()
		return nil
	}
	end := expr.Span()
	p.consumeStmtEnd()
	return &ast.ExprStmt{Expression: expr, SourceSpan: token.Merge(start, end)}
}

func (p *Parser) parseReturn() ast.Stmt {
	start := p.advance().Span
	if p.at(token.Newline) || p.at(token.Semicolon) || p.at(token.RBrace) {
		p.consumeStmtEnd()
		return &ast.ReturnStmt{SourceSpan: start}
	}
	v := p.parseExpression(precAssignment)
	end := v.Span()
	p.consumeStmtEnd()
	return &ast.ReturnStmt{Value: v, SourceSpan: token.Merge(start, end)}
}
func (p *Parser) parseIf() ast.Stmt {
	start := p.advance().Span
	var c ast.Expr
	if p.match(token.LParen) {
		p.skipInlineNewlines()
		c = p.parseExpression(precAssignment)
		p.skipInlineNewlines()
		p.expect(token.RParen, "expected ')' after if condition")
	} else {
		c = p.parseExpression(precAssignment)
	}
	p.skipNewlines()
	then := p.parseStatement()
	p.skipNewlines()
	var other ast.Stmt
	if p.match(token.Else) {
		p.skipNewlines()
		other = p.parseStatement()
	}
	end := start
	if then != nil {
		end = then.Span()
	}
	if other != nil {
		end = other.Span()
	}
	return &ast.IfStmt{Condition: c, Then: then, Else: other, SourceSpan: token.Merge(start, end)}
}
func (p *Parser) parseWhile() ast.Stmt {
	start := p.advance().Span
	var c ast.Expr
	if p.match(token.LParen) {
		c = p.parseExpression(precAssignment)
		p.expect(token.RParen, "expected ')' after while condition")
	} else {
		c = p.parseExpression(precAssignment)
	}
	p.skipNewlines()
	body := p.parseStatement()
	end := start
	if body != nil {
		end = body.Span()
	}
	return &ast.WhileStmt{Condition: c, Body: body, SourceSpan: token.Merge(start, end)}
}
func (p *Parser) parseFor() ast.Stmt {
	start := p.advance().Span
	p.skipInlineNewlines()
	paren := p.match(token.LParen)
	p.skipInlineNewlines()
	var init ast.Stmt
	var cond, post ast.Expr
	if !p.at(token.Semicolon) {
		save := p.pos
		s := p.current().Span
		if t := p.parseTypeRef(); t != nil && isNameKind(p.current().Kind) {
			n := p.advance()
			var v ast.Expr
			if p.match(token.Assign) {
				v = p.parseExpression(precAssignment)
			}
			init = &ast.VarDeclStmt{Name: n.Lexeme, Type: t, Initializer: v, SourceSpan: token.Merge(s, p.previous().Span)}
		} else {
			p.pos = save
			e := p.parseExpression(precAssignment)
			if e != nil {
				init = &ast.ExprStmt{Expression: e, SourceSpan: e.Span()}
			}
		}
	}
	p.expect(token.Semicolon, "expected ';' after for initializer")
	p.skipInlineNewlines()
	if !p.at(token.Semicolon) {
		cond = p.parseExpression(precAssignment)
	}
	p.expect(token.Semicolon, "expected ';' after for condition")
	p.skipInlineNewlines()
	if !(paren && p.at(token.RParen)) && !p.at(token.LBrace) {
		post = p.parseExpression(precAssignment)
	}
	if paren {
		p.expect(token.RParen, "expected ')' after for clauses")
	}
	p.skipNewlines()
	body := p.parseStatement()
	end := start
	if body != nil {
		end = body.Span()
	}
	return &ast.ForStmt{Init: init, Condition: cond, Post: post, Body: body, SourceSpan: token.Merge(start, end)}
}

func (p *Parser) parseSwitch() ast.Stmt {
	start := p.advance().Span
	var value ast.Expr
	if p.match(token.LParen) {
		value = p.parseExpression(precAssignment)
		p.expect(token.RParen, "expected ')' after switch expression")
	} else {
		value = p.parseExpression(precAssignment)
	}
	p.skipNewlines()
	p.expect(token.LBrace, "expected '{' after switch expression")
	p.skipNewlines()
	var cases []*ast.SwitchCase
	for !p.at(token.RBrace) && !p.at(token.EOF) {
		case_start := p.current().Span
		item := &ast.SwitchCase{}
		if p.match(token.Case) {
			item.Value = p.parseExpression(precAssignment)
			p.expect(token.Colon, "expected ':' after case value")
		} else if p.match(token.Default) {
			item.Default = true
			p.expect(token.Colon, "expected ':' after default")
		} else {
			p.errorAt(p.current(), "expected case or default in switch")
			p.advance()
			continue
		}
		p.skipNewlines()
		for !p.at(token.Case) && !p.at(token.Default) && !p.at(token.RBrace) && !p.at(token.EOF) {
			before := p.pos
			statement := p.parseStatement()
			if statement != nil {
				item.Statements = append(item.Statements, statement)
			}
			if p.pos == before {
				p.advance()
			}
			p.skipNewlines()
		}
		end := p.previous().Span
		item.SourceSpan = token.Merge(case_start, end)
		cases = append(cases, item)
	}
	end := p.expect(token.RBrace, "expected '}' after switch").Span
	return &ast.SwitchStmt{Value: value, Cases: cases, SourceSpan: token.Merge(start, end)}
}

const (
	precAssignment = 1
	precOr         = 2
	precAnd        = 3
	precEquality   = 4
	precCompare    = 5
	precAdd        = 6
	precMul        = 7
	precPrefix     = 8
	precPostfix    = 9
)

func infixPrecedence(k token.Kind) (int, bool) {
	switch k {
	case token.Assign, token.PlusAssign, token.MinusAssign, token.StarAssign, token.SlashAssign, token.PercentAssign:
		return precAssignment, true
	case token.OrOr:
		return precOr, false
	case token.AndAnd:
		return precAnd, false
	case token.Equal, token.NotEqual:
		return precEquality, false
	case token.Less, token.LessEqual, token.Greater, token.GreaterEqual:
		return precCompare, false
	case token.Plus, token.Minus:
		return precAdd, false
	case token.Star, token.Slash, token.Percent:
		return precMul, false
	}
	return 0, false
}
func isAssignment(k token.Kind) bool {
	switch k {
	case token.Assign, token.PlusAssign, token.MinusAssign, token.StarAssign, token.SlashAssign, token.PercentAssign:
		return true
	}
	return false
}

func (p *Parser) parseExpression(minPrec int) ast.Expr {
	left := p.parsePrefix()
	if left == nil {
		return nil
	}
	left = p.parsePostfix(left)
	for {
		if p.at(token.Newline) || p.at(token.Semicolon) || p.at(token.Comma) || p.at(token.RParen) || p.at(token.RBracket) || p.at(token.RBrace) || p.at(token.FatArrow) {
			break
		}
		prec, rightAssoc := infixPrecedence(p.current().Kind)
		if prec < minPrec || prec == 0 {
			break
		}
		op := p.advance()
		next := prec + 1
		if rightAssoc {
			next = prec
		}
		right := p.parseExpression(next)
		if right == nil {
			p.errorAt(p.current(), "expected expression after operator")
			break
		}
		sp := token.Merge(left.Span(), right.Span())
		if isAssignment(op.Kind) {
			left = &ast.AssignExpr{Operator: op.Lexeme, Target: left, Value: right, SourceSpan: sp}
		} else {
			left = &ast.BinaryExpr{Operator: op.Lexeme, Left: left, Right: right, SourceSpan: sp}
		}
	}
	return left
}

func (p *Parser) parsePrefix() ast.Expr {
	t := p.current()
	switch t.Kind {
	case token.Identifier, token.This:
		// A typed lambda begins with its return type, for example
		// `void(int32 value) => {}`.  Try that grammar before treating the
		// leading identifier as an ordinary expression.
		if t.Kind == token.Identifier && p.looks_like_explicit_lambda() {
			if lambda := p.try_parse_explicit_lambda(); lambda != nil {
				return lambda
			}
		}
		p.advance()
		return &ast.IdentifierExpr{Name: t.Lexeme, SourceSpan: t.Span}
	case token.Integer:
		p.advance()
		return &ast.LiteralExpr{Kind: ast.LiteralInteger, Raw: t.Lexeme, SourceSpan: t.Span}
	case token.Float:
		p.advance()
		return &ast.LiteralExpr{Kind: ast.LiteralFloat, Raw: t.Lexeme, SourceSpan: t.Span}
	case token.String:
		p.advance()
		return &ast.LiteralExpr{Kind: ast.LiteralString, Raw: t.Lexeme, SourceSpan: t.Span}
	case token.InterpolatedString:
		p.advance()
		return &ast.LiteralExpr{Kind: ast.LiteralInterpolatedString, Raw: t.Lexeme, SourceSpan: t.Span}
	case token.Character:
		p.advance()
		return &ast.LiteralExpr{Kind: ast.LiteralCharacter, Raw: t.Lexeme, SourceSpan: t.Span}
	case token.True, token.False:
		p.advance()
		return &ast.LiteralExpr{Kind: ast.LiteralBool, Raw: t.Lexeme, SourceSpan: t.Span}
	case token.Null:
		p.advance()
		return &ast.LiteralExpr{Kind: ast.LiteralNull, Raw: t.Lexeme, SourceSpan: t.Span}
	case token.Bang, token.Minus, token.Plus, token.Ampersand, token.Star, token.Tilde:
		op := p.advance()
		v := p.parseExpression(precPrefix)
		if v == nil {
			return nil
		}
		return &ast.UnaryExpr{Operator: op.Lexeme, Operand: v, Prefix: true, SourceSpan: token.Merge(op.Span, v.Span())}
	case token.New:
		start := p.advance().Span
		v := p.parseExpression(precPrefix)
		if v == nil {
			return nil
		}
		return &ast.NewExpr{Value: v, SourceSpan: token.Merge(start, v.Span())}
	case token.Cast:
		return p.parseCast()
	case token.LBracket:
		return p.parseArray()
	case token.LParen:
		if lam := p.tryParseLambda(); lam != nil {
			return lam
		}
		start := p.advance().Span
		p.skipInlineNewlines()
		e := p.parseExpression(precAssignment)
		p.skipInlineNewlines()
		end := p.expect(token.RParen, "expected ')' after expression").Span
		if e != nil {
			_ = start
			_ = end
		}
		return e
	default:
		p.errorAt(t, "expected expression")
		return nil
	}
}

func (p *Parser) parsePostfix(left ast.Expr) ast.Expr {
	for {
		switch p.current().Kind {
		case token.LParen:
			p.advance()
			p.skipInlineNewlines()
			var args []ast.Expr
			if !p.at(token.RParen) {
				for {
					e := p.parseExpression(precAssignment)
					if e != nil {
						args = append(args, e)
					}
					p.skipInlineNewlines()
					if !p.match(token.Comma) {
						break
					}
					p.skipInlineNewlines()
				}
			}
			end := p.expect(token.RParen, "expected ')' after arguments").Span
			left = &ast.CallExpr{Callee: left, Arguments: args, SourceSpan: token.Merge(left.Span(), end)}
		case token.LBracket:
			p.advance()
			p.skipInlineNewlines()
			idx := p.parseExpression(precAssignment)
			p.skipInlineNewlines()
			end := p.expect(token.RBracket, "expected ']' after index").Span
			left = &ast.IndexExpr{Object: left, Index: idx, SourceSpan: token.Merge(left.Span(), end)}
		case token.Dot:
			p.advance()
			name := p.expect(token.Identifier, "expected member name after '.'")
			left = &ast.MemberExpr{Object: left, Name: name.Lexeme, SourceSpan: token.Merge(left.Span(), name.Span)}
		case token.Less:
			// Generic specialization is only accepted when it can be closed and is immediately callable/member-accessed.
			save := p.pos
			p.advance()
			var args []*ast.TypeRef
			ok := true
			for {
				a := p.parseTypeRef()
				if a == nil {
					ok = false
					break
				}
				args = append(args, a)
				if !p.match(token.Comma) {
					break
				}
			}
			if !ok || !p.match(token.Greater) || (p.current().Kind != token.LParen && p.current().Kind != token.Dot) {
				p.pos = save
				return left
			}
			left = &ast.GenericSpecializationExpr{Target: left, TypeArguments: args, SourceSpan: token.Merge(left.Span(), p.previous().Span)}
		case token.Increment, token.Decrement:
			op := p.advance()
			left = &ast.UnaryExpr{Operator: op.Lexeme, Operand: left, Prefix: false, SourceSpan: token.Merge(left.Span(), op.Span)}
		default:
			return left
		}
	}
}

func (p *Parser) parseArray() ast.Expr {
	start := p.advance().Span
	p.skipInlineNewlines()
	var elems []ast.Expr
	if !p.at(token.RBracket) {
		for {
			e := p.parseExpression(precAssignment)
			if e != nil && p.match(token.Colon) {
				p.skipInlineNewlines()
				v := p.parseExpression(precAssignment)
				if v != nil {
					e = &ast.KeyValueExpr{Key: e, Value: v, SourceSpan: token.Merge(e.Span(), v.Span())}
				}
			}
			if e != nil {
				elems = append(elems, e)
			}
			p.skipInlineNewlines()
			if !p.match(token.Comma) {
				break
			}
			p.skipInlineNewlines()
			if p.at(token.RBracket) {
				break
			}
		}
	}
	end := p.expect(token.RBracket, "expected ']' after array literal").Span
	return &ast.ArrayExpr{Elements: elems, SourceSpan: token.Merge(start, end)}
}
func (p *Parser) parseCast() ast.Expr {
	start := p.advance().Span
	p.expect(token.Less, "expected '<' after cast")
	t := p.parseTypeRef()
	p.expect(token.Greater, "expected '>' after cast type")
	p.expect(token.LParen, "expected '(' after cast type")
	v := p.parseExpression(precAssignment)
	end := p.expect(token.RParen, "expected ')' after cast expression").Span
	return &ast.CastExpr{Type: t, Value: v, SourceSpan: token.Merge(start, end)}
}

// try_parse_lambda parses a contextually typed lambda such as `(value) => {}`
// or the explicit-parameter form `(int32 value) => {}`. Parameter types may be
// omitted only when semantic analysis receives a callable type from the context.
func (p *Parser) tryParseLambda() ast.Expr {
	save := p.pos
	start := p.current().Span
	p.advance()
	p.skipInlineNewlines()
	params, ok := p.parse_lambda_parameters(true)
	if !ok || !p.match(token.RParen) || !p.match(token.FatArrow) {
		p.pos = save
		return nil
	}
	p.skipNewlines()
	if !p.at(token.LBrace) {
		p.pos = save
		return nil
	}
	body := p.parseBlock()
	return &ast.LambdaExpr{Parameters: params, Body: body, SourceSpan: token.Merge(start, body.Span())}
}

// looks_like_explicit_lambda performs a diagnostic-free token lookahead before the
// parser enters type grammar. This prevents ordinary generic calls and relational
// expressions from producing speculative type errors while still recognizing a
// signature that is immediately followed by the lambda arrow.
func (p *Parser) looks_like_explicit_lambda() bool {
	index := p.pos
	if index >= len(p.tokens) || p.tokens[index].Kind != token.Identifier {
		return false
	}
	index++
	if index < len(p.tokens) && p.tokens[index].Kind == token.Less {
		depth := 0
		for index < len(p.tokens) {
			switch p.tokens[index].Kind {
			case token.Less:
				depth++
			case token.Greater:
				depth--
				if depth == 0 {
					index++
					goto generic_done
				}
			case token.EOF:
				return false
			}
			index++
		}
		return false
	}
generic_done:
	for index < len(p.tokens) && p.tokens[index].Kind == token.Star {
		index++
	}
	if index >= len(p.tokens) || p.tokens[index].Kind != token.LParen {
		return false
	}
	paren_depth := 0
	for index < len(p.tokens) {
		switch p.tokens[index].Kind {
		case token.LParen:
			paren_depth++
		case token.RParen:
			paren_depth--
			if paren_depth == 0 {
				index++
				for index < len(p.tokens) && p.tokens[index].Kind == token.Newline {
					index++
				}
				return index < len(p.tokens) && p.tokens[index].Kind == token.FatArrow
			}
		case token.EOF:
			return false
		}
		index++
	}
	return false
}

// try_parse_explicit_lambda parses the canonical standalone/auto lambda form:
// `ReturnType(TypedParameters...) => { ... }`. The explicit signature makes the
// lambda self-typed and therefore legal without a receiving callable context.
func (p *Parser) try_parse_explicit_lambda() ast.Expr {
	save := p.pos
	start := p.current().Span
	return_type := p.parseTypeRef()
	if return_type == nil || return_type.Callable != nil || !p.at(token.LParen) {
		p.pos = save
		return nil
	}
	p.advance()
	p.skipInlineNewlines()
	parameters, ok := p.parse_lambda_parameters(false)
	if !ok || !p.match(token.RParen) || !p.match(token.FatArrow) {
		p.pos = save
		return nil
	}
	p.skipNewlines()
	if !p.at(token.LBrace) {
		p.pos = save
		return nil
	}
	body := p.parseBlock()
	return &ast.LambdaExpr{ReturnType: return_type, Parameters: parameters, Body: body, SourceSpan: token.Merge(start, body.Span())}
}

// parse_lambda_parameters parses named lambda parameters. When allow_inferred is
// true, a bare identifier is retained with a nil Type so semantic analysis can
// obtain its type from the receiving callable signature.
func (p *Parser) parse_lambda_parameters(allow_inferred bool) ([]*ast.Parameter, bool) {
	var parameters []*ast.Parameter
	if p.at(token.RParen) {
		return parameters, true
	}
	for {
		parameter_start := p.current().Span
		if allow_inferred && p.at(token.Identifier) && (p.peek_kind(1) == token.Comma || p.peek_kind(1) == token.RParen) {
			name := p.advance()
			parameters = append(parameters, &ast.Parameter{Name: name.Lexeme, SourceSpan: name.Span})
		} else {
			parameter_type := p.parseTypeRef()
			if parameter_type == nil || !p.at(token.Identifier) {
				return nil, false
			}
			name := p.advance()
			parameters = append(parameters, &ast.Parameter{Name: name.Lexeme, Type: parameter_type, SourceSpan: token.Merge(parameter_start, name.Span)})
		}
		p.skipInlineNewlines()
		if !p.match(token.Comma) {
			break
		}
		p.skipInlineNewlines()
	}
	return parameters, true
}

// peek_kind returns the token kind at a small positive lookahead without moving the parser.
func (p *Parser) peek_kind(offset int) token.Kind {
	index := p.pos + offset
	if index < 0 || index >= len(p.tokens) {
		return token.EOF
	}
	return p.tokens[index].Kind
}

func (p *Parser) synchronizeStmt() {
	for !p.at(token.EOF) && !p.at(token.Newline) && !p.at(token.Semicolon) && !p.at(token.RBrace) {
		p.advance()
	}
	p.consumeStmtEnd()
}
func (p *Parser) synchronizeDecl() {
	for !p.at(token.EOF) && !p.at(token.Newline) && !p.at(token.Semicolon) && !p.at(token.RBrace) {
		p.advance()
	}
	p.consumeStmtEnd()
}
func (p *Parser) synchronizeTo(kinds ...token.Kind) {
	for !p.at(token.EOF) {
		for _, k := range kinds {
			if p.at(k) {
				return
			}
		}
		p.advance()
	}
}
func unquote(s string) string {
	if len(s) >= 2 {
		return s[1 : len(s)-1]
	}
	return s
}
