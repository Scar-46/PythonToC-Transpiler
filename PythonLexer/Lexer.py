import ply.lex as lex
import sys

NO_INDENT = 0
MAY_INDENT = 1
MUST_INDENT = 2

# ------------ Alphabet ------------
errorList = []
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
    'INDENT',
    'DEDENT',
    'ENDMARKER'
]

def newToken(newType, lineno):
    token = lex.LexToken()
    token.type = newType
    token.value = None
    token.lineno = lineno
    token.lexpos = -100
    return token

def t_INDENT(t):
    r'\s+?'
    return t

def t_DEDENt(t):
    r'\s*-+'
    return t

# TODO(Dwayne): Why would this be necessary
def t_continueline(t):
    r'\\(\n)+'

# Aims to tokenize all NEWLINE characters that are not inside parenthesis.
# TODO(Dwayne): Needs testing.
def t_newline(t):
    r'\n+' # one or more newlines ar the end no?
    t.lineno += len(t.value)
    t.type = "NEWLINE"
    if lexer.parenthesisCount == 0:
        return t

def t_WS(t):
    r" [ \t\f]+ "
    value = t.value
    value = value.rsplit("\f", 1)[-1]
    pos = 0
    while True:
        pos = value.find("\t")
        if pos == -1:
            break
        n = 8 - (pos % 8)
        value = value[:pos] + " " * n + value[pos+1:]
    t.value = value
    if t.lexer.atLinestart and t.lexer.parenthesisCount == 0:
        return t

def INDENT(lineno):
    return newToken("INDENT", lineno)

def DEDENT(leneno):
    return lineno("DEDENT", lineno)

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
            depth = len(token.value)
            lastSeenWhitespace = True
            continue
        if token.type == "NEWLINE":
            depth = 0
            #if lastSeenWhitespace or token.atLineStart:
            if not (lastSeenWhitespace or token.atLineStart):
                yield token
            # yield token
            # continue
        lastSeenWhitespace = False
        if token.must_indent:
            if not (depth > levels[-1]):
                # raise IndentationError("Expected an indented block")
                print(f"Indentation Error in line {token.lineno}.")
                sys.exit()
            levels.append(depth)
            yield INDENT(token.lineno)
        elif token.atLineStart:
            if depth == levels[-1]:
                # raise IndentationError("IndentationError: not in new block")
                print(f"Indentation Error in line {token.lineno}.")
                sys.exit()
            else:
                try:
                    i = levels.index(depth)
                except ValueError:
                    # raise IndentationError("IndentationError: not in new block")
                    print(f"Indentation Error in line {token.lineno}.")
                for _ in range(i + 1, len(levels)):
                    yield DEDENT(token.lineno)
                    levels.pop()
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
    def __init(self):
        self.lexer = lex.lex()
        self.token_stream = None
    def input(self, data, addEndMarker=True):
        self.lexer.parenthesisCount = 0
        data += "\n"
        self.lexer.input(data)
        self.token_stream = filter(self.lexer, addEndMarker)
    def token(self):
        try:
            return self.token_stream.next()
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
