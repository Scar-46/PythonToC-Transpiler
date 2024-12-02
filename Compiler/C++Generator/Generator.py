import sys
sys.path.insert(0, 'Compiler/ICGenerator')

from node import Node
from SymbolTable import SymbolTable

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
        self.symbol_table = SymbolTable()

    def emit(self, code_str, add_newline=False):
        indent = '    ' * self.indent_level if self.indent else ''
        result = f"\n{indent}{code_str}" if self.indent else f"{indent}{code_str}"
        self.indent = add_newline
        return result

    def get_code(self):
        return ''.join(self.code)

    def visit(self, node):
        method_name = f"visit_{node.node_type}"
        visitor = getattr(self, method_name, self.error_visit)
        return visitor(node)
        
    def error_visit(self, node):
        raise Exception(f"No visit_{node.node_type} method")

    ###################### Staments methods ######################
    def visit_staments(self, node):
        code = []
        temp_code = []
        for child in node.children:
           temp_code.append(self.visit(child))

        code.append(self.symbol_table.exit_and_declare(self.indent_level) + "\n")
        code.extend(temp_code)
        return ''.join(code)
    
    def visit_function_def(self, node):
        code = [self.emit(f"auto {node.value}(", add_newline=False)]
        temp_code = []
        self.symbol_table.add_symbol(node.value, symbol_type="function")
        self.symbol_table.enter_scope()
        if node.children[0].node_type == "block":
            code.append(self.emit(") {", add_newline=True))
            temp_code.append(self.visit(node.children[0]))  # Visit block
            temp_code.append(self.emit("}", add_newline=True))
        else:
            code.append(self.visit(node.children[0]))  # Visit parameters
            code.append(self.emit(") {", add_newline=True))
            temp_code.append(self.visit(node.children[1]))  # Visit block
            temp_code.append(self.emit("}", add_newline=True))
        code.append(self.symbol_table.exit_and_declare(self.indent_level + 1))
        code.extend(temp_code)
        return ''.join(code)

    def visit_parameters(self, node):
        params = []
        for param in node.children:
            if param.value != "self":  # Ignore 'self'
                params.append(f"auto {param.value}")  # TODO: Fix type (add type inference here)
        return self.emit(", ".join(params), add_newline=False)

    def visit_block(self, node):
        self.indent_level += 1
        code_strs = []
        for stmt in node.children:
            code_strs.append(self.visit(stmt))
        self.indent_level -= 1
        return ''.join(code_strs)

    def visit_return(self, node):
        code = [self.emit("return ", add_newline=False)]
        code.append(self.visit(node.children[0]))  # Visit expression
        code.append(self.emit(";", add_newline=True))
        return ''.join(code)

    def visit_binary_operation(self, node):
        code = []
        code.append(self.visit(node.children[0]))  # Visit left operand
        code.append(self.emit(f" {node.value} ", add_newline=False))  # Add operator with spaces
        code.append(self.visit(node.children[1]))  # Visit right operand
        return ''.join(code)

    #------------------------ CLASS ------------------------
    def visit_class_def(self, node): #TODO: Check how to traslate _init_
        inheritance = ""
        if len(node.children) > 1 and node.children[0].node_type == 'identifier':
            inheritance = f" : public {node.children[0].value}"

        # Start class definition with inheritance (if any)
        code = [self.emit(f"class {node.value}{inheritance} {{", add_newline=True)]
        temp_code = []
        self.symbol_table.add_symbol(node.value, symbol_type="class")
        self.symbol_table.enter_scope()
        code.append(self.emit("public:", add_newline=True))
        if len(node.children) > 1:
            temp_code.append(self.visit(node.children[1]))
        else:
            temp_code.append(self.visit(node.children[0]))
        temp_code.append(self.emit("};", add_newline=True))
        code.append(self.symbol_table.exit_and_declare(self.indent_level + 1))
        code.extend(temp_code)
        return ''.join(code)
            
    def visit_attribute_access(self, node):
        code_strs = []
        if node.children[0].value == "self":
            self.symbol_table.add_symbol_over(node.value, symbol_type="variable")
            code_strs.append(self.emit("this", add_newline=False))
            code_strs.append(self.emit(f"->{node.value}", add_newline=False))
        else:
            code_strs.append(self.visit(node.children[0]))
            code_strs.append(self.emit(f".{node.value}", add_newline=False))
        return ''.join(code_strs)

    #------------------------ IF ------------------------
    def visit_if_stmt(self, node):
        code_strs = []
        code_strs.append(self.emit("if (", add_newline=False))
        code_strs.append(self.visit(node.children[0]))  # Condition
        code_strs.append(self.emit(") {", add_newline=True))

        if len(node.children) > 1:  # If block
            code_strs.append(self.visit(node.children[1]))
        code_strs.append(self.emit("}", add_newline=True))

        if len(node.children) > 2:  # Else or elif
            code_strs.append(self.visit(node.children[2]))

        return ''.join(code_strs)

    def visit_elif_stmt(self, node):
        code_strs = []
        code_strs.append(self.emit("else if (", add_newline=False))
        code_strs.append(self.visit(node.children[0]))  # Condition
        code_strs.append(self.emit(") {", add_newline=True))

        if len(node.children) > 1:  # Elif block
            code_strs.append(self.visit(node.children[1]))
        code_strs.append(self.emit("}", add_newline=True))

        if len(node.children) > 2:  # Check for else block
            code_strs.append(self.visit(node.children[2]))

        return ''.join(code_strs)


    def visit_else_block(self, node):
        code_strs = []
        code_strs.append(self.emit("else {", add_newline=True))
        code_strs.append(self.visit(node.children[0]))  # Else block body
        code_strs.append(self.emit("}", add_newline=True))
        return ''.join(code_strs)

    # ------------------------ Comparison ------------------------
    def visit_comparison(self, node):
        code_strs = []
        code_strs.append(self.visit(node.children[0]))  # Left operand
        operator_node = node.children[1]
        code_strs.append(self.emit(f" {operator_node.value} ", add_newline=False))
        code_strs.append(self.visit(operator_node.children[0]))  # Right operand
        return ''.join(code_strs)

    # ------------------------ Logical Operation ------------------------
    def visit_logical_op(self, node):
        code_strs = []
        operator = "&&" if node.value == "and" else "||"
        code_strs.append(self.emit("(", add_newline=False))
        code_strs.append(self.visit(node.children[0]))  # Left operand
        code_strs.append(self.emit(f" {operator} ", add_newline=False))
        code_strs.append(self.visit(node.children[1]))  # Right operand
        code_strs.append(self.emit(")", add_newline=False))
        return ''.join(code_strs)

    #------------------------ WHILE ------------------------
    def visit_while_stmt(self, node):
        code_strs = []
        code_strs.append(self.emit("while ", add_newline=False))
        code_strs.append(self.visit(node.children[0]))  # Visit the condition group
        code_strs.append(self.emit(" {", add_newline=True))
        code_strs.append(self.visit(node.children[1]))  # Block node
        code_strs.append(self.emit("}", add_newline=True))
        return ''.join(code_strs)

    def visit_simple_stmt(self, node):
        return self.emit(f"{node.value};", add_newline=True)

    #------------------------ FOR ------------------------
    def visit_for_stmt(self, node):
        code_strs = []
        target = node.children[0].value  # `i` from target_list
        range_call = node.children[1]  # function_call
        range_args = range_call.children[1].children

        # Determine range arguments
        if len(range_args) == 1:  # range(stop)
            start, stop, step = "0", range_args[0].value, "1"
        elif len(range_args) == 2:  # range(start, stop)
            start, stop, step = range_args[0].value, range_args[1].value, "1"
        elif len(range_args) == 3:  # range(start, stop, step)
            start, stop, step = range_args[0].value, range_args[1].value, range_args[2].value

        code_strs.append(self.emit(f"for (int {target} = {start}; {target} < {stop}; {target} += {step}) {{", add_newline=True))
        code_strs.append(self.visit(node.children[2]))  # Body of the loop
        code_strs.append(self.emit("}", add_newline=True))
        return ''.join(code_strs)

    def visit_function_call(self, node):
        code_strs = []
        function_name = node.children[0].value

        if function_name in self.built_in_map:
            code_strs.append(self.emit(self.built_in_map[function_name], add_newline=False))
            if len(node.children) > 1:
                code_strs.append(self.visit(node.children[1]))
            code_strs.append(self.emit(";", add_newline=True))
        else:
            code_strs.append(self.visit(node.children[0]))  # Function name
            code_strs.append(self.emit("(", add_newline=False))
            if len(node.children) > 1:  # Arguments
                code_strs.append(self.visit(node.children[1]))
            code_strs.append(self.emit(")", add_newline=False))
        return ''.join(code_strs)

# ------------------------ ARGUMENTS ------------------------
    def visit_arguments(self, node):
        code_strs = []
        for i, arg in enumerate(node.children):
            code_strs.append(self.visit(arg))
            if i < len(node.children) - 1:
                code_strs.append(self.emit(", ", add_newline=False))
        return ''.join(code_strs)

# ------------------------ EXPRESSIONS ------------------------
    def visit_expressions(self, node):
        code_strs = []
        for i, expr in enumerate(node.children):
            code_strs.append(self.visit(expr))
            if i < len(node.children) - 1:
                code_strs.append(self.emit(", ", add_newline=False))
        return ''.join(code_strs)

###################### Structures Methods ######################
    def visit_tuple(self, node):
        code_strs = []
        code_strs.append(self.emit("std::make_tuple(", add_newline=False))
        for i, child in enumerate(node.children):
            code_strs.append(self.visit(child))
            if i < len(node.children) - 1:
                code_strs.append(self.emit(", ", add_newline=False))
        code_strs.append(self.emit(")", add_newline=False))
        return ''.join(code_strs)

    def visit_dictionary(self, node):
        code_strs = []
        code_strs.append(self.emit("std::map<auto, auto> {", add_newline=True))
        self.indent_level += 1
        for i, child in enumerate(node.children[0].children):
            code_strs.append(self.visit(child))
            if i < len(node.children[0].children) - 1:
                code_strs.append(self.emit(",", add_newline=True))
        self.indent_level -= 1
        code_strs.append(self.emit("\n}", add_newline=False))
        return ''.join(code_strs)

    def visit_key_value_pair(self, node):
        code_strs = []
        code_strs.append(self.emit("{", add_newline=False))
        code_strs.append(self.visit(node.children[0]))  # Key
        code_strs.append(self.emit(", ", add_newline=False))
        code_strs.append(self.visit(node.children[1]))  # Value
        code_strs.append(self.emit("}", add_newline=False))
        return ''.join(code_strs)

    ###################### Atomic Methods ######################
    def visit_identifier(self, node):
        return self.emit(node.value, add_newline=False)

    def visit_number(self, node):
        return self.emit(str(node.value), add_newline=False)

    def visit_string(self, node):
        escaped_string = node.value.replace('"', '\\"')  # Escape double quotes
        return self.emit(f"\"{escaped_string}\"", add_newline=False)

    def visit_group(self, node):
        code_strs = []
        code_strs.append(self.emit("(", add_newline=False))
        for child in node.children:
            code_strs.append(self.visit(child))
        code_strs.append(self.emit(")", add_newline=False))
        return ''.join(code_strs)
    
# ------------------------ ASSIGNMENT ------------------------
    def visit_assign_chain(self, node):  # TODO: Needs adjustment for C++ style
        code_strs = []
        code_strs.append(self.visit(node.children[0]))  # Target list
        code_strs.append(self.visit(node.children[1]))  # Value
        code_strs.append(self.emit(";", add_newline=True))
        return ''.join(code_strs)

    def visit_target_chain(self, node):  # TODO: Simplify
        code_strs = []
        for target_list_node in node.children:
            if target_list_node.node_type == "identifier":
                self.symbol_table.add_symbol(target_list_node.value, symbol_type="variable")
            code_strs.append(self.visit(target_list_node))
            code_strs.append(self.emit(" = ", add_newline=False))
        return ''.join(code_strs)

    def visit_target_list(self, node):
        code_strs = []
        for i, child in enumerate(node.children):
            code_strs.append(self.visit(child))
            if i < len(node.children) - 1:
                code_strs.append(self.emit(", ", add_newline=False))
        return ''.join(code_strs)

    def visit_subscript(self, node):
        code_strs = []
        code_strs.append(self.visit(node.children[0]))  # Identifier
        code_strs.append(self.emit("[", add_newline=False))
        code_strs.append(self.visit(node.children[1]))  # Slice
        code_strs.append(self.emit("]", add_newline=False))
        return ''.join(code_strs)

    def visit_slice(self, node):  # TODO: Adjust for C++ style
        code_strs = []
        for i, child in enumerate(node.children):
            code_strs.append(self.visit(child))
            if i < len(node.children) - 1:
                code_strs.append(self.emit(":", add_newline=False))
        return ''.join(code_strs)

    def visit_aug_assign(self, node):
        code_strs = []
        code_strs.append(self.visit(node.children[0]))  # Target
        code_strs.append(self.emit(f" {node.children[1].value} ", add_newline=False))  # Operator
        code_strs.append(self.visit(node.children[2]))  # Value
        code_strs.append(self.emit(";", add_newline=True))
        return ''.join(code_strs)