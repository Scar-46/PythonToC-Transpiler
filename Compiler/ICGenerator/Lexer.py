import re
import TokenRules
import ply.lex as lex
from common import log_error

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
def assignIndentations(token_stream, format_error=True):
    token = None
    expression_depth: int = 0
    scope_depth: int = 0
    previous_scope_depths: list[int] = [0]
    previous_line_start: bool = False
    empty_line: bool = True
    
    # For every token in the stream:
    # - Keep track of expression and scope depth accordingly
    # - Remove whitespaces and newlines
    # - Place INDENT and DEDENT tokens in their place
    for token in token_stream:
        if (token.type != "NEWLINE" and token.type != "WHITESPACE"):
            empty_line = False
            if token.must_indent:
                if not (scope_depth > previous_scope_depths[-1]):
                    if format_error:
                        log_error("expected an indent", "syntax", token)
                    else:
                        raise IndentationError(f"expected an indented at {token.lineno}")

                previous_scope_depths.append(scope_depth)
                yield INDENT(token.lineno)
            
            elif previous_line_start:
                if scope_depth > previous_scope_depths[-1]:
                    if format_error:
                        log_error("unexpected indentation", "syntax", token)
                    else:
                        raise IndentationError(f"unexpected indentation at {token.lineno}")

                elif scope_depth < previous_scope_depths[-1]:
                    try:
                        i = previous_scope_depths.index(scope_depth)
                        for _ in range(i+1, len(previous_scope_depths)):
                            previous_scope_depths.pop()
                            yield DEDENT(token.lineno)
                    except ValueError:
                        if format_error:
                            log_error("unmatched indentation", "syntax", token)
                        else:
                            raise IndentationError(f"unmatched indentation at {token.lineno}")
            yield token

        match token.type:
            case "L_PARENTHESIS" | "L_CB" | "L_SQB":
                expression_depth += 1
                previous_line_start = False

            case "R_PARENTHESIS" | "R_CB" | "R_SQB":
                expression_depth -= 1
                previous_line_start = False
            
            case "NEWLINE":
                if (expression_depth <= 0) and not empty_line:
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
def filter(lexer, addEndMarker=True, format_error=None):
    token_stream = iter(lexer.token, None)
    token_stream = identifyIndentations(token_stream)
    token_stream = assignIndentations(token_stream, format_error=format_error)

    tok = None
    for tok in token_stream:
        yield tok

    if addEndMarker:
        yield NEW_TOKEN("ENDMARKER", 1 if tok is None else tok.lineno)

# Lexer wrapper for Fangless Python 
class Lexer(object):
    def __init__(self, format_error=True):
        self.lexer = lex.lex(
            module=TokenRules, 
            reflags=int(re.MULTILINE)
        )
        self.format_error = format_error

    @property
    def lexer(self):
        return self._lexer
    
    @lexer.setter
    def lexer(self, value):
        self._lexer = value
    
    def input(self, data: str, addEndMarker=True):
        self.lexer.lineno = 0
        self.lexer.input(data)
        self.input_is_empty = not data
        self.token_stream = filter(self.lexer, addEndMarker, self.format_error)

    def token(self):
        if self.input_is_empty:
            return None
        try:
            return next(self.token_stream)
        except StopIteration:
            return None
           