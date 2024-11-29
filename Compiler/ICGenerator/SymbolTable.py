class SymbolTable:
    def __init__(self):
        self.scopes = [{}]  # Initialize with a global scope

    def enter_scope(self):
        self.scopes.append({})

    def exit_scope(self):
        if len(self.scopes) > 1:
            self.scopes.pop()
        else:
            raise RuntimeError("Cannot exit the global scope.")

    def lookup(self, name):
        for scope in reversed(self.scopes):
            if name in scope:
                return scope[name]
        raise KeyError(f"Symbol '{name}' not found.")

    def add_symbol(self, name, symbol_type):
        current_scope = self.scopes[-1]
        if name in current_scope:
            raise ValueError(f"Error: The symbol '{name}' is already defined in the current scope.")
        current_scope[name] = {"type": symbol_type}

    def get_symbol(self, name):

        return self.lookup(name)

    def print_table(self):
        print("Symbol Table:")
        for i, scope in enumerate(self.scopes):
            print(f"Scope {i}: {scope}")
