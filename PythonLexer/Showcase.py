import os
from Parser import Parser

def read_file(file_name):
    try:
        with open(file_name, 'r') as file:
            return file.read()
    except FileNotFoundError:
        print(f"File {file_name} not found.")
        exit(1)

test_parser = Parser()
parsed_text = test_parser.parse(read_file("test.txt"))

print(parsed_text)
