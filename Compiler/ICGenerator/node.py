class Node:
    def __init__(self, node_type, value=None, children=None):
        self.node_type = node_type
        self.value = value
        self.children = children if children is not None else []

    def add_child(self, child_node):
        self.children.append(child_node)

    def __repr__(self, level=0):
        indent = "  " * level  # Create an indentation based on the tree level
        msg = f"{indent}<Node type: {self.node_type}, value: {self.value}>"
        for child in self.children:
            msg += f"\n{child.__repr__(level + 1)}"
        return msg
