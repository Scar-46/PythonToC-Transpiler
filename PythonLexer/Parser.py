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

def p_statement_newline(p):
    """statement_newline : compound_stmt NEWLINE
                         | simple_stmts
                         | NEWLINE 
                         | ENDMARKER 
    """

# simple_stmts:
#     | simple_stmt !';' NEWLINE  # Not needed, there for speedup
#     | ';'.simple_stmt+ [';'] NEWLINE 
#TODO: Fix with correct interpretation of official grammar
def p_simple_stmts(p):
    raise NotImplementedError

# SIMPLE STATEMENTS
# NOTE: assignment MUST precede expression, else parsing a simple assignment will throw a SyntaxError
# TODO: Remove statemnts that will not be implemented
def p_simple_stmt(p):
    """simple_stmt : assignment
                   | type_alias
                   | star_expressions
                   | return_stmt
                   | PASS
                   | del_stmt
                   | assert_stmt
                   | BREAK 
                   | CONTINUE 
                   | global_stmt
    """

def p_compound_stmt(p):
    """compound_stmt : function_def
                     | if_stmt
                     | class_def
                     | with_stmt
                     | for_stmt
                     | while_stmt
    """
# SIMPLE STATEMENTS
# =================
# NOTE: annotated_rhs may start with 'yield'; yield_expr must start with 'yield'
# assignment:
#     | NAME ':' expression ['=' annotated_rhs ] 
#     | ('(' single_target ')' 
#          | single_subscript_attribute_target) ':' expression ['=' annotated_rhs ] 
#     | (star_targets '=' )+ (yield_expr | star_expressions) !'=' [TYPE_COMMENT] 
#     | single_target augassign ~ (yield_expr | star_expressions) 
def p_assignment(p):
    raise NotImplementedError

# annotated_rhs: yield_expr | star_expressions
def p_annotated_rhs(p):
    raise NotImplementedError

# augassign
def p_augmentation_assignment(p):
    """augmentation_assignment : SUM_ASSIGNMENT
                               | SUBTRACTION_ASSIGNMENT
                               | PRODUCT_ASSIGNMENT
                               | DIVISION_ASSIGNMENT
                               | MODULO_ASSIGNMENT
                               | EXPONENTIATION_ASSIGNMENT
                               | INTEGER_DIVISION_ASSIGNMENT
    """

# return_stmt:
#     | 'return' [star_expressions] 
def p_return_stmt(p):
    """return_stmt : RETURN star_expressions
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
    """class_def : class_def_raw
    """

# class_def_raw:
#     | 'class' NAME [type_params] ['(' [arguments] ')' ] ':' block 
def p_class_def_raw(p):
    raise NotImplementedError

def p_function_def(p):
    """function_def : function_def_raw 
    """

# function_def_raw:
#     | 'def' NAME [type_params] '(' [params] ')' ['->' expression ] ':' [func_type_comment] block 
#     | ASYNC 'def' NAME [type_params] '(' [params] ')' ['->' expression ] ':' [func_type_comment] block 
def p_function_def_raw(p):
    raise NotImplementedError

def p_params(p):
    """params : parameters
    """

# parameters:
#     | slash_no_default param_no_default* param_with_default* [star_etc] 
#     | slash_with_default param_with_default* [star_etc] 
#     | param_no_default+ param_with_default* [star_etc] 
#     | param_with_default+ [star_etc] 
#     | star_etc 
def p_parameters(p):
    raise NotImplementedError

# TODO: Keep implementing parameters grammar rules
# Some duplication here because we can't write (',' | &')'),
# which is because we don't support empty alternatives (yet).

# slash_no_default:
#     | param_no_default+ '/' ',' 
#     | param_no_default+ '/' &')' 
# slash_with_default:
#     | param_no_default* param_with_default+ '/' ',' 
#     | param_no_default* param_with_default+ '/' &')' 

# star_etc:
#     | '*' param_no_default param_maybe_default* [kwds] 
#     | '*' param_no_default_star_annotation param_maybe_default* [kwds] 
#     | '*' ',' param_maybe_default+ [kwds] 
#     | kwds 

# kwds:
#     | '**' param_no_default 

# One parameter.  This *includes* a following comma and type comment.
#
# There are three styles:
# - No default
# - With default
# - Maybe with default
#
# There are two alternative forms of each, to deal with type comments:
# - Ends in a comma followed by an optional type comment
# - No comma, optional type comment, must be followed by close paren
# The latter form is for a final parameter without trailing comma.
#

# param_no_default:
#     | param ',' TYPE_COMMENT? 
#     | param TYPE_COMMENT? &')' 
# param_no_default_star_annotation:
#     | param_star_annotation ',' TYPE_COMMENT? 
#     | param_star_annotation TYPE_COMMENT? &')' 
# param_with_default:
#     | param default ',' TYPE_COMMENT? 
#     | param default TYPE_COMMENT? &')' 
# param_maybe_default:
#     | param default? ',' TYPE_COMMENT? 
#     | param default? TYPE_COMMENT? &')' 
# param: NAME annotation? 
# param_star_annotation: NAME star_annotation 
# annotation: ':' expression 
# star_annotation: ':' star_expression 
# default: '=' expression  | invalid_default

# If statement
# ------------
# if_stmt:
#     | 'if' named_expression ':' block elif_stmt 
#     | 'if' named_expression ':' block [else_block] 
def p_if_stmt(p):
    """if_stmt : IF named_expression COLON block elif_stmt
               | IF named_expression COLON block else_block
    """

# elif_stmt:
#     | 'elif' named_expression ':' block elif_stmt 
#     | 'elif' named_expression ':' block [else_block]  
def p_elif_stmt(p):
    """elif_stmt : ELIF named_expression COLON block elif_stmt
                 | ELIF named_expression COLON block else_block
    """

def p_else_block(p):
    """else_block : ELSE COLON block
    """

# while_stmt:
#     | 'while' named_expression ':' block [else_block]
def p_while_stmt(p):
    """while_stmt : WHILE named_expression COLON block else_block
    """

# for_stmt:
#     | 'for' star_targets 'in' ~ star_expressions ':' [TYPE_COMMENT] block [else_block] 
#     | ASYNC 'for' star_targets 'in' ~ star_expressions ':' [TYPE_COMMENT] block [else_block] 
def p_for_stmt(p):
    raise NotImplementedError

# TODO: Remove if this is not defined in requirements
# Match statement
# ---------------

# Type statement
# ---------------

# EXPRESSIONS
# -----------

# expressions:
#     | expression (',' expression )+ [','] 
#     | expression ',' 
#     | expression
def p_expressions(p):
    raise NotImplementedError

def p_expression(p):
    """expression : disjunction IF disjunction ELSE expression
                  | disjunction
    """

# star_expressions:
#     | star_expression (',' star_expression )+ [','] 
#     | star_expression ',' 
#     | star_expression
def p_star_expressions(p):
    raise NotImplementedError

def p_star_expression(p):
    # """star_expression : STAR bitwise_or
    #                    | expression
    # """
    raise NotImplementedError

# star_named_expressions: ','.star_named_expression+ [','] 
def p_star_named_expressions(p):
    raise NotImplementedError

def p_star_named_expression(p):
    """star_named_expression : STAR bitwise_or
                             | named_expression
    """

# assignment_expression:
#     | NAME ':=' ~ expression 
def p_assignment_expression(p):
    raise NotImplementedError

# named_expression:
#     | assignment_expression
#     | expression !':='
def p_named_expression(p):
    raise NotImplementedError

# disjunction:
#     | conjunction ('or' conjunction )+ 
#     | conjunction
def p_disjunction(p):
    raise NotImplementedError

# conjunction:
#     | inversion ('and' inversion )+ 
#     | inversion
def p_conjunction(p):
    raise NotImplementedError

def p_inversion(p):
    """inversion : NOT inversion
                 | comparison
    """

# Comparison operators
# --------------------

# comparison:
#     | bitwise_or compare_op_bitwise_or_pair+ 
#     | bitwise_or
def p_comparison(p):
    # """comparison : bitwise_or compare_op_bitwise_or_pair
    #               | bitwise_or
    # """
    raise NotImplementedError

def p_compare_op_bitwise_or_pair(p):
    # """compare_op_bitwise_or_pair : eq_bitwise_or
    #                               | noteq_bitwise_or
    #                               | lte_bitwise_or
    #                               | lt_bitwise_or
    #                               | gte_bitwise_or
    #                               | gt_bitwise_or
    #                               | notin_bitwise_or
    #                               | in_bitwise_or
    #                               | isnot_bitwise_or
    #                               | is_bitwise_or
    # """
    raise NotImplementedError

# eq_bitwise_or: '==' bitwise_or 
# noteq_bitwise_or:
#     | ('!=' ) bitwise_or 
# lte_bitwise_or: '<=' bitwise_or 
# lt_bitwise_or: '<' bitwise_or 
# gte_bitwise_or: '>=' bitwise_or 
# gt_bitwise_or: '>' bitwise_or 
# notin_bitwise_or: 'not' 'in' bitwise_or 
# in_bitwise_or: 'in' bitwise_or 
# isnot_bitwise_or: 'is' 'not' bitwise_or 
# is_bitwise_or: 'is' bitwise_or

# Bitwise operators
# -----------------

# bitwise_or:
#     | bitwise_or '|' bitwise_xor 
#     | bitwise_xor

# bitwise_xor:
#     | bitwise_xor '^' bitwise_and 
#     | bitwise_and

# bitwise_and:
#     | bitwise_and '&' shift_expr 
#     | shift_expr

# shift_expr:
#     | shift_expr '<<' sum 
#     | shift_expr '>>' sum 
#     | sum

# Arithmetic operators
# --------------------

# sum:
#     | sum '+' term 
#     | sum '-' term 
#     | term

# term:
#     | term '*' factor 
#     | term '/' factor 
#     | term '//' factor 
#     | term '%' factor 
#     | term '@' factor 
#     | factor

# factor:
#     | '+' factor 
#     | '-' factor 
#     | '~' factor 
#     | power

# power:
#     | await_primary '**' factor 
#     | await_primary

# Primary elements
# ----------------

# Primary elements are things like "obj.something.something", "obj[something]", "obj(something)", "obj" ...

# await_primary:
#     | AWAIT primary 
#     | primary

# primary:
#     | primary '.' NAME 
#     | primary genexp 
#     | primary '(' [arguments] ')' 
#     | primary '[' slices ']' 
#     | atom

# slices:
#     | slice !',' 
#     | ','.(slice | starred_expression)+ [','] 

# slice:
#     | [expression] ':' [expression] [':' [expression] ] 
#     | named_expression 

# atom:
#     | NAME
#     | 'True' 
#     | 'False' 
#     | 'None' 
#     | strings
#     | NUMBER
#     | (tuple | group | genexp)
#     | (list | listcomp)
#     | (dict | set | dictcomp | setcomp)
#     | '...' 

# group:
#     | '(' (yield_expr | named_expression) ')' 

# LITERALS
# ========

# fstring_middle:
#     | fstring_replacement_field
#     | FSTRING_MIDDLE 
# fstring_replacement_field:
#     | '{' (yield_expr | star_expressions) '='? [fstring_conversion] [fstring_full_format_spec] '}' 
# fstring_conversion:
#     | "!" NAME 
# fstring_full_format_spec:
#     | ':' fstring_format_spec* 
# fstring_format_spec:
#     | FSTRING_MIDDLE 
#     | fstring_replacement_field
# fstring:
#     | FSTRING_START fstring_middle* FSTRING_END 

# string: STRING 
# strings: (fstring|string)+ 

# list:
#     | '[' [star_named_expressions] ']' 

# tuple:
#     | '(' [star_named_expression ',' [star_named_expressions]  ] ')' 

# set: '{' star_named_expressions '}' 

# # Dicts
# # -----

# dict:
#     | '{' [double_starred_kvpairs] '}' 

# double_starred_kvpairs: ','.double_starred_kvpair+ [','] 

# double_starred_kvpair:
#     | '**' bitwise_or 
#     | kvpair

# kvpair: expression ':' expression 

# # Comprehensions & Generators
# # ---------------------------

# for_if_clauses:
#     | for_if_clause+ 

# for_if_clause:
#     | ASYNC 'for' star_targets 'in' ~ disjunction ('if' disjunction )* 
#     | 'for' star_targets 'in' ~ disjunction ('if' disjunction )* 

# listcomp:
#     | '[' named_expression for_if_clauses ']' 

# setcomp:
#     | '{' named_expression for_if_clauses '}' 

# genexp:
#     | '(' ( assignment_expression | expression !':=') for_if_clauses ')' 

# dictcomp:
#     | '{' kvpair for_if_clauses '}' 

# # FUNCTION CALL ARGUMENTS
# # =======================

# arguments:
#     | args [','] &')' 

# args:
#     | ','.(starred_expression | ( assignment_expression | expression !':=') !'=')+ [',' kwargs ] 
#     | kwargs 

# kwargs:
#     | ','.kwarg_or_starred+ ',' ','.kwarg_or_double_starred+ 
#     | ','.kwarg_or_starred+
#     | ','.kwarg_or_double_starred+

# starred_expression:
#     | '*' expression 

# kwarg_or_starred:
#     | NAME '=' expression 
#     | starred_expression 

# kwarg_or_double_starred:
#     | NAME '=' expression 
#     | '**' expression 

# ASSIGNMENT TARGETS
# ==================

# Generic targets
# ---------------

# NOTE: star_targets may contain *bitwise_or, targets may not.
# star_targets:
#     | star_target !',' 
#     | star_target (',' star_target )* [','] 

# star_targets_list_seq: ','.star_target+ [','] 

# star_targets_tuple_seq:
#     | star_target (',' star_target )+ [','] 
#     | star_target ',' 

# star_target:
#     | '*' (!'*' star_target) 
#     | target_with_star_atom

# target_with_star_atom:
#     | t_primary '.' NAME !t_lookahead 
#     | t_primary '[' slices ']' !t_lookahead 
#     | star_atom

# star_atom:
#     | NAME 
#     | '(' target_with_star_atom ')' 
#     | '(' [star_targets_tuple_seq] ')' 
#     | '[' [star_targets_list_seq] ']' 

# single_target:
#     | single_subscript_attribute_target
#     | NAME 
#     | '(' single_target ')' 

# single_subscript_attribute_target:
#     | t_primary '.' NAME !t_lookahead 
#     | t_primary '[' slices ']' !t_lookahead 

# t_primary:
#     | t_primary '.' NAME &t_lookahead 
#     | t_primary '[' slices ']' &t_lookahead 
#     | t_primary genexp &t_lookahead 
#     | t_primary '(' [arguments] ')' &t_lookahead 
#     | atom &t_lookahead 

# t_lookahead: '(' | '[' | '.'

# # Targets for del statements
# # --------------------------

# del_targets: ','.del_target+ [','] 

# del_target:
#     | t_primary '.' NAME !t_lookahead 
#     | t_primary '[' slices ']' !t_lookahead 
#     | del_t_atom

# del_t_atom:
#     | NAME 
#     | '(' del_target ')' 
#     | '(' [del_targets] ')' 
#     | '[' [del_targets] ']' 

# # TYPING ELEMENTS
# # ---------------

# # type_expressions allow */** but ignore them
# type_expressions:
#     | ','.expression+ ',' '*' expression ',' '**' expression 
#     | ','.expression+ ',' '*' expression 
#     | ','.expression+ ',' '**' expression 
#     | '*' expression ',' '**' expression 
#     | '*' expression 
#     | '**' expression 
#     | ','.expression+ 

# func_type_comment:
#     | NEWLINE TYPE_COMMENT &(NEWLINE INDENT)   # Must be followed by indented block
#     | TYPE_COMMENT
    

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