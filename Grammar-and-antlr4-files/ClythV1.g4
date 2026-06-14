grammar ClythV1;

// ============================================================
// Clyth Language Spec V1 Grammar
//
// Goals:
// - C-like systems language frontend suitable for AST construction.
// - Supports comments, declarations, structs, functions, control flow,
//   C interop, includes, lists, fixed arrays, maps, sets, `is`, `in`,
//   ordinary calls such as print/printf, and template strings.
// - MECC syntax is parsed as an opt-in modifier/block for later semantic lowering.
// - Struct methods are supported in two forms:
//
//     MyStruct {
//         MyStruct constructor() {}
//         void destructor() {}
//         void _privateMethod() {}
//     }
//
//     bool MyStruct.additionalMethod() {}
//
// Notes:
// - This grammar is intentionally permissive in a few places so semantic passes
//   can decide what is legal for V1 versus future extensions.
// - `statement` is allowed at top-level for parser tests/script-mode experiments.
//   Semantic analysis can reject top-level statements later.
// ============================================================


// ============================================================
// Parser rules
// ============================================================

program
    : topLevelItem* EOF
    ;

topLevelItem
    : includeDecl
    | externDecl
    | structDecl
    | methodBlock
    | functionDecl       // must appear before methodDecl so normal top-level functions stay functions
    | methodDecl         // mainly for qualified methods: ReturnType StructName.methodName(...)
    | statement
    ;


// -----------------------------
// Top-level declarations
// -----------------------------

includeDecl
    : INCLUDE includeTarget SEMI?
    ;

includeTarget
    : IDENTIFIER
    | STRING_LITERAL
    | TEMPLATE_STRING_LITERAL
    ;

externDecl
    : EXTERN externAbi? type IDENTIFIER LPAREN externParamList? RPAREN SEMI?
    ;

externAbi
    : C_ABI
    | IDENTIFIER
    ;

externParamList
    : externParam (COMMA externParam)* COMMA? VARARGS?
    | VARARGS
    ;

externParam
    : type IDENTIFIER?
    ;

structDecl
    : STRUCT IDENTIFIER LBRACE structField* RBRACE SEMI?
    ;

structField
    : type IDENTIFIER commaOrSemi?
    ;


// -----------------------------
// Methods
// -----------------------------
//
// Method names may be normal identifiers or constructor/destructor keywords.
// The parser accepts private-looking names such as `_computeArea`; semantic
// analysis can interpret underscore-prefixed methods as private.
//
// Inside a method block:
//
//     MyStruct {
//         void foo() {}
//     }
//
// Top-level qualified method:
//
//     bool MyStruct.foo() {}
//
// Normal top-level functions still parse as functionDecl because functionDecl
// appears before methodDecl in topLevelItem.

methodBlock
    : IDENTIFIER LBRACE methodDecl* RBRACE
    ;

methodDecl
    : MECC? type methodSimpleName LPAREN paramList? RPAREN block
    | MECC? type IDENTIFIER DOT methodSimpleName LPAREN paramList? RPAREN block
    ;

methodSimpleName
    : IDENTIFIER
    | CONSTRUCTOR
    | DESTRUCTOR
    ;

functionDecl
    : MECC? type IDENTIFIER LPAREN paramList? RPAREN block
    ;

paramList
    : param (COMMA param)* COMMA?
    ;

param
    : type IDENTIFIER
    ;


// -----------------------------
// Statements
// -----------------------------

block
    : LBRACE statement* RBRACE
    ;

statement
    : varDecl
    | assignmentStmt
    | exprStmt
    | returnStmt
    | ifStmt
    | whileStmt
    | forStmt
    | breakStmt
    | continueStmt
    | meccBlock
    | block
    ;

varDecl
    : type IDENTIFIER (ASSIGN expression)? SEMI?
    ;

assignmentStmt
    : assignable assignmentOp expression SEMI?
    ;

assignable
    : IDENTIFIER postfixSuffix*
    ;

assignmentOp
    : ASSIGN
    | PLUS_ASSIGN
    | MINUS_ASSIGN
    | STAR_ASSIGN
    | SLASH_ASSIGN
    | PERCENT_ASSIGN
    ;

exprStmt
    : expression SEMI?
    ;

returnStmt
    : RETURN expression? SEMI?
    ;

ifStmt
    : IF parenExpr block elseClause?
    | IF expression block elseClause?
    ;

elseClause
    : ELSE ifStmt
    | ELSE block
    ;

whileStmt
    : WHILE parenExpr block
    | WHILE expression block
    ;

forStmt
    : FOR block
    | FOR forEachHeader block
    | FOR LPAREN forEachHeader RPAREN block
    | FOR forClassicHeader block
    | FOR LPAREN forClassicHeader RPAREN block
    ;

forEachHeader
    : forEachBinding IN expression
    ;

forEachBinding
    : forEachVar (COMMA forEachVar)*
    ;

forEachVar
    : type IDENTIFIER
    | IDENTIFIER
    ;

forClassicHeader
    : forInit? SEMI expression? SEMI expression?
    ;

forInit
    : varDeclNoSemi
    | assignmentNoSemi
    | expression
    ;

varDeclNoSemi
    : type IDENTIFIER (ASSIGN expression)?
    ;

assignmentNoSemi
    : assignable assignmentOp expression
    ;

breakStmt
    : BREAK SEMI?
    ;

continueStmt
    : CONTINUE SEMI?
    ;

meccBlock
    : MECC block
    ;

parenExpr
    : LPAREN expression RPAREN
    ;


// ============================================================
// Expression precedence
// ============================================================

expression
    : logicalOr
    ;

logicalOr
    : logicalAnd (OR logicalAnd)*
    ;

logicalAnd
    : equality (AND equality)*
    ;

equality
    : relation ((EQ | NEQ) relation)*
    ;

relation
    : comparison ((IS | IN) comparison)*
    ;

comparison
    : additive ((GT | LT | GTE | LTE) additive)*
    ;

additive
    : multiplicative ((PLUS | MINUS) multiplicative)*
    ;

multiplicative
    : unary ((STAR | SLASH | PERCENT) unary)*
    ;

unary
    : (NOT | BANG | MINUS | TILDE | AMP) unary
    | postfix
    ;

postfix
    : primary postfixSuffix*
    ;

postfixSuffix
    : DOT IDENTIFIER
    | LBRACKET expression RBRACKET
    | LPAREN argumentList? RPAREN
    | INCREMENT
    | DECREMENT
    ;

primary
    : literal
    | collectionLiteral
    | allocationExpression
    | IDENTIFIER
    | LPAREN expression RPAREN
    ;

allocationExpression
    : (MALLOC | ISO_MALLOC) LPAREN type RPAREN
    ;

argumentList
    : expression (COMMA expression)* COMMA?
    ;

literal
    : TEMPLATE_STRING_LITERAL
    | STRING_LITERAL
    | NUMERIC_LITERAL
    | BOOLEAN_LITERAL
    | NULL_LITERAL
    ;


// ============================================================
// Collection literals
// ============================================================

collectionLiteral
    : mapLiteral
    | setLiteral
    | listLiteral
    ;

listLiteral
    : LBRACKET expressionList? RBRACKET
    ;

mapLiteral
    : LBRACE mapEntryList? RBRACE
    ;

mapEntryList
    : mapEntry (COMMA mapEntry)* COMMA?
    ;

mapEntry
    : expression COLON expression
    ;

// Set literals use the same `{ ... }` surface form as maps.
// During AST/semantic analysis:
// - `{ a: b }` is a map literal.
// - `{ a, b, c }` is a set literal.
// - `{}` is syntactically accepted as mapLiteral by default and can be
//   semantically interpreted from target type when assigned.
setLiteral
    : LBRACE expressionList COMMA? RBRACE
    ;

expressionList
    : expression (COMMA expression)* COMMA?
    ;


// ============================================================
// Types
// ============================================================

type
    : mapType
    | collectionType
    ;

mapType
    : collectionType COLON collectionType
    ;

collectionType
    : baseType typeSuffix*
    ;

baseType
    : BUILTIN_TYPE
    | IDENTIFIER
    ;

typeSuffix
    : LBRACKET RBRACKET                 // dynamic list: int32[]
    | LBRACKET NUMERIC_LITERAL RBRACKET // fixed array: int32[10]
    | LPAREN RPAREN                     // set: int32()
    ;

commaOrSemi
    : COMMA
    | SEMI
    ;


// ============================================================
// Lexer rules
// ============================================================

// Keywords.
// Keep these before IDENTIFIER.

INCLUDE : 'include';
EXTERN  : 'extern';
C_ABI   : 'C';
STRUCT  : 'struct';

MECC       : 'mecc';
MALLOC     : 'malloc';
ISO_MALLOC : 'iso_malloc';

CONSTRUCTOR : 'constructor';
DESTRUCTOR  : 'destructor';

IF       : 'if';
ELSE     : 'else';
FOR      : 'for';
WHILE    : 'while';
RETURN   : 'return';
BREAK    : 'break';
CONTINUE : 'continue';

IS  : 'is';
IN  : 'in';
NOT : 'not';

BOOLEAN_LITERAL
    : 'true'
    | 'false'
    ;

NULL_LITERAL
    : 'null'
    ;


// Punctuation.

LPAREN   : '(';
RPAREN   : ')';
LBRACE   : '{';
RBRACE   : '}';
LBRACKET : '[';
RBRACKET : ']';

COMMA : ',';
COLON : ':';
SEMI  : ';';
DOT   : '.';


// Operators.
// Multi-character operators must appear before single-character prefixes.

OR  : '||';
AND : '&&';

EQ  : '==';
NEQ : '!=';

GTE : '>=';
LTE : '<=';
GT  : '>';
LT  : '<';

INCREMENT : '++';
DECREMENT : '--';

PLUS_ASSIGN    : '+=';
MINUS_ASSIGN   : '-=';
STAR_ASSIGN    : '*=';
SLASH_ASSIGN   : '/=';
PERCENT_ASSIGN : '%=';

ASSIGN : '=';

PLUS    : '+';
MINUS   : '-';
STAR    : '*';
SLASH   : '/';
PERCENT : '%';

BANG  : '!';
TILDE : '~';
AMP   : '&';

VARARGS : '...';


// Types and literals.

BUILTIN_TYPE
    : 'uint8'
    | 'uint16'
    | 'uint32'
    | 'uint64'
    | 'uint'
    | 'int8'
    | 'int16'
    | 'int32'
    | 'int64'
    | 'int'
    | 'numeric'
    | 'float32'
    | 'float64'
    | 'float'
    | 'double'
    | 'string'
    | 'char'
    | 'bool'
    | 'void'
    | 'auto'
    ;

NUMERIC_LITERAL
    : DIGIT+ NUMERIC_SUFFIX?
    | DIGIT+ DOT DIGIT+ NUMERIC_SUFFIX?
    ;

// Backtick strings are separated so the AST can detect template strings
// early and later parse `${...}` interpolation segments.
TEMPLATE_STRING_LITERAL
    : '`' ('\\' . | ~[`\\\r\n])* '`'
    ;

STRING_LITERAL
    : '"' ('\\' . | ~["\\\r\n])* '"'
    | '\'' ('\\' . | ~['\\\r\n])* '\''
    ;

IDENTIFIER
    : [a-zA-Z_] [a-zA-Z_0-9]*
    ;

fragment DIGIT
    : [0-9]
    ;

fragment NUMERIC_SUFFIX
    : 'i8'
    | 'i16'
    | 'i32'
    | 'i64'
    | 'ui8'
    | 'ui16'
    | 'ui32'
    | 'ui64'
    | 'f32'
    | 'f64'
    ;


// Whitespace and comments.

SINGLE_LINE_COMMENT
    : '//' ~[\r\n]* -> skip
    ;

MULTI_LINE_COMMENT
    : '/*' .*? '*/' -> skip
    ;

WS
    : [ \t\r\n]+ -> skip
    ;
