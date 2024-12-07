// Copyright (c) 2024 Syntax Errors.
#include "./Builtin.hpp"
#include "./Boolean.hpp"
#include "./String.hpp"

namespace Builtin {
    var asString(const std::vector<var> &params) {
        if (params.size() == 0) {
            return (var) std::make_shared<String>("");
        }

        if (params.size() > 0) {
            std::cerr << "abs: Invalid number of arguments\n";
            return nullptr;
        }

        ObjectPtr obj = params[0].getValue();
        if (! obj) {
            std::cerr << "str: Invalid None parameter.\n";
            return nullptr;
        }

        return (var) obj->Call("__str__", {});
    }

    var asBoolean(const std::vector<var>& params) {
        if (params.size() == 0) {
            return (var) std::make_shared<Boolean>(false);
        }

        if (params.size() > 0) {
            std::cerr << "bool: Invalid number of arguments\n";
            return nullptr;
        }

        ObjectPtr obj = params[0].getValue();
        if (! obj) {
            std::cerr << "bool: Invalid None parameter.\n";
            return nullptr;
        }

        return (var) obj->Call("__bool__", {});
    }

}