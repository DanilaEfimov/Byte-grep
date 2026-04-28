#pragma once

#include "Byte.h"

#include <vector>
#include <string>

class Pattern
{
public:
	using byte_t = uint8_t;
	
	enum Flags {
		False = 0,
		True = 1,
		Any
	};

	struct PatternByte {
		byte_t value;
		byte_t mask;

		PatternByte(byte_t value, byte_t mask)
			: value(value), mask(mask)
		{};

		PatternByte() = default;
	};

	std::vector<PatternByte> pattern;

	Pattern();
	~Pattern() = default;

	bool isConcrete(int pos) const;
	bool isTrue(int pos) const;
	bool isFalse(int pos) const;

	static bool matchs(const Byte& byte, const PatternByte& pb) noexcept;
	bool matchs(const std::vector<Byte>& value) const;
	bool matchs(const std::vector<Byte>& value, size_t offset) const;

	operator std::string() const noexcept;
};
