from Parser import Parser
from ErrorLogger import ErrorLogger, LOGGER
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
        parser.parse(code)
    except Exception as e:
        print(f"Error during parsing: {e}")
    if LOGGER.error_count() <= 0:
        exit_code = 0
        print("File parsed successfully!")
    else:
        exit_code = 1
        LOGGER.print_error(sys.argv[1])
    exit(exit_code)

if __name__ == "__main__":
    main()
