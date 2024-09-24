import ply.lex as lex
import os

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

# List of token names.
tokens = [
    'KEYWORD',
    'IDENTIFIER',
    'L_NUMERICAL',
    'L_STRING',
    'L_BOOLEAN',
    'O_ARITHMETIC',
    'O_RELATIONAL',
    'O_LOGICAL',
    'O_ASSIGNMENT',
    'O_OTHER',
    'D_PARENTHESES',
    'D_BRACKETS',
    'D_BRACES',
    'D_COLON',
    'D_COMMA',
    'D_DOT',
    'D_AT_SIGN',
    'C_ONE_LINE_COMMENT',
    'WHITESPACE',
    'TAB',
    'NEWLINE'
    'IDENT',
    'DEDENT',
] + list(keywords.values())

#TODO: Include INDENT management
def t_NEWLINE(token):
    r'\n+'
    token.lexer.lineno += len(token.value)

def find_column(input_text, token):
    # Find the last newline character before the token
    last_newline = input_text.rfind('\n', 0, token.lexpos)
    
    # If there's no newline, the token is on the first line
    if last_newline < 0:
        column = token.lexpos + 1
    else:
        column = token.lexpos - last_newline
    return column

#TODO: Add colors and fix position translation logic
# Error handling rule
def t_error(t):
    #TODO: Dynamically set this value
    file_path = "test.txt"
    file_abs_path = os.path.abspath(file_path)

    error_msg = f"Illegal character '{t.value[0] if t.value[0] != '\n' else '\\n'}' at ({t.lineno}, {t.lexpos})"
    error_msg += f" in file: '{file_abs_path}:{t.lineno}:{t.lexpos}'"

    print(error_msg)
    t.lexer.skip(1)

# Regular expression rules for keywords/identifiers
def t_IDENTIFIER(t):
    r'[a-zA-Z_][a-zA-Z0-9_]*'
    if t.value in keywords:
        t.type = keywords[t.value]
    return t

# Build the lexer
lexer = lex.lex()


def read_file(file_name):
    try:
        with open(file_name, 'r') as file:
            return file.read()
    except FileNotFoundError:
        print(f"File {file_name} not found.")
        exit(1)

#Give the lexer the test file
file_name = 'test.txt'
lexer.input(read_file(file_name))

# Tokenize
while True:
    tok = lexer.token()
    if not tok:
        break
    print(tok)