// Copyright (c) 2024 Syntax Errors.
#include <memory>

#include "./Boolean.hpp"

Boolean::Boolean(bool value) : Primitive(value) {}

Boolean::operator ObjectPtr() {
    return std::make_shared<Boolean>(*this);
};

Boolean::operator bool() const { return this->value; }

void Boolean::print(std::ostream& os) const {
    os << ((this->value)? "True" : "False");
}


