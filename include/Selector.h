#pragma once

#include "Byte.h"
#include "Pattern.h"

class Selector
{
public:

	using indx_t = size_t;

	static constexpr indx_t npos = static_cast<indx_t>(-1);

	static indx_t first_of(const std::vector<Byte>& bytes, const Pattern& pattern);

	static indx_t first_of(const std::vector<Byte>& bytes, const Pattern::PatternByte& pb);

	static indx_t last_of(const std::vector<Byte>& bytes, const Pattern& pattern);

	static indx_t last_of(const std::vector<Byte>& bytes, const Pattern::PatternByte& pb);

	static std::vector<indx_t> all_of(const std::vector<Byte>& bytes, const Pattern& pattern);

	static std::vector<indx_t> all_of(const std::vector<Byte>& bytes, const Pattern::PatternByte& pb);
};
