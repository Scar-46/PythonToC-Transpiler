// Copyright (c) 2024 Syntax Errors.
#pragma once

#include "../Object/var.hpp"

// Implement orphan built in functions
namespace Builtin {
  var abs(const std::vector<var>& params) {
    if (params.size() != 1) {
      std::cerr << "abs: Invalid number of arguments\n";
      return nullptr;
    }

    return params[0]->Call("__abs__", {});
  }

  var round(const std::vector<var>& params) {
    if (params.size() != 1) {
      std::cerr << "abs: Invalid number of arguments\n";
      return nullptr;
    }

    return params[0]->Call("__round__", {});
  }
}
