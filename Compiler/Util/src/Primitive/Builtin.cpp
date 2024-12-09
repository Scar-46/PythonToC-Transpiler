// Copyright (c) 2024 Syntax Errors.
#include "./Builtin.hpp"
#include "./Boolean.hpp"
#include "./String.hpp"

namespace Builtin {
    var asString(const std::vector<ObjectPtr> &params) {
        if (params.size() == 0) {
            return (var) std::make_shared<String>("");
        }

        if (params.size() > 1) {
            std::cerr << "str: Invalid number of arguments\n";
            return nullptr;
        }

        ObjectPtr obj = params[0];
        if (! obj) {
            std::cerr << "str: Invalid None parameter.\n";
            return nullptr;
        }

        return (var) obj->Call("__str__", {});
    }

    var asBoolean(const std::vector<ObjectPtr>& params) {
        if (params.size() == 0) {
            return (var) std::make_shared<Boolean>(false);
        }

        if (params.size() > 1) {
            std::cerr << "bool: Invalid number of arguments\n";
            return nullptr;
        }

        ObjectPtr obj = params[0];
        if (! obj) {
            std::cerr << "bool: Invalid None parameter.\n";
            return nullptr;
        }

        return (var) obj->Call("__bool__", {});
    }

}