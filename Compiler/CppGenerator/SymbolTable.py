class SymbolTable:
    def __init__(self):
        self.scopes = [{}]
        self.class_name = []

    def enter_scope(self):
        self.scopes.append({})

    def exit_scope(self):
        if len(self.scopes) > 1:
            self.scopes.pop()
        else:
            raise RuntimeError("Cannot exit the global scope.")

    def add_class(self, name):
        self.class_name.append(name)

    def get_class(self):
        return self.class_name[-1]

    def pop_class(self):
        return self.class_name.pop()
    
    def add_symbol(self, name, symbol_type):
        for scope in reversed(self.scopes):
            if name in scope:
                return
        current_scope = self.scopes[-1]
        current_scope[name] = {"type": symbol_type}

    def add_symbol_over(self, name, symbol_type, params=None):
        current_scope = self.scopes[-2]
        if symbol_type == "function":
            current_scope[name] = {"type": symbol_type, "parameters": params}
        else:
            current_scope[name] = {"type": symbol_type}

    def exit_and_declare(self, indent_level):
        variables = self.scopes.pop()
        indent = '    ' * (indent_level)
        declarations = ""
        for var, details in variables.items():
            if details["type"] == "function" and len(self.class_name) == 0:
                declarations += f"\n{indent}var {var}({details["parameters"]});"
            elif details["type"] == "variable":
                declarations += f"\n{indent}var {var};"
        return declarations

    def get_symbol(self, name):
        return self.lookup(name)

    def print_table(self):
        print("Symbol Table:")
        for i, scope in enumerate(self.scopes):
            print(f"Scope {i}: {scope}")