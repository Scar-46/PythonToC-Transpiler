BUILTIN_FUNCTIONS = {
    # I/O Functions
    "print": lambda args: f"std::cout << {' << '.join(args)} << std::endl",
    "input": lambda args: "std::cin >> " + args[0] if args else "std::cin",

    # Sequence and Container Functions
    "len": lambda args: f"{args[0]}.size()",
    "sum": lambda args: f"std::accumulate({args[0]}.begin(), {args[0]}.end(), 0)",
    "min": lambda args: f"*std::min_element({args[0]}.begin(), {args[0]}.end())",
    "max": lambda args: f"*std::max_element({args[0]}.begin(), {args[0]}.end())",
    "sorted": lambda args: f"std::sort({args[0]}.begin(), {args[0]}.end())",

    # Mathematical Functions
    "abs": lambda args: f"std::abs({args[0]})",
    "pow": lambda args: f"std::pow({args[0]}, {args[1]})",
    "round": lambda args: f"std::round({args[0]})",

    # Type Conversion Functions
    "str": lambda args: f"std::to_string({args[0]})",
    "int": lambda args: f"static_cast<int>({args[0]})",
    "float": lambda args: f"static_cast<float>({args[0]})",
}

def translate_function(name, arguments):
    translator = BUILTIN_FUNCTIONS.get(name)

    if translator:
        return translator(arguments)
    else:
        # Default handling for unknown functions: pass arguments as-is
        return f"{name}({', '.join(arguments)})"