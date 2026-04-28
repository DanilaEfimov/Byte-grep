#include "Pattern.h"
#include "common.h"

Pattern::Pattern()
	: pattern()
{}

bool Pattern::isConcrete(int pos) const
{
	int byte = pos / 8;
	int bit = pos % 8;

	const PatternByte& pb = this->pattern[byte];

	return getBit<uint8_t>(pb.mask, bit);
}

bool Pattern::isTrue(int pos) const
{
	if (!this->isConcrete(pos))
		return false;

	int byte = pos / 8;
	int bit = pos % 8;

	const PatternByte& pb = this->pattern[byte];

	return True == getBit<byte_t>(pb.value, bit);
}

bool Pattern::isFalse(int pos) const
{
	if (!this->isConcrete(pos))
		return false;

	int byte = pos / 8;
	int bit = pos % 8;

	const PatternByte& pb = this->pattern[byte];

	return False == getBit<uint8_t>(pb.value, bit);
}

bool Pattern::matchs(const Byte& byte, const PatternByte& pb) noexcept
{
	auto excepted = static_cast<uint8_t>(pb.value & pb.mask);
	auto actual = static_cast<uint8_t>(byte.getValue()) & pb.mask;

	return actual == excepted;
}

bool Pattern::matchs(const std::vector<Byte>& value) const
{
	size_t pattern_size = this->pattern.size();
	if (value.size() < pattern_size)
		return false;

	for (size_t i = 0; i < pattern_size; i++) {
		if (!Pattern::matchs(value[i], this->pattern[i]))
			return false;
	}

	return true;
}

bool Pattern::matchs(const std::vector<Byte>& value, size_t offset) const
{
	size_t pattern_size = this->pattern.size();
	if (value.size() < pattern_size + offset)
		return false;

	for (size_t i = 0; i < pattern_size; i++) {
		if (!Pattern::matchs(value[i + offset], this->pattern[i]))
			return false;
	}

	return true;
}

Pattern::operator std::string() const noexcept
{
	std::string res;

	int size = this->pattern.size() * 8;
	for (int i = 0; i < size; i++) {
		if (!this->isConcrete(i)) {
			res += ANY_BIT;
		}
		else {
			if (this->isTrue(i)) {
				res += CONCRETE_TRUE_BIT;
			}
			else {
				res += CONCRETE_FALSE_BIT;
			}
		}

		if (i % 8 == 7 && i < size - 1)
			res += SEPARATOR;
	}

	return res;
}
