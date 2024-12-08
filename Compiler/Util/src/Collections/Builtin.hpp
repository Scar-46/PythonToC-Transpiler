// Copyright (c) 2024 Syntax Errors.
#pragma once

#include "../Object/var.hpp"

// Implement orphan built in functions
namespace Builtin {
  // Get an iterator to the collection / container
  var iter(const std::vector<var>& params);

  // Get the next iterator in a sequence from another
  var next(const std::vector<var>& params);

  // Get amount of elements in the collection / container
  var len(const std::vector<var>& params);

  // Sum all of the elements in the collection / contaienr
  var sum(const std::vector<var>& params);

  // Get the lesser of the elements in the collection / contaienr
  var min(const std::vector<var>& params);

  // Get the greatest of the elements in the collection / contaienr
  var max(const std::vector<var>& params);
}
