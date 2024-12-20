// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <algorithm>
#include <numeric>

#include "../Object/object.hpp"
#include "../Object/var.hpp"
#include "../Numeric/Integer.hpp"
#include "../Primitive/Boolean.hpp"
#include "../functions.hpp"

// Base template class containers of variables
template <typename Derived, template <typename...> typename ContainerType>
class Collection : public Object {
 private:

  void init() {
    _methods["pop"] = std::bind(&Collection::pop, this, std::placeholders::_1);
    _methods["clear"] = std::bind(&Collection::clear, this, std::placeholders::_1);
    _methods["remove"] = std::bind(&Collection::remove, this, std::placeholders::_1);
    // _methods["slice"] = std::bind(&Collection::slice, this, std::placeholders::_1);
    _methods["__len__"] = std::bind(&Collection::len, this, std::placeholders::_1);
    _methods["__min__"] = std::bind(&Collection::min, this, std::placeholders::_1);
    _methods["__max__"] = std::bind(&Collection::max, this, std::placeholders::_1);
    _methods["__sum__"] = std::bind(&Collection::max, this, std::placeholders::_1);
    _methods["__bool__"] = std::bind(&Collection::asBoolean, this, std::placeholders::_1);
  }
 
 protected:
  ContainerType<var> _elements;

  size_t normalizeIndex(int index) const {
    if (index < 0) {
      index += (_elements.size());
    }
    if (index < 0 || static_cast<size_t>(index) > _elements.size()) {
      throw std::out_of_range("Index out of bounds");
    }

    return static_cast<size_t>(index);
  }

 public:
  // Default constructor
  Collection() { init(); }

  // Copy constructor
  explicit Collection(const ContainerType<var>& elements) : _elements(elements) { init(); }
  explicit Collection(const Collection<Derived, ContainerType>& other) : Object(other), _elements(other._elements) { init(); }
  
  virtual ~Collection() override = default;

  inline const ContainerType<var>& getValue() const { return _elements; }

  // ------------------ Native operators ------------------

  // Combine hashes from all elements in collection
  virtual std::size_t hash() const override {
    std::size_t seed = std::size(_elements);

    for (auto& i : _elements) {
        seed ^= i.hash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    return seed;
  }

  virtual bool equals(const Object& other) const override {
    if (!isSameType(other)) { return false ;}

    auto otherObj = dynamic_cast<const Collection<Derived, ContainerType>*>(&other);
    return this->_elements == otherObj->_elements;
  }

  // ------------------ Management Methods ------------------

  // Remove specified element from collection
  virtual Method::result_type remove(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("remove: Invalid number of arguments");
    }

    auto victimSlot = std::find(
        std::begin(_elements), std::end(_elements), var(params[0])
    );

    if (victimSlot != _elements.end()) { _elements.erase(victimSlot); }
    return nullptr;
  }

  // Remove any element from collection
  virtual Method::result_type pop(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("pop: Invalid number of arguments");
    }

    if (! _elements.empty()) {
      _elements.erase(_elements.begin());
    }

    return nullptr;
  }

  // Get a slice of the elements on the collection
  // virtual Method::result_type slice(const std::vector<ObjectPtr>& params) {
  //   return generalizedSlice(
  //     _elements,
  //     params,
  //     [&](ContainerType<var>& result, const var& element) { result.insert(_elements.end(), element); },
  //     [](const ContainerType<var>& resultContainer) {
  //       return std::make_shared<Derived>(resultContainer);
  //     });
  // }

  // Remove all elements from collection
  virtual Method::result_type clear(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("clear: Invalid number of arguments");
    }

    _elements.clear();
    return nullptr;
  }

  // Amount of elements in the collection
  virtual Method::result_type len(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("__len__: Invalid number of arguments");
    }

    return std::make_shared<Integer>(_elements.size());
  }

  // Returns true if collection is not empty
  virtual Method::result_type asBoolean(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("__bool__: Invalid number of arguments");
    }

    return std::make_shared<Boolean>(_elements.size() != 0);
  }

  // Sum of all elements in the collection
  virtual Method::result_type sum(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("__sum__: Invalid number of arguments");
    }

    return var(
        std::accumulate(
            std::begin(_elements), std::end(_elements), (var) Integer(0)
        )
    ).getValue();
  }

  // Return lesser element in the collection
  virtual Method::result_type min(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("__min__: Invalid number of arguments");
    }

    auto lesserSlot = std::min_element(std::begin(_elements), std::end(_elements));
    if (lesserSlot == _elements.end()) {
      return nullptr;
    }

    return lesserSlot->getValue();
  }

  // Return greatest element in the collection
  virtual Method::result_type max(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("__max__: Invalid number of arguments");
    }

    auto greatestSlot = std::max_element(std::begin(_elements), std::end(_elements));
    if (greatestSlot == _elements.end()) {
        return nullptr;
    }

    return greatestSlot->getValue();    
  }

  // ------------------ Iteration ------------------

  class CollectionIterator : public Object::ObjectIterator {
   private:
    const Collection<Derived, ContainerType>& _collection;
    ContainerType<var>::const_iterator _currentIt;

   public:
    explicit CollectionIterator(const Collection<Derived, ContainerType>& collection):
        _collection(collection), _currentIt(_collection._elements.begin()) {}

    bool hasNext() const override {
      return _currentIt != _collection._elements.end();
    }

    ObjectPtr next() override {
      if (!this->hasNext()) {
        throw std::out_of_range("Iterator out of range");
      }

      var obj = *_currentIt;
      _currentIt = std::next(_currentIt);

      return obj.getValue();
    }

    ObjectIt clone() const override {
      return std::make_shared<CollectionIterator>(*this);
    }
  };

  // Override iteration methods
  virtual ObjectIt getIterator() const override {
    return std::make_shared<CollectionIterator>(*this);
  }
};