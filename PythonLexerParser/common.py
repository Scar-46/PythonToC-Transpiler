from ErrorLogger import LOGGER

# Compute column.
def find_column(input, token):
    line_start = input.rfind('\n', 0, token.lexpos) + 1
    return (token.lexpos - line_start) + 1

def get_input(input, token):
    line_start = input.rfind('\n', 0, token.lexpos) + 1
    line_end = input.find('\n', token.lexpos)
    if line_end == -1:
        line_end = len(input)
    return input[line_start:line_end]

def log_error(message: str, type: str, token=None):
    if token:
        try:
            lexer = token.lexer.lexer
        except AttributeError:
            lexer = token.lexer
        LOGGER.log_error(
            message=message,
            line=token.lineno + 1,
            column=find_column(lexer.lexdata, token),
            source_code=get_input(lexer.lexdata, token),
            error_type=type
        )
    else:
        LOGGER.log_error(
            message=message,
            error_type=type
        )