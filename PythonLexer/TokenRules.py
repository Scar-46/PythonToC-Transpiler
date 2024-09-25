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
    'finally': 'FINALLY',
    'from': 'FROM',
    'global': 'GLOBAL',
    'in': 'IN',
    'is': 'IS',
    'None': 'NONE',
    'nonlocal': 'NONLOCAL',
    'raise': 'RAISE',
}

# Regular language tokens
tokens = [
    # Literals
    # - String
    'STRING',

    # - Numerical
    'F_NUMBER',
    'I_NUMBER',

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
    'AT',

    # - Separators
    'COLON',
    'COMMA',

    # Operators
    # - Relational
    'EQUALITY',
    'INEQUALITY',
    'GREATER_EQUAL',
    'LESSER_EQUAL',
    'GREATER',
    'LESSER',
    # - Assignment
    'EXPONENTIATION_ASSIGNMENT',
    'PRODUCT_ASSIGNMENT',
    'INTEGER_DIVISION_ASSIGNMENT',
    'DIVISION_ASSIGNMENT',
    'SUM_ASSIGNMENT',
    'SUBTRACTION_ASSIGNMENT',
    'MODULUS_ASSIGNMENT',
    'ASSIGNMENT',
    # - Arithmetic
    'EXPONENTIATION',
    'PRODUCT',
    'INTEGER_DIVISION',
    'DIVISION',
    'SUM',
    'SUBTRACTION',
    'MODULUS',
    'BITWISE_OR',
    'BITWISE_AND',

    'NEWLINE',
    'WHITESPACE',
    'INDENT',
    'DEDENT',
    'ENDMARKER',
] + list(keywords.values())

# Comments
def t_comment(t):
    r'\#[^\n\r]*((\n\r)|(\n)|(\r))?'
    pass

# Literals
def t_STRING(t):
    (
        r'('
            r'"((\\t)|(\\n)|(\\")|(\\\\)|[^\n\r\\"])*"'
        r')|('
            r'\'((\\t)|(\\n)|(\\\')|(\\\\)|[^\n\r\\\'])*\''
        r')'
    )
    t.value = bytes(t.value, "utf-8").decode("unicode_escape")[1:-1]
    return t

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
t_AT = r'@'

# - Separators
t_COLON = r':'
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
t_PRODUCT_ASSIGNMENT = r'\*='
t_INTEGER_DIVISION_ASSIGNMENT = r'\/\/='
t_DIVISION_ASSIGNMENT = r'\/='
t_SUM_ASSIGNMENT = r'\+='
t_SUBTRACTION_ASSIGNMENT = r'-='
t_MODULUS_ASSIGNMENT = r'%='
t_ASSIGNMENT = r'='

# - Arithmetic
t_EXPONENTIATION = r'\*\*'
t_PRODUCT = r'\*'
t_INTEGER_DIVISION = r'\/\/'
t_DIVISION = r'\/'
t_SUM = r'\+'
t_SUBTRACTION = r'-'
t_MODULUS = r'%'
t_BITWISE_OR = r'\|'
t_BITWISE_AND = r'&'

def t_F_NUMBER(t):
    r'\d*\.\d+(e(\+|-)?\d+)?'
    t.value = float(t.value)
    return t

def t_I_NUMBER(t):
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
    t.lexer.lineno += len(t.value)
    return t

def t_WHITESPACE(t):
    r'[ \t\f]+'
    return t

# Unmatched token
def t_error(t):
    decoded = t.value.encode('unicode_escape').decode('utf-8')
    print(f"Unrecognized sequence: %s" % decoded)
    t.lexer.skip(1)