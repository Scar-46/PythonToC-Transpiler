import os
import Lexer
from prettytable import PrettyTable

def read_file(file_name):
    try:
        with open(file_name, 'r') as file:
            return file.read()
    except FileNotFoundError:
        print(f"File {file_name} not found.")
        exit(1)

test_lexer = Lexer.Lexer()
test_lexer.input(read_file("test.txt"))

lexemes = PrettyTable(['Line #', 'Character #', 'Type', 'Lexeme'])

try:
    tok = test_lexer.token()
    while tok:
        lexemes.add_row([tok.lineno, tok.lexpos, tok.type, tok.value])
        tok = test_lexer.token()
except IndentationError:
    #TODO: manages lex errors.
    pass

    
   

print(lexemes)
