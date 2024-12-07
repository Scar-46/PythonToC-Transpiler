// Copyright (c) 2024 Syntax Errors.
#pragma once

#include "./Numeric.hpp"

// Integer class
// TODO(Dwayne): Implement like division.
class Integer : public BaseNumeric<Integer, int32_t> {
	public:
		explicit Integer(int32_t value);

		operator ObjectPtr() override;

		ObjectPtr addHelper(const Object& other) const override;

		ObjectPtr subtractHelper(const Object& other) const override;

		ObjectPtr multiplyHelper(const Object& other) const override;

		ObjectPtr divideHelper(const Object& other) const override;

		bool equalsHelper(const Object& other) const override;

		bool lessHelper(const Object& other) const override;

		bool greaterHelper(const Object& other) const override;
};
