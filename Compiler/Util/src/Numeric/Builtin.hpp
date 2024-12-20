// Copyright (c) 2024 Syntax Errors.
#pragma once

#include "../Object/var.hpp"

// Implement orphan built in functions
namespace Builtin {
  // Get absolute value of number
  var abs(const std::vector<ObjectPtr>& params);

  // Get rounded number
  var round(const std::vector<ObjectPtr>& params);

  // Get power from base raised to power, modulo
  var pow(const std::vector<ObjectPtr>& params);

  // Get interpretation as integer number
  var asInteger(const std::vector<ObjectPtr>& params);

  // Get interpretation as double-precision floating point number
  var asDouble(const std::vector<ObjectPtr>& params);
}
