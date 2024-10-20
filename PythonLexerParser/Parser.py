from TokenRules import tokens
from Lexer import Lexer
import ply.yacc as yacc

# Python 3 grammar: https://docs.python.org/3/reference/grammar.html

# ========================= START OF THE GRAMMAR =========================

# General grammatical elements and rules:
#
# * Strings with double quotes (") denote SOFT KEYWORDS
# * Strings with single quotes (') denote KEYWORDS
# * Upper case names (NAME) denote tokens in the Grammar/Tokens file
# * Rule names starting with "invalid_" are used for specialized syntax errors
#     - These rules are NOT used in the first pass of the parser.
#     - Only if the first pass fails to parse, a second pass including the invalid
#       rules will be executed.
#     - If the parser fails in the second phase with a generic syntax error, the
#       location of the generic failure of the first pass will be used (this avoids
#       reporting incorrect locations due to the invalid rules).
#     - The order of the alternatives involving invalid rules matter
#       (like any rule in PEG).
#
# Grammar Syntax (see PEP 617 for more information):
#
# rule_name: expression
#   Optionally, a type can be included right after the rule name, which
#   specifies the return type of the C or Python function corresponding to the
#   rule:
# rule_name[return_type]: expression
#   If the return type is omitted, then a void * is returned in C and an Any in
#   Python.
# e1 e2
#   Match e1, then match e2.
# e1 | e2
#   Match e1 or e2.
#   The first alternative can also appear on the line after the rule name for
#   formatting purposes. In that case, a | must be used before the first
#   alternative, like so:
#       rule_name[return_type]:
#            | first_alt
#            | second_alt
# ( e )
#   Match e (allows also to use other operators in the group like '(e)*')
# [ e ] or e?
#   Optionally match e.
# e*
#   Match zero or more occurrences of e.
# e+
#   Match one or more occurrences of e.
# s.e+
#   Match one or more occurrences of e, separated by s. The generated parse tree
#   does not include the separator. This is otherwise identical to (e (s e)*).
# &e
#   Succeed if e can be parsed, without consuming any input.
# !e
#   Fail if e can be parsed, without consuming any input.
# ~
#   Commit to the current alternative, even if it fails to parse.
#

# precedence = (
#     ('left', 'PLUS', 'MINUS'),
#     ('left', 'TIMES', 'DIVIDE'),
# )

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

# TODO: TARGET CAN BE TOO WIDE FOR THE ASSIGNMENT!!
def p_assignment(p):
    """assignment : targets augmentation_assignment expressions
                  | targets ASSIGNMENT assignment
                  | targets ASSIGNMENT expressions
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
    """class_def : CLASS IDENTIFIER L_PARENTHESIS arguments R_PARENTHESIS COLON block  
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
    """while_stmt : WHILE expression COLON block else_block
                  | WHILE expression COLON block
    """

# for_stmt:
#     | 'for' star_targets 'in' ~ star_expressions
# TODO: Add Range() function, and fix the IDENTIFIER, and expresions.
def p_for_stmt(p):
    """for_stmt : FOR targets IN expressions COLON else_block
                | FOR targets IN expressions COLON block
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
    """disjunction : conjunction OR disjunction
                   | conjunction 
    """

def p_conjunction(p):
    """conjunction : inversion AND inversion
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
#     | primary '.' NAME 
#     | primary genexp 
#     | primary '(' [arguments] ')' 
#     | primary '[' slices ']' 
#     | atom
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
             | expression COLON expression
             | expression COLON
             | COLON expression
             | COLON COLON
             | COLON
             | expression
    """

#NOTE: MAY HAVE SOME ISSUES OF PRECEDENCE SINCE DICT AND SET ARE SIMILAR
#NOTE: ERROR: Infinite recursion detected for symbol 'dict'
#      ERROR: Infinite recursion detected for symbol 'set'
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
#TODO: Check how this should work
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
    """kvpairs : kvpair COMMA kvpairs
               | kvpair COMMA
               | kvpair
               
    """

def p_kvpair(p):
    """kvpair : expression COLON expression
    """

# ASSIGNMENT TARGETS
# ==================
def p_targets(p):
    """targets : primary
    """

# TODO: THIS SHOULD BE CHANGE!!
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
    def __init__(self, lexer=None):
        if lexer is None:
            lexer = Lexer()
        self.lexer = lexer
        self.parser = yacc.yacc(start="file", debug=True)

    def parse(self, code):
        self.lexer.input(code)
        result = self.parser.parse(lexer=self.lexer, debug=True)
        return result