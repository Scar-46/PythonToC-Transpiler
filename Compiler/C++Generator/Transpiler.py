import sys
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

def main():
    code = read_file(sys.argv[1])
    parser = Parser()
    generator = CodeGenerator()

    ast = parser.parse(code)
    generator.visit(ast)
    print(''.join(generator.code))

    if error_logger.error_count() <= 0:
        exit_code = 0
        # TODO: make this message prettier
        print("############# File transpiled successfully! #############")
    else:
        exit_code = 1
        error_logger.print_error(sys.argv[1])
    exit(exit_code)

if __name__ == "__main__":
    main()
