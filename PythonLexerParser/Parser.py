from TokenRules import tokens
from Lexer import Lexer
import ply.yacc as yacc
from common import log_error
import Node as Node
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
    if len(p) == 3:
            if isinstance(p[2], Node):
                p[0] = p[1]
                p[0].add_child(p[2])
            else: # Ignore NEWLINE
                p[0] = p[1]
    else:  # Single statement
        p[0] = Node("statements", children=[p[1]])
    

def p_statement(p):
    """statement : compound_stmt
                 | simple_stmts
    """
    p[0] = Node("statement", children=[p[1]])
    

def p_simple_stmts(p):
    """simple_stmts : simple_stmts SEMICOLON simple_stmt NEWLINE
                    | simple_stmt NEWLINE
                    | simple_stmt
    """
    if len(p) == 5:
        p[0] = p[1]
        p[0].add_child(p[3])
    elif len(p) <= 3: # Ignore NEWLINE
        p[0] = Node("simple_stmts", children=[p[1]])


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
    if isinstance(p[1], Node):
        p[0] = p[1]
    else: # Handling reserved keywords like PASS, BREAK, CONTINUE
        p[0] = Node("simple_stmt", value=p[1])


def p_compound_stmt(p):
    """compound_stmt : function_def
                     | if_stmt
                     | class_def
                     | for_stmt
                     | while_stmt
    """
    p[0] = p[1]


# SIMPLE STATEMENTS
# =================
def p_assignment(p):
    """assignment : L_PARENTHESIS single_target R_PARENTHESIS ASSIGNMENT expressions
                  | single_target augmentation_assignment expressions
                  | target_assignment_chain expressions
    """
    if len(p) == 6:  # Parenthesized assignment
        p[0] = Node("assign", children=[p[2], p[5]])
    elif len(p) == 4:  # Augmented assignment
        p[0] = Node("aug_assign", children=[p[1], p[2], p[3]])
    else:  # Chained assignment
        p[0] = Node("assign_chain", children=[p[1], p[2]])


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
    p[0] = Node("aug_operator", value=p[1])  # Store the operator as a value


def p_return_stmt(p):
    """return_stmt : RETURN expressions
                   | RETURN
    """
    if len(p) == 3:  # With expression
        p[0] = Node("return", children=[p[2]])
    else:  # Empty return
        p[0] = Node("return")

    
def p_global_stmt(p):
    """global_stmt : GLOBAL namelist
    """
    p[0] = Node("global", children=[p[2]])


def p_del_stmt(p):
    """del_stmt : DEL namelist
    """
    p[0] = Node("del", children=[p[2]])


def p_namelist(p):
    """namelist : namelist COMMA IDENTIFIER
                | IDENTIFIER
    """
    if len(p) == 4:  # Multiple identifiers
        p[0] = Node("namelist", children=p[1].children + [Node("identifier", value=p[3])])
    else:  # Single identifier
        p[0] = Node("namelist", children=[Node("identifier", value=p[1])])

# COMPOUND STATEMENTS
# ===================

# Common elements
# ---------------
def p_block(p):
    """block : NEWLINE INDENT statements DEDENT
             | simple_stmts
    """
    if len(p) == 5:  # Indented block
        p[0] = Node("block", children=p[3])
    else:  # Simple statements
        p[0] = Node("block", children=[p[1]])


# Class definitions
# -----------------
def p_class_def(p):
    """class_def : CLASS IDENTIFIER L_PARENTHESIS argument R_PARENTHESIS COLON block  
                 | CLASS IDENTIFIER L_PARENTHESIS R_PARENTHESIS COLON block
                 | CLASS IDENTIFIER COLON block
    """
    if len(p) == 8:  # Class with arguments
        p[0] = Node("class_def", children=[Node("identifier", value=p[2]), p[4], p[7]])
    elif len(p) == 7:  # Class with empty parentheses
        p[0] = Node("class_def", children=[Node("identifier", value=p[2]), p[6]])
    else:  # Class without parentheses
        p[0] = Node("class_def", children=[Node("identifier", value=p[2]), p[4]])


def p_function_def(p):
    """function_def : DEF IDENTIFIER L_PARENTHESIS parameters R_PARENTHESIS COLON block
                    | DEF IDENTIFIER L_PARENTHESIS R_PARENTHESIS COLON block
    """
    if len(p) == 8:  # Function with parameters
        p[0] = Node("function_def", children=[Node("identifier", value=p[2]), p[4], p[7]])
    else:  # Function without parameters
        p[0] = Node("function_def", children=[Node("identifier", value=p[2]), p[6]])



# parameters
def p_parameters(p):
    """parameters : parameters COMMA IDENTIFIER
                  | IDENTIFIER
    """
    if len(p) == 4:  # Multiple parameters
        p[0] = Node("parameters", children=p[1].children + [Node("identifier", value=p[3])])
    else:  # Single parameter
        p[0] = Node("parameters", children=[Node("identifier", value=p[1])])


# If statement
def p_if_stmt(p):
    """if_stmt : IF expression COLON block elif_stmt
               | IF expression COLON block else_block
               | IF expression COLON block
    """
    if len(p) == 6:  # If with elif or else block
        p[0] = Node("if_stmt", children=[p[2], p[4], p[5]])
    else:  # If without elif or else block
        p[0] = Node("if_stmt", children=[p[2], p[4]])


# elif_stmt
def p_elif_stmt(p):
    """elif_stmt : ELIF expression COLON block elif_stmt
                 | ELIF expression COLON block else_block
                 | ELIF expression COLON block
    """
    if len(p) == 6:  # Elif with next elif or else block
        p[0] = Node("elif_stmt", children=[p[2], p[4], p[5]])
    else:  # Elif without next elif or else block
        p[0] = Node("elif_stmt", children=[p[2], p[4]])


def p_else_block(p):
    """else_block : ELSE COLON block
    """
    p[0] = Node("else_block", children=[p[3]])


# while_stmt:
def p_while_stmt(p):
    """while_stmt : WHILE expression COLON block
    """
    p[0] = Node("while_stmt", children=[p[2], p[4]])


def p_for_stmt(p):
    """for_stmt : FOR targets IN expressions COLON block
    """
    p[0] = Node("for_stmt", children=[p[2], p[4], p[6]])

    
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

# slices:
def p_slices(p):
    """slices : slices COMMA slice
              | slice
    """

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

def p_number(p):
    """number : NUMBER
              | F_NUMBER
              | BIN_NUMBER
              | HEX_NUMBER
              | OCT_NUMBER
    """
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

def p_group(p):
    """group : L_PARENTHESIS expression R_PARENTHESIS
    """

def p_set(p):
    """set : L_CB expressions R_CB
    """

# DICTIONARY
def p_dict(p):
    """dict : L_CB kvpairs R_CB
            | L_CB R_CB
    """

def p_kvpairs(p):
    """kvpairs : kvpair_list COMMA
               | kvpair_list
    """

def p_kvpair(p):
    """kvpair : expression COLON expression
    """

def p_kvpair_list(p):
    """kvpair_list : kvpair_list COMMA kvpair
                   | kvpair
    """
    
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