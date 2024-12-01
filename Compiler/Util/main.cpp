#include "util.hpp"
#include <vector>

int main(void) {
    ObjectPtr string = std::make_shared<String>("Hello world!");
    ObjectPtr integer = std::make_shared<Integer>(243);

    std::vector<ObjectPtr> variables = std::vector<ObjectPtr>(string, integer);

    for (auto variable : variables) {
      variable->print();
    }
}