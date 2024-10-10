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

# STARTING RULES
# ==============
# They define 3 starting rules, but i do not know if it applies to our case

def p_file(p):
    """file : statements ENDMARKER
    """

# GENERAL STATEMENTS
# ==================

# statements: statement+  (One or more statement)
def p_statements(p):
    """statements : statements statement
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
    """simple_stmts : simple_stmts SEMICOLON simple_stmt
                    | simple_stmt NEWLINE
    """

# REMOVED: Assert_stmt
# SIMPLE STATEMENTS
def p_simple_stmt(p):
    """simple_stmt : assignment
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

# TODO: Check this later
def p_assignment(p):
    """assignment : IDENTIFIER augmentation_assignment expressions
                  | IDENTIFIER augmentation_assignment IDENTIFIER
    """
    print('assignment rule - verified!')

# augassign
def p_augmentation_assignment(p):
    """augmentation_assignment : SUM_ASSIGNMENT
                               | SUBTRACTION_ASSIGNMENT
                               | PRODUCT_ASSIGNMENT
                               | DIVISION_ASSIGNMENT
                               | MODULUS_ASSIGNMENT
                               | EXPONENTIATION_ASSIGNMENT
                               | INTEGER_DIVISION_ASSIGNMENT
    """
    print('augmentation_assignment rule - verified!')

# return_stmt:
#     | 'return' [star_expressions] 
def p_return_stmt(p):
    """return_stmt : RETURN expressions
    """

def p_global_stmt(p):
    """global_stmt : GLOBAL IDENTIFIER namelist
    """

def p_del_stmt(p):
    """del_stmt : DEL IDENTIFIER COMMA namelist
    """

def p_namelist(p):
    """namelist : COMMA IDENTIFIER namelist
                | empty
    """

# COMPOUND STATEMENTS
# ===================

# Common elements
# ---------------
def p_block(p):
    """block : NEWLINE INDENT statements DEDENT
             | simple_stmts
    """
    print('block rule - verified!')

# Class definitions
# -----------------

def p_class_def(p):
    """class_def : CLASS IDENTIFIER COLON block
                 | CLASS IDENTIFIER L_PARENTHESIS R_PARENTHESIS COLON block
                 | CLASS IDENTIFIER L_PARENTHESIS arguments R_PARENTHESIS COLON block   
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
#TODO: This need to be changed
def p_parameters(p):
    """parameters : IDENTIFIER COMMA parameters
                  | IDENTIFIER
    """
    print('parameters rule - verified!')

# If statement
# ------------
# if_stmt:
#     | 'if' named_expression ':' block elif_stmt 
#     | 'if' named_expression ':' block [else_block] 
def p_if_stmt(p):
    """if_stmt : IF named_expression COLON block elif_stmt
               | IF named_expression COLON block else_block
               | IF named_expression COLON block
    """


# elif_stmt:
#     | 'elif' named_expression ':' block elif_stmt 
#     | 'elif' named_expression ':' block [else_block]  
def p_elif_stmt(p):
    """elif_stmt : ELIF named_expression COLON block elif_stmt
                 | ELIF named_expression COLON block else_block
                 | ELIF named_expression COLON block
    """

def p_else_block(p):
    """else_block : ELSE COLON block
    """


# while_stmt:
#     | 'while' named_expression ':' block [else_block]
def p_while_stmt(p):
    """while_stmt : WHILE named_expression COLON block else_block
                  | WHILE named_expression COLON block
    """
    print('while_stmt rule - verified!')

# for_stmt:
#     | 'for' star_targets 'in' ~ star_expressions ':' [TYPE_COMMENT] block [else_block] 
#     | ASYNC 'for' star_targets 'in' ~ star_expressions ':' [TYPE_COMMENT] block [else_block] 
# TODO: Add Range() function.
def p_for_stmt(p):
    """for_stmt : FOR IDENTIFIER IN expressions COLON else_block
                | FOR IDENTIFIER IN expressions COLON block
    """
    print('for_stmt rule - verified!')

# EXPRESSIONS
# ===================

def p_expressions(p):
    """expressions : expression 
                   | expression COMMA
                   | expression COMMA expressions
    """

def p_expression(p):
    """expression : disjunction
                  | disjunction IF disjunction ELSE expression
    """

#TODO: Check if this works
def p_disjunction(p):
    """disjunction : conjunction 
                   | conjunction OR disjunction
    """

#TODO: Check if this works
def p_conjunction(p):
    """conjunction : inversion 
                   | inversion AND inversion
    """

def p_inversion(p):
    """inversion : NOT inversion 
                 | comparison
    """

#TODO: Ask if we will use assignment expression ":="
def p_named_expression(p):
    """named_expression : expression
    """

# COMPARISON OPERATORS
# =======================

def p_comparison(p):
    """comparison : bitwise_or 
                  | bitwise_or compare_op_list
    """

def p_compare_op_list(p):
    """compare_op_list : compare_op 
                       | compare_op compare_op_list
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
    """bitwise_or : bitwise_or BITWISE_OR bitwise_xor 
                  | bitwise_xor 
    """

#TODO: XOR is not available in the tokens, should be added
def p_bitwise_xor(p):
    """bitwise_xor : bitwise_xor empty bitwise_and 
                   | bitwise_and
    """

def p_bitwise_and(p):
    """bitwise_and : bitwise_and t_BITWISE_AND shift_expr 
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
    """sum : sum SUM term
           | sum SUBTRACTION term
           | term
    """


def p_term(p):
    """term : term PRODUCT factor 
            | term DIVISION factor 
            | term INTEGER_DIVISION factor 
            | term MODULUS factor 
            | term '@' factor 
            | factor
    """

#TODO: Check if '~' is nedded.
def p_factor(p):
    """factor : SUM factor 
              | SUBTRACTION factor 
              | power
    """

#TODO: Implement this
def p_power(p):
    """power : empty
    """
# FUNCTION CALL ARGUMENTS
# =======================

#TODO: Check how this should work
def p_arguments(p):
    """arguments :
    """


def p_empty(p):
    'empty :'
    pass

# ========================= END OF THE GRAMMAR ===========================

def p_error(p):
    if p:
        error_msg = f"Syntax Error near '{p.value}' in line {p.lineno}"
        print(error_msg)
        raise SyntaxError(error_msg)
    else:
        print("Syntax error at EOF")
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