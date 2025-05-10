grammar clyth_grammar;

// Parser Section:
program : statement* | EOF
        ;
expressions: expression
           | binary_expression
           | unary_expression
           ;

expression: function_call
          | '('expression')'
          | unary_expression
          | IDENTIFIER
          | LITERAL
          ;

statement : var_declaration
          | ref_declaration
          | obj_declaration
          | function_declaration
          | function_call
          | if_statement
          | else_statement
          | return_statement
          | variable_assignment
          | reference_assignment
          ;
unary_expression: '&'expression
                | '-'expression
                | NOT expression
                | BIT_NOT expression
                ;

binary_expression: expression '>' binary_expression
                 | expression '<' binary_expression
                 | expression '>=' binary_expression
                 | expression '<=' binary_expression
                 | expression '==' binary_expression
                 | expression '!=' binary_expression
                 | expression '+' binary_expression
                 | expression '-' binary_expression
                 | expression '/' binary_expression
                 | expression '*' binary_expression
                 | expression '>' expression
                 | expression '<' expression
                 | expression '>=' expression
                 | expression '<=' expression
                 | expression '==' expression
                 | expression '!=' expression
                 | expression '-' expression
                 | expression '+' expression
                 | expression '/' expression
                 | expression '*' expression
                 ;

variable_assignment: IDENTIFIER '=' expressions ';'?
                   | IDENTIFIER '*=' expressions ';'?
                   | IDENTIFIER '/=' expressions ';'?
                   | IDENTIFIER '+=' expressions ';'?
                   | IDENTIFIER '-=' expressions ';'?
                   ;
var_declaration:  'const'? (TYPE | IDENTIFIER) IDENTIFIER ('=' LITERAL | IDENTIFIER | obj_declaration)? ';'?;
ref_declaration: 'const'? 'ref' '<'(TYPE | IDENTIFIER)'>' (reference_assignment | IDENTIFIER) ';'?;
reference_assignment: IDENTIFIER '=' '&'IDENTIFIER;
obj_declaration: 'struct' IDENTIFIER scope ';'?
                | 'class' IDENTIFIER '['(class_inherited_list)']' scope;
obj_instantiation: 'new'? IDENTIFIER'('(parameters_list)?')';
function_declaration: 'async'? (TYPE | IDENTIFIER) IDENTIFIER'('function_parameters_list?')' scope;
function_call: IDENTIFIER'('parameters_list?')' ';'?;
return_statement: RETURN expressions*? ';'?;
control_flow_statements: if_statement | else_statement;
if_statement: 'if' '('expressions*?')' scope else_statement?
             | 'if' expressions*? scope else_statement?;
else_statement: 'else' scope # else_statement_only
              | 'else' if_statement # else_if_statement
              ;
scope: '{' statement*? '}';

parameters_list: parameter parameter_tail*?;
parameter: expressions;
parameter_tail: ',' parameter;

function_parameters_list: function_parameter function_parameter_tail*?;
function_parameter: 'const'? (TYPE | IDENTIFIER) IDENTIFIER # nonref_parameter
                  | 'const'? 'ref' '<'(TYPE | IDENTIFIER)'>' IDENTIFIER # ref_parameter
                  ; 
function_parameter_tail: ',' function_parameter;
class_inherited_list: IDENTIFIER identifier_tail*?;
identifier_tail: ',' IDENTIFIER;

// Lexer Section:
FUNC_MODIFIERS: 'async';

SINGLE_LINE_COMMENT : '//' ~[\r\n]* -> skip;
MULTI_LINE_COMMENT: '/*'.*? '*/' -> skip;
INTERNAL_MACRO : '#' [\r\n]*;

OBJ_DECLARATION_KEYWORDS: 'class'
                        | 'struct'
                        ;



LITERAL: STRING_LITERAL | NUMERIC_LITERAL | BOOLEAN_LITERAL;
BOOLEAN_LITERAL : 'true' | 'false';

NUMERIC_LITERAL: [0-9]+POSTFIX_LITERAL_TYPE?
                | [0-9]+'.'[0-9]+POSTFIX_LITERAL_TYPE?
                ;
FORMATTED_STRING_LITERAL: '`' .*? '`';
STRING_LITERAL: '"' .*? '"'
              | '\'' .*? '\''
              | FORMATTED_STRING_LITERAL;

TYPE: VARIABLE_TYPES | GENERIC_TYPES;

GENERIC_TYPES: 'numeric'
             | 'collection'
             | 'object'
             | 'primitive'
             ;

VARIABLE_TYPES : 'uint8'
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
               | 'auto'
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

NOT : '!'
    | 'not'
    ;
OR : '||'
    | 'or'
    ;
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

IDENTIFIER: [a-zA-Z_]+[a-zA-Z_0-9]?;

WS: [ \r\n\t]+ -> skip;