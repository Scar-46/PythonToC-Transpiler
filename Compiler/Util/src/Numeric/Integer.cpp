// Copyright (c) 2024 Syntax Errors.
#include "./Integer.hpp"
#include "./Double.hpp"

Integer::Integer(int32_t value) : Numeric(value) {}

Integer::operator ObjectPtr(){
    return std::make_shared<Integer>(*this);
};

ObjectPtr Integer::addHelper(const Object& other) const {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
        return std::make_shared<Integer>(value + otherObj->getValue());
    }

    throw std::runtime_error("Integer does not support addition with given type");
}

ObjectPtr Integer::subtractHelper(const Object& other) const {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
        return std::make_shared<Integer>(value - otherObj->getValue());
    }

    throw std::runtime_error("Integer does not support subtraction with given type");
}

ObjectPtr Integer::multiplyHelper(const Object& other) const {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
        return std::make_shared<Integer>(value + otherObj->getValue());
    }

    throw std::runtime_error("Integer does not support multiplication with given type");
}

ObjectPtr Integer::divideHelper(const Object& other) const {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
        return std::make_shared<Integer>(value + otherObj->getValue());
    }

    throw std::runtime_error("Integer does not support division with given type");
}

bool Integer::equalsHelper(const Object& other) const {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
        return value == otherObj->getValue();
    }

    return false;
}

bool Integer::lessHelper(const Object& other) const {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
        return value < otherObj->getValue();
    }

    return false;
}

bool Integer::greaterHelper(const Object& other) const {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
        return value > otherObj->getValue();
    }

    return false;
}
