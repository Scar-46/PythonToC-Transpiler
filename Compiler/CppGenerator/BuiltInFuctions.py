BUILTIN_FUNCTIONS = {
    # I/O Functions
    "print": lambda args: "std::cout << " + " << \" \" << ".join(f"{arg}" for arg in args) + " << std::endl" if args else "std::cout << std::endl",
    "input": lambda args: "std::cin >> " + args[0] if args else "std::cin",

    # Sequence and Container Functions
    "len": lambda args: f"std::size({args[0]})",
    "sum": lambda args: f"std::accumulate(std::begin({args[0]}), std::end({args[0]}), 0)",
    "min": lambda args: f"*std::min_element(std::begin({args[0]}), std::end({args[0]}))",
    "max": lambda args: f"*std::max_element(std::begin({args[0]}), std::end({args[0]}))",
    "sorted": lambda args: f"std::sort(std::begin({args[0]}), std::end({args[0]}))",

    # Mathematical Functions
    "abs": lambda args: f"std::abs({args[0]})",
    "pow": lambda args: f"std::pow({args[0]}, {args[1]})",
    "round": lambda args: f"std::round({args[0]})",

    # Type Conversion Functions
    "str": lambda args: f"std::to_string({args[0]})",
    "int": lambda args: f"static_cast<int>({args[0]})",
    "float": lambda args: f"static_cast<float>({args[0]})",

    # Built-in types' constructors
    "dict": lambda args: f"Map({'' if len(args) == 0 else f"{args[0]}, "+ ', '.join(f"{arg}" for arg in args[1:])})",
    "set": lambda args: f"Set({'' if len(args) == 0 else f"{args[0]}, "+ ', '.join(f"{arg}" for arg in args[1:])})",
}

def translate_function(name, arguments):
    translator = BUILTIN_FUNCTIONS.get(name)

    if translator:
        return translator(arguments)
    else:
        # Default handling for unknown functions: pass arguments as-is
        return f"{name}({', '.join(arguments)})"