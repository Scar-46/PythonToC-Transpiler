import sys
sys.path.insert(0, 'Compiler/ICGenerator')

from node import Node

class CodeGenerator():
    def __init__(self):
        self.code = []

    def emit(self, code_str):
        self.code.append(code_str)

    def get_code(self):
        return ''.join(self.code)

    def visit(self, node):
        method_name = f"visit_{node.node_type}"
        visitor = getattr(self, method_name, self.error_visit)
        visitor(node)
        
    def error_visit(self, node):
        raise Exception(f"No visit_{node.node_type} method")

    def visit_function_def(self, node):
        self.emit(f"void {node.value}(")  # TODO: Fix type
        self.visit(node.children[0])  # Visit parameters
        self.emit(") {\n")
        self.visit(node.children[1])  # Visit block
        self.emit("}\n")

    def visit_parameters(self, node):
        params = []
        for param in node.children:
            params.append(f"int {param.value}")  # TODO: Fix type
        self.emit(", ".join(params))

    def visit_block(self, node):
        self.emit("    ")  # Indent block
        for stmt in node.children:
            print(stmt.node_type)
            self.visit(stmt)
            self.emit("\n")

    def visit_return(self, node):
        self.emit("return ")
        self.visit(node.children[0])  # Visit expression

    def visit_binary_operation(self, node): # Maybe can be deleted
        self.visit(node.children[0])  # Visit left operand
        self.emit(f" {node.value} ")
        self.visit(node.children[1])  # Visit right operand

    def visit_identifier(self, node):
        self.emit(node.value)

    def visit_number(self, node):
        self.emit(str(node.value))

    def visit_group(self, node):
        self.emit("(")
        for child in node.children:
            self.visit(child)
        self.emit(")")

    def visit_assign_chain(self, node):
        target_nodes = node.children[0].children  # This is the target list, e.g., [n_1]
        value_node = node.children[1]  # The right-hand side, e.g., 1

        # Check if there are multiple targets (for tuple-like assignments)
        if len(target_nodes) > 1:
            # Handle tuple unpacking (e.g., a, b, c = 1, 2, 3)
            self.emit("auto [")
            targets = [target_node.value for target_node in target_nodes]
            self.emit(", ".join(targets))
            self.emit("] = ")
            self.visit(value_node)
        else:
            # Regular single assignment (like n_1 = 1)
            target = target_nodes[0].value  # Single target variable
            self.emit(f"{target} = ")
            self.visit(value_node)

   # def visit_target_list(self, node):
        

    def visit_for_stmt(self, node):
        target = node['children'][0]['value']
        self.emit(f"for {target} in ")
        self.visit(node['children'][1])  # Visit range or iterable
        self.emit(":\n")
        self.visit(node['children'][2])  # Visit loop body
