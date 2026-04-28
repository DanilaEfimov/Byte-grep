// static grep-like counter

#pragma once

#include "Byte.h"
#include "Pattern.h"

#include <vector>

class Counter
{
public:

	using indx_t = size_t;

	static size_t count(const std::vector<Byte>& bytes, const Pattern& pattern);

	static size_t count(const std::vector<Byte>& bytes, const Pattern::PatternByte& pb);
};
