grammar ClythV1;

// ============================================================
// Clyth Language Spec V1 Grammar
//
// Goals:
// - C-like systems language frontend suitable for AST construction.
// - Fixed arrays are a base language primitive: T[N].
// - Dynamic arrays are a base language primitive: T[].
// - Dynamic containers are explicit generic runtime/library types: List<T>, Map<K:V>, Set<T>.
// - [] parses as a neutral list literal.
// - {} parses as a neutral curly collection literal; semantic analysis decides Map vs Set.
// - Struct methods are supported through method blocks and qualified method declarations.
// - MECC syntax is parsed as an opt-in modifier/block for later semantic lowering.
// ============================================================

program
    : topLevelItem* EOF
    ;

topLevelItem
    : includeDecl
    | externDecl
    | structDecl
    | methodBlock
    | functionDecl
    | methodDecl
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
    : listLiteral
    | curlyLiteral
    ;

listLiteral
    : LBRACKET expressionList? RBRACKET
    ;

curlyLiteral
    : LBRACE curlyEntryList? RBRACE
    ;

curlyEntryList
    : curlyEntry (COMMA curlyEntry)* COMMA?
    ;

curlyEntry
    : expression (COLON expression)?
    ;

expressionList
    : expression (COMMA expression)* COMMA?
    ;

// ============================================================
// Types
// ============================================================
//
// V1 canonical container model:
// - T[N]        = fixed array in the base language.
// - T[]         = dynamic array in the base language.
// - pointer<T>  = explicit raw interop pointer type.
// - List<T>     = dynamic list runtime/library type.
// - Map<K:V>    = dynamic map runtime/library type.
// - Set<T>      = dynamic set runtime/library type.
//
// Note: genericArgList supports both COMMA and COLON separators so Clyth can
// accept Map<K,V> and map<K:V>-style spellings while semantic analysis
// canonicalizes them into one internal type representation.

type
    : fixedArrayType
    | dynamicArrayType
    | genericType
    | baseType
    ;

fixedArrayType
    : typeAtom LBRACKET NUMERIC_LITERAL RBRACKET
    ;

dynamicArrayType
    : typeAtom LBRACKET RBRACKET
    ;

genericType
    : IDENTIFIER LT genericArgList GT
    ;

genericArgList
    : type ((COMMA | COLON) type)*
    ;

typeAtom
    : genericType
    | baseType
    ;

baseType
    : BUILTIN_TYPE
    | IDENTIFIER
    ;

commaOrSemi
    : COMMA
    | SEMI
    ;

// ============================================================
// Lexer rules
// ============================================================

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
