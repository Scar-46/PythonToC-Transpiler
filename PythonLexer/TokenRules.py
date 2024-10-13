# module: TokenRules.py
# Lexing rule definitions

# ------------ Lexemes and patterns ------------
# Fixed-size reserved keywords
keywords = {
    # Logical Operators
    'and': 'AND',
    'or': 'OR',
    'not': 'NOT',
    # Flow Control structures
    'if': 'IF',
    'else': 'ELSE',
    'elif': 'ELIF',
    'for': 'FOR',
    'while': 'WHILE',
    'break': 'BREAK',
    'pass': 'PASS',
    'continue': 'CONTINUE',
    # Definitions
    'def': 'DEF',
    'as': 'AS',
    'class': 'CLASS',
    'return': 'RETURN',
    # Boolean 
    'True': 'TRUE',
    'False': 'FALSE',
    # Other
    'del': 'DEL',
    'global': 'GLOBAL',
    'in': 'IN',
    'is': 'IS',
    'None': 'NONE',
}

# Regular language tokens
tokens = [
    # Literals
    # - String
    'TRIPLE_STRING',
    'STRING',

    # - Numerical
    'F_NUMBER',
    'HEX_NUMBER',
    'OCT_NUMBER',
    'BIN_NUMBER',
    'NUMBER',

    # Variables
    'IDENTIFIER',

    # Delimiters
    # - Grouping
    'L_PARENTHESIS',
    'R_PARENTHESIS',
    'L_SQB',
    'R_SQB',
    'L_CB',
    'R_CB',

    # - Accesors
    'DOT',

    # - Separators
    'COLON',
    'SEMICOLON',
    'COMMA',

    # Operators
    # - Relational
    'EQUALITY',
    'INEQUALITY',
    'GREATER_EQUAL',
    'LESSER_EQUAL',
    'GREATER',
    'LESSER',
    'L_SHIFT',
    'R_SHIFT',

    # - Assignment
    'EXPONENTIATION_ASSIGNMENT',
    'MULTIPLICATION_ASSIGNMENT',
    'FLOOR_DIVISION_ASSIGNMENT',
    'DIVISION_ASSIGNMENT',
    'ADDITION_ASSIGNMENT',
    'SUBTRACTION_ASSIGNMENT',
    'MODULO_ASSIGNMENT',
    'ASSIGNMENT',

    # - Arithmetic\
    'STAR',
    'DOUBLE_STAR',
    'SLASH',
    'DOUBLE_SLASH',
    'PLUS',
    'MINUS',
    'PERCENT',
    'PIPE',
    'AMPERSAND',
    'CARET',

    'NEWLINE',
    'WHITESPACE',
    'INDENT',
    'DEDENT',
    'ENDMARKER',
] + list(keywords.values())

# Comments
def t_comment(t):
    r'\#[^\n\r]*$'
    pass

# Delimiters
# - Grouping
t_L_PARENTHESIS = r'\('
t_R_PARENTHESIS = r'\)'
t_L_SQB = r'\['
t_R_SQB = r'\]'
t_L_CB = r'\{'
t_R_CB = r'\}'

# - Accesors
t_DOT = r'\.'

# - Separators
t_COLON = r':'
t_SEMICOLON = r';'
t_COMMA = r','

# Operators
# - Relational
t_EQUALITY = r'=='
t_INEQUALITY = r'!='
t_GREATER_EQUAL = r'>='
t_LESSER_EQUAL = r'<='
t_GREATER = r'>'
t_LESSER = r'<'

# - Assignment
t_EXPONENTIATION_ASSIGNMENT = r'\*\*='
t_MULTIPLICATION_ASSIGNMENT = r'\*='
t_FLOOR_DIVISION_ASSIGNMENT = r'\/\/='
t_DIVISION_ASSIGNMENT = r'\/='
t_ADDITION_ASSIGNMENT = r'\+='
t_SUBTRACTION_ASSIGNMENT = r'-='
t_MODULO_ASSIGNMENT = r'%='
t_ASSIGNMENT = r'='

# - Arithmetic
t_DOUBLE_STAR = r'\*\*'
t_STAR = r'\*'
t_DOUBLE_SLASH = r'\/\/'
t_SLASH = r'\/'
t_PLUS = r'\+'
t_MINUS = r'-'
t_PERCENT = r'%'
t_PIPE = r'\|'
t_AMPERSAND = r'&'
t_CARET = r'\^'

# Literals
# Rule for matching single and double-quoted strings
def t_TRIPLE_STRING(t):
    r'("""((\\n|\\t|\\\\|\\"|\\\'|\')|[^\\])*?"""|\'\'\'((\\n|\\t|\\\\|\\\'|\\\")|[^\\])*?\'\'\')'
    t.value = bytes(t.value[3:-3], "utf-8").decode("unicode_escape")
    return t

# Rule for matching triple-quoted strings (both """ and ''')
def t_STRING(t):
    r'("((\\["\'\\tn]|[^"\\])*)")|(\'((\\["\'\\tn]|[^\'\\])*)\')'
    t.value = bytes(t.value[1:-1], "utf-8").decode("unicode_escape")
    return t


def t_HEX_NUMBER(t):
    r'0[xX][0-9a-fA-F]+'
    t.value = int(t.value, 16)
    return t

def t_OCT_NUMBER(t):
    r'0[oO][0-7]+'
    t.value = int(t.value, 8)
    return t

def t_BIN_NUMBER(t):
    r'0[bB][01]+'
    t.value = int(t.value, 2)
    return t

def t_F_NUMBER(t):
    r'(\.\d+|\d+\.\d*)((e(\+|-)?\d+)?)|\d+(e(\+|-)?\d+)'
    t.value = float(t.value)
    return t

def t_NUMBER(t):
    r'\d+'
    t.value = int(t.value)
    return t

def t_BOOLEAN(t):
    r'True|False'
    t.type = t.value.upper()
    t.value = (t.value == r'True')
    return t

def t_IDENTIFIER(t):
    r'[a-zA-Z_][a-zA-Z0-9_]*'
    t.type = keywords.get(t.value,'IDENTIFIER')
    return t

def t_escaped_newline(t):
    r'\\(\n\r|\n|\r)'
    t.lexer.lineno += 1
    pass

def t_NEWLINE(t):
    r'(\n\r|\n|\r)+'
    if t.lexer.lineno is not None:
        t.lexer.lineno += len(t.value)
    return t

def t_WHITESPACE(t):
    r'[ \t\f]+'
    return t

# May want to log errors on errorlist to give them formatting.
class LexingError(Exception):
    pass

def t_error(t):
    t.lexer.skip(1)
    raise LexingError(f"unrecognized sequence: {t.value} on line {t.lineno}")