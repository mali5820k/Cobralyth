grammar Cobralyth;

// Parser Section:
program : statement* | EOF
        ;

statement: expression
        | variable_dec
        | if_statement
        | for_statement
        ;

expression: BOOLEAN_LITERAL
        ;

variable_dec: (VARIABLE_TYPE IDENTIFIER | IDENTIFIER IDENTIFIER ) ('=' (NUMERIC_LITERAL | BOOLEAN_LITERAL | STRING_LITERAL | 'new'? IDENTIFIER))?';'?
        ;

if_statement: 'if' ('('expression')' | expression ) '{' statement*? '}' else_statement?;
else_statement: 'else' (if_statement? | '{' statement*? '}');

for_statement: 'for' ('('expression?';'expression?';'expression?')' | expression?';'expression?';'expression?) '{' statement*? '}';

// Lexer Section:
SINGLE_LINE_COMMENT : '//' ~[\r\n]* -> skip;
MULTI_LINE_COMMENT: '/*'.*? '*/' -> skip;

STRUCT_KEYWORD: 'struct';

STRING_LITERAL: '"' ('\\'. | ~["\n\r\t] )*? '"'
              | '\'' ('\\'. | ~['\n\r\t] )*? '\''
              | '`' ('\\'. | ~[`\n\r\t] )*? '`'
              ;

BOOLEAN_LITERAL : 'true' | 'false';

NUMERIC_LITERAL: [0-9]+POSTFIX_LITERAL_TYPE?
                | [0-9]+'.'[0-9]+POSTFIX_LITERAL_TYPE?
                ;

VARIABLE_TYPE : 'uint8'
               | 'uint16'
               | 'uint32'
               | 'uint64'
               | 'uint'
               | 'int8'
               | 'int16'
               | 'int32'
               | 'int64'
               | 'int'
               | 'float32'
               | 'float64'
               | 'float'
               | 'double'
               | 'string'
               | 'char'
               | 'bool'
               | 'void'
               ;

POSTFIX_LITERAL_TYPE  : 'i8'
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

IDENTIFIER: [a-zA-Z_]+[a-zA-Z_0-9]?;

WS: [ \r\n\t]+ -> skip;