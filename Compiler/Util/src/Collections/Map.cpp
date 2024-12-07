// Copyright (c) 2024 Syntax Errors.
#include "Map.hpp"

// ------------------ Private methods ------------------

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

// ------------------ Constructors and destructor ------------------
Map::Map() : elements() { init(); }

Map::Map(const Map& other) : Object(other), elements(other.elements) {
  this->init();
}

Map::Map(std::initializer_list<std::pair<var, var>> initList) {
  for (const auto& pair : initList) {
    elements[pair.first] = pair.second;
  }
  this->init();
}

// ------------------ Native overrides ------------------

ObjectPtr Map::add(unused const Object& other) const {
  return std::make_shared<Map>(*this);
}

ObjectPtr Map::subscript(const Object& other) const {
  for (const auto& kv : this->elements) {
    if (kv.first->equals(other)) {
      return kv.second.getValue();
    }
  }
  std::cerr << "Key not found\n";
  return nullptr;
}

bool Map::equals(const Object& other) const {
  auto otherMap = dynamic_cast<const Map*>(&other);
  if (!otherMap) {
    throw std::invalid_argument("Cannot compare map with given type");
  }
  return elements == otherMap->elements;
}

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

ObjectPtr Map::clone() const {
  return std::make_shared<Map>(*this);
}

// ------------------ Native operators ------------------

var Map::operator[](const var& key) const {
  auto it = elements.find(key);
  if (it != elements.end()) {
    return it->second;
  } else {
    std::cerr << "Key not found\n";
    return var();
  }
}

Map Map::operator+(const Map& other) const {
  Map result = *this;
  for (const auto& pair : other.elements) {
    result.elements[pair.first] = pair.second;
  }
  return result;
}

#include "Map.hpp"

// ------------------ Management Methods ------------------
using Method = std::function<ObjectPtr(const std::vector<ObjectPtr>&)>;

Method::result_type Map::addElement(const std::vector<ObjectPtr>& params) {
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

Method::result_type Map::pop(const std::vector<ObjectPtr>& params) {
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

Method::result_type Map::clear(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("clear: Invalid number of arguments");
  }

  elements.clear();
  return nullptr;
}

size_t Map::size() const {
  return elements.size();
}

Method::result_type Map::keys(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("keys: Invalid number of arguments");
  }

  std::vector<var> keyList;
  for (const auto& kv : elements) {
    keyList.push_back(kv.first);
  }
  return std::make_shared<List>(keyList);
}

Method::result_type Map::values(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("values: Invalid number of arguments");
  }

  std::vector<var> valueList;
  for (const auto& kv : elements) {
    valueList.push_back(kv.second);
  }
  return std::make_shared<List>(valueList);
}

Method::result_type Map::items(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("items: Invalid number of arguments");
  }

  std::vector<var> itemList;
  for (const auto& kv : elements) {
    itemList.push_back(var(Pair(kv.first, kv.second)));
  }
  return std::make_shared<List>(itemList);
}

Method::result_type Map::get(const std::vector<ObjectPtr>& params) {
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

Method::result_type Map::len(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("__len__: Invalid number of arguments");
  }

  return std::make_shared<Integer>(elements.size());
}

Method::result_type Map::min(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("__min__: Invalid number of arguments");
  }

  auto lesserSlot = std::min_element(std::begin(elements), std::end(elements));
  if (lesserSlot == elements.end()) {
    return nullptr;
  }

  return std::make_shared<Integer>(elements.size());
}

Method::result_type Map::max(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("__max__: Invalid number of arguments");
  }

  auto greatestSlot = std::max_element(std::begin(elements), std::end(elements));
  if (greatestSlot == elements.end()) {
    return nullptr;
  }

  return std::make_shared<Integer>(elements.size());
}

Method::result_type Map::sum(const std::vector<ObjectPtr>& params) {
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
using ObjectIt = std::shared_ptr<Object::ObjectIterator>;

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

ObjectIt Map::MapIterator::clone() const {
  return std::make_unique<MapIterator>(*this);
}

ObjectIt Map::getIterator() const {
  return std::make_unique<MapIterator>(*this);
}
