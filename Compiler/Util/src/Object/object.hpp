// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <compare>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <utility>

#include <functional>
#include <map>
#include <type_traits>

#define implicit
#define unused [[maybe_unused]]

// Forward-declarations
class Object;
using ObjectPtr = std::shared_ptr<Object>;

class Object {
 protected:
  // Callable methods by name and parameters
  using Method = std::function<ObjectPtr(const std::vector<ObjectPtr>&)>;
  std::map<std::string, Method> _methods;

 public:
  virtual ~Object() = default;

  // ------------------ Native operators ------------------
  
  virtual operator ObjectPtr();

  // Conversion operator to bool (can be customized based on logic)
  virtual explicit operator bool() const;

  // Conversion to hash (for associative containers)
  virtual std::size_t hash() const;

  // Comparison operators
  virtual bool equals(unused const Object& other) const;

  virtual bool less(unused const Object& other) const;

  virtual bool greater(unused const Object& other) const;

  // Arithmetic operations
  virtual ObjectPtr add(unused const Object& other) const;

  virtual ObjectPtr subtract(unused const Object& other) const;

  virtual ObjectPtr multiply(unused const Object& other) const;

  virtual ObjectPtr divide(unused const Object& other) const;

  virtual ObjectPtr subscript(unused const Object& other) const;

  // Shift operations
  virtual ObjectPtr shiftLeft(unused const Object& other) const;

  virtual ObjectPtr shiftRight(unused const Object& other) const;

  // ------------------ Native methods ------------------

  // Print contents
  virtual void print(std::ostream& os) const = 0;
  
  // Clone itself
  virtual ObjectPtr clone() const = 0;

  // Check type equivalence
  virtual bool isSameType(const Object& other) const;

  // ------------------ Per-instance methods ------------------

  // Call method supported by object instance
  ObjectPtr Call(const std::string& name, std::initializer_list<ObjectPtr> params);

  // ------------------ Iterator ------------------

  class ObjectIterator;
  using ObjectIt = std::unique_ptr<Object::ObjectIterator>;

  // Default iteration behavior
  class ObjectIterator {
   public:
    virtual ~ObjectIterator() = default;

    virtual bool hasNext() const = 0;
    virtual ObjectPtr next() = 0;
    virtual ObjectIt clone() const = 0;
  };

  virtual ObjectIt getIterator() const;
};

// Hashing for associative containers
namespace std {
  template<> struct hash<Object> {
    size_t operator()(const Object& s) const noexcept
    { return s.hash();}
  };
}
