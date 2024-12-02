#pragma once
// Copyright (c) 2024 Syntax Errors.
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

class Object;
using ObjectPtr = std::shared_ptr<Object>;

class Factory {
 private:
  using Creator = std::function<ObjectPtr(const void*)>;
  static std::unordered_map<std::type_index, Creator>& registry() {
    static std::unordered_map<std::type_index, Creator> instance;
    return instance;
  }

 public:
  template <typename Derived, typename ValueType>
  static void register_type() {
    registry()[std::type_index(typeid(ValueType))] = [](const void* value) {
      return std::make_shared<Derived>(*static_cast<const ValueType*>(value));
    };
    std::cout << "Registering";
  }

  template <typename ValueType>
  static ObjectPtr create(const ValueType& value) {
    auto it = registry().find(std::type_index(typeid(ValueType)));
    if (it != registry().end()) {
      return it->second(&value);
    }
    throw std::runtime_error("Type not registered in Factory");
  }
};
