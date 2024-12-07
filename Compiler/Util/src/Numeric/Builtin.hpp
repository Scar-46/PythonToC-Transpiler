// Copyright (c) 2024 Syntax Errors.
#pragma once

#include "../Object/var.hpp"

// Implement orphan built in functions
namespace Builtin {
  // Get absolute value of number
  var abs(const std::vector<var>& params);

  // Get rounded number
  var round(const std::vector<var>& params);

  // Get power from base raised to power, modulo
  var round(const std::vector<var>& params);
}
