// Copyright (c) 2024 Syntax Errors.
#include <algorithm>

#include "./String.hpp"
#include "../Numeric/Integer.hpp"     // NOLINT
#include "../functions.hpp"           // NOLINT

void String::init() {
    _methods["slice"] = std::bind(&String::slice, this, std::placeholders::_1);
}

 String::String(std::string value) : Primitive(std::move(value)) { init(); }

// ------------------ Native overrides ------------------

String::Method::result_type String::slice(const std::vector<ObjectPtr>& params) {
    return generalizedSlice(
        this->value,
        params,
        [](std::string& result, const char& element) { result += element; },
        [](const std::string& resultContainer) {
        return std::make_shared<String>(resultContainer);
        }
    );    // NOLINT
}

// ------------------ Native operators ------------------

String::operator ObjectPtr() {
    return std::make_shared<String>(*this);
};

ObjectPtr String::add(const Object& other) const {
    auto otherObj = dynamic_cast<const String*>(&other);
    if (otherObj) {
        return std::make_shared<String>(value + otherObj->getValue());
    }

    throw std::runtime_error("Cannot concat non string type");
}

ObjectPtr String::subscript(const Object& other) const  {
    auto otherObj = dynamic_cast<const Integer*>(&other);
    if (otherObj) {
        auto result = value[otherObj->getValue()];
        return std::make_shared<String>(std::string(1, result));
    }

    throw std::runtime_error("Cannot Index with non integer type");
}

String String::operator+(const String& other) {
    return String(this->value + other.value);
}

String String::operator+(const std::string& other) {
    return String(this->value + other);
}

String String::operator+(const char* other) {
    return String(this->value + std::string(other));
}

String String::operator+(const char other) {
    return String(this->value + other);
}

String::operator bool() const  {
    return value.empty();
}

// ------------------ Iteration ------------------

String::StringIterator::StringIterator(const std::string& str) : str(str), currentIndex(0) {}

bool String::StringIterator::hasNext() const  {
    return currentIndex < str.size();
}

ObjectPtr String::StringIterator::next()  {
    if (!hasNext()) {
    throw std::out_of_range("Iterator out of range");
    }
    // Wrap each character as a `String` object
    return std::make_shared<String>(std::string(1, str[currentIndex++]));
}

String::ObjectIt String::StringIterator::clone() const {
    return std::make_unique<StringIterator>(*this);
}

String::ObjectIt String::getIterator() const  {
    return std::make_unique<StringIterator>(value);
}
