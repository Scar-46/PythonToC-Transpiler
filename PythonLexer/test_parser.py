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

def parse_code(parser, code):
    try:
        return parser.parse(code)
    except SyntaxError as e:
        pytest.fail(f"Parser raised SyntaxError: {e}")

# The parser should allow empty strings
def test_empty_input(parser):
    code = ''
    with pytest.raises(SyntaxError, match="Syntax error at EOF"):
        parser.parse(code) 


# Function call statement with newline at the end
def test_input_with_newline(parser):
    code = 'print("Hello World")\n'
    result = parse_code(parser, code)
    assert result is None

def test_input_without_newline(parser):
    code = 'print("Hello World")'
    result = parse_code(parser, code)
    assert result is None


# Invalid function call statement
def test_invalid_input(parser):
    code = 'print("Hello"'
    with pytest.raises(SyntaxError):
        parser.parse(code)

# Test invalid assigment
def test_invalid_assigment(parser):
    code = '20 = 47'
    with pytest.raises(SyntaxError):
        parser.parse(code)


def test_assigment(parser):
    code = 'a,b = 20,50'
# Invalid function call statement
def test_newline_input(parser):
    code = '\n'
    result = parse_code(parser, code)
    assert result is None

# Test case for reading from a file
def test_read_file(parser):
    code = read_file("PythonLexer\sample.py")
    result = parse_code(parser, code)
    assert result is None
