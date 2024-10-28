from TokenRules import LexingError, tokens, find_column, get_input
from Lexer import Lexer, IndentationError
import ply.yacc as yacc

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

    ('left', 'PLUS', 'MINUS'),

    ('left', 'STAR', 'SLASH', 'PERCENT', 'DOUBLE_SLASH'),

    ('right', 'DOUBLE_STAR'),

    ('left', 'L_PARENTHESIS', 'L_SQB', 'DOT'),
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

# SIMPLE STATEMENTS
# =================

# TODO: Target_chain eats up more expressions than it should, since it uses primary instead of target_primary!!
def p_assignment(p):
    """assignment : target_chain augmentation_assignment expressions
                  | target_chain ASSIGNMENT expressions
    """
def p_target_chain(p):
    """target_chain : target_chain ASSIGNMENT targets
                    | targets"""
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
               | L_PARENTHESIS expression R_PARENTHESIS
               | primary L_PARENTHESIS R_PARENTHESIS
               | primary L_SQB slices R_SQB
               | primary DOT IDENTIFIER
               | atomic
    """

# slices:
#     | slice !',' 
#     | ','.(slice | starred_expression)+ [',']
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
# '[' [star_named_expressions] ']' 
def p_list(p):
    """list : L_SQB expressions R_SQB
            | L_SQB R_SQB
    """

# | '(' [star_named_expression ',' [star_named_expressions]  ] ')' 
def p_tuple(p):
    """tuple : L_PARENTHESIS expression COMMA expressions R_PARENTHESIS
             | L_PARENTHESIS expression COMMA R_PARENTHESIS
             | L_PARENTHESIS R_PARENTHESIS
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
# ASSIGNMENT TARGETS
# ==================
def p_targets(p):
    """targets : primary COMMA targets
               | primary
    """

# TODO: THIS SHOULD BE CHANGED, because implementations as asked for in python does not work properly!!
def p_target(p):
    """target : target_primary DOT IDENTIFIER
              | target_primary L_SQB slices R_SQB
              | target_atomic
    """

def p_target_primary(p):
    """target_primary : target_primary DOT IDENTIFIER
                      | target_primary L_SQB slices R_SQB
                      | target_primary L_PARENTHESIS R_PARENTHESIS
                      | target_primary L_PARENTHESIS arguments R_PARENTHESIS
                      | target_atomic
    """

def p_target_atomic(p):
    """target_atomic : IDENTIFIER
                     | L_SQB target_primary R_SQB
                     | L_PARENTHESIS targets R_PARENTHESIS
    """
    
def p_empty(p):
    'empty :'
    pass

# ========================= END OF THE GRAMMAR ===========================

def p_error(p):
    if p:
        error_msg = f"Syntax Error near '{p.value if p.value else p.type}' in line {p.lineno}"
        raise SyntaxError(error_msg)
    else:
        raise SyntaxError("Syntax error at EOF")
    
class Parser(object):
    def __init__(self, lexer=None, error_logger=None):
        if lexer is None:
            lexer = Lexer(error_logger == None)
        self.lexer = lexer
        self.error_logger = error_logger
        self.parser = yacc.yacc(start="file", debug=True)

    def parse(self, code):
        result = None
        try:
            self.lexer.input(code)
            result = self.parser.parse(lexer=self.lexer, debug=True)
        except LexingError as e:
            if not self.error_logger:
                raise e
            else:
               self.build_error(e, "lexing")
        except (IndentationError, SyntaxError) as e:
            if not self.error_logger:
                raise e
            else:
               self.build_error(e, "syntax")
        except Exception as e:
            if not self.error_logger:
                raise e
            else:
               self.build_error(e, "error")
        return result

    def build_error(self, error: Exception, error_type: str):
        assert len(error.args) > 1
        token = error.args[1]
        self.error_logger.log_error(
            error.args[0],
            token.lineno + 1,
            find_column(token.lexer.lexdata, token),
            get_input(token.lexer.lexdata, token),
            error_type
        )