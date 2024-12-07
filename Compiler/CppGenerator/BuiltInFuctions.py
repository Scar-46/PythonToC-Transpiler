BUILTIN_FUNCTIONS = {
    # I/O Functions
    "print": lambda args: "std::cout << " + " << \" \" << ".join(f"{arg}" for arg in args) + " << std::endl" if args else "std::cout << std::endl",
    "input": lambda args: "std::cin >> " + args[0] if args else "std::cin",

    # Sequence and Container Functions
    "len": lambda args: f"Builtin::len({', '.join(f"{arg}" for arg in args)})",
    "sum": lambda args: f"Builtin::sum({', '.join(f"{arg}" for arg in args)})",
    "min": lambda args: f"Builtin::min({', '.join(f"{arg}" for arg in args)})",
    "max": lambda args: f"Builtin::max({', '.join(f"{arg}" for arg in args)})",
    "sorted": lambda args: f"std::sort(std::begin({args[0]}), std::end({args[0]}))", # TODO

    # Mathematical Functions
    "abs": lambda args: f"std::abs({args[0]})",
    "round": lambda args: f"std::round({args[0]})",
    "pow": lambda args: f"std::pow({args[0]}, {args[1]})",

    # Type Conversion Functions
    "str": lambda args: f"std::to_string({args[0]})",
    "int": lambda args: f"static_cast<int>({args[0]})",
    "float": lambda args: f"static_cast<float>({args[0]})",

    # Built-in types' constructors
    "dict": lambda args: f"Map({', '.join(f"{arg}" for arg in args)})",
    "set": lambda args: f"Set({', '.join(f"{arg}" for arg in args)})",
    "list": lambda args: f"List({', '.join(f"{arg}" for arg in args)})",
}

def translate_function(name, arguments):
    translator = BUILTIN_FUNCTIONS.get(name)

    if translator:
        return translator(arguments)
    else:
        # Default handling for unknown functions: pass arguments as-is
        return f"{name}({', '.join(arguments)})"