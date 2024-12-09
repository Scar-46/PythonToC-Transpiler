import pytest
from ICGenerator.Parser import Parser
from ICGenerator.common import error_logger

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

@pytest.fixture(autouse=True)
def clear_errors():
    error_logger.clear_errors()
    yield
    error_logger.clear_errors()

def parse_code(parser, code):
    try:
        return parser.parse(code)
    except SyntaxError as e:
        pytest.fail(f"Parser raised SyntaxError: {e}")

# The parser should allow empty strings
def test_empty_input(parser):
    code = ''
    parse_code(parser, code)
    assert error_logger.error_count() > 0

# Function call statement with newline at the end
def test_input_with_newline(parser):
    code = 'print("Hello World")\n'
    parse_code(parser, code)
    assert error_logger.error_count() == 0

def test_input_without_newline(parser):
    code = 'print("Hello World")'
    parse_code(parser, code)
    assert error_logger.error_count() == 0


# Invalid function call statement
def test_invalid_input(parser):
    code = 'print("Hello"'
    parse_code(parser, code)
    assert error_logger.error_count() > 0

# Invalid function call statement
def test_newline_input(parser):
    code = '\n'
    parse_code(parser, code)
    assert error_logger.error_count() == 0

def test_assigment(parser):
    code = 'hola, adios = greetings = ("Hello", "Goodbye")'
    parse_code(parser, code)
    assert error_logger.error_count() == 0

def test_target_list_in_for(parser):
    code = 'for hola, adios in range(10):\n\tprint("Salut!")'
    parse_code(parser, code)
    assert error_logger.error_count() == 0


def test_tuple(parser):
    code = "a,b = (4, 10)"
    parse_code(parser, code)
    assert error_logger.error_count() == 0

def test_tuple(parser):
    code = "a.foo()"
    parse_code(parser, code)
    assert error_logger.error_count() == 0

# Test case for reading from a file
def test_read_file(parser):
    code = read_file("ICGenerator/Tests/sample1.py")
    parse_code(parser, code)
    assert error_logger.error_count() == 0
