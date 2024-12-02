import sys
import os

sys.path.insert(0, 'Compiler/ICGenerator')

from Parser import Parser
from common import error_logger

from Generator import CodeGenerator


def read_file(file_name):
    try:
        with open(file_name, 'r') as file:
            return file.read()
    except FileNotFoundError:
        print(f"File {file_name} not found.")
        exit(1)

def write_to_file(folder_name, file_name, content):
    try:
        os.makedirs(folder_name, exist_ok=True)
        file_path = os.path.join(folder_name, file_name)
        with open(file_path, 'w') as file:
            file.write(content)
    except Exception as e:
        print(f"Error writing to file {file_name}: {e}")
        exit(1)

def main():
    code = read_file(sys.argv[1])
    parser = Parser()
    generator = CodeGenerator()

    ast = parser.parse(code)
    transpiled_code = generator.visit(ast)
    
    if error_logger.error_count() <= 0:
        output_folder = "Output"
        output_file = "CodeTranspiled.cpp"
        write_to_file(output_folder, output_file, transpiled_code)
        print(f"############# File transpiled successfully and saved in '{output_folder}/{output_file}'! #############")
        exit(0)
    else:
        error_logger.print_error(sys.argv[1])
        exit(1)

if __name__ == "__main__":
    main()
