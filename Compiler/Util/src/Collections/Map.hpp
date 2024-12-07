// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <unordered_map>
#include <memory>
#include <utility>
#include <vector>

#include "../Object/object.hpp"
#include "../Object/var.hpp"
#include "./Pair.hpp"
#include "./List.hpp"
#include <list>

class Map : public Object {
 private:
  std::unordered_map<var, var> elements;

  void init() override {
    _methods["keys"] = std::bind(&Map::keys, this, std::placeholders::_1);
    _methods["values"] = std::bind(&Map::values, this, std::placeholders::_1);
    _methods["items"] = std::bind(&Map::items, this, std::placeholders::_1);
    _methods["addElement"] = std::bind(&Map::addElement, this, std::placeholders::_1);
    _methods["pop"] = std::bind(&Map::pop, this, std::placeholders::_1);
    _methods["clear"] = std::bind(&Map::clear, this, std::placeholders::_1);
    _methods["get"] = std::bind(&Map::get, this, std::placeholders::_1);
    _methods["__len__"] = std::bind(&Map::len, this, std::placeholders::_1);
  }

 public:
  Map(): elements() { this->init(); }

  Map(const Map& other) : Object(other), elements(other.elements) {
      this->init();  // Initialize methods for the new object
  }
  Map(std::initializer_list<std::pair<var, var>> initList) {
    for (const auto& pair : initList) {
      elements[pair.first] = pair.second;
    }
    this->init();
  }
  template <typename... Args>
  explicit Map(Args&&... args) {
    if constexpr (sizeof...(args) > 0) {
      (addPair(std::forward<Args>(args)), ...);
    }
    this->init();
  }

  // ------------------ Native overrides ------------------

  // Override the addition method to implement map addition
  ObjectPtr add(unused const Object& other) const override {
    // Implement map addition logic
    return std::make_shared<Map>(*this);
  }

  // Get value from associated key-value pair
  ObjectPtr subscript(const Object& other) const override {
    for (const auto& kv : this->elements) {
      if (kv.first->equals(other)) {
        return kv.second.getValue();
      }
    }
    std::cerr << "Key not found\n";
    return nullptr;
  }

  // Test equality with other maps
  bool equals(const Object& other) const override {
    auto otherMap  = dynamic_cast<const Map*>(&other);
    if (!otherMap) {
      throw std::invalid_argument("Cannot compare map with given type");
    }
    return elements == otherMap->elements;
  }

  // Print contents
  void print(std::ostream& os) const override {
    os << "{";
    for (auto it = elements.begin(); it != elements.end(); ++it) {
      os << it->first << ": " << it->second;
      if (std::next(it) != elements.end()) {
        os << ", ";
      }
    }
    os << "}";
  }

  // Clone itself
  ObjectPtr clone() const override {
    return std::make_shared<Map>(*this);
  }

  // ------------------ Native operators ------------------

  // Access elements in key-value pairs by key
  var operator[](const var& key) const {
    auto it = elements.find(key);
    if (it != elements.end()) {
      return it->second;
    } else {
      std::cerr << "Key not found\n";
      return var();  // Or throw an exception
    }
  }

  // Return merge of itself and another map
  Map operator+(const Map& other) const {
    Map result = *this;   // Start with a copy of the current map
    for (const auto& pair : other.elements) {
      result.elements[pair.first] = pair.second;  // Overwrite or insert new key-value pairs
    }
    return result;
  }

  // ------------------ Management Methods ------------------

  // Add key-value entry 
  Method::result_type addElement(const std::vector<ObjectPtr>& params) {
    if (params.size() != 2) {
      throw std::runtime_error("addElement: Invalid number of arguments");
    }

    var key = params[0];
    var value = params[1];

    if (!key.getValue()) {
      throw std::runtime_error("Map: cannot add null key");
    }

    auto it = this->elements.find(key);
    if (it == this->elements.end()) {
      std::cout << "New Key" << std::endl;
      this->elements.insert({key, value});
    }

    return nullptr;
  }

  // Remove key-value entry by given key
  Method::result_type pop(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("pop: Invalid number of arguments");
    }

    var key = params[0];
    auto it = elements.find(key);

    if (it != elements.end()) {
      var removedElement = it->second;
      elements.erase(it);
      return removedElement.getValue();
    } else {
      std::cerr << "Key not found\n";
      return nullptr;
    }
  }

  // Remove all key-value entries
  Method::result_type clear(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("clear: Invalid number of arguments");
    }

    elements.clear();
    return nullptr;
  }

  size_t size() const {
    return elements.size();
  }

  // Amount of key-value entries in the map
  Method::result_type len(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("__len__: Invalid number of arguments");
    }

    return std::make_shared<Integer>(elements.size());
  }

  // Returns a list of all keys in the map
  Method::result_type keys(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("keys: Invalid number of arguments");
    }

    std::vector<var> keyList;
    for (const auto& kv : elements) {
      keyList.push_back(kv.first);
    }
    return std::make_shared<List>(keyList);
  }

  // Returns a list of all values in the map
  Method::result_type values(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("values: Invalid number of arguments");
    }
  
    std::vector<var> valueList;
    for (const auto& kv : elements) {
      valueList.push_back(kv.second);
    }
    return std::make_shared<List>(valueList);
  }

  // Returns a list of key-value pairs as Pair objects
  Method::result_type items(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("items: Invalid number of arguments");
    }

    std::vector<var> itemList;
    for (const auto& kv : elements) {
      itemList.push_back(var(Pair(kv.first, kv.second)));
    }
    return std::make_shared<List>(itemList);
  }

  // Get value associated with key-value pair by key
  Method::result_type get(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("get: Invalid number of arguments");
    }

    var key = params[0];
    auto it = elements.find(key);

    if (it != elements.end()) {
      var match = it->second;
      return match.getValue();
    }
    
    std::cerr << "get: Key not found\n";
    return nullptr;
  }

  // ------------------ Iterator ------------------

  class MapIterator : public Object::ObjectIterator {
   private:
    const Map& _map;
    size_t _currentIndex;

   public:
    explicit MapIterator(const Map& list) : _map(list), _currentIndex(0) {}

    bool hasNext() const override {
      return _currentIndex < _map.size();
    }

    ObjectPtr next() override {
      if (!this->hasNext()) {
        throw std::out_of_range("Iterator out of range");
      }

      auto it = _map.elements.begin();
      std::advance(it, _currentIndex++);

      // TODO(Dwayne): find an approiate way of returning the key-value pair.
      return std::make_shared<Pair>(it->first, it->second);
    }

    ObjectIt clone() const override {
      return std::make_unique<MapIterator>(*this);
    }
  };

  // Override iteration methods
  ObjectIt getIterator() const override {
    return std::make_unique<MapIterator>(*this);
  }

};
