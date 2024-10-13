import re
import TokenRules
import ply.lex as lex

# Errors
errorList = []

# Construct new tokens for a given lexer and line (value and position omitted)
def NEW_TOKEN(newType: str, lineno: int):
    token = lex.LexToken()
    token.type = newType
    token.value = None
    token.lineno = lineno
    token.lexpos = 0
    return token

def INDENT(lineno: int):
    return NEW_TOKEN("INDENT", lineno)

def DEDENT(lineno: int):
    return NEW_TOKEN("DEDENT", lineno)

# Identify identations and their respective level for a given lexing stream
def identifyIndentations(token_stream):
    POSSIBLE = 1
    MANDATORY = 2
    FORBIDDEN = 3

    indentation = FORBIDDEN

    for token in token_stream:
        token.must_indent = False

        match token.type:
            case "COLON":
                indentation = POSSIBLE

            case "NEWLINE":
                if indentation == POSSIBLE:
                    indentation = MANDATORY

            case _:
                if token.type != "WHITESPACE":
                    token.must_indent = indentation == MANDATORY
                    indentation = FORBIDDEN
        yield token

# Replace indentations with IDENT and DEDENT tokens for a given token stream
def assignIndentations(token_stream):
    token = None
    expression_depth: int = 0
    scope_depth: int = 0
    previous_scope_depths: list[int] = [0]
    previous_line_start: bool = False
    
    # For every token in the stream:
    # - Keep track of expression and scope depth accordingly
    # - Remove whitespaces and newlines
    # - Place INDENT and DEDENT tokens in their place
    for token in token_stream:
        if (token.type != "NEWLINE" and token.type != "WHITESPACE"):
            if token.must_indent:
                if not (scope_depth > previous_scope_depths[-1]):
                    raise IndentationError(f"expected an indented block on line {token.lineno}")

                previous_scope_depths.append(scope_depth)
                yield INDENT(token.lineno)
            
            elif previous_line_start:
                if scope_depth > previous_scope_depths[-1]:
                    raise IndentationError(f"unexpected indentation on line {token.lineno}")
                elif scope_depth < previous_scope_depths[-1]:
                    try:
                        i = previous_scope_depths.index(scope_depth)
                        for _ in range(i+1, len(previous_scope_depths)):
                            previous_scope_depths.pop()
                            yield DEDENT(token.lineno)
                    except ValueError:
                        raise IndentationError(f"unmatched indentation on line {token.lineno}")
            yield token

        match token.type:
            case "L_PARENTHESIS":
                expression_depth += 1
                previous_line_start = False

            case "R_PARENTHESIS":
                expression_depth -= 1
                previous_line_start = False
            
            case "NEWLINE":
                if (expression_depth <= 0):
                    scope_depth = 0
                    previous_line_start = True
                    yield NEW_TOKEN("NEWLINE", token.lineno)

            case "WHITESPACE":
                if (previous_line_start):
                    scope_depth = len(token.value)

            case _:
                previous_line_start = False
    if len(previous_scope_depths) > 1:
        assert token is not None
        for z in range(1, len(previous_scope_depths)):
            yield NEW_TOKEN("NEWLINE", token.lineno)
            yield DEDENT(token.lineno)
            

# Construct a tab-filtered (INDENT and DEDENT) lexeme stream for a given lexer
def filter(lexer, addEndMarker=True):
    token_stream = iter(lexer.token, None)
    token_stream = identifyIndentations(token_stream)
    token_stream = assignIndentations(token_stream)

    tok = None
    last_token = None
    for tok in token_stream:
        last_token = tok
        yield tok
    
    if last_token and last_token.type != "NEWLINE":
        yield NEW_TOKEN("NEWLINE", 1 if tok is None else tok.lineno)

    if addEndMarker:
        yield NEW_TOKEN("ENDMARKER", 1 if tok is None else tok.lineno)

def empty_input():
    raise NotImplementedError("empty_input")

# Compute column.
#     input is the input text string
#     token is a token instance
def find_column(input, token):
    line_start = input.rfind('\n', 0, token.lexpos) + 1
    return (token.lexpos - line_start) + 1

# Lexer wrapper for Fangless Python 
class Lexer(object):
    def __init__(self):
        self.lexer = lex.lex(
            module=TokenRules, 
            reflags=int(re.MULTILINE)
        )

    def input(self, data: str, addEndMarker=True):
        self.lexer.lineno = 0
        self.lexer.input(data)
        self.input_is_empty = not data
        self.token_stream = filter(self.lexer, addEndMarker)

    def token(self):
        if self.input_is_empty:
            return None
        try:
            return next(self.token_stream)
        except StopIteration:
            return None
           