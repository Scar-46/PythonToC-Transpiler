// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "../Object/object.hpp"
#include "../Object/var.hpp"
#include "./Pair.hpp"
#include "./List.hpp"


class Map : public Object {
 private:
  std::map<var, var> elements;

 public:
  Map() = default;
  Map(std::initializer_list<std::pair<var, var>> initList) {
    for (const auto& pair : initList) {
      elements[pair.first] = pair.second;
    }
  }
  template <typename... Args>
  Map(Args&&... args) {
    if constexpr (sizeof...(args) > 0) {
      (addPair(std::forward<Args>(args)), ...);
    }
  }
  
  void addPair(const Pair& pair) {
    elements[pair.getFirst()] = pair.getSecond();
  }

  // ------------------ Overrides ------------------

  // Override the addition method to implement map addition
  ObjectPtr add(unused const Object& other) const override {
    // Implement map addition logic
    return std::make_shared<Map>(*this);
  }

  // Override the subscript method to implement indexation
  ObjectPtr subscript(const Object& other) const override {
    for (const auto& kv : this->elements) {
      if (kv.first->equals(other)) {
        return kv.second.getValue();
      }
    }
    std::cerr << "Key not found\n";
    return nullptr;
  }

  bool equals(const Object& other) const override {
    auto otherMap  = dynamic_cast<const Map*>(&other);
    if (!otherMap) {
      throw std::invalid_argument("add method requires a List");
    }
    return elements == otherMap->elements;
  }

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

  ObjectPtr clone() const override {
    return std::make_shared<Map>(*this);
  }

  // ------------------ Map Methods ------------------
  void addElement(const var& key, const var& value) {
    if (!key) {
      throw std::runtime_error("Map: cannot add null key");
    }
    auto it = this->elements.find(key);
    if (it == this->elements.end()) {
      std::cout << "New Key" << std::endl;
      this->elements.insert({key, value});
    }
  }

  var popElement(const var& key) {
    auto it = elements.find(key);
    if (it != elements.end()) {
      var removedElement = it->second;
      elements.erase(it);
      return removedElement;
    } else {
      std::cerr << "Key not found\n";
      return var();
    }
  }

  void clear() {
    elements.clear();
  }

  size_t size() const {
    return elements.size();
  }

  // ------------------ Keys, Values, and Items Methods ------------------

    // Returns a list of all keys in the map
    ObjectPtr keys() const {
      std::vector<var> keyList;
      for (const auto& kv : elements) {
        keyList.push_back(kv.first);
      }
      return std::make_shared<List>(keyList);
    }

    // Returns a list of all values in the map
    ObjectPtr values() const {
      std::vector<var> valueList;
      for (const auto& kv : elements) {
        valueList.push_back(kv.second);
      }
      return std::make_shared<List>(valueList);
    }

  // Returns a list of key-value pairs as Pair objects
  ObjectPtr items() const {
    std::vector<var> itemList;
    for (const auto& kv : elements) {
      itemList.push_back(var(Pair(kv.first, kv.second)));
    }
    return std::make_shared<List>(itemList);
  }


  // ------------------ Operator Overloading ------------------
  // Overload the [] operator to access elements by key
  var operator[](const var& key) const {
    auto it = elements.find(key);
    if (it != elements.end()) {
      return it->second;
    } else {
      std::cerr << "Key not found\n";
      return var();  // Or throw an exception
    }
  }

  // Overload the + operator to merge two maps
  Map operator+(const Map& other) const {
    Map result = *this;   // Start with a copy of the current map
    for (const auto& pair : other.elements) {
      result.elements[pair.first] = pair.second;  // Overwrite or insert new key-value pairs
    }
    return result;
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
