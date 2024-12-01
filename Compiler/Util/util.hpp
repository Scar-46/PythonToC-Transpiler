#pragma once
// Copyright (c) 2024 Syntax Errors.
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>


class Object;

using ObjectPtr = std::shared_ptr<Object>;

class Object {
 public:
  virtual ~Object() = default;

  virtual void print() const = 0;

  virtual ObjectPtr clone() const = 0;
};

class DynamicContext {
  std::unordered_map<std::string, ObjectPtr> variables;

 public:
  void set(const std::string& name, ObjectPtr value) {
    variables[name] = value;
  }

  ObjectPtr get(const std::string& name) const {
    auto it = variables.find(name);
    if (it != variables.end()) {
      return it->second;
    }
    throw std::runtime_error("Variable not found: " + name);
  }
};

class Integer : public Object {
  int value;

 public:
  explicit Integer(int value) : value(value) {}
  ~Integer() override = default;

  inline void print() const override {
    std::cout << "Integer: " << value << std::endl;
  }

  inline ObjectPtr clone() const override {
    return std::make_shared<Integer>(value);
  }
};

class String : public Object {
  std::string value;

 public:
  explicit String(std::string value) : value(value) {}
  ~String() override = default;

  inline void print() const override {
    std::cout << "String: " << value << std::endl;
  }

  inline ObjectPtr clone() const override {
    return std::make_shared<String>(value);
  }
};

class DynamicContext {
  std::unordered_map<std::string, ObjectPtr> variables;

 public:
  void set(const std::string& name, ObjectPtr value) {
    variables[name] = value;
  }

  ObjectPtr get(const std::string& name) const {
    auto it = variables.find(name);
    if (it != variables.end()) {
      return it->second;
    }
    throw std::runtime_error("Variable not found: " + name);
  }
};
