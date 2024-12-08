// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <memory>
#include <utility>
#include "../Object/object.hpp"
#include "../Object/var.hpp"

class Pair : public Object {
private:
    std::pair<var, var> value;

public:
  // ------------------ Constructors and destructor ------------------

    // Default constructor
    Pair();
    // Parameterized constructor
    Pair(var first, var second);
    // Copy constructor
    Pair(const Pair& other);
    // Move constructor
    Pair(Pair&& other) noexcept;
    // Destructor
    ~Pair() noexcept override;

  // ------------------ Native overrides ------------------

    operator ObjectPtr() override;
    // Clone method
    ObjectPtr clone() const override;

    // Print function
    void print(std::ostream& os) const override;

  // ------------------ Native operators ------------------
  
    // Copy assignment operator
    Pair& operator=(const Pair& other);
    // Move assignment operator
    Pair& operator=(Pair&& other) noexcept;
    // Equality operators
    bool operator==(const Pair& other) const;
    bool operator!=(const Pair& other) const;
    // Comparison operators
    bool operator<(const Pair& other) const;
    bool operator<=(const Pair& other) const;
    bool operator>(const Pair& other) const;
    bool operator>=(const Pair& other) const;
    bool equals(const Object& other) const override;
    bool less(const Object& other) const override;
    bool greater(const Object& other) const override;

  // ------------------ Accessors ------------------
    var getFirst() const;
    void setFirst(const var& first);
    var getSecond() const;
    void setSecond(const var& second);
    // Swap method
    void swap(Pair& other) noexcept;

};
// Non-member swap for ADL
inline void swap(Pair& lhs, Pair& rhs) noexcept;
