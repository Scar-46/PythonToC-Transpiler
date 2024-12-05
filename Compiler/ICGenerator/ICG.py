from Parser import Parser
from common import error_logger
from node import Node
import graphviz
import sys

def read_file(file_name):
    try:
        with open(file_name, 'r') as file:
            return file.read()
    except FileNotFoundError:
        print(f"File {file_name} not found.")
        exit(1)

# Populate GraphViz graph based on built AST
def viz_graph(ast: Node, graph: graphviz.Digraph, node_id: int = 0) -> int:
    # Add node to graphviz
    graph.node(
        name=str(node_id), 
        label=f"({str(ast.node_type)})" + (" " + str(ast.value) if ast.value != None else "")
    )

    # Add children nodes to the graphviz in pre-order
    child_id = node_id + 1
    for child in ast.children:
        graph.edge(head_name=str(child_id), tail_name=str(node_id))
        child_id = viz_graph(child, graph, child_id)

    # Return the next available id for future nodes
    return child_id

def main():
    # Sanitize arguments
    if (
        len(sys.argv) < 2 or len(sys.argv) > 3 # Number of args
        or (len(sys.argv) == 3 and sys.argv[2] != "drawGraph") # Invalid 2nd arg
    ):
        print("Usage: ICG.py <filename> [drawGraph]")
        return -1

    code = read_file(sys.argv[1])
    drawGraph = len(sys.argv) == 3

    parser = Parser()
    try:
        # Parse AST
        result = parser.parse(code)
    except Exception as e:
        print(f"Unexpected Error during parsing: {e}")

    if error_logger.error_count() <= 0:
        exit_code = 0

        # Produce Graphviz DOT source for graph
        if (drawGraph):
            print(f"Building AST visualization for: {sys.argv[1]}")
            dot = graphviz.Digraph('ast', comment=f"AST for {sys.argv[1]}")
            viz_graph(result, dot)

            print("Writing DOT code to graph.dot")
            open('graph.dot', 'w').write(dot.source)
        # Print to CLI
        else:
            print(result)

        # TODO: make this message prettier
        print("############# File parsed successfully! #############")
    else:
        exit_code = 1
        error_logger.print_error(sys.argv[1])
    exit(exit_code)

if __name__ == "__main__":
    main()
