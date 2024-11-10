grammar clyth_grammar;

// Parser Section:
program: expression* | EOF
        ;
expression: var_declaration
            | functionCall
            | functionDeclaration
            | objectDeclaration
            | collectionInstantiation
            | objectInstantiation
            | methodCall
            | returnStatement
            ;

unaryExpression: NOT expression
                | BIT_NOT expression
                ;

binaryExpression: expression AND expression
                | expression OR expression
                | expression BIT_NAND expression
                | expression BIT_NOR expression
                | expression BIT_XOR expression
                | expression BIT_OR expression
                ;

var_declaration: VAR_DECLARATION_KEYWORDS IDENTIFIER ':' typeAnnotation '=' (LITERAL | expression) ';'?
                | VAR_DECLARATION_KEYWORDS IDENTIFIER ':' typeAnnotation '=' (LITERAL | expression) ';'?
                | VAR_DECLARATION_KEYWORDS IDENTIFIER '=' (LITERAL | expression) ';'?
                | VAR_DECLARATION_KEYWORDS IDENTIFIER ':' typeAnnotation ';'?
                | VAR_DECLARATION_KEYWORDS IDENTIFIER ';'?
                ;
functionDeclaration: FUNC_IDENTIFIER IDENTIFIER LEFT_PAREN ((IDENTIFIER COLON TYPE)+) COLON TYPE RIGHT_PAREN LEFT_CURLY_BRACE expression+ RIGHT_CURLY_BRACE;
functionCall: IDENTIFIER LEFT_PAREN (IDENTIFIER+) RIGHT_PAREN;
methodCall: IDENTIFIER '.' IDENTIFIER LEFT_PAREN (IDENTIFIER+) RIGHT_PAREN;
objectDeclaration: OBJ_DECLARATION_KEYWORDS IDENTIFIER LEFT_PAREN (IDENTIFIER+) RIGHT_PAREN;
typeAnnotation: IDENTIFIER '<' (TYPE | TYPE ','?)*? '>'
                | TYPE;
collectionInstantiation: typeAnnotation '[' (LITERAL ','? | IDENTIFIER ','? | objectDeclaration ','? | functionCall ','?)*? ']';
objectInstantiation: typeAnnotation '(' (LITERAL ','? | IDENTIFIER ','?)*? ')';
returnStatement : RETURN expression;

// Lexer Section:
SINGLE_LINE_COMMENT : '///' ~[\r\n]* -> skip;
MULTI_LINE_COMMENT: '/*'.*? '*/' -> skip;
TYPE: STRING | NUMERIC_TYPES | GENERIC_TYPES;

VAR_DECLARATION_KEYWORDS: 'let'
                        | 'const'
                        ;
OBJ_DECLARATION_KEYWORDS: 'class'
                        | 'struct'
                        ;

IDENTIFIER: [a-zA-Z_]+[a-zA-Z_0-9]?;
FUNC_IDENTIFIER: 'func'
                | 'function';
LITERAL: STRING_LITERAL | NUMERIC_LITERAL;
NUMERIC_LITERAL: [0-9]+
                |[0-9]+'.'[0-9]+'f'
                |[0-9]+'.'[0-9]+'d'
                ;
STRING_LITERAL: '"' .*? '"';

DOUBLE_QUOTES: '"';
SEMICOLON : ';';
COLON : ':';
DOT : '.';
LEFT_PAREN : '(';
RIGHT_PAREN : ')';
LEFT_SQUARE_BRACKET : '[';
RIGHT_SQUARE_BRACKET : ']';
LEFT_CURLY_BRACE : '{';
RIGHT_CURLY_BRACE : '}';
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

EQUALS : '=';
GREATER_THAN : '>';
LESS_THAN : '<';
EQUALS_EQUALS : '==';
GREATER_EQUALS : '>=';
RETURN : 'return';
STRING: 'string';
GENERIC_TYPES: 'numeric'
             | 'collection'
             | 'object'
             | 'primitive'
             ;
NUMERIC_TYPES : 'int8'
              | 'int16'
              | 'int32'
              | 'int64'
              | 'uint8'
              | 'uint16'
              | 'uint32'
              | 'uint64'
              | 'float32'
              | 'float64'
              ;


WS: [ \r\n\t]+ -> skip;