BUILTIN_FUNCTIONS = {
    # I/O Functions
    "print": lambda args: "std::cout << " + " << \" \" << ".join(f"{arg}" for arg in args) + " << std::endl" if args else "std::cout << std::endl",
    "input": lambda args: "std::cin >> " + args[0] if args else "std::cin",

    # Sequence and Container Functions
    "iter": lambda args: "Builtin::iter({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
    "next": lambda args: "Builtin::next({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
    "len": lambda args: "Builtin::len({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
    "sum": lambda args: "Builtin::sum({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
    "min": lambda args: "Builtin::min({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
    "max": lambda args: "Builtin::max({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
    # TODO: Implement threeway comparison between objects of all types to sort containers
    # "sorted": lambda args: f"std::sort(std::begin({args[0]}), std::end({args[0]}))",

    # Mathematical Functions
    "abs": lambda args: "Builtin::abs({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
    "round": lambda args: "Builtin::round({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
    "pow": lambda args: "Builtin::pow({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",

    # Type Conversion Functions
    "str": lambda args: "Builtin::str({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
    "int": lambda args: "Builtin::int({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
    "float": lambda args: "Builtin::float({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",

    # Built-in types' constructors
    "tuple": lambda args: "Builtin::tuple({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
    "list": lambda args: "Builtin::list({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
    "set": lambda args: "Builtin::set({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
    "dict": lambda args: "Builtin::dict({" + f"{', '.join(f"{arg}" for arg in args)}" + "})",
}

def translate_function(name, arguments):
    translator = BUILTIN_FUNCTIONS.get(name)

    if translator:
        return translator(arguments)
    else:
        # Default handling for unknown functions: pass arguments as-is
        return f"{name}({', '.join(arguments)})"