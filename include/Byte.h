// Byte immutable interface

#pragma once

#include <string>

class Byte
{
public:
	using value_t = unsigned char;

	struct position {
		size_t idx;

		operator std::string() const;
	};

private:
	position pos;
	value_t val;

public:
	Byte() = default;
	Byte(const position& pos, const value_t& val);
	Byte(const position& pos);
	~Byte() = default;

	value_t getValue() const;
	position getPosition() const;

	bool operator==(const Byte& other) const;
};
