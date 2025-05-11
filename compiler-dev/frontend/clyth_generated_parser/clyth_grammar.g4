grammar clyth_grammar;

// Parser Section:
program : statement* | EOF
        ;
expressions: expression
           | binary_expression
           | unary_expression
           | postfix_expression
           ;

expression: method_call
          | function_call
          | '('expression')'
          | unary_expression
          | var_dereference
          | IDENTIFIER
          | LITERAL
          ;

statement : var_declaration
          | ref_declaration
          | obj_declaration
          | method_declaration
          | function_declaration
          | method_call
          | function_call
          | control_flow_statements
          | return_statement
          | var_assignment
          | reference_assignment
          | internal_macro
          ;
unary_expression: '&'expression
                | '-'expression
                | '!' expression
                | '~' expression
                ;
postfix_expression: expression'++'
                  | expression'--'
                  | expression '*=' expression
                  | expression '/=' expression
                  | expression '+=' expression
                  | expression '-=' expression
                  | expression '%=' expression
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
                 | expression '||' binary_expression
                 | expression '&&' binary_expression
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
                 | expression '||' expression
                 | expression '&&' expression
                 ;

var_assignment: IDENTIFIER '=' expressions ';'? # variable_assignment
                | IDENTIFIER '*=' expressions ';'? # variable_times_equals_assignment
                | IDENTIFIER '/=' expressions ';'? # variable_div_equals_assignment
                | IDENTIFIER '+=' expressions ';'? # variable_plus_equals_assignment
                | IDENTIFIER '-=' expressions ';'? # variable_minus_equals_assignment
                | IDENTIFIER'.'IDENTIFIER '=' expressions ';'? # class_member_assignment
                | IDENTIFIER'.'IDENTIFIER '*=' expressions ';'? # class_member_times_equals_assignment
                | IDENTIFIER'.'IDENTIFIER '/=' expressions ';'? # class_member_div_equals_assignment
                | IDENTIFIER'.'IDENTIFIER '+=' expressions ';'? # class_member_plus_equals_assignment
                | IDENTIFIER'.'IDENTIFIER '-=' expressions ';'? # class_member_minus_equals_assignment
                ;
var_dereference : IDENTIFIER'.'IDENTIFIER;
var_declaration:  ('public' | 'private' | 'protected')? 'const'? (TYPE | IDENTIFIER) IDENTIFIER ('=' LITERAL | IDENTIFIER | obj_instantiation)? ';'? # variable_declaration
               |  ('public' | 'private' | 'protected')? 'const'? (TYPE | IDENTIFIER)'[]' IDENTIFIER ('=' (list_elements | set_elements))? ';'? # dynamic_array_declaration
               |  ('public' | 'private' | 'protected')? 'const'? (TYPE | IDENTIFIER)'['(NUMERIC_LITERAL | IDENTIFIER)']' IDENTIFIER ('=' (LITERAL | IDENTIFIER | obj_declaration | list_elements | set_elements))? ';'? # static_array_declaration
               |  ('public' | 'private' | 'protected')? 'const'? (TYPE | IDENTIFIER)':'(TYPE | IDENTIFIER) IDENTIFIER ('=' map_elements)? ';'? # map_declaration
               ;
list_elements: '[' elements_list? ']'
             | '[]'
             ;
set_elements: '(' elements_list? ')'
             | '()'
             ;
map_elements: '{' map_elements_list? '}'
            | '{}'
            ;
map_elements_list: map_element map_elements_tail*?;
map_element: element ':' element;
map_elements_tail: ',' map_element | ',';

elements_list: element elements_tail*?;
element: LITERAL | IDENTIFIER | obj_instantiation | list_elements | set_elements;
elements_tail: ',' element | ',';

ref_declaration: 'const'? 'ref' '<'(TYPE | IDENTIFIER)'>' (reference_assignment | IDENTIFIER) ';'?;
reference_assignment: IDENTIFIER '=' '&'IDENTIFIER;

obj_declaration: 'struct' IDENTIFIER scope ';'? # struct_declaration
                | 'class' IDENTIFIER ('from' '['(class_inherited_list)']')? scope # class_declaration
                | 'interface' IDENTIFIER ('from' '['(class_inherited_list)']')? scope # interface_declaration
                ;
obj_instantiation: 'new'? IDENTIFIER'('(parameters_list)')'
                 | 'new'? IDENTIFIER'()'
                 ;

function_declaration: 'async'? (TYPE | IDENTIFIER) IDENTIFIER'('function_parameters_list')' scope # function_declaration_params
                    | 'async'? (TYPE | IDENTIFIER) IDENTIFIER'()' scope # function_declaration_no_params
                    ;
function_call: IDENTIFIER'('parameters_list')' ';'?
             | IDENTIFIER'()' ';'?
             ;

method_declaration: ('public' | 'private' | 'protected')? 'async'? (TYPE | IDENTIFIER) IDENTIFIER'('function_parameters_list?')' scope # class_method_declaration_params
                    | ('public' | 'private' | 'protected')? 'async'? (TYPE | IDENTIFIER) IDENTIFIER'()' scope # class_method_declaration_no_params
                    | ('public' | 'private' | 'protected')? IDENTIFIER '('function_parameters_list?')' scope # class_constructor_declaration_params
                    | ('public' | 'private' | 'protected')? IDENTIFIER '()' scope # class_constructor_declaration_no_params
                    | ('public' | 'private' | 'protected')? (TYPE | IDENTIFIER) IDENTIFIER '('function_parameters_list')' ';'? # interface_method_declaration_params
                    | ('public' | 'private' | 'protected')? (TYPE | IDENTIFIER) IDENTIFIER '()' ';'? # interface_method_declaration_no_params
                    ;
method_call: IDENTIFIER'.'IDENTIFIER '('parameters_list')' ';'?
           | IDENTIFIER'.'IDENTIFIER '()' ';'?
           ;

return_statement: 'return' expressions*? ';'?;
control_flow_statements: if_statement else_statement?
                       | for_statement
                       | foreach_statement
                       | while_statement
                       | 'break' ';'?
                       | 'continue' ';'?
                       ;
if_statement: 'if' '('expressions*?')' scope else_statement?
             | 'if' expressions*? scope else_statement?;
else_statement: 'else' scope # else_statement_only
              | 'else' if_statement # else_if_statement
              ;
for_statement: 'for' '('var_declaration';' binary_expression';' postfix_expression')' scope
             | 'for' var_declaration';' binary_expression';' postfix_expression scope
             | 'for' '('var_assignment';' binary_expression';' postfix_expression')' scope 
             | 'for' var_assignment';' binary_expression';' postfix_expression scope
             | 'for' '('';' binary_expression';' postfix_expression ')' scope
             | 'for' ';' binary_expression';' postfix_expression';' scope
             | 'for' '('';'';'')' scope
             | 'for' ';'';' scope
             | 'for' scope
             ;
foreach_statement: ('for'|'foreach') '('((TYPE | IDENTIFIER) IDENTIFIER (',' (TYPE | IDENTIFIER)? IDENTIFIER)?) 'in' expression')' scope
                 | ('for' | 'foreach') ((TYPE | IDENTIFIER) IDENTIFIER (',' (TYPE | IDENTIFIER)? IDENTIFIER)?) 'in' expression scope
                 ;

while_statement: 'while' '('expressions')' scope
               | 'while' expressions scope
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
internal_macro: INTERNAL_MACRO_BODY ';'?;

// Lexer Section:
SINGLE_LINE_COMMENT : '//' ~[\r\n]* -> skip;
MULTI_LINE_COMMENT: '/*'.*? '*/' -> skip;
INTERNAL_MACRO_BODY : '#' (~[\r\n;]*);

OBJ_DECLARATION_KEYWORDS: 'class'
                        | 'struct'
                        ;



LITERAL: STRING_LITERAL | NUMERIC_LITERAL | BOOLEAN_LITERAL;
BOOLEAN_LITERAL : 'true' | 'false';

NUMERIC_LITERAL: [0-9]+POSTFIX_LITERAL_TYPE?
                | [0-9]+'.'[0-9]+POSTFIX_LITERAL_TYPE?
                ;
FORMATTED_STRING_LITERAL: '`' ('\\'. | ~[`\n\r\t] )*?   '`';
STRING_LITERAL: '"' ('\\'. | ~["\n\r\t] )*? '"'
              | '\'' ('\\'. | ~['\n\r\t] )*? '\''
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

IDENTIFIER: [a-zA-Z_]+[a-zA-Z_0-9]?;

WS: [ \r\n\t]+ -> skip;