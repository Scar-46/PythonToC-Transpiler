// Copyright (c) 2024 Syntax Errors.
#pragma once
#include <map>

#include "../Object/object.hpp"

class Dictionary : public Object {
 private:
  std::map<ObjectPtr, ObjectPtr> entries;

 public:
  void add_entry(const ObjectPtr& key, ObjectPtr value) {
    entries[key] = value;
  }

  //   void subscript
  ObjectPtr subscript(const ObjectPtr& key) {
    throw std::runtime_error("Hehe");
  }
};
