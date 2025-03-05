grammar clyth_grammar;

// Parser Section:
program : expression* | EOF
        ;
expression : var_declaration
           | ref_declaration
           | obj_declaration
           | function_declaration
        //    | function_call
        //    | class_method_call
           | binary_statements
           ;

sub_expression : IDENTIFIER
               | LITERAL
            //    | function_call
            //    | class_method_call
               ;

binary_statements: sub_expression '*' sub_expression
                 | sub_expression '/' sub_expression
                 | sub_expression '+' sub_expression
                 | sub_expression '-' sub_expression
                 | IDENTIFIER '*=' sub_expression
                 | IDENTIFIER '/=' sub_expression
                 | IDENTIFIER '+=' sub_expression
                 | IDENTIFIER '-=' sub_expression
                 ;

var_declaration: VAR_DECLARATION_KEYWORDS IDENTIFIER ('=' (LITERAL | IDENTIFIER | obj_instantiation))? ';'?;
ref_declaration: 'ref' '<'(IDENTIFIER | VARIABLE_TYPES)'>' IDENTIFIER ('=' '&'IDENTIFIER)? ';'?;
obj_declaration: 'struct' IDENTIFIER scope ';'?
                | 'class' IDENTIFIER '['(class_inherited_list)']' scope;
obj_instantiation: 'new'? IDENTIFIER'('(paramaters_list)?')';
function_declaration: (VARIABLE_TYPES | IDENTIFIER) IDENTIFIER '('function_parameters_list')' scope;
scope: '{' expression '}';

paramaters_list: parameter parameter_tail*?;
parameter: IDENTIFIER | LITERAL;
parameter_tail: ',' parameter;

function_parameters_list: function_parameter function_parameter_tail*?;
function_parameter: (TYPE | VARIABLE_TYPES | IDENTIFIER) IDENTIFIER;
function_parameter_tail: ',' function_parameter;
class_inherited_list: IDENTIFIER identifier_tail*?;
identifier_tail: ',' IDENTIFIER;

// Lexer Section:
SINGLE_LINE_COMMENT : '//' ~[\r\n]* -> skip;
MULTI_LINE_COMMENT: '/*'.*? '*/' -> skip;

VAR_DECLARATION_KEYWORDS: 'const'? 'auto'
                        | 'const'? VARIABLE_TYPES
                        ;


OBJ_DECLARATION_KEYWORDS: 'class'
                        | 'struct'
                        ;



LITERAL: STRING_LITERAL | NUMERIC_LITERAL;

NUMERIC_LITERAL: [0-9]+POSTFIX_LITERAL_TYPE?
                | [0-9]+'.'[0-9]+POSTFIX_LITERAL_TYPE?
                ;
FORMATTED_STRING_LITERAL: '`' .*? '`';
STRING_LITERAL: '"' .*? '"' | FORMATTED_STRING_LITERAL;

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

IDENTIFIER: [a-zA-Z_]+[a-zA-Z_0-9]?;

WS: [ \r\n\t]+ -> skip;