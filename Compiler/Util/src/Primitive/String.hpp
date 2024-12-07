// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <string>

#include "./Primitive.hpp"            // NOLINT

// String class
class String : public Primitive<String, std::string> {
	private:
		using Primitive::value;

		void init();

 	public:
		explicit String(std::string value);
		operator ObjectPtr();

		ObjectPtr add(const Object& other) const override;

		ObjectPtr subscript(const Object& other) const override;

		String operator+(const String& other);

		String operator+(const std::string& other);

		String operator+(const char* other);

		String operator+(const char other);

		explicit operator bool() const override;

		// Override iteration methods
		class StringIterator : public Object::ObjectIterator {
			private:
				const std::string& str;
				size_t currentIndex;

			public:
				explicit StringIterator(const std::string& str);

				bool hasNext() const override;

				ObjectPtr next() override;

				ObjectIt clone() const override;
		};

		ObjectIt getIterator() const override;

		Method::result_type slice(const std::vector<ObjectPtr>& params);
};
