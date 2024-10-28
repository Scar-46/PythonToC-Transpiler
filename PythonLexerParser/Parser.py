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

def p_file_error(p):
    """file : statements_error ENDMARKER
    """

# GENERAL STATEMENTS
# ==================

# statements: statement+  (One or more statement)
def p_statements(p):
    """statements : statements statement
                  | statements NEWLINE
                  | statement
    """

def p_statements_error(p):
    """statements_error : statements_error statement
                  | statements statement_error
                  | statements_error statement_error
                  | statements_error NEWLINE
                  | statement_error
                  | statements_error wild_error
                  | statements wild_error
    """

# statement: simple_stmts | compound_stmt
def p_statement(p):
    """statement : compound_stmt
                 | simple_stmts
    """

def p_statement_error(p):
    """statement_error : compound_stmt_error
                 | simple_stmts_error
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

def p_simple_stmts_error(p):
    """simple_stmts_error : simple_stmts_error SEMICOLON simple_stmt NEWLINE
                    | simple_stmts SEMICOLON simple_stmt_error NEWLINE
                    | simple_stmts_error SEMICOLON simple_stmt_error NEWLINE
                    | simple_stmt_error NEWLINE
                    | simple_stmt_error
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

def p_simple_stmt_error(p):
    """simple_stmt_error : assignment_error
                   | expressions_error
                   | return_stmt_error
                   | del_stmt_error
                   | global_stmt_error
    """

def p_compound_stmt(p):
    """compound_stmt : function_def
                     | if_stmt
                     | class_def
                     | for_stmt
                     | while_stmt
    """

def p_compound_stmt_error(p):
    """compound_stmt_error : function_def_error
                     | if_stmt_error
                     | class_def_error
                     | for_stmt_error
                     | while_stmt_error
    """

# def p_reserved_keyword_usage_error(p):
#     """reserved_keyword_usage_error : reserved_keyword error NEWLINE
#     """
#     print("Reseved keyword usage error")
#     raise SyntaxError(f"Invalid use of reserved keyword '{p.value}'", p)

# REDESERVED KEYWORDS
# ================================================
# s
# SIMPLE STATEMENTS
# =================

# TODO: Target_chain eats up more expressions than it should, since it uses primary instead of target_primary!!
def p_assignment(p):
    """assignment : target_chain augmentation_assignment expressions
                  | target_chain ASSIGNMENT expressions
    """

def p_assignment_error(p):
    """assignment_error : target_chain_error augmentation_assignment expressions
                  | target_chain augmentation_assignment expressions_error
                  | target_chain_error augmentation_assignment expressions_error
                  | target_chain_error ASSIGNMENT expressions
                  | target_chain ASSIGNMENT expressions_error
                  | target_chain_error ASSIGNMENT expressions_error
                  | target_chain_error augmentation_assignment wild_error
                  | target_chain augmentation_assignment wild_error
                  | target_chain_error ASSIGNMENT wild_error
                  | target_chain ASSIGNMENT wild_error
    """

def p_target_chain(p):
    """target_chain : target_chain ASSIGNMENT targets
                    | targets
    """

def p_target_chain_error(p):
    """target_chain_error : target_chain_error ASSIGNMENT targets
                    | target_chain ASSIGNMENT targets_error
                    | target_chain_error ASSIGNMENT targets_error
                    | targets_error
                    | target_chain_error ASSIGNMENT wild_error
                    | target_chain ASSIGNMENT wild_error
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

def p_return_stmt_error(p):
    """return_stmt_error : RETURN expressions_error
                | RETURN wild_error
    """

def p_global_stmt(p):
    """global_stmt : GLOBAL namelist
    """

def p_global_stmt_error(p):
    """global_stmt_error : GLOBAL namelist_error
                | GLOBAL wild_error
    """

def p_del_stmt(p):
    """del_stmt : DEL namelist
    """

def p_del_stmt_error(p):
    """del_stmt_error : DEL namelist_error
                | DEL wild_error
    """

def p_namelist(p):
    """namelist : namelist COMMA IDENTIFIER
                | IDENTIFIER
    """

# TODO: Check correctness
def p_namelist_error(p):
    """namelist_error : wild_error COMMA IDENTIFIER
    """

# COMPOUND STATEMENTS
# ===================

# Common elements
# ---------------
def p_block(p):
    """block : NEWLINE INDENT statements DEDENT
             | simple_stmts
    """

def p_block_error(p):
    """block_error : NEWLINE INDENT statements_error DEDENT
             | simple_stmts_error
    """

# Class definitions
# -----------------
def p_class_def(p):
    """class_def : CLASS IDENTIFIER L_PARENTHESIS argument R_PARENTHESIS COLON block  
                 | CLASS IDENTIFIER L_PARENTHESIS R_PARENTHESIS COLON block
                 | CLASS IDENTIFIER COLON block
    """

def p_class_def_error(p):
    """class_def_error : CLASS IDENTIFIER L_PARENTHESIS argument_error R_PARENTHESIS COLON block
                 | CLASS IDENTIFIER L_PARENTHESIS argument R_PARENTHESIS COLON block_error  
                 | CLASS IDENTIFIER L_PARENTHESIS argument_error R_PARENTHESIS COLON block_error  
                 | CLASS IDENTIFIER L_PARENTHESIS R_PARENTHESIS COLON block_error
                 | CLASS IDENTIFIER COLON block_error
                 | CLASS IDENTIFIER L_PARENTHESIS argument_error R_PARENTHESIS COLON wild_error
                 | CLASS IDENTIFIER L_PARENTHESIS argument R_PARENTHESIS COLON wild_error  
                 | CLASS IDENTIFIER L_PARENTHESIS R_PARENTHESIS COLON wild_error
                 | CLASS IDENTIFIER COLON wild_error
                 | CLASS IDENTIFIER wild_error
                 | CLASS wild_error
    """

def p_function_def(p):
    """function_def : DEF IDENTIFIER L_PARENTHESIS parameters R_PARENTHESIS COLON block
                    | DEF IDENTIFIER L_PARENTHESIS R_PARENTHESIS COLON block
    """

def p_function_def_error(p):
    """function_def_error : DEF IDENTIFIER L_PARENTHESIS parameters_error R_PARENTHESIS COLON block
                    | DEF IDENTIFIER L_PARENTHESIS parameters R_PARENTHESIS COLON block_error
                    | DEF IDENTIFIER L_PARENTHESIS parameters_error R_PARENTHESIS COLON block_error
                    | DEF IDENTIFIER L_PARENTHESIS R_PARENTHESIS COLON block_error
                    | DEF IDENTIFIER L_PARENTHESIS parameters_error R_PARENTHESIS COLON wild_error
                    | DEF IDENTIFIER L_PARENTHESIS parameters R_PARENTHESIS COLON wild_error
                    | DEF IDENTIFIER L_PARENTHESIS R_PARENTHESIS COLON wild_error
                    | DEF IDENTIFIER wild_error
                    | DEF wild_error
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

def p_parameters_error(p):
    """parameters_error : parameters_error COMMA IDENTIFIER
                  | IDENTIFIER
    """

# If statement
def p_if_stmt(p):
    """if_stmt : IF expression COLON block elif_stmt
               | IF expression COLON block else_block
               | IF expression COLON block
    """

def p_if_stmt_error(p):
    """if_stmt_error : IF expression_error COLON block elif_stmt
                 | IF expression_error COLON block_error elif_stmt
                 | IF expression_error COLON block_error elif_stmt_error
                 | IF expression COLON block_error elif_stmt
                 | IF expression COLON block_error elif_stmt_error
                 | IF expression COLON block elif_stmt_error
                 | IF expression_error COLON block elif_stmt_error
                 | IF expression_error COLON block else_block
                 | IF expression_error COLON block_error else_block
                 | IF expression_error COLON block_error else_block_error
                 | IF expression COLON block_error else_block
                 | IF expression COLON block_error else_block_error
                 | IF expression COLON block else_block_error
                 | IF expression_error COLON block else_block_error
                 | IF expression_error COLON block
                 | IF expression COLON block_error
                 | IF expression_error COLON block_error
                 | IF expression_error COLON block wild_error
                 | IF expression_error COLON block_error wild_error
                 | IF expression COLON block_error wild_error
                 | IF expression COLON block wild_error
                 | IF expression_error COLON wild_error
                 | IF expression COLON wild_error
    """

# elif_stmt:
def p_elif_stmt(p):
    """elif_stmt : ELIF expression COLON block elif_stmt
                 | ELIF expression COLON block else_block
                 | ELIF expression COLON block
    """

def p_elif_stmt_error(p):
    """elif_stmt_error : ELIF expression_error COLON block elif_stmt
                 | ELIF expression_error COLON block_error elif_stmt
                 | ELIF expression_error COLON block_error elif_stmt_error
                 | ELIF expression COLON block_error elif_stmt
                 | ELIF expression COLON block_error elif_stmt_error
                 | ELIF expression COLON block elif_stmt_error
                 | ELIF expression_error COLON block elif_stmt_error
                 | ELIF expression_error COLON block else_block
                 | ELIF expression_error COLON block_error else_block
                 | ELIF expression_error COLON block_error else_block_error
                 | ELIF expression COLON block_error else_block
                 | ELIF expression COLON block_error else_block_error
                 | ELIF expression COLON block else_block_error
                 | ELIF expression_error COLON block else_block_error
                 | ELIF expression_error COLON block
                 | ELIF expression COLON block_error
                 | ELIF expression_error COLON block_error
                 | ELIF expression_error COLON block wild_error
                 | ELIF expression_error COLON block_error wild_error
                 | ELIF expression COLON block_error wild_error
                 | ELIF expression COLON block wild_error
                 | ELIF expression_error COLON wild_error
                 | ELIF expression COLON wild_error
    """

def p_else_block(p):
    """else_block : ELSE COLON block
    """

def p_else_block_error(p):
    """else_block_error : ELSE COLON block_error
                   | ELSE COLON wild_error
    """

# while_stmt:
def p_while_stmt(p):
    """while_stmt : WHILE expression COLON block
    """

def p_while_stmt_error(p):
    """while_stmt_error : WHILE expression_error COLON block
                   | WHILE expression COLON block_error
                   | WHILE expression_error COLON block_error
                   | WHILE expression_error COLON wild_error
                   | WHILE expression COLON wild_error
    """

# for_stmt:
#     | 'for' star_targets 'in' ~ star_expressions
# TODO: Add Range() function, and fix the IDENTIFIER, and expresions.
def p_for_stmt(p):
    """for_stmt : FOR targets IN expressions COLON block
    """

def p_for_stmt_error(p):
    """for_stmt_error : FOR targets_error IN expressions COLON block
                   | FOR targets_error IN expressions_error COLON block
                   | FOR targets_error IN expressions_error COLON block_error
                   | FOR targets IN expressions_error COLON block
                   | FOR targets IN expressions_error COLON block_error
                   | FOR targets_error IN expressions COLON block_error
                   | FOR targets IN expressions COLON block_error
                   | FOR targets_error IN expressions COLON wild_error
                   | FOR targets_error IN expressions_error COLON wild_error
                   | FOR targets IN expressions_error COLON wild_error
                   | FOR targets IN expressions COLON wild_error
    """
    # print("invalid for")

# EXPRESSIONS
# ===================
def p_expressions(p):
    """expressions : expressions COMMA expression
                   | expression
    """

def p_expressions_error(p):
    """expressions_error : expressions_error COMMA expression
                   | expressions COMMA expression_error
                   | expressions_error COMMA expression_error
                   | expression_error
                   | expressions_error COMMA wild_error
                   | expressions COMMA wild_error
    """

def p_expression(p):
    """expression : disjunction IF disjunction ELSE expression
                  | disjunction
    """

def p_expression_error(p):
    """expression_error : disjunction_error IF disjunction ELSE expression
                  | disjunction_error IF disjunction_error ELSE expression
                  | disjunction_error IF disjunction_error ELSE expression_error
                  | disjunction IF disjunction_error ELSE expression
                  | disjunction IF disjunction_error ELSE expression_error
                  | disjunction_error IF disjunction ELSE expression_error
                  | disjunction IF disjunction ELSE expression_error
                  | disjunction_error
                  | disjunction_error IF disjunction ELSE wild_error
                  | disjunction_error IF disjunction_error ELSE wild_error
                  | disjunction IF disjunction_error ELSE wild_error
                  | disjunction IF disjunction ELSE wild_error
    """

def p_disjunction(p):
    """disjunction : disjunction OR conjunction
                   | conjunction 
    """

def p_disjunction_error(p):
    """disjunction_error : disjunction_error OR conjunction
                   | disjunction OR conjunction_error
                   | disjunction_error OR conjunction_error
                   | conjunction_error
                   | disjunction_error OR wild_error
                   | disjunction OR wild_error
    """

def p_conjunction(p):
    """conjunction : conjunction AND inversion
                   | inversion
    """

def p_conjunction_error(p):
    """conjunction_error : conjunction_error AND inversion
                   | conjunction AND inversion_error
                   | conjunction_error AND inversion_error
                   | conjunction_error AND wild_error
                   | conjunction AND wild_error
                   | inversion_error
    """

def p_inversion(p):
    """inversion : NOT inversion 
                 | comparison
    """

def p_inversion_error(p):
    """inversion_error : NOT inversion_error 
                 | comparison_error
                 | NOT wild_error
    """

# COMPARISON OPERATORS
# =======================

def p_comparison(p):
    """comparison : bitwise_or compare_op_list
                  | bitwise_or
    """

def p_comparison_error(p):
    """comparison_error : bitwise_or_error compare_op_list
                  | bitwise_or compare_op_list_error
                  | bitwise_or_error compare_op_list_error
                  | bitwise_or_error
                  | bitwise_or_error wild_error
                  | bitwise_or wild_error
    """

def p_compare_op_list(p):
    """compare_op_list : compare_op_list compare_op
                       | compare_op 
    """

def p_compare_op_list_error(p):
    """compare_op_list_error : compare_op_list_error compare_op
                       | compare_op_list compare_op_error
                       | compare_op_list_error compare_op_error
                       | compare_op_error
                       | compare_op_list_error wild_error
                       | compare_op_list wild_error
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

def p_compare_op_error(p):
    """compare_op_error : EQUALITY bitwise_or_error 
                  | INEQUALITY bitwise_or_error
                  | GREATER_EQUAL bitwise_or_error
                  | LESSER_EQUAL bitwise_or_error
                  | GREATER bitwise_or_error
                  | LESSER bitwise_or_error
                  | NOT IN bitwise_or_error
                  | IS NOT bitwise_or_error
                  | IN bitwise_or_error
                  | IS bitwise_or_error
                  | EQUALITY wild_error
                  | INEQUALITY wild_error
                  | GREATER_EQUAL wild_error
                  | LESSER_EQUAL wild_error
                  | GREATER wild_error
                  | LESSER wild_error
                  | NOT IN wild_error
                  | IS NOT wild_error
                  | IN wild_error
                  | IS wild_error
    """

# BITWISE OPERATORS
# =======================

def p_bitwise_or(p):
    """bitwise_or : bitwise_or PIPE bitwise_xor 
                  | bitwise_xor 
    """

def p_bitwise_or_error(p):
    """bitwise_or_error : bitwise_or_error PIPE bitwise_xor
                  | bitwise_or PIPE bitwise_xor_error
                  | bitwise_or_error PIPE bitwise_xor_error 
                  | bitwise_xor_error
                  | bitwise_or_error PIPE wild_error
                  | bitwise_or PIPE wild_error
                  
    """

#TODO: XOR is not available in the tokens, should be added
def p_bitwise_xor(p):
    """bitwise_xor : bitwise_xor CARET bitwise_and 
                   | bitwise_and
    """

def p_bitwise_xor_error(p):
    """bitwise_xor_error : bitwise_xor_error CARET bitwise_and
                   | bitwise_xor CARET bitwise_and_error 
                   | bitwise_xor_error CARET bitwise_and_error
                   | bitwise_and_error
                   | bitwise_xor_error CARET wild_error
                   | bitwise_xor CARET wild_error
    """

def p_bitwise_and(p):
    """bitwise_and : bitwise_and AMPERSAND shift_expr 
                   | shift_expr
    """

def p_bitwise_and_error(p):
    """bitwise_and_error : bitwise_and_error AMPERSAND shift_expr
                   | bitwise_and AMPERSAND shift_expr_error
                   | bitwise_and_error AMPERSAND shift_expr_error 
                   | shift_expr_error
                   | bitwise_and_error AMPERSAND wild_error
                   | bitwise_and AMPERSAND wild_error
    """

def p_shift_expr(p):
    """shift_expr : shift_expr L_SHIFT sum
                  | shift_expr R_SHIFT sum
                  | sum
    """

def p_shift_expr_error(p):
    """shift_expr_error : shift_expr_error L_SHIFT sum
                  | shift_expr L_SHIFT sum_error
                  | shift_expr_error R_SHIFT sum_error
                  | sum_error
                  | shift_expr_error L_SHIFT wild_error
                  | shift_expr L_SHIFT wild_error
                  | shift_expr_error R_SHIFT wild_error
    """

# ARITHMETIC OPERATORS
# =======================
def p_sum(p):
    """sum : sum PLUS term
           | sum MINUS term
           | term
    """

def p_sum_error(p):
    """sum_error : sum_error PLUS term
           | sum PLUS term_error
           | sum_error PLUS term_error
           | sum_error MINUS term
           | sum MINUS term_error
           | sum_error MINUS term_error
           | term_error
           | sum_error PLUS wild_error
           | sum PLUS wild_error
           | sum_error MINUS wild_error
           | sum MINUS wild_error
    """

def p_term(p):
    """term : term STAR factor 
            | term SLASH factor 
            | term DOUBLE_SLASH factor 
            | term PERCENT factor
            | factor
    """

def p_term_error(p):
    """term_error : term_error STAR factor
            | term STAR factor_error
            | term_error STAR factor_error 
            | term_error SLASH factor 
            | term SLASH factor_error 
            | term_error SLASH factor_error
            | term_error DOUBLE_SLASH factor
            | term DOUBLE_SLASH factor_error 
            | term_error DOUBLE_SLASH factor_error
            | term_error PERCENT factor
            | term PERCENT factor_error
            | term_error PERCENT factor_error
            | factor_error
            | term_error STAR wild_error
            | term STAR wild_error
            | term_error SLASH wild_error
            | term SLASH wild_error
            | term_error DOUBLE_SLASH wild_error
            | term DOUBLE_SLASH wild_error 
            | term_error PERCENT wild_error
            | term PERCENT wild_error
    """

def p_factor(p):
    """factor : PLUS factor 
              | MINUS factor 
              | power
    """

def p_factor_error(p):
    """factor_error : PLUS factor_error 
              | MINUS factor_error
              | power_error
              | PLUS wild_error 
              | MINUS wild_error
    """

def p_power(p):
    """power : primary DOUBLE_STAR factor
             | primary
    """

def p_power_error(p):
    """power_error : primary_error DOUBLE_STAR factor
             | primary DOUBLE_STAR factor_error
             | primary_error DOUBLE_STAR factor_error
             | primary_error
             | primary_error DOUBLE_STAR wild_error
             | primary DOUBLE_STAR wild_error
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

def p_primary_error(p):
    """primary_error : primary_error L_PARENTHESIS arguments R_PARENTHESIS
               | primary L_PARENTHESIS arguments_error R_PARENTHESIS
               | primary_error L_PARENTHESIS arguments_error R_PARENTHESIS
               | L_PARENTHESIS expression_error R_PARENTHESIS
               | primary_error L_PARENTHESIS R_PARENTHESIS
               | primary_error L_SQB slices R_SQB
               | primary L_SQB slices_error R_SQB
               | primary_error L_SQB slices_error R_SQB
               | primary_error DOT IDENTIFIER
               | atomic_error
               | primary L_PARENTHESIS arguments wild_error
               | primary_error L_PARENTHESIS arguments wild_error
               | primary_error L_PARENTHESIS wild_error
    """

# slices:
#     | slice !',' 
#     | ','.(slice | starred_expression)+ [',']
def p_slices(p):
    """slices : slices COMMA slice
              | slice
    """

def p_slices_error(p):
    """slices_error : slices_error COMMA slice
              | slices COMMA slice_error
              | slices_error COMMA slice_error
              | slice_error
              | slices_error COMMA wild_error
              | slices COMMA wild_error
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

def p_slice_error(p):
    """slice_error : expression_error COLON expression COLON expression
             | expression_error COLON expression_error COLON expression
             | expression_error COLON expression_error COLON expression_error
             | expression_error COLON expression COLON expression_error
             | expression COLON expression_error COLON expression
             | expression COLON expression_error COLON expression_error
             | expression COLON expression COLON expression_error
             | COLON expression_error COLON expression
             | COLON expression COLON expression_error
             | COLON expression_error COLON expression_error
             | expression_error COLON COLON expression
             | expression COLON COLON expression_error
             | expression_error COLON COLON expression_error
             | COLON expression_error COLON
             | COLON COLON expression_error
             | expression_error COLON COLON
             | expression_error
             | expression_error COLON expression COLON wild_error
             | expression_error COLON expression_error COLON wild_error
             | expression COLON expression_error COLON wild_error
             | expression COLON expression COLON wild_error
             | COLON expression_error COLON wild_error
             | COLON expression COLON wild_error
             | expression_error COLON COLON wild_error
             | expression COLON COLON wild_error
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

def p_atomic_error(p):
    """atomic_error : tuple_error
              | list_error
              | dict_error
              | set_error
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

def p_argument_error(p):
    """argument_error : expression_error"""

#TODO: Check how this should work may want to include keyword arguments
def p_arguments(p):
    """arguments : expressions
    """

def p_arguments_error(p):
    """arguments_error : expressions_error
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

def p_list_error(p):
    """list_error : L_SQB expressions_error R_SQB
    """

# | '(' [star_named_expression ',' [star_named_expressions]  ] ')' 
def p_tuple(p):
    """tuple : L_PARENTHESIS expression COMMA expressions R_PARENTHESIS
             | L_PARENTHESIS expression COMMA R_PARENTHESIS
             | L_PARENTHESIS R_PARENTHESIS
    """

def p_tuple_error(p):
    """tuple_error : L_PARENTHESIS expression_error COMMA expressions R_PARENTHESIS
             | L_PARENTHESIS expression COMMA expressions_error R_PARENTHESIS
             | L_PARENTHESIS expression_error COMMA expressions_error R_PARENTHESIS
             | L_PARENTHESIS expression_error COMMA R_PARENTHESIS
    """

def p_set(p):
    """set : L_CB expressions R_CB
    """

def p_set_error(p):
    """set_error : L_CB expressions_error R_CB
    """

# DICTIONARY
def p_dict(p):
    """dict : L_CB kvpairs R_CB
            | L_CB R_CB
    """

def p_dict_error(p):
    """dict_error : L_CB kvpairs_error R_CB
    """

def p_kvpairs(p):
    """kvpairs : kvpair_list COMMA
               | kvpair_list
    """

def p_kvpairs_error(p):
    """kvpairs_error : kvpair_list_error COMMA
               | kvpair_list_error
    """

def p_kvpair(p):
    """kvpair : expression COLON expression
    """

def p_kvpair_error(p):
    """kvpair_error : expression_error COLON expression
                   | expression COLON expression_error
                   | expression_error COLON expression_error
                   | expression COLON wild_error
                   | expression_error COLON wild_error
    """

def p_kvpair_list(p):
    """kvpair_list : kvpair_list COMMA kvpair
                   | kvpair
    """

def p_kvpair_list_error(p):
    """kvpair_list_error : kvpair_list_error COMMA kvpair
                   | kvpair_list COMMA kvpair_error
                   | kvpair_list_error COMMA kvpair_error
                   | kvpair_error
                   | kvpair_list COMMA wild_error
                   | kvpair_list_error COMMA wild_error
    """

# ASSIGNMENT TARGETS
# ==================
def p_targets(p):
    """targets : primary COMMA targets
               | primary
    """

def p_targets_error(p):
    """targets_error : primary_error COMMA targets
               | primary COMMA targets_error
               | primary_error COMMA targets_error
               | primary_error
               | primary COMMA wild_error 
               | primary_error COMMA wild_error
    """

# TODO: THIS SHOULD BE CHANGED, because implementations as asked for in python does not work properly!!
def p_target(p):
    """target : target_primary DOT IDENTIFIER
              | target_primary L_SQB slices R_SQB
              | target_atomic
    """

def p_target_error(p):
    """target_error : target_primary_error DOT IDENTIFIER
              | target_primary_error L_SQB slices R_SQB
              | target_primary L_SQB slices_error R_SQB
              | target_primary_error L_SQB slices_error R_SQB
              | target_atomic_error
    """

def p_target_primary(p):
    """target_primary : target_primary DOT IDENTIFIER
                      | target_primary L_SQB slices R_SQB
                      | target_primary L_PARENTHESIS R_PARENTHESIS
                      | target_primary L_PARENTHESIS arguments R_PARENTHESIS
                      | target_atomic
    """

def p_target_primary_error(p):
    """target_primary_error : target_primary_error DOT IDENTIFIER
                      | target_primary_error L_SQB slices R_SQB
                      | target_primary L_SQB slices_error R_SQB
                      | target_primary_error L_SQB slices_error R_SQB
                      | target_primary_error L_PARENTHESIS R_PARENTHESIS
                      | target_primary_error L_PARENTHESIS arguments R_PARENTHESIS
                      | target_primary L_PARENTHESIS arguments_error R_PARENTHESIS
                      | target_primary_error L_PARENTHESIS arguments_error R_PARENTHESIS
                      | target_atomic_error
    """

def p_target_atomic(p):
    """target_atomic : IDENTIFIER
                     | L_SQB target_primary R_SQB
                     | L_PARENTHESIS targets R_PARENTHESIS
    """

def p_target_atomic_error(p):
    """target_atomic_error : L_SQB target_primary_error R_SQB
                     | L_PARENTHESIS targets_error R_PARENTHESIS
    """
    
def p_empty(p):
    'empty :'
    pass

# COMMON ERRORS
# ==================
def p_wild_error(p):
    """wild_error : wild_error error
               | error
    """

# ========================= END OF THE GRAMMAR ===========================

def p_error(p):
    if p:
        error_msg = f"Syntax Error near '{p.value if p.value else p.type}' in line {p.lineno}"
        # raise SyntaxError(error_msg)
    else:
        # raise SyntaxError("Syntax error at EOF")
        pass
    
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
        if len(error.args) < 2:
            print(error)
            return
        token = error.args[1]
        self.error_logger.log_error(
            error.args[0],
            token.lineno + 1,
            find_column(token.lexer.lexdata, token),
            get_input(token.lexer.lexdata, token),
            error_type
        )