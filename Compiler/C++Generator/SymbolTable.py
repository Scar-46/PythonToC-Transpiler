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

    def lookup(self, name):
        for scope in reversed(self.scopes):
            if name in scope:
                return scope[name]
        return None

    def add_symbol(self, name, symbol_type): #TODO: This should not add if it is already in the table
        current_scope = self.scopes[-1]
        current_scope[name] = {"type": symbol_type}

    def add_symbol_over(self, name, symbol_type):
        current_scope = self.scopes[-2]
        current_scope[name] = {"type": symbol_type}

    def get_symbol(self, name):

        return self.lookup(name)

    def print_table(self):
        print("Symbol Table:")
        for i, scope in enumerate(self.scopes):
            print(f"Scope {i}: {scope}")
