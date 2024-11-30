import sys
sys.path.insert(0, 'Compiler/ICGenerator')

from node import Node

class CodeGenerator():

    built_in_map = {
        "print": "std::cout <<",
        "len": "std::size",
        "abs": "std::abs",
        "min": "std::min",
        "max": "std::max",
    }

    def __init__(self):
        self.code = []
        self.indent_level = 0
        self.indent = False

    def emit(self, code_str, add_newline=False):

        if self.indent:
            indent = '    ' * self.indent_level
            self.code.append(f"\n{indent}{code_str}")
        else:
            self.code.append(f"{code_str}")
        self.indent = add_newline

    def get_code(self):
        return ''.join(self.code)

    def visit(self, node):
        method_name = f"visit_{node.node_type}"
        visitor = getattr(self, method_name, self.error_visit)
        visitor(node)
        
    def error_visit(self, node):
        raise Exception(f"No visit_{node.node_type} method")

    ###################### Staments methods ######################
    def visit_staments(self, node):
        for child in node.children:
            self.visit(child)

    def visit_function_def(self, node):
        self.emit(f"auto {node.value}(", add_newline=False)
        if node.children[0].node_type == "block":
            self.emit(") {", add_newline=True)
            self.visit(node.children[0])  # Visit block
            self.emit("}", add_newline=True)
        else:
            self.visit(node.children[0])  # Visit parameters
            self.emit(") {", add_newline=True)
            self.visit(node.children[1])  # Visit block
            self.emit("}", add_newline=True)

    def visit_parameters(self, node):
        params = []
        for param in node.children:
            if param.value != "self":  # Ignore 'self'
                params.append(f"auto {param.value}")  # TODO: Fix type (you might want to add type inference here)
        self.emit(", ".join(params), add_newline=False)


    def visit_block(self, node):
        self.indent_level += 1
        for stmt in node.children:
            self.visit(stmt)
        self.indent_level -= 1

    def visit_return(self, node):
        self.emit("return ", add_newline=False)
        self.visit(node.children[0])  # Visit expression
        self.emit(";", add_newline=True)

    def visit_binary_operation(self, node):
        self.visit(node.children[0])  # Visit left operand
        self.emit(f" {node.value} ", add_newline=False)  # Add operator with spaces
        self.visit(node.children[1])  # Visit right operand

    #------------------------ CLASS ------------------------
    def visit_class_def(self, node): #TODO: Check how to traslate _init_
        inheritance = ""
        if len(node.children) > 1 and node.children[0].node_type == 'identifier':
            inheritance = f" : public {node.children[0].value}"

        # Start class definition with inheritance (if any)
        self.emit(f"class {node.value}{inheritance} {{", add_newline=True)
        self.emit("public:", add_newline=True)
        if len(node.children) > 1:
            self.visit(node.children[1])
        else:
            self.visit(node.children[0])

    def visit_attribute_access(self, node):
        if node.children[0].value == "self":
            self.emit("this", add_newline=False)
            self.emit(f"->{node.value}", add_newline=False)
        else:
            self.visit(node.children[0])
            self.emit(f".{node.value}", add_newline=False)

    #------------------------ IF ------------------------
    def visit_if_stmt(self, node):
        self.emit("if (", add_newline=False)
        self.visit(node.children[0])  # Condition
        self.emit(") {", add_newline=True)

        if len(node.children) > 1:
            self.visit(node.children[1])  # If block
        self.emit("}",add_newline=True)

        if len(node.children) > 2:  # Else or elif
            self.visit(node.children[2])

    def visit_elif_stmt(self, node):
        self.emit("else if (", add_newline=False)
        self.visit(node.children[0])
        self.emit(") {", add_newline=True)
        
        if len(node.children) > 1:
            self.visit(node.children[1])
        self.emit("}", add_newline=True)

        if len(node.children) > 2:  # Check if there's an `else` block
            self.visit(node.children[2])


    def visit_else_block(self, node):
        # Emit the `else` block header
        self.emit("else {", add_newline=True)        
        # Process the body of the `else` block
        else_body = node.children[0]  # else block
        self.visit(else_body)
        
        # Close the `else` block
        self.emit("}", add_newline=True)

    #TODO: This must be simplify in the Parser
    def visit_comparison(self, node):
        left = node.children[0]  # Left
        operator_node = node.children[1].children[0]  # Operator
        right = operator_node.children[0]  # Right

        self.visit(left)
        self.emit(f" {operator_node.value} ", add_newline=False)
        self.visit(right)

    def visit_logical_op(self, node):
        if node.value == "and":
            operator = "&&"
        elif node.value == "or":
            operator = "||"

        self.emit("(", add_newline=False)
        self.visit(node.children[0])

        self.emit(f" {operator} ", add_newline=False)

        self.visit(node.children[1])
        self.emit(")",add_newline=False)


    #------------------------ WHILE ------------------------
    def visit_while_stmt(self, node):
        self.emit("while ", add_newline=False)
        self.visit(node.children[0])  # Visit the condition group
        self.emit(" {", add_newline=True)
        block_node = node.children[1]
        self.visit(block_node)
        self.emit("}", add_newline=True)

    def visit_simple_stmt(self, node):
        self.emit(f"{node.value};", add_newline=True)

    #------------------------ FOR ------------------------
    def visit_for_stmt(self, node):
        target = node.children[0].children[0].value  # `i` from target_list

        range_call = node.children[1]  # function_call
        range_args = range_call.children[1].children[0].children

        if len(range_args) == 1:  # range(stop)
            start = "0"
            stop = range_args[0].value
            step = "1"
        elif len(range_args) == 2:  # range(start, stop)
            start = range_args[0].value
            stop = range_args[1].value
            step = "1"
        elif len(range_args) == 3:  # range(start, stop, step)
            start = range_args[0].value
            stop = range_args[1].value
            step = range_args[2].value

        self.emit(f"for (int {target} = {start}; {target} < {stop}; {target} += {step}) {{", add_newline=True)
        self.visit(node.children[2])
        self.emit("}", add_newline=True)

    def visit_function_call(self, node): #TODO: Fix ";"
        function_name = node.children[0].value

        if function_name in self.built_in_map:
            self.emit(self.built_in_map[function_name], add_newline=False)
            if len(node.children) > 1:
                self.visit(node.children[1])
            self.emit(";", add_newline=True)
        else:
            self.visit(node.children[0])
            self.emit("(", add_newline=False)
            if len(node.children) > 1:
                self.visit(node.children[1])
            self.emit(")", add_newline=False)

    def visit_arguments(self, node):
        for i, arg in enumerate(node.children):
            self.visit(arg)
            if i < len(node.children) - 1:
                self.emit(", ", add_newline=False)

    def visit_expressions(self, node):
        # Visit each expression in the list (e.g., range arguments)
        for i, expr in enumerate(node.children):
            self.visit(expr)
            if i < len(node.children) - 1:
                self.emit(", ", add_newline=False)

    ###################### Tuple methods ######################
    def visit_tuple(self, node):
        self.emit("std::make_tuple(", add_newline=False)
        for i, child in enumerate(node.children):
            self.visit(child)
            if i < len(node.children) - 1:
                self.emit(", ", add_newline=False)
        self.emit(")", add_newline=False)

    ###################### Atomic methods ######################
    def visit_identifier(self, node):
        self.emit(node.value, add_newline=False)

    def visit_number(self, node):
        self.emit(str(node.value), add_newline=False)
    
    def visit_string(self, node):
        escaped_string = node.value.replace('"', '\\"')  # Escape double quotes
        self.emit(f"\"{escaped_string}\"", add_newline=False)  # Emit as a double-quoted C++ string

    def visit_group(self, node):
        self.emit("(", add_newline=False)
        for child in node.children:
            self.visit(child)
        self.emit(")", add_newline=False)

    #------------------------ ASSIGMENT ------------------------
    def visit_assign_chain(self, node):
        target_list_node = node.children[0]
        value_node = node.children[1]

        if target_list_node.children[0].node_type == "identifier":
            self.emit("auto ", add_newline=False)
        self.visit(target_list_node)
        self.emit(" = ", add_newline=False)
        self.visit(value_node)
        self.emit(";", add_newline=True)

    def visit_target_list(self, node):
        for i, child in enumerate(node.children):
            self.visit(child)
            if i < len(node.children) -1:
                self.emit(", ", add_newline=False)

    def visit_subscript(self, node):
        identifier_node = node.children[0]
        slice_node = node.children[1]
        
        self.visit(identifier_node)
        self.emit("[", add_newline=False)
        self.visit(slice_node)
        self.emit("]", add_newline=False)
        
    def visit_slice(self, node): #TODO: This must be change for C++ style
        for i, child in enumerate(node.children):
            self.visit(child)
            if i < len(node.children) -1:
                self.emit(":", add_newline=False)

    def visit_aug_assign(self, node):
        self.visit(node.children[0])
        self.emit(f" {node.children[1].value} ", add_newline=False)
        self.visit(node.children[2])
        self.emit(";", add_newline=True)

