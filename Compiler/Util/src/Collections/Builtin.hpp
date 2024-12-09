// Copyright (c) 2024 Syntax Errors.
#pragma once

#include "../Object/var.hpp"
#include "../Collections/Pair.hpp"

// Implement orphan built in functions
namespace Builtin {
  // Get an iterator to the collection / container
  var iter(const std::vector<ObjectPtr>& params);

  // Get the next iterator in a sequence from another
  var next(const std::vector<ObjectPtr>& params);

  // Get amount of elements in the collection / container
  var len(const std::vector<ObjectPtr>& params);

  // Sum all of the elements in the collection / contaienr
  var sum(const std::vector<ObjectPtr>& params);

  // Get the lesser of the elements in the collection / contaienr
  var min(const std::vector<ObjectPtr>& params);

  // Get the greatest of the elements in the collection / contaienr
  var max(const std::vector<ObjectPtr>& params);

  // Construct a tuple
  var tuple(const std::vector<ObjectPtr>& params);

  // Construct a list
  var list(const std::vector<ObjectPtr>& params);

  // Construct a set
  var set(const std::vector<ObjectPtr>& params);

  // Construct a map
  var dict(const std::vector<ObjectPtr>& params);

  // Construct a tuple from inline definition
  var inlineTuple(const std::vector<ObjectPtr>& params);

  // Construct a list from inline definition
  var inlineList(const std::vector<ObjectPtr>& params);

  // Construct a list from inline definition
  var inlineSet(const std::vector<ObjectPtr>& params);

  // Construct a map from inline definition
  var inlineDict(const std::vector<Pair>& params);
}
