import re # Regex module
import TokenRules # Token rules
import ply.lex as lex # Lexing library

# ------------ Identation fixing ------------
# Construct new tokens for a given lexer and line (value and position omitted)
def NEW_TOKEN(newType: str, lineno: int):
    token = lex.LexToken()
    token.type = newType
    token.value = None
    token.lineno = lineno
    token.lexpos = None
    return token

# Consruct virtual indentation tokens
# - Entering new identation level
def INDENT(lineno: int):
    return NEW_TOKEN("INDENT", lineno)
# - Existing current identation level
def DEDENT(lineno: int):
    return NEW_TOKEN("DEDENT", lineno)

# Identify identations and their respective level for a given lexing stream
def identifyIndentations(token_stream):
    # Keep track on whether a deeper indentation level is forbidden, possibly
    # mandatory or actually mandatory for a given token
    POSSIBLE = 1
    MANDATORY = 2
    FORBIDDEN = 3

    indentation = FORBIDDEN

    # For each token
    for token in token_stream:
        # Assume forbidden indentation unless proven otherwise
        token.must_indent = False

        match token.type:
            # Colons allow for potential future indentation
            case "D_COLON":
                indentation = POSSIBLE

            # Newlines mandate indentation if it was potentially allowed
            # beforehand
            case "NEWLINE":
                if indentation == POSSIBLE:
                    indentation = MANDATORY

            # Any other token besides whitespace seals the deal
            # and nullifies deeper indentation unless allowed otherwise 
            case _:
                if token.type != "WHITESPACE":
                    token.must_indent = indentation == MANDATORY
                    indentation = FORBIDDEN

        yield token

# Replace indentations with IDENT and DEDENT tokens for a given token stream
def assignIndentations(token_stream):
    # Keep track of..

    # Current nested expression level (nested expressions nullify new lines) 
    expression_depth: int = 0

    # The current scope level of the expression and their parent scopes'
    scope_depth: int = 0
    previous_scope_depths: list[int] = [0]
    
    # Whether the previous token was the first on the current line
    previous_line_start: bool = False
    
    # For every token in the stream:
    # - Keep track of expression and scope depth accordingly
    # - Remove whitespaces and newlines
    # - Place INDENT and DEDENT tokens in their place
    for token in token_stream:
        # Wrap a given token (except whitespaces and newlines) between INDENTs 
        # and DEDENTs
        if (token.type != "NEWLINE" and token.type != "WHITESPACE"):
            # If the token must be indented, append its greater scope onto the stack
            # and insert an INDENT
            if token.must_indent:
                if not (scope_depth > previous_scope_depths[-1]):
                    raise IndentationError("Missing indentation in new block")

                previous_scope_depths.append(scope_depth)
                yield INDENT(token.lineno)
            
            # If the token musn't be indented yet begins at the start, pop all greater
            # scopes from the stack and insert a DEDENT for each
            elif previous_line_start:
                if scope_depth > previous_scope_depths[-1]:
                    raise IndentationError("Excessive indentation in new block")
                elif scope_depth < previous_scope_depths[-1]:
                    try:
                        i = previous_scope_depths.index(scope_depth)
                        for _ in range(i+1, len(previous_scope_depths)):
                            yield DEDENT(token.lineno)
                            previous_scope_depths.pop()
                    except ValueError:
                        raise IndentationError("Unmatched indentation in new block")

            yield token

        # Update the line start flag and expression and scope depth given
        # the current token and previous information on them 
        match token.type:
            case "D_LEFT_PARENTHESIS":
                expression_depth += 1
                previous_line_start = False

            case "D_RIGHT_PARENTHESIS":
                expression_depth -= 1
                previous_line_start = False
            
            case "NEWLINE":
                if (expression_depth <= 0):
                    scope_depth = 0
                    previous_line_start = True

            case "WHITESPACE":
                if (previous_line_start):
                    scope_depth = len(token.value)

            case _:
                previous_line_start = False

# Construct a tab-filtered (INDENT and DEDENT) lexeme stream for a given lexer
def filter(lexer, addEndMarker=True):
    # Create a token stream
    token_stream = iter(lexer.token, None)
    # Mark the mandatory and optional indentations
    token_stream = identifyIndentations(token_stream)
    # Replace the whitespaces and newlines with INDENT and DEDENT for the given markings
    token_stream = assignIndentations(token_stream)

    # Forward every token from the filtered steam 
    tok = None
    for tok in token_stream:
        yield tok

    # Return an end marker given the last line processed by the lexer
    if addEndMarker:    
        yield NEW_TOKEN("ENDMARKER", 1 if tok is None else tok.lineno)

# ------------ Lexing ------------
# Errors
errorList = []

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
        self.token_stream = filter(self.lexer, addEndMarker)

    def token(self):
        try:
            return next(self.token_stream)
        except StopIteration:
            return None
           