// Copyright (c) 2024 Syntax Errors.
#pragma once

#include "../Object/var.hpp"

// Implement orphan built in functions
namespace Builtin {
  // Get variable representation as string
  var asString(const std::vector<ObjectPtr>& params);

  // Get variable representation as boolean
  var asBoolean(const std::vector<ObjectPtr>& params);
}