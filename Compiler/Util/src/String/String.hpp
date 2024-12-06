// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

#include "../Object/object.hpp"       // NOLINT
#include "../Integer/Integer.hpp"     // NOLINT
#include "../Double/Double.hpp"       // NOLINT
#include "../functions.hpp"           // NOLINT

// String class
class String : public BaseObject<String, std::string> {
 private:
  using BaseObject::value;

  void init() {
    _methods["slice"] = std::bind(&String::slice, this, std::placeholders::_1);
  }

 public:
  explicit String(std::string value) : BaseObject(std::move(value)) { init(); }
  operator ObjectPtr() override{
    return std::make_shared<String>(*this);
  };

  ObjectPtr add(const Object& other) const override {
    auto otherObj = dynamic_cast<const String*>(&other);
    if (otherObj) {
      return std::make_shared<String>(value + otherObj->getValue());
    }
    throw std::runtime_error("Cannot concat non string type");
  }

  ObjectPtr subscript(const Object& other) const override {
    auto otherObj = dynamic_cast<const Integer*>(&other);
    if (otherObj) {
      auto result = value[otherObj->getValue()];
      return std::make_shared<String>(std::string(1, result));
    }
    throw std::runtime_error("Cannot Index with non integer type");
  }

  String operator+(const String& other) {
    return String(this->value + other.value);
  }

  String operator+(const std::string& other) {
    return String(this->value + other);
  }

  String operator+(const char* other) {
    return String(this->value + std::string(other));
  }

  String operator+(const char other) {
    return String(this->value + other);
  }

  explicit operator bool() const override {
    return value.empty();
  }

  // Override iteration methods
  class StringIterator : public Object::ObjectIterator {
   private:
    const std::string& str;
    size_t currentIndex;

   public:
    explicit StringIterator(const std::string& str) : str(str), currentIndex(0) {}

    bool hasNext() const override {
      return currentIndex < str.size();
    }

    ObjectPtr next() override {
      if (!hasNext()) {
        throw std::out_of_range("Iterator out of range");
      }
      // Wrap each character as a `String` object
      return std::make_shared<String>(std::string(1, str[currentIndex++]));
    }

    ObjectIt clone() const override {
        return std::make_unique<StringIterator>(*this);
    }
  };

  ObjectIt getIterator() const override {
    return std::make_unique<StringIterator>(value);
  }

  ObjectPtr slice(std::initializer_list<ObjectPtr> params) {
    return generalizedSlice(
      this->value,
      params,
      [](std::string& result, const char& element) { result += element; },
      [](const std::string& resultContainer) {
        return std::make_shared<String>(resultContainer);
      }
    );    // NOLINT
  }
};
