class SymbolTable:
    def __init__(self):
        self.scopes = [{}]

    def enter_scope(self):
        self.scopes.append({})

    def exit_scope(self):
        if len(self.scopes) > 1:
            self.scopes.pop()
        else:
            raise RuntimeError("Cannot exit the global scope.")

    def add_symbol(self, name, symbol_type):
        for scope in reversed(self.scopes):
            if name in scope:
                return
        current_scope = self.scopes[-1]
        current_scope[name] = {"type": symbol_type}

    def add_symbol_over(self, name, symbol_type):
        current_scope = self.scopes[-2]
        current_scope[name] = {"type": symbol_type}

    def exit_and_declare(self, indent_level):
        variables = self.scopes.pop()
        indent = '    ' * (indent_level)
        declarations = ""
        for var, details in variables.items():
            if details["type"] == "function":
                declarations += f"\n{indent}auto {var}();"
            else:
                declarations += f"\n{indent}auto {var};"
        return declarations

    def get_symbol(self, name):

        return self.lookup(name)

    def print_table(self):
        print("Symbol Table:")
        for i, scope in enumerate(self.scopes):
            print(f"Scope {i}: {scope}")
