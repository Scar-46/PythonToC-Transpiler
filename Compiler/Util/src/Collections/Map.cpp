// Copyright (c) 2024 Syntax Errors.
#include "Map.hpp"

void Map::init() {
  _methods["keys"] = std::bind(&Map::keys, this, std::placeholders::_1);
  _methods["values"] = std::bind(&Map::values, this, std::placeholders::_1);
  _methods["items"] = std::bind(&Map::items, this, std::placeholders::_1);
  _methods["addElement"] = std::bind(&Map::addElement, this, std::placeholders::_1);
  _methods["pop"] = std::bind(&Map::pop, this, std::placeholders::_1);
  _methods["clear"] = std::bind(&Map::clear, this, std::placeholders::_1);
  _methods["get"] = std::bind(&Map::get, this, std::placeholders::_1);
  _methods["__len__"] = std::bind(&Map::len, this, std::placeholders::_1);
  _methods["__min__"] = std::bind(&Map::min, this, std::placeholders::_1);
  _methods["__max__"] = std::bind(&Map::max, this, std::placeholders::_1);
  _methods["__sum__"] = std::bind(&Map::sum, this, std::placeholders::_1);
}

Map::Map()
  : elements() {
  this->init();
}

Map::Map(const Map& other)
  : Object(other)
  , elements(other.elements) {
  this->init();  // Initialize methods for the new object
}

Map::Map(std::initializer_list<std::pair<var, var>> initList) {
  for (const auto& pair : initList) {
    elements[pair.first] = pair.second;
  }
  this->init();
}

template <typename... Args>
Map::Map(Args&&... args) {
  if constexpr (sizeof...(args) > 0) {
    (addPair(std::forward<Args>(args)), ...);
  }
  this->init();
}

// ------------------ Native overrides ------------------
// Override the addition method to implement map addition
ObjectPtr Map::add(unused const Object& other) const {
  // Implement map addition logic
  return std::make_shared<Map>(*this);
}

// Get value from associated key-value pair
ObjectPtr Map::subscript(const Object& other) const {
  for (const auto& kv : this->elements) {
    if (kv.first->equals(other)) {
      return kv.second.getValue();
    }
  }
  std::cerr << "Key not found\n";
  return nullptr;
}

// Test equality with other maps
bool Map::equals(const Object& other) const {
  auto otherMap  = dynamic_cast<const Map*>(&other);
  if (!otherMap) {
    throw std::invalid_argument("Cannot compare map with given type");
  }
  return elements == otherMap->elements;
}

// Print contents
void Map::print(std::ostream& os) const {
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
ObjectPtr Map::clone() const {
  return std::make_shared<Map>(*this);
}

  // ------------------ Native operators ------------------

// Access elements in key-value pairs by key
var Map::operator[](const var& key) const {
  auto it = elements.find(key);
  if (it != elements.end()) {
    return it->second;
  } else {
    std::cerr << "Key not found\n";
    return var();  // Or throw an exception
  }
}

// Return merge of itself and another map
Map Map::operator+(const Map& other) const {
  Map result = *this;   // Start with a copy of the current map
  for (const auto& pair : other.elements) {
    result.elements[pair.first] = pair.second;  // Overwrite or insert new key-value pairs
  }
  return result;
}

// ------------------ Management Methods ------------------
// Add key-value entry
Object::Method::result_type Map::addElement(const std::vector<ObjectPtr>& params) {
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
Object::Method::result_type Map::pop(const std::vector<ObjectPtr>& params) {
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
Object::Method::result_type Map::clear(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("clear: Invalid number of arguments");
  }

  elements.clear();
  return nullptr;
}

size_t Map::size() const {
  return elements.size();
}

// Returns a list of all keys in the map
Object::Method::result_type Map::keys(const std::vector<ObjectPtr>& params) {
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
Object::Method::result_type Map::values(const std::vector<ObjectPtr>& params) {
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
Object::Method::result_type Map::items(const std::vector<ObjectPtr>& params) {
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
Object::Method::result_type Map::get(const std::vector<ObjectPtr>& params) {
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

  // Amount of key-value entries in the map
Object::Method::result_type Map::len(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("__len__: Invalid number of arguments");
  }

  return std::make_shared<Integer>(elements.size());
}

// Smallest key in map
Object::Method::result_type Map::min(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("__min__: Invalid number of arguments");
  }

  auto lesserSlot = std::min_element(std::begin(elements), std::end(elements));
  if (lesserSlot == elements.end()) {
    return nullptr;
  }

  return std::make_shared<Integer>(elements.size());
}

// Greatest key in map
Object::Method::result_type Map::max(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("__max__: Invalid number of arguments");
  }

  auto greatestSlot = std::max_element(std::begin(elements), std::end(elements));
  if (greatestSlot == elements.end()) {
    return nullptr;
  }

  return std::make_shared<Integer>(elements.size());
}

// Sum of all keys in the map
Object::Method::result_type Map::sum(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("__sum__: Invalid number of arguments");
  }

  var result = Integer(0);

  for (const auto& kv : elements) {
    result = result->add(kv.first);
  }

  return result.getValue();
}

// ------------------ Iterator ------------------
Map::MapIterator::MapIterator(const Map& list) : _map(list), _currentIndex(0) {}

bool Map::MapIterator::hasNext() const {
  return _currentIndex < _map.size();
}

ObjectPtr Map::MapIterator::next() {
  if (!this->hasNext()) {
    throw std::out_of_range("Iterator out of range");
  }

  auto it = _map.elements.begin();
  std::advance(it, _currentIndex++);

  return std::make_shared<Pair>(it->first, it->second);
}

Object::ObjectIt Map::MapIterator::clone() const {
  return std::make_unique<MapIterator>(*this);
}

// Override iteration methods
Object::ObjectIt Map::getIterator() const  {
  return std::make_unique<MapIterator>(*this);
}
