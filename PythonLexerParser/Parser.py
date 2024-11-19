from TokenRules import tokens
from Lexer import Lexer
import ply.yacc as yacc
from common import log_error
from node import Node

# Based on PEG grammar for Python
# Python 3 grammar: https://docs.python.org/3/reference/grammar.html

precedence = (
    # Lowest precedence
    ('left', 'OR'),
    ('left', 'AND'),
    ('left', 'NOT'),

    ('left', 'LESSER', 'LESSER_EQUAL', 'GREATER', 'GREATER_EQUAL', 'EQUALITY', 'NOT'),

    ('left', 'PIPE'),               # Bitwise OR: |
    ('left', 'CARET'),              # Bitwise XOR: ^
    ('left', 'AMPERSAND'),          # Bitwise AND: &

    ('left', 'PLUS', 'MINUS',
     'ADDITION_ASSIGNMENT', 'SUBTRACTION_ASSIGNMENT'),

    ('left', 'STAR', 'SLASH', 'PERCENT', 'DOUBLE_SLASH',
     'MULTIPLICATION_ASSIGNMENT', 'DIVISION_ASSIGNMENT', 'MODULO_ASSIGNMENT', 'FLOOR_DIVISION_ASSIGNMENT'),
    

    ('right', 'DOUBLE_STAR', 'EXPONENTIATION_ASSIGNMENT'),

    ('left', 'L_PARENTHESIS', 'L_SQB', 'DOT'),

    ('right', 'ASSIGNMENT')
)


# STARTING RULES
# ==============
# They define 3 starting rules, but i do not know if it applies to our case

def p_file(p):
    """file : statements ENDMARKER
            | ENDMARKER
    """

# GENERAL STATEMENTS
# ==================

# statements: statement+  (One or more statement)
def p_statements(p):
    """statements : statements statement
                  | statements NEWLINE
                  | statement
    """

# statement: simple_stmts | compound_stmt
def p_statement(p):
    """statement : compound_stmt
                 | simple_stmts
    """

# simple_stmts:
#     | simple_stmt !';' NEWLINE  # Not needed, there for speedup
#     | ';'.simple_stmt+ [';'] NEWLINE 
# TODO: Check recursion
def p_simple_stmts(p):
    """simple_stmts : simple_stmts SEMICOLON simple_stmt NEWLINE
                    | simple_stmt NEWLINE
                    | simple_stmt
    """

# REMOVED: Assert_stmt
# SIMPLE STATEMENTS
def p_simple_stmt(p):
    """simple_stmt : assignment
                   | expressions
                   | return_stmt
                   | PASS
                   | del_stmt
                   | BREAK 
                   | CONTINUE 
                   | global_stmt
    """

def p_compound_stmt(p):
    """compound_stmt : function_def
                     | if_stmt
                     | class_def
                     | for_stmt
                     | while_stmt
    """

# REDESERVED KEYWORDS
# ================================================
# TARGETS
# ==================
# TODO: THIS SHOULD BE CHANGED, because implementations as asked for in python does not work properly!!
def p_target(p):
    """target : primary DOT IDENTIFIER
              | primary L_SQB slices R_SQB
              | primary
    """

def p_single_target(p):
    """single_target : single_subscript_attribute_target
                     | L_PARENTHESIS single_target R_PARENTHESIS
                     | IDENTIFIER
    """

def p_single_subscript_attribute_target(p):
    """single_subscript_attribute_target : primary DOT IDENTIFIER
                                         | primary L_SQB slices R_SQB
    """

def p_targets(p):
    """targets : targets COMMA target
               | target
    """

def p_target_assigment_list(p):
    """target_assigment_list : target_assigment_list COMMA single_target
                             | single_target
    """

def p_target_assignment_chain(p):
    """target_assignment_chain : target_assignment_chain target_assigment_list ASSIGNMENT
                               | target_assigment_list ASSIGNMENT"""

def p_target_tuple_seq(p):
    """target_tuple_seq : target_tuple_seq target
                        | target COMMA"""

def p_target_atomic(p):
    """target_atomic : L_PARENTHESIS targets R_PARENTHESIS
                     | L_SQB target_tuple_seq R_SQB
                     | L_PARENTHESIS R_PARENTHESIS
                     | L_SQB R_SQB
                     | IDENTIFIER
    """
# SIMPLE STATEMENTS
# =================
def p_assignment(p):
    """assignment : L_PARENTHESIS single_target R_PARENTHESIS ASSIGNMENT expressions
                  | single_target augmentation_assignment expressions
                  | target_assignment_chain expressions
    """

# augassign
def p_augmentation_assignment(p):
    """augmentation_assignment : ADDITION_ASSIGNMENT
                               | SUBTRACTION_ASSIGNMENT
                               | MULTIPLICATION_ASSIGNMENT
                               | DIVISION_ASSIGNMENT
                               | MODULO_ASSIGNMENT
                               | EXPONENTIATION_ASSIGNMENT
                               | FLOOR_DIVISION_ASSIGNMENT
    """

# return_stmt:
#     | 'return' [star_expressions] 
def p_return_stmt(p):
    """return_stmt : RETURN expressions
                   | RETURN
    """

def p_global_stmt(p):
    """global_stmt : GLOBAL namelist
    """

def p_del_stmt(p):
    """del_stmt : DEL namelist
    """

def p_namelist(p):
    """namelist : namelist COMMA IDENTIFIER
                | IDENTIFIER
    """
    if len(p) == 4:
        p[0] = p[1]
        p[0].add_child(p[3])
    else: 
        p[0] = Node('NameList', children=[p[1]])


# COMPOUND STATEMENTS
# ===================

# Common elements
# ---------------
def p_block(p):
    """block : NEWLINE INDENT statements DEDENT
             | simple_stmts
    """

# Class definitions
# -----------------
def p_class_def(p):
    """class_def : CLASS IDENTIFIER L_PARENTHESIS argument R_PARENTHESIS COLON block  
                 | CLASS IDENTIFIER L_PARENTHESIS R_PARENTHESIS COLON block
                 | CLASS IDENTIFIER COLON block
    """

def p_function_def(p):
    """function_def : DEF IDENTIFIER L_PARENTHESIS parameters R_PARENTHESIS COLON block
                    | DEF IDENTIFIER L_PARENTHESIS R_PARENTHESIS COLON block
    """

# parameters:
#     | slash_no_default param_no_default* param_with_default* [star_etc] 
#     | slash_with_default param_with_default* [star_etc] 
#     | param_no_default+ param_with_default* [star_etc] 
#     | param_with_default+ [star_etc] 
#     | star_etc 
#TODO: Add default parameters and I don't know if slash is needed 
#TODO: This needs to be changed
def p_parameters(p):
    """parameters : parameters COMMA IDENTIFIER
                  | IDENTIFIER
    """

# If statement
def p_if_stmt(p):
    """if_stmt : IF expression COLON block elif_stmt
               | IF expression COLON block else_block
               | IF expression COLON block
    """


# elif_stmt:
def p_elif_stmt(p):
    """elif_stmt : ELIF expression COLON block elif_stmt
                 | ELIF expression COLON block else_block
                 | ELIF expression COLON block
    """

def p_else_block(p):
    """else_block : ELSE COLON block
    """


# while_stmt:
def p_while_stmt(p):
    """while_stmt : WHILE expression COLON block
    """

# for_stmt:
#     | 'for' star_targets 'in' ~ star_expressions
# TODO: Add Range() function, and fix the IDENTIFIER, and expresions.
def p_for_stmt(p):
    """for_stmt : FOR targets IN expressions COLON block
    """

# EXPRESSIONS
# ===================
def p_expressions(p):
    """expressions : expressions COMMA expression
                   | expression
    """

def p_expression(p):
    """expression : disjunction IF disjunction ELSE expression
                  | disjunction
    """

def p_disjunction(p):
    """disjunction : disjunction OR conjunction
                   | conjunction 
    """

def p_conjunction(p):
    """conjunction : conjunction AND inversion
                   | inversion
    """

def p_inversion(p):
    """inversion : NOT inversion 
                 | comparison
    """

# COMPARISON OPERATORS
# =======================

def p_comparison(p):
    """comparison : bitwise_or compare_op_list
                  | bitwise_or
    """

def p_compare_op_list(p):
    """compare_op_list : compare_op_list compare_op
                       | compare_op 
    """

#NOTE: This can be changed
def p_compare_op(p):
    """compare_op : EQUALITY bitwise_or 
                  | INEQUALITY bitwise_or 
                  | GREATER_EQUAL bitwise_or 
                  | LESSER_EQUAL bitwise_or 
                  | GREATER bitwise_or 
                  | LESSER bitwise_or 
                  | NOT IN bitwise_or 
                  | IS NOT bitwise_or 
                  | IN bitwise_or 
                  | IS bitwise_or 
    """

# BITWISE OPERATORS
# =======================

def p_bitwise_or(p):
    """bitwise_or : bitwise_or PIPE bitwise_xor 
                  | bitwise_xor 
    """

#TODO: XOR is not available in the tokens, should be added
def p_bitwise_xor(p):
    """bitwise_xor : bitwise_xor CARET bitwise_and 
                   | bitwise_and
    """

def p_bitwise_and(p):
    """bitwise_and : bitwise_and AMPERSAND shift_expr 
                   | shift_expr
    """

def p_shift_expr(p):
    """shift_expr : shift_expr L_SHIFT sum
                  | shift_expr R_SHIFT sum
                  | sum
    """

# ARITHMETIC OPERATORS
# =======================
def p_sum(p):
    """sum : sum PLUS term
           | sum MINUS term
           | term
    """

def p_term(p):
    """term : term STAR factor 
            | term SLASH factor 
            | term DOUBLE_SLASH factor 
            | term PERCENT factor
            | factor
    """

def p_factor(p):
    """factor : PLUS factor 
              | MINUS factor 
              | power
    """

def p_power(p):
    """power : primary DOUBLE_STAR factor
             | primary
    """

# PRIMARY ELEMENTS
# =======================

# primary:
def p_primary(p):
    """primary : primary L_PARENTHESIS arguments R_PARENTHESIS
               | primary L_PARENTHESIS R_PARENTHESIS
               | primary L_SQB slices R_SQB
               | primary DOT IDENTIFIER
               | atomic
    """
    # Function call: primary ( arguments )
    if len(p) == 5 and p[2] == '(' and p[4] == ')':
        p[0] = Node('FunctionCall')
        p[0].add_child(p[1])
        p[0].add_child(p[3])
    # Attribute access: primary . IDENTIFIER
    elif len(p) == 4 and p[2] == '.':
        p[0] = Node('AttributeAccess', value=p[3])
        p[0].add_child(p[1])
    # Subscript/slice: primary [ slices ]
    elif len(p) == 5 and p[2] == '[' and p[4] == ']':
        p[0] = Node('Subscript')
        p[0].add_child(p[1])
        p[0].add_child(p[3])
    # Atomic case
    else:
        p[0] = p[1]

# slices:
def p_slices(p):
    """slices : slices COMMA slice
              | slice
    """
    if len(p) == 4:
        p[0] = Node('Slices', children=[p[1], p[3]])
    else:
        p[0] = Node('Slices', children=[p[1]])

# slice:
#     | [expression] ':' [expression] [':' [expression] ] 
#     | named_expression 
#TODO: This should be change
def p_slice(p):
    """slice : expression COLON expression COLON expression
             | COLON expression COLON expression
             | expression COLON COLON expression
             | COLON expression COLON
             | COLON COLON expression
             | expression COLON COLON
             | COLON COLON
             | expression
             | COLON
    """
    children = []
    
    for i in range(1, len(p)):
        # Add NON-COLON elements
        if p[i] != 'COLON':
            children.append(p[i])

    p[0] = Node('Slice', children=children)

def p_atomic(p):
    """atomic : IDENTIFIER
              | TRUE
              | FALSE
              | NONE
              | strings
              | number
              | tuple
              | group
              | list
              | dict
              | set
    """
    # Literales
    if p[1] == 'TRUE' or p[1] == 'FALSE' or p[1] == 'NONE':
        p[0] = Node('Literal', value=p[1])
    # Identifier
    elif p[1] == 'IDENTIFIER':
        p[0] = Node('Identifier', value=p[1])
    # nested, number, and strings
    else:
        p[0] = p[1]

def p_number(p):
    """number : NUMBER
              | F_NUMBER
              | BIN_NUMBER
              | HEX_NUMBER
              | OCT_NUMBER
    """
    p[0] = Node("Number", value=p[1])


# FUNCTION CALL ARGUMENTS
# =======================
def p_argument(p):
    """argument : expression"""

#TODO: Check how this should work may want to include keyword arguments
def p_arguments(p):
    """arguments : expressions
    """

# LITERALS
# ========
def p_strings(p):
    """strings : STRING
               | TRIPLE_STRING
    """
    p[0] = Node("String", value=p[1])

# LIST, TUPLE, SET, AND DICTIONARY
# =======================
def p_list(p):
    """list : L_SQB expressions R_SQB
            | L_SQB R_SQB
    """

def p_tuple(p):
    """tuple : L_PARENTHESIS expression COMMA expressions R_PARENTHESIS
             | L_PARENTHESIS expression COMMA R_PARENTHESIS
             | L_PARENTHESIS R_PARENTHESIS
    """
    # L_PARENTHESIS expression COMMA expressions R_PARENTHESIS
    if len(p) == 6:
        p[0] = Node('Tuple', children=[p[2], p[4]])
    # L_PARENTHESIS expression COMMA R_PARENTHESIS
    elif len(p) == 5:
        p[0] = Node('Tuple', children=[p[2]])
    # L_PARENTHESIS R_PARENTHESIS
    else:
        p[0] = Node('Tuple')

def p_group(p):
    """group : L_PARENTHESIS expression R_PARENTHESIS
    """
    p[0] = Node('Group', children=[p[2]])

def p_set(p):
    """set : L_CB expressions R_CB
    """
    p[0] = Node('Set', children=[p[2]])

# DICTIONARY
def p_dict(p):
    """dict : L_CB kvpairs R_CB
            | L_CB R_CB
    """
    if len(p) == 4:  
        p[0] = Node('Dictionary', children=[p[2]])
    else:
        p[0] = Node('Dictionary')

def p_kvpairs(p):
    """kvpairs : kvpair_list COMMA
               | kvpair_list
    """
    p[0] = p[1] 

def p_kvpair(p):
    """kvpair : expression COLON expression
    """
    p[0] = Node('KeyValuePair', children=[p[1], p[3]])

def p_kvpair_list(p):
    """kvpair_list : kvpair_list COMMA kvpair
                   | kvpair
    """
    if len(p) == 4:
        p[0] = p[1]
        p[0].add_child(p[3])
    else: 
        p[0] = Node('KeyValuePairList', children=[p[1]])
    
    
def p_empty(p):
    'empty :'
    pass

# ========================= END OF THE GRAMMAR ===========================

def p_error(token):
    if token:
        error_msg = f"Syntax Error near '{token.value if token.value else token.type}'"
    else:
        error_msg = "Syntax error at EOF"
    log_error(message=error_msg, type="syntax", token=token)
    
class Parser(object):
    def __init__(self, lexer=None):
        if lexer is None:
            lexer = Lexer()
        self._lexer = lexer
        self._parser = yacc.yacc(start="file", debug=True)

    def parse(self, code):
        result = None
        try:
            self._lexer.input(code)
            result = self._parser.parse(lexer=self._lexer, debug=True)
        except Exception as e:
            # TODO: This should be unreachable
            print("Error: ", e)
        return result