grammar Cobralyth;

// Parser Section:
program : statement* | EOF
        ;

statement: variable_dec ';'?
        | variable_assignment ';'?
        | if_statement
        | for_statement
        | while_statement
        | function_dec
        | function_call ';'?
        | 'break'
        | 'continue'
        | 'return'
        ;

// Need to implement PEMDAS and ensure parenthesis and operators are all correctly processed (left vs right associative)
// Also need to implement the op_equals version of each operator type - including the boolean '==' and assignment operator '='.
expression: '('expression')'
        | '!' expression
        | expression ('>' | '<' | '>=' | '<=' | '==' | '!=' | '&&' | '||')  expression
        | <assoc=right> expression '^' expression
        | expression ('*' | '/') expression
        | expression ('+' | '-') expression
        | expression '%' expression
        | expression '**'
        | expression '++'
        | expression '--'
        | function_call
        | method_call
        | BOOLEAN_LITERAL
        | STRING_LITERAL
        | NUMERIC_LITERAL
        | NULL_LITERAL
        | IDENTIFIER
        ;

variable_dec: (VARIABLE_TYPE IDENTIFIER | IDENTIFIER IDENTIFIER ) ('=' (NUMERIC_LITERAL | BOOLEAN_LITERAL | STRING_LITERAL | 'new'? IDENTIFIER))?
        ;

variable_assignment: IDENTIFIER '=' expression;

if_statement: 'if' ('('expression')' | expression ) '{' statement*? '}' else_statement?;
else_statement: 'else' (if_statement? | '{' statement*? '}');

for_statement: 'for' ( '('(variable_dec | variable_assignment)?';'expression?';'expression?')' 
                        | (variable_dec | variable_assignment)?';'expression?';'expression?) '{' statement*? '}';
while_statement: 'while' ( '('expression')' | expression ) '{' statement*? '}';
param: IDENTIFIER IDENTIFIER
        | (VARIABLE_TYPE | IDENTIFIER) IDENTIFIER'='(STRING_LITERAL | NUMERIC_LITERAL | BOOLEAN_LITERAL | NULL_LITERAL)
        ;
function_dec: (VARIABLE_TYPE IDENTIFIER | IDENTIFIER IDENTIFIER ) '('(param (',' param)*?)?')' '{' statement*? '}'; 
function_call: IDENTIFIER'('(expression (',' expression)*?)?')';
method_call: IDENTIFIER'.'IDENTIFIER'('(expression (',' expression)*?)?')';


// Lexer Section:
SINGLE_LINE_COMMENT : '//' ~[\r\n]* -> skip;
MULTI_LINE_COMMENT: '/*'.*? '*/' -> skip;

STRUCT_KEYWORD: 'struct';

STRING_LITERAL: '"' ('\\'. | ~["\n\r\t] )*? '"'
              | '\'' ('\\'. | ~['\n\r\t] )*? '\''
              | '`' ('\\'. | ~[`\n\r\t] )*? '`'
              ;

BOOLEAN_LITERAL: 'true' | 'false';

NULL_LITERAL: 'null';

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