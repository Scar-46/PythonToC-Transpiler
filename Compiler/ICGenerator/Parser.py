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

    ('left', 'LESSER', 'LESSER_EQUAL', 'GREATER', 'GREATER_EQUAL', 'EQUALITY'),

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
    p[0] = p[1]


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
                p[0] = Node("statements", children=p[1].children + [p[2]])
            else: # Ignore NEWLINE
                p[0] = p[1]
    else:  # Single statement
        p[0] = Node("statements", children=[p[1]])
    

def p_statement(p):
    """statement : compound_stmt
                 | simple_stmts
    """
    p[0] = p[1]
    

def p_simple_stmts(p):
    """simple_stmts : simple_stmts SEMICOLON simple_stmt NEWLINE
                    | simple_stmt NEWLINE
                    | simple_stmt
    """
    if len(p) == 5:
        p[0] = p[1]
        p[0].add_child(p[3])
    elif len(p) <= 3:
        p[0] = p[1]


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
    if not isinstance(p[1], Node):
        p[0] = Node('simple_stmt', value=p[1])
    else:
        p[0] = Node('simple_stmt', children=[p[1]])


def p_compound_stmt(p):
    """compound_stmt : function_def
                     | if_stmt
                     | class_def
                     | for_stmt
                     | while_stmt
    """
    p[0] = p[1]

def p_targets(p):
    """targets : targets COMMA primary
               | primary COMMA primary
    """
    if p[1].node_type == "target_list":
        targets=p[1].children + [p[3]]
    else:
        targets=[p[1], p[3]]

    p[0] = Node("target_list", children=targets)

def p_target_assignment_chain(p):
    """target_assignment_chain : target_assignment_chain targets ASSIGNMENT
                               | target_assignment_chain primary ASSIGNMENT
                               | targets ASSIGNMENT
                               | primary ASSIGNMENT
    """
    if len(p) == 4:
        targets = p[1].children

        if p[2].node_type == "target_list":
            targets += [p[2]]
        else:
            targets += [Node('target_list', children=[p[2]])]

    elif len(p) == 3:
        if p[1].node_type == "target_list":
            targets = [p[1]]
        else:
            targets = [Node('target_list', children=[p[1]])]

    p[0] = Node("target_chain", children=targets)

# SIMPLE STATEMENTS
# =================
def p_assignment(p):
    """assignment : L_PARENTHESIS primary R_PARENTHESIS ASSIGNMENT expressions
                  | primary augmentation_assignment expressions
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
def p_block(p): #TODO: This can be simplify
    """block : NEWLINE INDENT statements DEDENT
             | simple_stmts
    """
    if len(p) == 5:  # Indented block
        p[0] = Node("block", children=p[3].children)
    else:  # Simple statements
        p[0] = Node("block", children=p[1].children)


# Class definitions
# -----------------
def p_class_def(p):
    """class_def : CLASS IDENTIFIER L_PARENTHESIS expression R_PARENTHESIS COLON block  
                 | CLASS IDENTIFIER L_PARENTHESIS R_PARENTHESIS COLON block
                 | CLASS IDENTIFIER COLON block
    """
    if len(p) == 8:  # Class with expressions
        p[0] = Node("class_def",  value=p[2], children=[p[4], p[7]])
    elif len(p) == 7:  # Class with empty parentheses
        p[0] = Node("class_def",  value=p[2], children=[p[6]])
    else:  # Class without parentheses
        p[0] = Node("class_def",  value=p[2], children=[p[4]])


def p_function_def(p):
    """function_def : DEF IDENTIFIER L_PARENTHESIS parameters R_PARENTHESIS COLON block
                    | DEF IDENTIFIER L_PARENTHESIS R_PARENTHESIS COLON block
    """
    
    if len(p) == 8:  # Function with parameters
        p[0] = Node("function_def", value=p[2], children=[p[4], p[7]])
    else:  # Function without parameters
        p[0] = Node("function_def", value=p[2], children=[p[6]])


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
                  | FOR primary IN expressions COLON block
    """
    if p[2].node_type == "target_list":
        target_list = p[2]
    else:
        target_list = Node("target_list", children=[p[2]])

    p[0] = Node("for_stmt", children=[target_list, p[4], p[6]])

# EXPRESSIONS
# ===================
def p_expressions(p):
    """expressions : expressions COMMA expression
                   | expression
    """
    if len(p) == 4:  # expressions , expression
        if p[1].node_type == "expressions":
            p[0] = Node("expressions", children=p[1].children + [p[3]])
        else:
            p[0] = Node("expressions", children=[p[1], p[3]])
    else:  # expression
        p[0] = p[1]


def p_expression(p):
    """expression : disjunction IF disjunction ELSE expression
                  | disjunction
    """
    if len(p) == 6:  # disjunction IF disjunction ELSE expression
        # Ternary
        p[0] = Node("ternary", children=[p[1], p[3], p[5]])
    else:  # disjunction
        p[0] = p[1]


def p_disjunction(p):
    """disjunction : disjunction OR conjunction
                   | conjunction 
    """
    if len(p) == 4:  # disjunction OR conjunction
        # p[0] = Node("disjunction", children=[p[1], p[3]], value="OR")
        p[0] = Node("logical_op", children=[p[1], p[3]], value=p[2])
    else:  # conjunction
        p[0] = p[1]


def p_conjunction(p):
    """conjunction : conjunction AND inversion
                   | inversion
    """
    if len(p) == 4:  # conjunction AND inversion
        # p[0] = Node("conjunction", children=[p[1], p[3]], value="AND")
        p[0] = Node("logical_op", children=[p[1], p[3]], value=p[2])
    else:  # inversion
        p[0] = p[1]


def p_inversion(p):
    """inversion : NOT inversion 
                 | comparison
    """
    if len(p) == 3:  # NOT inversion
        p[0] = Node("inversion", children=[p[2]], value=p[1])
    else:  # comparison
        p[0] = p[1]


# COMPARISON OPERATORS
# =======================

def p_comparison(p):
    """comparison : bitwise_or compare_op_list
                  | bitwise_or
    """
    if len(p) == 3:  # bitwise_or compare_op_list
        p[0] = Node("comparison", children=[p[1], p[2]])
    else:  # bitwise_or
        p[0] = p[1]


def p_compare_op_list(p):
    """compare_op_list : compare_op_list compare_op
                       | compare_op 
    """
    if len(p) == 3:  # compare_op_list compare_op
        if p[1].node_type == "compare_op_list":
            p[0] = Node("compare_op_list", children=p[1].children + [p[2]])
        else:
            p[0] = Node("compare_op_list", children=[p[1], p[2]])
    else:  # compare_op
        p[0] = p[1]


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
    p[0] = Node("compare_op", value=p[1], children=[p[2]])


# BITWISE OPERATORS
# =======================

def p_bitwise_or(p):
    """bitwise_or : bitwise_or PIPE bitwise_xor 
                  | bitwise_xor 
    """
    if len(p) == 4:  # bitwise_or | bitwise_xor
        p[0] = Node("bitwise_or", children=[p[1], p[3]], value="|")
    else:  # bitwise_xor
        p[0] = p[1]


def p_bitwise_xor(p):
    """bitwise_xor : bitwise_xor CARET bitwise_and 
                   | bitwise_and
    """
    if len(p) == 4:  # bitwise_xor ^ bitwise_and
        p[0] = Node("bitwise_xor", children=[p[1], p[3]], value="^")
    else:  # bitwise_and
        p[0] = p[1]


def p_bitwise_and(p):
    """bitwise_and : bitwise_and AMPERSAND shift_expr 
                   | shift_expr
    """
    if len(p) == 4:  # bitwise_and & shift_expr
        p[0] = Node("bitwise_and", children=[p[1], p[3]], value="&")
    else:  # shift_expr
        p[0] = p[1]


def p_shift_expr(p):
    """shift_expr : shift_expr L_SHIFT sum
                  | shift_expr R_SHIFT sum
                  | sum
    """
    if len(p) == 4:  # shift_expr << sum or shift_expr >> sum
        operator = "<<" if p[2] == "<<" else ">>"
        p[0] = Node("shift_expr", children=[p[1], p[3]], value=operator)
    else:  # sum
        p[0] = p[1]


# ARITHMETIC OPERATORS
# =======================
def p_sum(p):
    """sum : sum PLUS term
           | sum MINUS term
           | term
    """
    if len(p) == 4:
        p[0] = Node("binary_operation", value=p[2], children=[p[1], p[3]])
    else:
        p[0] = p[1]

def p_term(p):
    """term : term STAR factor 
            | term SLASH factor 
            | term DOUBLE_SLASH factor 
            | term PERCENT factor
            | factor
    """
    if len(p) == 4:
        p[0] = Node("binary_operation", value=p[2], children=[p[1], p[3]])
    else:
        p[0] = p[1]

def p_factor(p):
    """factor : PLUS factor 
              | MINUS factor 
              | power
    """
    if len(p) == 3:
        p[0] = Node("unary_operation", value=p[1], children=[p[2]])
    else:
        p[0] = p[1]

def p_power(p):
    """power : primary DOUBLE_STAR factor
             | primary
    """
    if len(p) == 4:
        p[0] = Node("binary_operation", value='**', children=[p[1], p[3]])
    else:
        p[0] = p[1]

# PRIMARY ELEMENTS
# =======================

# primary:
def p_primary(p): #TODO: Simplify this
    """primary : primary L_PARENTHESIS expressions R_PARENTHESIS
               | primary L_PARENTHESIS R_PARENTHESIS
               | primary L_SQB slices R_SQB
               | primary L_SQB expression R_SQB
               | primary DOT IDENTIFIER
               | atomic
    """
    # Function call: primary ( expressions )
    if len(p) == 5 and p[2] == '(' and p[4] == ')':
        p[0] = Node("function_call", children=[p[1], p[3]])
    elif len(p) == 4 and p[2] == '(' and p[3] == ')':
        p[0] = Node("function_call", children=[p[1]])
    elif len(p) == 4 and p[2] == '.':
        p[0] = Node("attribute_access", children=[p[1]], value=p[3])
    elif len(p) == 5 and p[2] == '[' and p[4] == ']':
        if (p[3].node_type == 'expression'):
            slices = Node('slices', children=[Node('slice', children=p[3])])
        else:
            slices = p[3]
        p[0] = Node("subscript", children=[p[1], slices])
    else:
        p[0] = p[1]

# slices:
def p_slices(p):
    """slices : slices COMMA slice
              | slice
    """
    if len(p) == 4:
        if p[1].node_type == "slices":
            p[0] = Node("slices", children=p[1].children + [p[3]])
        else:
            p[0] = Node("slices", children=[p[1], p[3]])
    else: 
        p[0] = p[1]


def p_slice(p):
    """slice : expression slice
             | COLON expression
             | COLON slice
             | COLON
    """
    children = []
    
    for i in range(1, len(p)):
        # Add NON-COLON elements
        if p[i] != ':':
            children.append(p[i])

    p[0] = Node('slice', children=children)

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
    if p[1] == 'True' or p[1] == 'False' or p[1] == 'None':
        p[0] = Node('literal', value=p[1])
    # nested, number, and strings
    elif isinstance(p[1], Node):
        p[0] = p[1]
    # Identifier
    else:
        p[0] = Node('identifier', value=p[1])
        

def p_number(p):
    """number : NUMBER
              | F_NUMBER
              | BIN_NUMBER
              | HEX_NUMBER
              | OCT_NUMBER
    """
    p[0] = Node("number", value=p[1])

# LITERALS
# ========
def p_strings(p):
    """strings : STRING
               | TRIPLE_STRING
    """
    p[0] = Node("string", value=p[1])

# LIST, TUPLE, SET, AND DICTIONARY
# =======================
def p_list(p):
    """list : L_SQB expressions R_SQB
            | L_SQB R_SQB
    """
    if len(p) == 4:  
        p[0] = Node('list', children=[p[2]])
    else:
        p[0] = Node('list')

def p_tuple(p):
    """tuple : L_PARENTHESIS expression COMMA expressions R_PARENTHESIS
             | L_PARENTHESIS expression COMMA R_PARENTHESIS
             | L_PARENTHESIS R_PARENTHESIS
    """
    # L_PARENTHESIS expression COMMA expressions R_PARENTHESIS
    if len(p) == 6:
        p[0] = Node('tuple', children=[p[2], p[4]])
    # L_PARENTHESIS expression COMMA R_PARENTHESIS
    elif len(p) == 5:
        p[0] = Node('tuple', children=[p[2]])
    # L_PARENTHESIS R_PARENTHESIS
    else:
        p[0] = Node('tuple')

def p_group(p):
    """group : L_PARENTHESIS expression R_PARENTHESIS
    """
    p[0] = Node('group', children=[p[2]])

def p_set(p):
    """set : L_CB expressions R_CB
    """
    p[0] = Node('set', children=[p[2]])

# DICTIONARY
def p_dict(p):
    """dict : L_CB kvpairs R_CB
            | L_CB R_CB
    """
    if len(p) == 4:  
        p[0] = Node('dictionary', children=[p[2]])
    else:
        p[0] = Node('dictionary')

def p_kvpairs(p):
    """kvpairs : kvpairs COMMA kvpair
               | kvpairs COMMA
               | kvpair
    """
    if len(p) == 4:
        kvpairs = p[1].children + [p[3]]
    elif len(p) == 3:
        kvpairs = p[1].children
    else: 
        kvpairs = [p[1]]

    p[0] = Node("kvpairs", children=kvpairs)

def p_kvpair(p):
    """kvpair : expression COLON expression
    """
    p[0] = Node('key_value_pair', children=[p[1], p[3]])


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
            result = self._parser.parse(lexer=self._lexer, debug=False)
        except Exception as e:
            print("Error: ", e)
        return result