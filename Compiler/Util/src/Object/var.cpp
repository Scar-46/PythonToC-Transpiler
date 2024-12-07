// Copyright (c) 2024 Syntax Errors.

#include "./var.hpp"
#include "./Boolean/Boolean.hpp"
#include "./Numeric/Double.hpp"
#include "./Numeric/Integer.hpp"
#include "./String/String.hpp"

// ------------------ Native methods ------------------

var::var(int32_t value) : value(std::make_shared<Integer>(value)) {}
var::var(double value) : value(std::make_shared<Double>(value)) {}
var::var(const std::string& value) : value(std::make_shared<String>(value)) {}
var::var(const char* value) : value(std::make_shared<String>(std::string(value))) {}
var::var(bool value) : value(std::make_shared<Boolean>(value)) {}

// ------------------ Iterator ------------------

Iterator& Iterator::operator++() {
    if (!objectIterator || isEnd) {
      throw std::runtime_error("Incrementing an invalid iterator");
    }
    if (!objectIterator->hasNext()) {
      isEnd = true;
    }
    return *this;
  }

bool Iterator::operator!=(const Iterator& other) const {
    return isEnd != other.isEnd;  // Simple end-check comparison
}

var Iterator::operator*() const {
    if (!objectIterator || isEnd || !objectIterator->hasNext()) {
        throw std::runtime_error("Dereferencing an invalid iterator");
    }

    return var(objectIterator->next());
}

var Iterator::operator*() {
    if (!objectIterator || isEnd || !objectIterator->hasNext()) {
        throw std::runtime_error("Dereferencing an invalid iterator");
    }

    return var(objectIterator->next());
}
