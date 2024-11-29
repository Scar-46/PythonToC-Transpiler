from Parser import Parser
from common import error_logger
import sys

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
    try:
        result = parser.parse(code)
        print(result)
    except Exception as e:
        print(f"Unexpected Error during parsing: {e}")
    if error_logger.error_count() <= 0:
        exit_code = 0
        # TODO: make this message prettier
        print("############# File parsed successfully! #############")
    else:
        exit_code = 1
        error_logger.print_error(sys.argv[1])
    exit(exit_code)

if __name__ == "__main__":
    main()
