from ICGenerator.node import Node
from CppGenerator.SymbolTable import SymbolTable
from CppGenerator.BuiltInFuctions import BUILTIN_FUNCTIONS, translate_function

# C++ code-snippets factory
# Takes a parser's AST and emits valid C++ code 
class CodeGenerator():
    # Initialize a new context for code generation
    def __init__(self):
        self.indent_level = 0
        self.indent = False
        self.symbol_table = SymbolTable()
    
#//////////////////////// AST pre-order navigation ////////////////////////
    # Yield a valid code string given an AST node and context acquired so far
    def visit(self, node):
        # Match the node type to the corresponding visitor handler
        method_name = f"visit_{node.node_type}"
        visitor = getattr(self, method_name, self.error_visit)

        # Let the visitor handle the node
        return visitor(node)

    # Called when missing a visitor method for a node type
    def error_visit(self, node):
        raise Exception(f"No visit_{node.node_type} method")
    
    # Add valid code to the current line or a new one
    def emit(self, code_str, add_newline=False):
        indent = '    ' * self.indent_level if self.indent else ''
        result = f"\n{indent}{code_str}" if self.indent else f"{indent}{code_str}"
        self.indent = add_newline
        return result

#//////////////////////// Statements methods ////////////////////////
    def visit_statements(self, node):
        temp_code = []
        global_statements = []
        for child in node.children:
            # Check if the statement is global
            if child.node_type not in {"function_def", "class_def"}:
                self.indent_level +=1
                global_statements.append(self.visit(child))
                self.indent_level -=1
            else:
                temp_code.append(self.visit(child))
        # Add declarations at the top
        code_strs = ["#include \"./util.hpp\""]
        code_strs.append(self.symbol_table.exit_and_declare(self.indent_level) + "\n")
        code_strs.extend(temp_code)
        # Generate the main function at the end
        if global_statements:
            code_strs.append(self.emit("int main(){"))
            self.indent_level += 1
            code_strs.extend(global_statements)
            self.emit("", add_newline=True)
            code_strs.append(self.emit("return 0;", add_newline=True))
            self.indent_level -= 1
            code_strs.append(self.emit("}"))
        return ''.join(code_strs)
    
    def visit_function_def(self, node):
        # Check for a constructor
        function_name = node.value
        is_constructor = function_name == "__init__"
        
        if is_constructor:
            function_name = self.symbol_table.get_class()[3:]  # Remove leading "class_"
        code_strs = []
        # Normal function
        self.symbol_table.enter_scope()
        if not is_constructor:
            code_strs.append(self.emit("var "))
        code_strs.append(self.emit(f"se_{function_name}(", add_newline=False))
        temp_code = []
        parameters = ""
        block_index = 0 if node.children[0].node_type == "block" else 1
        if block_index == 1:
            parameters = self.visit(node.children[0]) # Visit parameters
            code_strs.append(parameters)
        code_strs.append(self.emit("){", add_newline=True))
        temp_code.append(self.visit(node.children[block_index]))  # Visit block
        self.indent_level += 1
        if not is_constructor:
            self.symbol_table.add_symbol_over(f"se_{function_name}", symbol_type="function", params=parameters)
            temp_code.append(self.emit("return var();", add_newline=True))
        self.indent_level -= 1
        temp_code.append(self.emit("}", add_newline=True))
        code_strs.append(self.symbol_table.exit_and_declare(self.indent_level + 1))
        code_strs.extend(temp_code)
        return ''.join(code_strs)

    def visit_parameters(self, node):
        code_strs = []
        for param in node.children:
            if param.value != "self":  # Ignore 'self'
                code_strs.append(self.emit("var ") + self.visit(param))
        return ', '.join(code_strs)
    
    def visit_default(self, node):
        code_strs = [self.visit(node.children[0])]
        code_strs.append(" = " + self.visit(node.children[1]))
        return ''.join(code_strs)

    def visit_block(self, node):
        self.indent_level += 1
        code_strs = []
        for stmt in node.children:
            code_strs.append(self.visit(stmt))
        self.indent_level -= 1
        return ''.join(code_strs)

    def visit_return(self, node):
        code_strs = [self.emit("return ", add_newline=False)]
        code_strs.append(self.visit(node.children[0]))  # Visit expression
        return ''.join(code_strs)

    def visit_binary_operation(self, node):
        code_strs =[self.visit(node.children[0])]  # Visit left operand
        code_strs.append(self.emit(f" {node.value} ", add_newline=False))  # Add operator with spaces
        code_strs.append(self.visit(node.children[1]))  # Visit right operand
        return ''.join(code_strs)

#------------------------ CLASS ------------------------ 
    def visit_class_def(self, node):  # TODO: Fix inheritance in constructor
        self.symbol_table.add_symbol("se_" + node.value, symbol_type="class")
        self.symbol_table.add_class("se_" + node.value)
        self.symbol_table.enter_scope()
        # Add inheritance if needed
        inheritance = (f" : public se_{node.children[0].value}" 
            if len(node.children) > 1 and node.children[0].node_type == "identifier" else "")
        code_strs = [self.emit(f"class se_{node.value}{inheritance} {{", add_newline=True)]
        code_strs.append(self.emit("public:", add_newline=True))
        temp_code = []
        # Visit class body
        body_index = 1 if len(node.children) > 1 else 0
        temp_code.append(self.visit(node.children[body_index]))
        temp_code.append(self.emit("};", add_newline=True))
        code_strs.append(self.symbol_table.exit_and_declare(self.indent_level + 1))
        self.symbol_table.pop_class()
        code_strs.extend(temp_code)
        return ''.join(code_strs)

    def visit_attribute_access(self, node):
        code_strs = []
        if node.children[0].value == "self":
            self.symbol_table.add_symbol_over(node.value, symbol_type="variable")
            code_strs.append(self.emit(f"this->{node.value}", add_newline=False))
        else:
            code_strs.append(self.visit(node.children[0]))
            code_strs.append(self.emit(f".{node.value}", add_newline=False))
        return ''.join(code_strs)

#------------------------ IF ------------------------
    def visit_if_stmt(self, node):
        code_strs = [self.emit("if(", add_newline=False)]
        code_strs.append(self.visit(node.children[0]))  # Condition
        code_strs.append(self.emit("){", add_newline=True))

        if len(node.children) > 1:  # If block
            code_strs.append(self.visit(node.children[1]))
        code_strs.append(self.emit("}", add_newline=True))

        if len(node.children) > 2:  # Else or elif
            code_strs.append(self.visit(node.children[2]))

        return ''.join(code_strs)

    def visit_elif_stmt(self, node):
        code_strs = [self.emit("else if(", add_newline=False)]
        code_strs.append(self.visit(node.children[0]))  # Condition
        code_strs.append(self.emit("){", add_newline=True))

        if len(node.children) > 1:  # Elif block
            code_strs.append(self.visit(node.children[1]))
        code_strs.append(self.emit("}", add_newline=True))

        if len(node.children) > 2:  # Check for else block
            code_strs.append(self.visit(node.children[2]))

        return ''.join(code_strs)

    def visit_else_block(self, node):
        code_strs = [self.emit("else{", add_newline=True)]
        code_strs.append(self.visit(node.children[0]))  # Else block body
        code_strs.append(self.emit("}", add_newline=True))
        return ''.join(code_strs)

# ------------------------ Comparison ------------------------
    def visit_comparison(self, node):
        code_strs = [self.visit(node.children[0])]  # Left operand
        operator_node = node.children[1]
        code_strs.append(self.emit(f" {operator_node.value} ", add_newline=False))
        code_strs.append(self.visit(operator_node.children[0]))  # Right operand
        return ''.join(code_strs)

# ------------------------ Logical Operation ------------------------
    def visit_logical_op(self, node):
        operator = "&&" if node.value == "and" else "||"
        code_strs = [self.emit("(", add_newline=False)]
        code_strs.append(self.visit(node.children[0]))  # Left operand
        code_strs.append(self.emit(f" {operator} ", add_newline=False))
        code_strs.append(self.visit(node.children[1]))  # Right operand
        code_strs.append(self.emit(")", add_newline=False))
        return ''.join(code_strs)

#------------------------ WHILE ------------------------
    def visit_while_stmt(self, node):
        code_strs = [self.emit("while(", add_newline=False)]
        code_strs.append(self.visit(node.children[0]))  # Visit the condition group
        code_strs.append(self.emit("){", add_newline=True))
        code_strs.append(self.visit(node.children[1]))  # Block node
        code_strs.append(self.emit("}", add_newline=True))
        return ''.join(code_strs)

    def visit_simple_stmt(self, node):
        code_strs = []
        if len(node.children) == 0:
            code_strs.append(self.emit(f"{node.value};", add_newline=True))
        else:
            code_strs.append(self.visit(node.children[0]))
            code_strs.append(self.emit(";", add_newline=True))
        return ''.join(code_strs)

#------------------------ FOR ------------------------
    def visit_for_stmt(self, node): #TODO: Multiple types of for must be supported
        target = node.children[0].children[0].value  # `i` from target_list
        range_call = node.children[1]  # function_call
        range_args = range_call.children[1].children

        # Determine range arguments
        if len(range_args) == 1:  # range(stop)
            start, stop, step = "0", range_args[0].value, "1"
        elif len(range_args) == 2:  # range(start, stop)
            start, stop, step = range_args[0].value, range_args[1].value, "1"
        elif len(range_args) == 3:  # range(start, stop, step)
            start, stop, step = range_args[0].value, range_args[1].value, range_args[2].value

        code_strs = [self.emit(f"for (int {target} = {start}; {target} < {stop}; {target} += {step}){{", add_newline=True)]
        code_strs.append(self.visit(node.children[2]))  # Body of the loop
        code_strs.append(self.emit("}", add_newline=True))
        return ''.join(code_strs)

    def visit_function_call(self, node):
        code_strs = []
        function_name : str = node.children[0].value

        # If the function is not built-in, handle it by evaluation
        if function_name in BUILTIN_FUNCTIONS:
            # Flush in case of possible identation for identifier
            code_strs.append(self.emit('', add_newline=False))

            # Append parameters
            parameters = [] if node.children == 1 \
                else [self.visit(node.children[1])] if node.children[1].node_type != "expressions" \
                    else [self.visit(expression) for expression in node.children[1].children]
 
            code_strs.append(
                self.emit(
                    translate_function(node.children[0].value, parameters), 
                    add_newline=False
                )
            )
        # Otherwise, let the built-in function visitor handle the function call
        else:
            code_strs.append(self.visit(node.children[0])) # Resolve function call
            code_strs.append(self.emit("(", add_newline=False))
            if len(node.children) > 1:  # Arguments
                code_strs.append(self.visit(node.children[1]))
            code_strs.append(self.emit(")", add_newline=False))

        return ''.join(code_strs)

# ------------------------ Arguments ------------------------
    def visit_arguments(self, node):
        code_strs = []
        for i, arg in enumerate(node.children):
            code_strs.append(self.visit(arg))
            if i < len(node.children) - 1:
                code_strs.append(self.emit(", ", add_newline=False))
        return ''.join(code_strs)

# ------------------------ Expressions ------------------------
    def visit_expressions(self, node):
        code_strs = []
        for i, expr in enumerate(node.children):
            code_strs.append(self.visit(expr))
            if i < len(node.children) - 1:
                code_strs.append(self.emit(", ", add_newline=False))
        return ''.join(code_strs)
    
    def visit_unary_operation(self, node):
        operator = node.value
        operand_code = self.visit(node.children[0])
        return self.emit(f"{operator}{operand_code}", add_newline=False)

#//////////////////////// Structures Methods ////////////////////////

# ------------------------ Tuple ------------------------
    def visit_tuple(self, node):
        code_strs = [self.emit("std::make_tuple(", add_newline=False)]
        for i, child in enumerate(node.children):
            code_strs.append(self.visit(child))
            if i < len(node.children) - 1:
                code_strs.append(self.emit(", ", add_newline=False))
        code_strs.append(self.emit(")", add_newline=False))
        return ''.join(code_strs)

# ------------------------ Dictionary ------------------------
    def visit_dictionary(self, node):
        code_strs = [self.emit("std::map<auto, auto> {", add_newline=True)]
        self.indent_level += 1
        for i, child in enumerate(node.children[0].children):
            code_strs.append(self.visit(child))
            if i < len(node.children[0].children) - 1:
                code_strs.append(self.emit(",", add_newline=True))
        self.indent_level -= 1
        self.emit("", add_newline=True)
        code_strs.append(self.emit("}", add_newline=False))
        return ''.join(code_strs)

    def visit_key_value_pair(self, node):
        code_strs = [self.emit("{", add_newline=False)]
        code_strs.append(self.visit(node.children[0]))  # Key
        code_strs.append(self.emit(", ", add_newline=False))
        code_strs.append(self.visit(node.children[1]))  # Value
        code_strs.append(self.emit("}", add_newline=False))
        return ''.join(code_strs)
    
# ------------------------ List ------------------------
    def visit_list(self, node):
        code_strs = [self.emit("{", add_newline=False)]
        
        for i, child in enumerate(node.children):
            if i > 0:
                code_strs.append(self.emit(", ", add_newline=False))
            code_strs.append(self.visit(child))
        
        code_strs.append(self.emit("}", add_newline=False))
        return ''.join(code_strs)
    
# ------------------------ Set ------------------------
    def visit_set(self, node):
        code_strs = [self.emit("{", add_newline=False)]
        for i, child in enumerate(node.children):
            if i > 0:
                code_strs.append(self.emit(", ", add_newline=False))
            code_strs.append(self.visit(child))
        code_strs.append(self.emit("}", add_newline=False))
        return ''.join(code_strs)

#//////////////////////// Atomic Methods ////////////////////////
    def visit_identifier(self, node):
        return self.emit("se_" + node.value, add_newline=False)

    def visit_number(self, node):
        return self.emit(str(node.value), add_newline=False)

    def visit_string(self, node):
        escaped_string = node.value.replace('"', '\\"')  # Escape double quotes
        return self.emit(f"\"{escaped_string}\"", add_newline=False)

    def visit_group(self, node):
        code_strs = [self.emit("(", add_newline=False)]
        for child in node.children:
            code_strs.append(self.visit(child))
        code_strs.append(self.emit(")", add_newline=False))
        return ''.join(code_strs)
    
# ------------------------ ASSIGNMENT ------------------------
    def visit_assign_chain(self, node):
        code_strs = [self.visit(node.children[0])]  # Target list
        code_strs.append(self.visit(node.children[1]))  # Value
        return ''.join(code_strs)

    def visit_target_chain(self, node):
        code_strs = []
        for target_list_node in node.children:
            code_strs.append(self.visit(target_list_node))
            code_strs.append(self.emit(" = ", add_newline=False))
        return ''.join(code_strs)

    def visit_target_list(self, node):
        code_strs = []
        for i, child in enumerate(node.children):
            if child.node_type == "identifier":
                self.symbol_table.add_symbol("se_" + child.value, symbol_type="variable")
            code_strs.append(self.visit(child))
            if i < len(node.children) - 1:
                code_strs.append(self.emit(", ", add_newline=False))
        return ''.join(code_strs)

    def visit_subscript(self, node):
        code_strs = [self.visit(node.children[0])]  # Identifier
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
        code_strs = [self.visit(node.children[0])]  # Target
        code_strs.append(self.emit(f" {node.children[1].value} ", add_newline=False))  # Operator
        code_strs.append(self.visit(node.children[2]))  # Value
        return ''.join(code_strs)