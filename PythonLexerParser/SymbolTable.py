class SymbolTable:
    def __init__(self):
        self.scopes = [{}]  # Initialize with a global scope

    def enter_scope(self):
        """Enter a new scope by adding an empty dictionary."""
        self.scopes.append({})

    def exit_scope(self):
        """Exit the current scope if it's not the global scope."""
        if len(self.scopes) > 1:
            self.scopes.pop()
        else:
            raise RuntimeError("Cannot exit the global scope.")

    def lookup(self, name):
        """
        Look up a symbol in the current and parent scopes.
        Returns the symbol's information if found.
        """
        for scope in reversed(self.scopes):
            if name in scope:
                return scope[name]
        raise KeyError(f"Symbol '{name}' not found.")

    def add_symbol(self, name, symbol_type):
        """
        Add a symbol to the current scope.
        Raises an error if the symbol already exists in the current scope.
        """
        current_scope = self.scopes[-1]
        if name in current_scope:
            raise ValueError(f"Error: The symbol '{name}' is already defined in the current scope.")
        current_scope[name] = {"type": symbol_type}

    def get_symbol(self, name):
        """
        Retrieve a symbol from any scope.
        Raises an error if the symbol is not found.
        """
        return self.lookup(name)

    def print_table(self):
        """Print the symbol table for all scopes."""
        print("Symbol Table:")
        for i, scope in enumerate(self.scopes):
            print(f"Scope {i}: {scope}")
