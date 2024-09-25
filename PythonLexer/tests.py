import unittest
from Lexer import Lexer, NEW_TOKEN

class TestLexer(unittest.TestCase):
    
    def setUp(self):
        # Initialize the lexer
        self.lexer = Lexer()

    def assertTokens(self, input_str, expected_tokens):
        """
        Helper method to assert that the lexer produces the expected tokens.
        """
        self.lexer.input(input_str)
        tokens = []
        token = self.lexer.token()
        while token:
            tokens.append(token)
            token = self.lexer.token()
        
        # Assert token types and values match the expected sequence
        self.assertEqual(len(tokens), len(expected_tokens), f"Expected {len(expected_tokens)} tokens, got {len(tokens)}")
        
        for i, (token, expected) in enumerate(zip(tokens, expected_tokens)):
            with self.subTest(i=i):
                self.assertEqual(token.type, expected.type, f"Token type mismatch at index {i}")
                self.assertEqual(token.lineno, expected.lineno, f"Token line number mismatch at index {i}")

    def test_keywords(self):
        # Test for reserved keywords
        input_str = "if elif else while break"
        expected_tokens = [
            NEW_TOKEN("IF", 0),
            NEW_TOKEN("ELIF", 0),
            NEW_TOKEN("ELSE", 0),
            NEW_TOKEN("WHILE", 0),
            NEW_TOKEN("BREAK", 0),
            NEW_TOKEN("ENDMARKER", 0)
        ]
        self.assertTokens(input_str, expected_tokens)

    def test_operators(self):
        # Test for different operators
        input_str = "== != >= <= + - * / // % **"
        expected_tokens = [
            NEW_TOKEN("EQUALITY", 0),
            NEW_TOKEN("INEQUALITY", 0),
            NEW_TOKEN("GREATER_EQUAL", 0),
            NEW_TOKEN("LESSER_EQUAL", 0),
            NEW_TOKEN("SUM", 0),
            NEW_TOKEN("SUBTRACTION", 0),
            NEW_TOKEN("PRODUCT", 0),
            NEW_TOKEN("DIVISION", 0),
            NEW_TOKEN("INTEGER_DIVISION", 0),
            NEW_TOKEN("MODULUS", 0),
            NEW_TOKEN("EXPONENTIATION", 0),
            NEW_TOKEN("ENDMARKER", 0)
        ]
        self.assertTokens(input_str, expected_tokens)
    
    def test_assignment_operators(self):
        # Test for different operators
        input_str = "= += -= *= /= //= %= **="
        expected_tokens = [
            NEW_TOKEN("ASSIGNMENT", 0),
            NEW_TOKEN("SUM_ASSIGNMENT", 0),
            NEW_TOKEN("SUBTRACTION_ASSIGNMENT", 0),
            NEW_TOKEN("PRODUCT_ASSIGNMENT", 0),
            NEW_TOKEN("DIVISION_ASSIGNMENT", 0),
            NEW_TOKEN("INTEGER_DIVISION_ASSIGNMENT", 0),
            NEW_TOKEN("MODULUS_ASSIGNMENT", 0),
            NEW_TOKEN("EXPONENTIATION_ASSIGNMENT", 0),
            NEW_TOKEN("ENDMARKER", 0)
        ]
        self.assertTokens(input_str, expected_tokens)

    def test_literals(self):
        # Test for string literals
        input_str = '"Hello" \'World\''
        expected_tokens = [
            NEW_TOKEN("STRING", 0),
            NEW_TOKEN("STRING", 0),
            NEW_TOKEN("ENDMARKER", 0)
        ]
        self.assertTokens(input_str, expected_tokens)

        # Test for integer and floating-point literals
        input_str = '012 35.67'
        expected_tokens = [
            NEW_TOKEN("I_NUMBER", 0),
            NEW_TOKEN("F_NUMBER", 0),
            NEW_TOKEN("ENDMARKER", 0)
        ]
        self.assertTokens(input_str, expected_tokens)

    def test_identifiers(self):
        # Test for identifiers
        input_str = "variable_name another_var func012"
        expected_tokens = [
            NEW_TOKEN("IDENTIFIER", 0),
            NEW_TOKEN("IDENTIFIER", 0),
            NEW_TOKEN("IDENTIFIER", 0),
            NEW_TOKEN("ENDMARKER", 0)
        ]
        self.assertTokens(input_str, expected_tokens)

    def test_indentation(self):
        # Test for indent and dedent tokens
        input_str = "def foo():\n\tif True:\n\t\tpass\n\treturn" 
        expected_tokens = [
            NEW_TOKEN("DEF", 0),
            NEW_TOKEN("IDENTIFIER", 0),
            NEW_TOKEN("L_PARENTHESIS", 0),
            NEW_TOKEN("R_PARENTHESIS", 0),
            NEW_TOKEN("COLON", 0),
            NEW_TOKEN("INDENT", 1),
            NEW_TOKEN("IF", 1),
            NEW_TOKEN("TRUE", 1),
            NEW_TOKEN("COLON", 1),
            NEW_TOKEN("INDENT", 2),
            NEW_TOKEN("PASS", 2),
            NEW_TOKEN("DEDENT", 3),
            NEW_TOKEN("RETURN", 3),
            NEW_TOKEN("DEDENT", 3),
            NEW_TOKEN("ENDMARKER", 3)
        ]
        self.assertTokens(input_str, expected_tokens)

if __name__ == '__main__':
    unittest.main()