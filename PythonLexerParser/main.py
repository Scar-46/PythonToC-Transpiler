from Parser import Parser
from ErrorLogger import ErrorLogger
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
    error_logger = ErrorLogger(sys.argv[1])
    parser = Parser(error_logger=error_logger)
    try:
        parser.parse(code)
    except Exception as e:
        print(f"Error during parsing: {e}")
    if error_logger.error_count() <= 0:
        exit_code = 0
    else:
        exit_code = 1
        error_logger.print_error()
    exit(exit_code)

if __name__ == "__main__":
    main()
