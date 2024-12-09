// Copyright (c) 2024 Syntax Errors.
#include "./Builtin.hpp"
#include "../Collections/Tuple.hpp"
#include "../Collections/List.hpp"
#include "../Collections/Set.hpp"
#include "../Collections/Map.hpp"

namespace Builtin {
  var iter(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      std::cerr << "iter: Invalid number of arguments\n";
      return nullptr;
    }

    return (Iterator) params[0]->getIterator();
  }

  var next(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      std::cerr << "next: Invalid number of arguments\n";
      return nullptr;
    }

    return params[0]->Call("__next__", {});
  }

  var len(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      std::cerr << "len: Invalid number of arguments\n";
      return nullptr;
    }

    return params[0]->Call("__len__", {});
  }

  var sum(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      std::cerr << "sum: Invalid number of arguments\n";
      return nullptr;
    }

    return params[0]->Call("__sum__", {});
  }

  var min(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      std::cerr << "min: Invalid number of arguments\n";
      return nullptr;
    }

    return params[0]->Call("__min__", {});
  }

  var max(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      std::cerr << "max: Invalid number of arguments\n";
      return nullptr;
    }

    return params[0]->Call("__max__", {});
  }

  var tuple(const std::vector<ObjectPtr>& params) {
    if (params.size() > 1) {
      std::cerr << "tuple: Invalid number of arguments\n";
      return nullptr;
    }

    if (params.empty()) {
      return (var) std::make_shared<Tuple>();
    }

    auto obj = params[0];

    // From None
    if (! obj) {
      return (var) std::make_shared<Tuple>();
    }

    // From tuple
    {
      if (auto tuple = std::dynamic_pointer_cast<Tuple>(obj)) {
        return (var) std::make_shared<Tuple>(tuple->getValue());
      }
    }

    // From list
    {
      if (auto list = std::dynamic_pointer_cast<List>(obj)) {
        return (var) std::make_shared<Tuple>(list->getValue());
      }
    }

    // From set
    {
      if (auto set = std::dynamic_pointer_cast<Set>(obj)) {
        const std::unordered_set<var>& elements = set->getValue();

        return (var) std::make_shared<Tuple>(
          std::vector<var>(elements.begin(), elements.end())
        );
      }
    }

    // From dict
    {
      if (auto map = std::dynamic_pointer_cast<Map>(obj)) {
        const std::unordered_map<var, var>& elements = map->getValue();
        std::vector<var> keys;

        for (const auto& item: elements) {
          keys.push_back(item.first);
        }

        return (var) std::make_shared<Tuple>(std::vector<var>(keys));
      }
    }

    std::cerr << "Unexpected type. Expected tuple, list, set or map.\n";
    return nullptr;
  }

  var list(const std::vector<ObjectPtr>& params) {
    if (params.size() > 1) {
      std::cerr << "list: Invalid number of arguments\n";
      return nullptr;
    }

    if (params.empty()) {
      return (var) std::make_shared<List>();
    }

    auto obj = params[0];

    // From None
    if (! obj) {
      return (var) std::make_shared<List>();
    }

    // From tuple
    {
      if (auto tuple = std::dynamic_pointer_cast<Tuple>(obj)) {
        return (var) std::make_shared<List>(tuple->getValue());
      }
    }

    // From list
    {
      if (auto list = std::dynamic_pointer_cast<List>(obj)) {
        return (var) std::make_shared<List>(list->getValue());
      }
    }

    // From set
    {
      if (auto set = std::dynamic_pointer_cast<Set>(obj)) {
        const std::unordered_set<var>& elements = set->getValue();

        return (var) std::make_shared<List>(
          std::vector<var>(elements.begin(), elements.end())
        );
      }
    }

    // From dict
    {
      if (auto map = std::dynamic_pointer_cast<Map>(obj)) {
        const std::unordered_map<var, var>& elements = map->getValue();
        std::vector<var> keys;

        for (const auto& item: elements) {
          keys.push_back(item.first);
        }

        return (var) std::make_shared<List>(std::vector<var>(keys));
      }
    }

    std::cerr << "Unexpected type. Expected tuple, list, set or map.\n";
    return nullptr;
  }

  var set(const std::vector<ObjectPtr>& params) {
    if (params.size() > 1) {
      std::cerr << "set: Invalid number of arguments\n";
      return nullptr;
    }

    if (params.empty()) {
      return (var) std::make_shared<Set>();
    }

    auto obj = params[0];

    // From None
    if (! obj) {
      return (var) std::make_shared<Set>();
    }

    // From tuple
    {
      if (auto tuple = std::dynamic_pointer_cast<Tuple>(obj)) {
        const std::vector<var>& elements = tuple->getValue();
        return (var) std::make_shared<Set>(
          std::unordered_set<var>(elements.begin(), elements.end())
        );
      }
    }

    // From list
    {
      if (auto list = std::dynamic_pointer_cast<List>(obj)) {
        const std::vector<var>& elements = list->getValue();
        return (var) std::make_shared<Set>(
          std::unordered_set<var>(elements.begin(), elements.end())
        );
      }
    }

    // From set
    {
      if (auto set = std::dynamic_pointer_cast<Set>(obj)) {
        const std::unordered_set<var>& elements = set->getValue();

        return (var) std::make_shared<Set>(elements);
      }
    }

    // From dict
    {
      if (auto map = std::dynamic_pointer_cast<Map>(obj)) {
        const std::unordered_map<var, var>& elements = map->getValue();
        std::vector<var> keys;

        for (const auto& item: elements) {
          keys.push_back(item.first);
        }

        return (var) std::make_shared<Set>(
          std::unordered_set<var>(keys.begin(), keys.end())
        );
      }
    }

    std::cerr << "Unexpected type. Expected tuple, list, set or map.\n";
    return nullptr;
  }

  var dict(const std::vector<ObjectPtr>& params) {
    if (params.size() > 1) {
      std::cerr << "dict: Invalid number of arguments\n";
      return nullptr;
    }

    if (params.empty()) {
        return (var) std::make_shared<Map>();
    }

    auto obj = params[0];

    // From None
    if (! obj) {
      return (var) std::make_shared<Map>();
    }

    // From dict
    {
      if (auto map = std::dynamic_pointer_cast<Map>(obj)) {
        return (var) std::make_shared<Map>(*map);
      }
    }

    // Find match in any other collection
    std::vector<Pair> pairs;
    bool selectedPairs = false;

    // From tuple
    if (! selectedPairs) {
      if (auto tuple = std::dynamic_pointer_cast<Tuple>(obj)) {
        selectedPairs = true;

        if (tuple->getValue().empty()) {
          return (var) std::make_shared<Map>();
        }

        for (const var& item: tuple->getValue()) {
          std::vector<var> kv;

          if (auto pairTuple = std::dynamic_pointer_cast<Tuple>(item.getValue())) {
            kv = pairTuple->getValue();
          } else if (auto pairList = std::dynamic_pointer_cast<List>(item.getValue())) {
            kv = pairList->getValue();
          } else if (auto pairSet = std::dynamic_pointer_cast<Set>(item.getValue())) {
            const std::unordered_set<var>& elements = pairSet->getValue();
            kv = std::vector<var>(elements.begin(), elements.end());
          } else {
            std::cerr << "dict: Only tuple of key-value pairs allowed.\n";
            return nullptr;
          }
          
          if (kv.size() != 2) {
            std::cerr << "dict: Invalid key-value pair inside tuple.\n";
            return nullptr;
          }

          pairs.push_back(Pair(kv[0], kv[1]));
        }
      }
    }

    // From list
    if (! selectedPairs) {
      if (auto list = std::dynamic_pointer_cast<List>(obj)) {
        selectedPairs = true;

        if (list->getValue().empty()) {
          return (var) std::make_shared<Map>();
        }

        for (const var& item: list->getValue()) {
          std::vector<var> kv;

          if (auto pairTuple = std::dynamic_pointer_cast<Tuple>(item.getValue())) {
            kv = pairTuple->getValue();
          } else if (auto pairList = std::dynamic_pointer_cast<List>(item.getValue())) {
            kv = pairList->getValue();
          } else if (auto pairSet = std::dynamic_pointer_cast<Set>(item.getValue())) {
            const std::unordered_set<var>& elements = pairSet->getValue();
            kv = std::vector<var>(elements.begin(), elements.end());
          } else {
            std::cerr << "dict: Only list of key-value pairs allowed.\n";
            return nullptr;
          }
          
          if (kv.size() != 2) {
            std::cerr << "dict: Invalid key-value pair inside list.\n";
            return nullptr;
          }

          pairs.push_back(Pair(kv[0], kv[1]));
        }
      }
    }

    // From set
    if (! selectedPairs) {
      if (auto set = std::dynamic_pointer_cast<Set>(obj)) {
        selectedPairs = true;

        if (set->getValue().empty()) {
          return (var) std::make_shared<Map>();
        }

        for (const var& item: set->getValue()) {
          std::vector<var> kv;

          if (auto pairTuple = std::dynamic_pointer_cast<Tuple>(item.getValue())) {
            kv = pairTuple->getValue();
          } else if (auto pairList = std::dynamic_pointer_cast<List>(item.getValue())) {
            kv = pairList->getValue();
          } else if (auto pairSet = std::dynamic_pointer_cast<Set>(item.getValue())) {
            const std::unordered_set<var>& elements = pairSet->getValue();
            kv = std::vector<var>(elements.begin(), elements.end());
          } else {
            std::cerr << "dict: Only set of key-value pairs allowed.\n";
            return nullptr;
          }
          
          if (kv.size() != 2) {
            std::cerr << "dict: Invalid key-value pair inside set.\n";
            return nullptr;
          }

          pairs.push_back(Pair(kv[0], kv[1]));
        }
      }
    }

    // Check if any match succeeded
    if (! selectedPairs) {
      std::cerr << "Unexpected type. Expected tuple, list, set or map.\n";
      return nullptr;
    }

    return (var) std::make_shared<Map>(pairs);
  }

  var inlineTuple(const std::vector<ObjectPtr>& params) {
    std::vector<var> elements;

    for (const ObjectPtr& obj : params) {
      elements.push_back(obj);
    }

    return (var) std::make_shared<Tuple>(elements);
  }

  var inlineList(const std::vector<ObjectPtr>& params) {
    std::vector<var> elements;

    for (const ObjectPtr& obj : params) {
      elements.push_back(obj);
    }

    return (var) std::make_shared<List>(elements);
  }

  var inlineSet(const std::vector<ObjectPtr>& params) {
    std::unordered_set<var> elements;

    for (const ObjectPtr& obj : params) {
      elements.insert(obj);
    }

    return (var) std::make_shared<Set>(elements);
  }

  var inlineDict(const std::vector<Pair>& params) {
    return (var) std::make_shared<Map>(params);
  }
}
