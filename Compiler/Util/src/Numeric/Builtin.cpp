// Copyright (c) 2024 Syntax Errors.
#include <cmath>

#include "./Builtin.hpp"
#include "./Integer.hpp"
#include "./Double.hpp"
#include "../Primitive/String.hpp"

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

  var asInteger(const std::vector<var>& params) {
    if (params.size() == 0) {
        return (var) std::make_shared<Integer>(0);
    }

    if (params.size() > 2) {
      std::cerr << "int: Invalid number of arguments\n";
      return nullptr;
    }

    ObjectPtr obj = params[0].getValue();
    {
        auto objDouble = dynamic_cast<const Double*>(obj.get());
        auto objInteger = dynamic_cast<const Integer*>(obj.get());

        if (objDouble || objInteger) {
            if (params.size() == 2) {
                std::cerr << "Base parameter is discarded when first parameter is Integer or Double.\n";
                return nullptr;
            }

            if (objInteger) {
                return (var) objInteger;
            }

            return (var) std::make_shared<Integer>(objDouble->getValue());
        }
    }

    auto objString = dynamic_cast<const String*>(obj.get());
    if (! objString) {
        std::cerr << "Unexpected first parameter type. Expected String, Integer or Double.\n";
        return nullptr;
    }

    const std::string& str = objString->getValue();

    std::size_t base = 10;

    if (params.size() == 2) {
        ObjectPtr obj = params[1].getValue();
        auto objInteger = dynamic_cast<const Integer*>(obj.get());

        if (! objInteger) {
            std::cerr << "Unexpected second parameter type. Expected Integer.\n";
            return nullptr;
        }

        if (auto paramBase = objInteger->getValue() > 0) {
            base = paramBase; 
        } else {
            std::cerr << "Integer base must be positive.\n";
            return nullptr;
        }
    }

    try {
        int32_t result = std::stoi(str, nullptr, base); 
        return (var) std::make_shared<Integer>(result);
    } catch (const std::exception& e) {
        std::cerr 
            << "Invalid integer conversion from String: "
            << e.what() << std::endl;

        return nullptr;
    }

    return nullptr;
  }

  var asDouble(const std::vector<var>& params) {
    if (params.size() == 0) {
        return (var) std::make_shared<Double>(0.0);
    }

    if (params.size() > 1) {
      std::cerr << "float: Invalid number of arguments\n";
      return nullptr;
    }

    ObjectPtr obj = params[0].getValue();
    
    if (auto objDouble = dynamic_cast<const Double*>(obj.get())) {
        return (var) objDouble;
    }

    if (auto objInteger = dynamic_cast<const Integer*>(obj.get())) {
        return (var) std::make_shared<Double>(objInteger->getValue());
    }

    if (auto objString = dynamic_cast<const String*>(obj.get())) {
        const std::string& str = objString->getValue();

        try {
            double result = std::stod(str, nullptr);
            return (var) std::make_shared<Double>(result);
        } catch (const std::exception& e) {
            std::cerr 
                << "Invalid float conversion from String: "
                << e.what() << std::endl;

            return nullptr;
        }
    }

    std::cerr << "Unexpected type. Expected String, Integer or Double.\n";
    return nullptr;
  }
}
