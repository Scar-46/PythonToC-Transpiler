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
    # Comments
    # - Line comment
    'C_LINE',

    # Literals
    # - String
    'L_LINE_STRING',
    # - Numerical
    'L_FLOATING_POINT',
    'L_INTEGER',
    # - Boolean
    'L_BOOLEAN',

    # Variables
    'IDENTIFIER',

    # Delimiters
    # - Grouping
    'D_LEFT_PARENTHESIS',
    'D_RIGHT_PARENTHESIS',
    'D_LEFT_SQUARE_BRACKET',
    'D_RIGHT_SQUARE_BRACKET',
    'D_LEFT_CURLY_BRACE',
    'D_RIGHT_CURLY_BRACE',
    # - Accesors
    'D_DOT',
    'D_AT_SIGN',
    # - Separators
    'D_COLON',
    'D_COMMA',

    # Operators
    # - Relational
    'O_EQUALITY',
    'O_INEQUALITY',
    'O_GREATER_EQUAL',
    'O_LESSER_EQUAL',
    'O_GREATER',
    'O_LESSER',
    # - Assignment
    'O_EXPONENTIATION_ASSIGNMENT',
    'O_PRODUCT_ASSIGNMENT',
    'O_INTEGER_DIVISION_ASSIGNMENT',
    'O_DIVISION_ASSIGNMENT',
    'O_SUM_ASSIGNMENT',
    'O_SUBSTRACTION_ASSIGNMENT',
    'O_MODULUS_ASSIGNMENT',
    'O_ASSIGNMENT',
    # - Logical
    'O_LOGICAL_AND',
    'O_LOGICAL_OR',
    'O_LOGICAL_NOT',
    # - Arithmetic
    'O_EXPONENTIATION',
    'O_PRODUCT',
    'O_INTEGER_DIVISION',
    'O_DIVISION',
    'O_SUM',
    'O_SUBSTRACTION',
    'O_MODULUS',
    'O_BITWISE_OR',
    'O_BITWISE_AND',

    # Context-sensitive deliminters
    'ESCAPED_NEWLINE',
    'NEWLINE',
    'WHITESPACE',

    # Virtual delimiters
    'INDENT',
    'DEDENT',
    'ENDMARKER',

] + list(keywords.values())

# Comments
# - Line comment
def t_C_LINE(t):
    r'\#[^\n\r]*((\n\r)|(\n)|(\r))?'
    pass

# Literals
# - Line string
def t_L_LINE_STRING(t):
    (
        r'('
            r'"((\\t)|(\\n)|(\\")|(\\\\)|[^\n\r\\"])*"'
        r')|('
            r'\'((\\t)|(\\n)|(\\\')|(\\\\)|[^\n\r\\\'])*\''
        r')'
    )
    t.value = bytes(t.value, "utf-8").decode("unicode_escape")[1:-1]
    return t

# - Numerical
def t_L_FLOATING_POINT(t):
    r'\d*\.\d+(e(\+|-)?\d+)?'
    t.value = float(t.value)
    return t

def t_L_INTEGER(t):
    r'\d+'
    t.value = int(t.value)
    return t

# - Boolean
def t_L_BOOLEAN(t):
    r'True|False'
    t.value = (t.value == r'True')
    return t

# Keywords (fallback identifier)
def t_IDENTIFIER(t):
    r'[a-zA-Z_][a-zA-Z0-9_]*'
    t.type = keywords.get(t.value,'IDENTIFIER')
    return t

# Delimiters
# - Grouping
t_D_LEFT_PARENTHESIS = r'\('
t_D_RIGHT_PARENTHESIS = r'\)'
t_D_LEFT_SQUARE_BRACKET = r'\['
t_D_RIGHT_SQUARE_BRACKET = r'\]'
t_D_LEFT_CURLY_BRACE = r'\{'
t_D_RIGHT_CURLY_BRACE = r'\}'
# - Accesors
t_D_DOT = r'\.'
t_D_AT_SIGN = r'@'
# - Separators
t_D_COLON = r':'
t_D_COMMA = r','

# Operators
# - Relational
t_O_EQUALITY = r'=='
t_O_INEQUALITY = r'!='
t_O_GREATER_EQUAL = r'>='
t_O_LESSER_EQUAL = r'<='
t_O_GREATER = r'>'
t_O_LESSER = r'<'
# - Assignment
t_O_EXPONENTIATION_ASSIGNMENT = r'\*\*='
t_O_PRODUCT_ASSIGNMENT = r'\*='
t_O_INTEGER_DIVISION_ASSIGNMENT = r'\/\/='
t_O_DIVISION_ASSIGNMENT = r'\/='
t_O_SUM_ASSIGNMENT = r'\+='
t_O_SUBSTRACTION_ASSIGNMENT = r'-='
t_O_MODULUS_ASSIGNMENT = r'%='
t_O_ASSIGNMENT = r'='
# - Logical
t_O_LOGICAL_AND = r'(&&)|(and)'
t_O_LOGICAL_OR = r'(\|\|)|(or)'
t_O_LOGICAL_NOT = r'(!)|(not)'
# - Arithmetic
t_O_EXPONENTIATION = r'\*\*'
t_O_PRODUCT = r'\*'
t_O_INTEGER_DIVISION = r'\/\/'
t_O_DIVISION = r'\/'
t_O_SUM = r'\+'
t_O_SUBSTRACTION = r'-'
t_O_MODULUS = r'%'
t_O_BITWISE_OR = r'\|'
t_O_BITWISE_AND = r'&'

# Context-sensitive delimiters
# - Escaped newline
def t_ESCAPED_NEWLINE(t):
    r'\\(\n\r|\n|\r)'
    t.lexer.lineno += 1
    pass

# - Newline
def t_NEWLINE(t):
    r'(\n\r|\n|\r)+'
    t.lexer.lineno += len(t.value)
    return t

# - Contiguous Whitespace
def t_WHITESPACE(t):
    r'[ \t\f]+'
    return t

# Unmatched token
def t_error(t):
    decoded = t.value.encode('unicode_escape').decode('utf-8')
    print("Unrecognized sequence: %s" % decoded)
    t.lexer.skip(1)
