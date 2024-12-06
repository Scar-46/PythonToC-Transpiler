// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <memory>
#include <tuple>

#include "./Object/object.hpp"
#include "./Integer/Integer.hpp"

// Helper to safely extract a value or use a default
template <typename T>
T getValueOrDefault(const ObjectPtr& obj, const T& defaultValue) {
  if (auto ptr = dynamic_cast<const T*>(&*obj)) {
    return *ptr;
  }
  return defaultValue;
}

// Iterate over tuple elements and assign values from params
template <typename Tuple, typename Iterator>
void assignValues(Tuple& values, Iterator& it, Iterator end) {  // NOLINT
  std::apply([&](auto&... args) {
    ((it != end ? args = getValueOrDefault(*it++, args) : args), ...);
  }, values);
}

template <typename Container, typename AddElementFn, typename ResultFactoryFn>
ObjectPtr generalizedSlice(
  const Container& container,
  std::initializer_list<ObjectPtr> params,
  AddElementFn addElementFn,
  ResultFactoryFn resultFactoryFn) {
  // Default values for start, end, and step
  std::tuple<Integer, Integer, Integer> values = {Integer(0), Integer(-1), Integer(1)};
  auto it = params.begin();
  assignValues(values, it, params.end());

  int start = std::get<0>(values).getValue();
  int end = std::get<1>(values).getValue();
  int step = std::get<2>(values).getValue();

  if (step == 0) {
    throw std::invalid_argument("Step cannot be zero");
  }

  size_t containerSize = container.size();

  // Normalize start and end indices
  auto normalizeIndex = [containerSize](int index) -> size_t {
    if (index < 0) {
      index += static_cast<int>(containerSize);
    }
    return std::clamp(index, 0, static_cast<int>(containerSize));
  };

  size_t normalizedStart = normalizeIndex(start);
  size_t normalizedEnd = normalizeIndex(end);

  // Create a new container for the result
  Container resultContainer;

  if (step > 0) {
    for (size_t i = normalizedStart; i < normalizedEnd; i += step) {
      addElementFn(resultContainer, container[i]);
    }
  } else {
    if (normalizedStart < normalizedEnd) {
      throw std::invalid_argument("For negative step, start must be greater than end.");
    }
    for (int i = static_cast<int>(normalizedStart); i >= static_cast<int>(normalizedEnd); i += step) {
      addElementFn(resultContainer, container[i]);
    }
  }

  // Use the result factory function to create the final ObjectPtr
  return resultFactoryFn(resultContainer);
}

