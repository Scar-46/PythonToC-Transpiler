import sys
import pytest
sys.path.insert(0, 'Compiler\ICGenerator')
from Lexer import Lexer, NEW_TOKEN

# Helper function to assert that the lexer produces the expected tokens.
def assert_tokens(input_str, expected_tokens):
    lexer = Lexer()
    lexer.input(input_str)
    tokens = []
    token = lexer.token()
    while token:
        tokens.append(token)
        token = lexer.token()

    assert len(tokens) == len(expected_tokens), f"Expected {len(expected_tokens)} tokens, got {len(tokens)}"
    
    for i, (token, expected) in enumerate(zip(tokens, expected_tokens)):
        assert token.type == expected.type, f"Token type mismatch at index {i}"
        assert token.lineno == expected.lineno, f"Token line number mismatch at index {i}"

# Test keywords
def test_keywords():
    input_str = "if elif else while break"
    expected_tokens = [
        NEW_TOKEN("IF", 0),
        NEW_TOKEN("ELIF", 0),
        NEW_TOKEN("ELSE", 0),
        NEW_TOKEN("WHILE", 0),
        NEW_TOKEN("BREAK", 0),
        NEW_TOKEN("ENDMARKER", 0)
    ]
    assert_tokens(input_str, expected_tokens)

# Test operators
def test_operators():
    input_str = "== != >= <= + - * / // % **"
    expected_tokens = [
        NEW_TOKEN("EQUALITY", 0),
        NEW_TOKEN("INEQUALITY", 0),
        NEW_TOKEN("GREATER_EQUAL", 0),
        NEW_TOKEN("LESSER_EQUAL", 0),
        NEW_TOKEN("PLUS", 0),
        NEW_TOKEN("MINUS", 0),
        NEW_TOKEN("STAR", 0),
        NEW_TOKEN("SLASH", 0),
        NEW_TOKEN("DOUBLE_SLASH", 0),
        NEW_TOKEN("PERCENT", 0),
        NEW_TOKEN("DOUBLE_STAR", 0),
        NEW_TOKEN("ENDMARKER", 0)
    ]
    assert_tokens(input_str, expected_tokens)

# Test assignment operators
def test_assignment_operators():
    input_str = "= += -= *= /= //= %= **="
    expected_tokens = [
        NEW_TOKEN("ASSIGNMENT", 0),
        NEW_TOKEN("ADDITION_ASSIGNMENT", 0),
        NEW_TOKEN("SUBTRACTION_ASSIGNMENT", 0),
        NEW_TOKEN("MULTIPLICATION_ASSIGNMENT", 0),
        NEW_TOKEN("DIVISION_ASSIGNMENT", 0),
        NEW_TOKEN("FLOOR_DIVISION_ASSIGNMENT", 0),
        NEW_TOKEN("MODULO_ASSIGNMENT", 0),
        NEW_TOKEN("EXPONENTIATION_ASSIGNMENT", 0),
        NEW_TOKEN("ENDMARKER", 0)
    ]
    assert_tokens(input_str, expected_tokens)

# Test literals
def test_literals():
    # Test for normal strings
    input_str = "\"Hello \\\" \\'\" 'World'"
    expected_tokens = [
        NEW_TOKEN("STRING", 0),
        NEW_TOKEN("STRING", 0),
        NEW_TOKEN("ENDMARKER", 0)
    ]
    assert_tokens(input_str, expected_tokens)

    # Test for numbers
    input_str = '012 35.67 0x1A 0o17 0b1010'
    expected_tokens = [
        NEW_TOKEN("NUMBER", 0),
        NEW_TOKEN("F_NUMBER", 0),
        NEW_TOKEN("HEX_NUMBER", 0),
        NEW_TOKEN("OCT_NUMBER", 0),
        NEW_TOKEN("BIN_NUMBER", 0),
        NEW_TOKEN("ENDMARKER", 0)
    ]
    assert_tokens(input_str, expected_tokens)

    # Test for triple strings
    input_str = '"""Triple\nQuote""" \'\'\'Another Triple\'\'\' """Hello " """ \'\'\'Hello \' \'\'\''
    expected_tokens = [
        NEW_TOKEN("TRIPLE_STRING", 0),
        NEW_TOKEN("TRIPLE_STRING", 0),
        NEW_TOKEN("TRIPLE_STRING", 0),
        NEW_TOKEN("TRIPLE_STRING", 0),
        NEW_TOKEN("ENDMARKER", 0)
    ]
    assert_tokens(input_str, expected_tokens)

# Test identifiers
def test_identifiers():
    input_str = "variable_name another_var func012"
    expected_tokens = [
        NEW_TOKEN("IDENTIFIER", 0),
        NEW_TOKEN("IDENTIFIER", 0),
        NEW_TOKEN("IDENTIFIER", 0),
        NEW_TOKEN("ENDMARKER", 0)
    ]
    assert_tokens(input_str, expected_tokens)

# Test indentation
def test_indentation():
    input_str = "def foo():\n\tif True:\n\t\tpass\n\treturn" 
    expected_tokens = [
        NEW_TOKEN("DEF", 0),
        NEW_TOKEN("IDENTIFIER", 0),
        NEW_TOKEN("L_PARENTHESIS", 0),
        NEW_TOKEN("R_PARENTHESIS", 0),
        NEW_TOKEN("COLON", 0),
        NEW_TOKEN("NEWLINE", 0),
        NEW_TOKEN("INDENT", 1),
        NEW_TOKEN("IF", 1),
        NEW_TOKEN("TRUE", 1),
        NEW_TOKEN("COLON", 1),
        NEW_TOKEN("NEWLINE", 1),
        NEW_TOKEN("INDENT", 2),
        NEW_TOKEN("PASS", 2),
        NEW_TOKEN("NEWLINE", 2),
        NEW_TOKEN("DEDENT", 3),
        NEW_TOKEN("RETURN", 3),
        NEW_TOKEN("NEWLINE", 3),
        NEW_TOKEN("DEDENT", 3),
        NEW_TOKEN("ENDMARKER", 3)
    ]
    assert_tokens(input_str, expected_tokens)
