import sys
sys.path.insert(0, 'PythonLexerParser')

import pytest
from Parser import Parser

def read_file(file_name):
    try:
        with open(file_name, 'r') as file:
            return file.read()
    except FileNotFoundError:
        print(f"File {file_name} not found.")
        exit(1)

@pytest.fixture
def parser():
    return Parser()

def parse_code(parser: Parser, code: str):
    try:
        return parser.parse(code)
    except SyntaxError as e:
        pytest.fail(f"Parser raised SyntaxError: {e}")

# The parser should allow empty strings
def test_empty_input(parser: Parser):
    code = ''
    with pytest.raises(SyntaxError, match="Syntax error at EOF"):
        parser.parse(code) 


# Function call statement with newline at the end
def test_input_with_newline(parser: Parser):
    code = 'print("Hello World")\n'
    result = parse_code(parser, code)
    assert result is None

def test_input_without_newline(parser: Parser):
    code = 'print("Hello World")'
    result = parse_code(parser, code)
    assert result is None


# Invalid function call statement
def test_invalid_input(parser: Parser):
    code = 'print("Hello"'
    with pytest.raises(SyntaxError):
        parser.parse(code)

# Invalid trailing newline
def test_newline_input(parser: Parser):
    code = '\n'
    result = parse_code(parser, code)
    assert result is None

def test_assigment(parser: Parser):
    code = 'hola, adios = greetings = ("Hello", "Goodbye")'
    result = parse_code(parser, code)
    assert result is None

def test_ternary_assigment(parser: Parser):
    code = 'result = "Positive" if x > 0 else "Negative"'
    result = parse_code(parser, code)
    print(result)
    assert result is None

def test_target_list_in_for(parser: Parser):
    code = 'for hola, adios in range(10):\n\tprint("Salut!")'
    result = parse_code(parser, code)
    assert result is None

# Test case for reading from a file
def test_read_file(parser: Parser):
    code = read_file("PythonLexerParser/Tests/sample.py")
    result = parse_code(parser, code)
    assert result is None
