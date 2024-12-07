// Copyright (c) 2024 Syntax Errors.
#pragma once

#include "../Object/var.hpp"

// Implement orphan built in functions
namespace Builtin {
  var len(const std::vector<var>& params) {
    if (params.size() != 1) {
      std::cerr << "len: Invalid number of arguments\n";
      return nullptr;
    }

    return params[0]->Call("__len__", {});
  }

  var sum(const std::vector<var>& params) {
    if (params.size() != 1) {
      std::cerr << "sum: Invalid number of arguments\n";
      return nullptr;
    }

    return params[0]->Call("__sum__", {});
  }

  var min(const std::vector<var>& params) {
    if (params.size() != 1) {
      std::cerr << "min: Invalid number of arguments\n";
      return nullptr;
    }

    return params[0]->Call("__min__", {});
  }

  var max(const std::vector<var>& params) {
    if (params.size() != 1) {
      std::cerr << "max: Invalid number of arguments\n";
      return nullptr;
    }

    return params[0]->Call("__max__", {});
  }
}
