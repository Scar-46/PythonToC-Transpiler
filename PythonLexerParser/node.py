class Node:
    def __init__(self, node_type, value=None, children=None):
        self.node_type = node_type
        self.value = value
        self.children = children if children is not None else []

    def add_child(self, child_node):
        self.children.append(child_node)

    def __repr__(self):
        msg = f"<Node type: {self.node_type}, value: {self.value}>"
        for child in self.children:
            msg += f"\n\t{child}"
        return msg
