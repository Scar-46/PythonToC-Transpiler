from Parser import Parser
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
        exit(1)

if __name__ == "__main__":
    main()
