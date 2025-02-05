grammar clyth_grammar_simplified;

// Parser Section:
program: expression* | EOF
        ;
expression: importStatement
            | varDeclaration
            | functionDeclaration
            | objectDeclaration
            | collectionInstantiation
            | objectInstantiation
            | returnStatement
            | unaryExpression
            | binaryExpression
            | IDENTIFIER
            | LITERAL
            ;

unaryExpression: NOT subExpression
                | BIT_NOT subExpression
                | '!' subExpression
                | '-' subExpression
                ;

binaryExpression: subExpression AND subExpression
                | subExpression OR subExpression
                | subExpression BIT_NAND subExpression
                | subExpression BIT_NOR subExpression
                | subExpression BIT_XOR subExpression
                | subExpression BIT_OR subExpression
                | subExpression '*' subExpression
                | subExpression '/' subExpression
                | subExpression '+' subExpression
                | subExpression '-' subExpression
                | subExpression '**' subExpression
                | subExpression '//' subExpression
                ;

subExpression: functionCall
             | BOOLEAN_TYPES
             | IDENTIFIER
             | LITERAL
             ;

importStatement: 'import' STRING_LITERAL;

variableAssignment: IDENTIFIER '=' expression;
variableFTransfer: IDENTIFIER '=' expression'.ftransfer';
variableTransfer: IDENTIFIER '=' expression'.transfer';
variableCopy: IDENTIFIER '=' expression'.copy';

varDeclaration:  VAR_DECLARATION_KEYWORDS IDENTIFIER '=' 'new'? (LITERAL | LITERAL NUMERIC_TYPES| expression) ';'?
                | VAR_DECLARATION_KEYWORDS IDENTIFIER '=' 'new'? TYPE ';'?
                | VAR_DECLARATION_KEYWORDS IDENTIFIER '=' 'new'? objectInstantiation
                | VAR_DECLARATION_KEYWORDS IDENTIFIER '=' 'new'? '*'TYPE ';'?
                | VAR_DECLARATION_KEYWORDS IDENTIFIER '=' 'new'? '&'(IDENTIFIER) ';'?
                ;

functionDeclaration: FUNC_KEYWORD IDENTIFIER '(' ((TYPE IDENTIFIER ','?)*?) ')' TYPE '{' expression*? '}';
functionCall: IDENTIFIER '(' (IDENTIFIER | LITERAL)*? ')';
methodCall: IDENTIFIER '.' functionCall;
objectDeclaration: OBJ_DECLARATION_KEYWORDS IDENTIFIER '(' (IDENTIFIER+) ')';
collectionInstantiation: listInstantiation
                        | mapInstantiation
                        | setInstantiation
                        ;
listInstantiation: TYPE '[' ((LITERAL | IDENTIFIER | objectDeclaration | functionCall)','?)*? ']'
                 | TYPE '[' ((NUMERIC_LITERAL | IDENTIFIER | functionCall)*?'..'(NUMERIC_LITERAL | IDENTIFIER | functionCall)) ']'
                 ;
mapInstantiation: 'mapof' TYPE ':' TYPE '[' ((LITERAL | IDENTIFIER | objectDeclaration | functionCall )':'(LITERAL | IDENTIFIER | objectDeclaration | functionCall )','?)*? ']';
setInstantiation: 'setof' TYPE '[' ((LITERAL | IDENTIFIER | objectDeclaration | functionCall)','?)*? ']';

objectInstantiation: TYPE|IDENTIFIER '(' (LITERAL ','? | IDENTIFIER ','?)*? ')';
returnStatement : RETURN expression;

listIndex: IDENTIFIER'['(NUMERIC_LITERAL | IDENTIFIER | functionCall)']';
mapIndex: IDENTIFIER'['(LITERAL | IDENTIFIER | functionCall)']';
setIndex: IDENTIFIER'['(LITERAL | IDENTIFIER | functionCall)']';

// Lexer Section:
SINGLE_LINE_COMMENT : '#' ~[\r\n]* -> skip;
MULTI_LINE_COMMENT: '#*'.*? '*#' -> skip;

FUNC_KEYWORD: 'func';
VAR_DECLARATION_KEYWORDS: 'let'
                        | 'const'
                        ;
OBJ_DECLARATION_KEYWORDS: 'class'
                        | 'struct'
                        ;
LITERAL: STRING_LITERAL | NUMERIC_LITERAL;

NUMERIC_LITERAL: [0-9]+NUMERIC_TYPES?
                | [0-9]+'.'[0-9]+('f32' | 'f64')?
                ;

FORMATTED_STRING_LITERAL: '`' .*? '`';
STRING_LITERAL: '"' .*? '"' | FORMATTED_STRING_LITERAL;

TYPE: STRING | NUMERIC_TYPES | GENERIC_TYPES;

GENERIC_TYPES: 'numeric'
             | 'collection'
             | 'object'
             | 'primitive'
             ;

NUMERIC_TYPES : 'i8'
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

BOOLEAN_TYPES : 'true' | 'false';

NOT : '!'
    | 'not';
OR : '||'
    | 'or';
AND : '&&'
    | 'and'
    ;

BIT_NOT : '~';
BIT_OR : '|';
BIT_XOR : '^';
BIT_NOR : '~|';
BIT_AND : '&';
BIT_NAND : '~&';
RETURN : 'return';
STRING: 'string';

IDENTIFIER: [a-zA-Z_]+[a-zA-Z_0-9]?;

WS: [ \r\n\t]+ -> skip;