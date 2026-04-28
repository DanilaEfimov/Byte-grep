#include "Byte.h"
#include "Selector.h"

#include <string>

Byte::Byte(const position& pos, const value_t& val)
	: pos(pos), val(val)
{}

Byte::Byte(const position& pos)
	: pos(pos), val()
{}

Byte::value_t Byte::getValue() const
{
	return this->val;
}

Byte::position Byte::getPosition() const
{
	return this->pos;
}

bool Byte::operator==(const Byte& other) const
{
	return this->val == other.getValue();
}

Byte::position::operator std::string() const
{
	if (this->idx == Selector::npos)
		return "no position";

	return std::to_string(this->idx);
}
