grammar ClythV1;

// ============================================================
// Clyth Language Spec V1 Grammar
//
// Goals:
// - C-like systems language frontend suitable for AST construction.
// - Supports V1 syntax samples: comments, declarations, structs,
//   functions, control flow, C interop, includes, lists, arrays,
//   maps, sets, `is`, `in`, print/printf calls, and template strings.
// - MECC is intentionally not represented in this grammar.
// ============================================================

// -----------------------------
// Parser rules
// -----------------------------

program
    : topLevelItem* EOF
    ;

// `statement` is intentionally allowed at top-level for parser test files.
// A later semantic pass can reject non-declaration top-level statements
// outside of test/script mode if desired.
topLevelItem
    : includeDecl
    | externDecl
    | structDecl
    | functionDecl
    | statement
    ;

includeDecl
    : INCLUDE includeTarget SEMI?
    ;

includeTarget
    : IDENTIFIER
    | STRING_LITERAL
    | TEMPLATE_STRING_LITERAL
    ;

externDecl
    : EXTERN externAbi? type IDENTIFIER '(' externParamList? ')' SEMI?
    ;

externAbi
    : C_ABI
    | IDENTIFIER
    ;

externParamList
    : externParam (',' externParam)* ','? (VARARGS)?
    | VARARGS
    ;

externParam
    : type IDENTIFIER?
    ;

structDecl
    : STRUCT IDENTIFIER '{' structField* '}' SEMI?
    ;

structField
    : type IDENTIFIER commaOrSemi?
    ;

functionDecl
    : type IDENTIFIER '(' paramList? ')' block
    ;

paramList
    : param (',' param)* ','?
    ;

param
    : type IDENTIFIER
    ;

block
    : '{' statement* '}'
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
    | block
    ;

varDecl
    : type IDENTIFIER ('=' expression)? SEMI?
    ;

assignmentStmt
    : assignable assignmentOp expression SEMI?
    ;

assignable
    : IDENTIFIER postfixSuffix*
    ;

assignmentOp
    : '='
    | '+='
    | '-='
    | '*='
    | '/='
    | '%='
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
    | FOR '(' forEachHeader ')' block
    | FOR forClassicHeader block
    | FOR '(' forClassicHeader ')' block
    ;

forEachHeader
    : forEachBinding IN expression
    ;

forEachBinding
    : forEachVar (',' forEachVar)*
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
    : type IDENTIFIER ('=' expression)?
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

parenExpr
    : '(' expression ')'
    ;

// -----------------------------
// Expression precedence
// -----------------------------

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
    | '[' expression ']'
    | '(' argumentList? ')'
    | INCREMENT
    | DECREMENT
    ;

primary
    : literal
    | collectionLiteral
    | IDENTIFIER
    | '(' expression ')'
    ;

argumentList
    : expression (',' expression)* ','?
    ;

literal
    : TEMPLATE_STRING_LITERAL
    | STRING_LITERAL
    | NUMERIC_LITERAL
    | BOOLEAN_LITERAL
    | NULL_LITERAL
    ;

// -----------------------------
// Collection literals
// -----------------------------

collectionLiteral
    : listLiteral
    | mapLiteral
    | setLiteral
    ;

listLiteral
    : '[' expressionList? ']'
    ;

mapLiteral
    : '{' mapEntryList? '}'
    ;

mapEntryList
    : mapEntry (',' mapEntry)* ','?
    ;

mapEntry
    : expression ':' expression
    ;

// Set literals use the same `{ ... }` surface form as maps.
// During AST construction:
// - `{ a: b }` is a map literal.
// - `{ a, b, c }` is a set literal.
// - `{}` is syntactically accepted as mapLiteral by default and can be
//   semantically interpreted from target type when assigned.
setLiteral
    : '{' expressionList ','? '}'
    ;

expressionList
    : expression (',' expression)* ','?
    ;

// -----------------------------
// Types
// -----------------------------

type
    : mapType
    | collectionType
    ;

mapType
    : collectionType ':' collectionType
    ;

collectionType
    : baseType typeSuffix*
    ;

baseType
    : BUILTIN_TYPE
    | IDENTIFIER
    ;

typeSuffix
    : '[' ']'                 // dynamic list: int32[]
    | '[' NUMERIC_LITERAL ']'  // fixed array: int32[10]
    | '(' ')'                 // set: int32()
    ;

commaOrSemi
    : ','
    | SEMI
    ;

// -----------------------------
// Lexer rules
// -----------------------------

INCLUDE : 'include';
EXTERN  : 'extern';
C_ABI   : 'C';
STRUCT  : 'struct';

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

SEMI : ';';

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

PLUS    : '+';
MINUS   : '-';
STAR    : '*';
SLASH   : '/';
PERCENT : '%';
BANG    : '!';
TILDE   : '~';
AMP     : '&';
DOT     : '.';

VARARGS : '...';

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

BOOLEAN_LITERAL
    : 'true'
    | 'false'
    ;

NULL_LITERAL
    : 'null'
    ;

NUMERIC_LITERAL
    : DIGIT+ NUMERIC_SUFFIX?
    | DIGIT+ '.' DIGIT+ NUMERIC_SUFFIX?
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

SINGLE_LINE_COMMENT
    : '//' ~[\r\n]* -> skip
    ;

MULTI_LINE_COMMENT
    : '/*' .*? '*/' -> skip
    ;

WS
    : [ \t\r\n]+ -> skip
    ;
