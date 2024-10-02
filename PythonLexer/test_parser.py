import pytest
from Parser import Parser

# Test case for simple valid input
def test_valid_input():
    parser = Parser()
    
    # A simple Python-like code snippet (could be enhanced based on your grammar)
    code = 'print("Hello World")\n'
    
    # Ensure that no syntax errors occur during parsing
    try:
        result = parser.parse(code)
        assert result is not None  # Ensure that the result is not None (depends on how your parser returns values)
    except SyntaxError:
        pytest.fail("Parser raised SyntaxError unexpectedly!")

# Test case for empty input
def test_empty_input():
    parser = Parser()
    code = ''  # Empty input should be handled without issues
    
    # Parse and ensure no errors occur
    try:
        result = parser.parse(code)
        assert result is not None
    except SyntaxError:
        pytest.fail("Parser raised SyntaxError for empty input!")

# Test case for input with a newline at the end (to check if ENDMARKER is handled correctly)
def test_input_with_newline():
    parser = Parser()
    code = 'print("Hello World")\n'
    
    try:
        result = parser.parse(code)
        assert result is not None
    except SyntaxError:
        pytest.fail("Parser raised SyntaxError for input with newline!")

# Test case for handling a SyntaxError
def test_invalid_input():
    parser = Parser()
    code = 'print("Hello'  # Missing closing quote to introduce syntax error
    
    with pytest.raises(SyntaxError):
        parser.parse(code)

