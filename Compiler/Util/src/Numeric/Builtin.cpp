// Copyright (c) 2024 Syntax Errors.
#include <cmath>

#include "./Builtin.hpp"
#include "./Integer.hpp"
#include "./Double.hpp"

// Implement orphan built in functions
namespace Builtin {
  var abs(const std::vector<var>& params) {
    if (params.size() != 1) {
      std::cerr << "abs: Invalid number of arguments\n";
      return nullptr;
    }

    ObjectPtr obj = params[0].getValue();

    if (
        ! dynamic_cast<const Double*>(obj.get())
        && ! dynamic_cast<const Integer*>(obj.get())
    )
    std::cerr << "Unexpected type. Expected Integer or Double\n";

    return obj->Call("__abs__", {});
  }

  var round(const std::vector<var>& params) {
    if (params.size() != 1) {
      std::cerr << "round: Invalid number of arguments\n";
      return nullptr;
    }

    ObjectPtr obj = params[0].getValue();

    {
      auto doubleObj = dynamic_cast<const Double*>(obj.get());
      if ( doubleObj ) { 
        return (var) std::make_shared<Double>(std::round(doubleObj->getValue())); 
      }
    }

    {
      auto integerObj = dynamic_cast<const Integer*>(obj.get());
      if ( integerObj ) { 
        return (var) integerObj; 
      }
    }

    std::cerr << "Unexpected type. Expected Integer or Double\n";
    return nullptr;
  }

  var pow(const std::vector<var>& params) {
    if (params.size() < 2 || params.size() > 3) {
      std::cerr << "pow: Invalid number of arguments\n";
      return nullptr;
    }

    bool integerPow = true;

    long double base;
    {
        ObjectPtr obj = params[0].getValue();

        if (auto objNumeric = dynamic_cast<const Double*>(obj.get()) ) { 
            base = objNumeric->getValue();
            integerPow = false;
        }

        else if (auto objNumeric = dynamic_cast<const Integer*>(obj.get()) ) { 
            base = objNumeric->getValue();
        }

        else {
            std::cerr << "Unexpected base type. Expected Integer or Double.\n";
            return nullptr;
        }
    }

    long double exponent;
    {
        ObjectPtr obj = params[1].getValue();

        if (auto objNumeric = dynamic_cast<const Double*>(obj.get()) ) { 
            exponent = objNumeric->getValue();
            integerPow = false;
        }

        else if (auto objNumeric = dynamic_cast<const Integer*>(obj.get()) ) { 
            exponent = objNumeric->getValue();
        }

        else {
            std::cerr << "Unexpected exponent type. Expected Integer or Double.\n";
            return nullptr;
        }
    }

    std::size_t modulo;
    if (params.size() == 3) {
        if (! integerPow) {
            std::cerr << "Modulo parameter is only allowed if base and exponent are both Integer.\n";
            return nullptr;
        }

        ObjectPtr obj = params[2].getValue();

        if (auto objNumeric = dynamic_cast<const Integer*>(obj.get()) ) { 
            auto passedModulo = objNumeric->getValue();

            if (passedModulo < 0) {
                std::cerr << "Invalid negative modulo.\n";
                return nullptr;
            }

            modulo = passedModulo;
        } else {
            std::cerr << "Unexpected modulo type. Expected Integer.\n";
            return nullptr;
        }
    }

    if (integerPow) {
        std::size_t result = std::pow(base, exponent);
        result %= modulo;

        return (var) std::make_shared<Integer>(result);
    }

    return (var) std::make_shared<Double>(std::pow(base, exponent));
  }
}
