from rich.console import Console
from rich.text import Text
from rich.markup import escape

class ErrorLogger:
    def __init__(self, filename=""):
        self._filename = filename
        self.errors = []
    
    def log_error(self, message, line=None, column=None, source_code=None, error_type="syntax"):
        error_info = {
            "message": message,
            "line": line,
            "column": column,
            "source_code": source_code,
            "error_type": error_type
        }
        self.errors.append(error_info)

    def error_count(self):
        return len(self.errors)
    
    def clear_errors(self):
        self.errors = []

    def print_error(self, filename: str):
        console = Console()
        for error in self.errors:
            message = error['message']
            line = error['line']
            column = error['column']
            source_code = error['source_code']
            error_type = error['error_type']

            # Color-coding the error type
            if error_type == "syntax":
                error_msg_text = Text(f"error[{error_type}]: ", style="bold red")
            elif error_type == "type":
                error_msg_text = Text(f"error[{error_type}]: ", style="bold yellow")
            else:
                error_msg_text = Text(f"error[{error_type}]: ", style="bold magenta")

            error_msg_text.append(message, style="bold white")

            console.print(error_msg_text)
            if not line or not column or not source_code:
                return
            # Highlight the specific line of source code
            source_line = escape(source_code)
            highlighted_line = Text(source_line)
            highlighted_line.stylize("bold", column - 1, column)

            # Display the source line with a caret pointing to the error location
            console.print(f"  --> Line {line}, Column {column}")
            console.print(f"    {source_line}")
            console.print("    " + " " * (column - 1) + "^")
        error_count = len(self.errors)
        console.print(f"[red bold]error[/]: could not transpile '{filename}' due to {error_count} previous error{"s" if error_count > 1 else ""}")