BUILTIN_FUNCTIONS = {
    # I/O Functions
    "print": lambda args: "std::cout << " + " << \" \" << ".join(f"{arg}" for arg in args) + " << std::endl" if args else "std::cout << std::endl",
    "input": lambda args: "std::cin >> " + args[0] if args else "std::cin",

    # Sequence and Container Functions
    "iter": lambda args: f"Builtin::iter({', '.join(f"{arg}" for arg in args)})",
    "next": lambda args: f"Builtin::next({', '.join(f"{arg}" for arg in args)})",
    "len": lambda args: f"Builtin::len({', '.join(f"{arg}" for arg in args)})",
    "sum": lambda args: f"Builtin::sum({', '.join(f"{arg}" for arg in args)})",
    "min": lambda args: f"Builtin::min({', '.join(f"{arg}" for arg in args)})",
    "max": lambda args: f"Builtin::max({', '.join(f"{arg}" for arg in args)})",
    # TODO: Implement threeway comparison between objects of all types to sort containers
    # "sorted": lambda args: f"std::sort(std::begin({args[0]}), std::end({args[0]}))",

    # Mathematical Functions
    "abs": lambda args: f"Builtin::abs({', '.join(f"{arg}" for arg in args)})",
    "round": lambda args: f"Builtin::round({', '.join(f"{arg}" for arg in args)})",
    "pow": lambda args: f"Builtin::pow({', '.join(f"{arg}" for arg in args)})",

    # Type Conversion Functions
    "str": lambda args: f"Builtin::asString({', '.join(f"{arg}" for arg in args)})",
    "int": lambda args: f"Builtin::asInteger({', '.join(f"{arg}" for arg in args)})",
    "float": lambda args: f"Builtin::asDouble({', '.join(f"{arg}" for arg in args)})",

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