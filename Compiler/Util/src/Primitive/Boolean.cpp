// Copyright (c) 2024 Syntax Errors.
#include "./Boolean.hpp"
#include "./String.hpp"

Boolean::Boolean(bool value) : Primitive(value) {}

Boolean::operator ObjectPtr() {
    return std::make_shared<Boolean>(*this);
};

Boolean::operator bool() const { return this->value; }

void Boolean::print(std::ostream& os) const {
    os << ((this->value)? "True" : "False");
}

void Boolean::init() {
    _methods["__bool__"] = std::bind(&Boolean::asBool, this, std::placeholders::_1);
    _methods["__str__"] = std::bind(&Boolean::asString, this, std::placeholders::_1);
}

Boolean::Method::result_type Boolean::asBool(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("__bool__: Invalid number of arguments");
    }

    return std::make_shared<Boolean>(*this);
}

Boolean::Method::result_type Boolean::asString(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("__str__: Invalid number of arguments");
    }

    return std::make_shared<String>(this->value ? "True" : "False");
}
