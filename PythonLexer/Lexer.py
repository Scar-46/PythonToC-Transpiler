import ply.lex as lex
import os


NO_INDENT = 0
MAY_INDENT = 1
MUST_INDENT = 2

errorList = []
tokens = []
# keyword_list = [
#     # Logical Operators
#     'and', 'or','not',
#     # Flow Control structures
#     'if', 'else', 'elif',
#     'for', 'while', 'break',
#     'pass', 'continue',
#     # Definitions
#     'def', 'as', 'class',
#     # Boolean 
#     'True', 'False',
#     # Other
#     'del', 'finally',  'from',
#     'global', 'in', 'is',
#     'None', 'nonlocal', 'raise',
#     'return'
# ]

# RESERVED = {}
# for keyword in keyword_list:
#     name = keyword.upper()
#     RESERVED[keyword] = name
#     tokens.append(name)
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

tokens = [
    'KEYWORD',
    'IDENTIFIER',
    # Operators
    'PLUS',
    'MINUS',
    'TIMES', # They called it star
    'DIVIDE',
    'MOD',
    'POWER',
    'LSHIFT',
    'RSHIFT',
    'LESS',
    'LESS_EQUAL',
    'GREATER',
    'GREATER_EQUAL',
    'EQUAL_EQUAL',
    'NOT_EQUAL',
    'TILDE',
    'AMPER',
    'CIRCUMFLEX',
    'LEFT_SHIFT',
    'RIGHT_SHIFT',
    # Assignment
    'EQUAL',
    'PLUS_EQUAL',
    'MINUS_EQUAL',
    'TIMES_EQUAL',
    'DIVIDE_EQUAL',
    'MOD_EQUAL',
    'POWER_EQUAL',
    'LSHIFT_EQUAL',
    'RSHIFT_EQUAL',
    'AND_EQUAL',
    'OR_EQUAL',
    'XOR_EQUAL',
    'LEFT_SHIFT_EQUAL',
    'RIGHT_SHIFT_EQUAL',
    # Literals
    'L_NUMERICAL',
    'STRING',
    'L_BOOLEAN',
    'O_ARITHMETIC',
    'O_RELATIONAL',
    'O_LOGICAL',
    'O_ASSIGNMENT',
    'O_OTHER',
    # Delimiters
    'LPARENTHESES',
    'RPARENTHESES',
    'LSQB',
    'RSQB',
    'LBRACE',
    'RBRACE',
    'COLON',
    'SEMICOLON',
    'COMA',
    'DOT',
    'AT',
    # Indentation and new line
    'WS',
    'NEWLINE',
    'INDENT',
    'DEDENT',
    'ENDMARKER'
] + list(keywords.values())

# Delimiters
t_COLON = r':'
t_SEMICOLON = r';'
t_COMA = r','
t_DOT = r'\.'
t_AT = r'a' # May need to remove this

# Delimiters
def t_LPARENTHESES(t):
    r'\('
    t.lexer.parenthesisCount += 1
    return t

def t_RPARENTHESES(t):
    r'\)'
    t.lexer.parenthesisCount -= 1
    return t

def t_LSQB(t):
    r'\['
    t.lexer.parenthesisCount += 1
    return t

def t_RSQB(t):
    r'\]'
    t.lexer.parenthesisCount -= 1
    return t

def t_LBRACE(t):
    r'\{'
    t.lexer.parenthesisCount += 1
    return t
def t_RBRACE(t):
    r'\}'
    t.lexer.parenthesisCount -= 1
    return t

def newToken(newType, lineno):
    token = lex.LexToken()
    token.type = newType
    token.value = None
    token.lineno = lineno
    token.lexpos = -100
    return token

def t_IDENTIFIER(t):
    r'[a-zA-Z_][a-zA-Z0-9_]*'
    if t.value in keywords:
        t.type = keywords[t.value]
    return t

def t_error(t):
    #TODO: Dynamically set this value
    file_path = "test.txt"
    file_abs_path = os.path.abspath(file_path)

    error_msg = f"Illegal character '{t.value[0] if t.value[0] != '\n' else '\\n'}' at ({t.lineno}, {t.lexpos})"
    error_msg += f" in file: '{file_abs_path}:{t.lineno}:{t.lexpos}'"

    print(error_msg)
    t.lexer.skip(1)

#ignore comments in source code
def t_comment(t):
	r'\s*\x23[^\n]*'
	pass

def t_STRING(t):
    # TODO: check for escape characters
	r'(\"(\\.|[^\"\n]|(\\\n))*\") | (\'(\\.|[^\'\n]|(\\\n))*\')'
	return t

def t_continueline(t):
    r'\\(\n)+'

# Aims to tokenize all NEWLINE characters that are not inside parenthesis.
def t_NEWLINE(t):
    r'\n+'
    t.lineno += len(t.value)
    if t.lexer.parenthesisCount == 0:
        return t

def t_WS(t):
    r' [ \t\f]+ '
    value = t.value
    value = value.rsplit("\f", 1)[-1]
    pos = 0
    # Converts \t into whitespaces
    while True:
        pos = value.find("\t")
        if pos == -1:
            break
        n = 8 - (pos % 8)
        value = value[:pos] + " " * n + value[pos+1:]
    t.value = value
    if t.lexer.atLineStart and t.lexer.parenthesisCount == 0:
        return t

# Ignore any other whitespace (not after a newline)
# def t_ignore_WS(t):
#     r' [ \t\f]+ '  # Match spaces, tabs, or form feed not preceded by a newline
#     pass  # Ignore it (no return value means it won't be processed as a token)
def INDENT(lineno):
    return newToken("INDENT", lineno)

def DEDENT(lineno):
    return newToken("DEDENT", lineno)

def identifyIndentations(lexer, token_stream):
    lexer.atLineStart = atLineStart = True
    indent = NO_INDENT
    for token in token_stream:
        token.atLineStart = atLineStart
        if token.type == "COLON":
            atLineStart = False
            indent = MAY_INDENT
            token.must_indent = False
        elif token.type == "NEWLINE":
            atLineStart = True
            if indent == MAY_INDENT:
                indent = MUST_INDENT
            token.must_indent = False
        elif token.type == "WS":
            assert token.atLineStart == True
            atLineStart = True
            token.must_indent = False
        else:
            token.must_indent = indent == MUST_INDENT
            atLineStart = False
            indent = NO_INDENT
        yield token
        lexer.atLineStart = atLineStart

def assignIndentations(token_stream):
    levels = [0]
    token = None
    depth = 0
    lastSeenWhitespace = False
    for token in token_stream:
        if token.type == "WS":
            assert depth == 0
            depth = len(token.value)
            lastSeenWhitespace = True
            continue
        if token.type == "NEWLINE":
            depth = 0
            if not (lastSeenWhitespace or token.atLineStart):
                yield token
            continue
        lastSeenWhitespace = False
        if token.must_indent:
            if not (depth > levels[-1]): 
                raise IndentationError(f"IndentationError: expected an indented block at ({token.lineno}. {token.lexpos})")
            levels.append(depth)
            yield INDENT(token.lineno)
        elif token.atLineStart:
            if depth == levels[-1]:
                pass
            elif depth > levels[-1]:
                raise IndentationError(f"IndentationError: unexpected indentation at ({token.lineno}. {token.lexpos})")
            # Token is part of a block on a prior level to this one: MUST add DEDENT
            else:
                try:
                    i = levels.index(depth)
                    for _ in range(i + 1, len(levels)):
                        yield DEDENT(token.lineno)
                        levels.pop()
                except ValueError:
                    raise IndentationError(f"IndentationError: inconsistent indentation at ({token.lineno}. {token.lexpos})")
        yield token

    if len(levels) > 1:
        assert token is not None
        for _ in range(1, len(levels)):
            yield DEDENT(token.lineno)

def filter(lexer, addEndMarker=True):
    token_stream = iter(lexer.token, None)
    token_stream = identifyIndentations(lexer, token_stream)
    token_stream = assignIndentations(token_stream)
    tok = None
    for tok in token_stream:
        yield tok
    if addEndMarker:
        lineno = 1
        if tok is not None:
            lineno = tok.lineno
        yield newToken("ENDMARKER", lineno)

# They create a ply.lex wrapper to handle indentation
class Lexer(object):
    def __init__(self):
        self.lexer = lex.lex()
        self.token_stream = None

    def input(self, data, addEndMarker=True):
        self.lexer.parenthesisCount = 0
        data += "\n"
        self.lexer.input(data)
        self.token_stream = filter(self.lexer, addEndMarker)

    def token(self):
        try:
            return next(self.token_stream)
        except StopIteration:
            return None
        
# End of lexer, probs needs to be in separate file.
def read_file(file_name):
    try:
        with open(file_name, 'r') as file:
            return file.read()
    except FileNotFoundError:
        print(f"File {file_name} not found.")
        exit (1)

#Give the lexer the test file
file_name = 'test.txt'
test_lexer = Lexer()
test_lexer.input(read_file(file_name))

tok = test_lexer.token()
while tok:
    print(tok)
    tok = test_lexer.token()

print ("Lexer finished")