#include "Translator.h"

#include <stdexcept>
#include <fstream>

void Translator::translate(std::fstream& in)
{
	if (!in.is_open())
		throw std::logic_error("Translator::translate<fstream&>:: file have to be opened to translate");

	in.seekg(0, std::ios::end);
	auto size = in.tellg();
	in.seekg(0, std::ios::beg);

	this->bytes.resize(size);

	std::vector<char> buffer(size);
	in.read(buffer.data(), size);

	for (int i = 0; i < size; i++) {
		this->bytes[i] = Byte(
			Byte::position(i), 
			Byte::value_t(buffer[i])
		);
	}
}

Translator::Translator(std::fstream& in)
	: bytes()
{
	this->translate(in);
}

Byte Translator::read(const Byte::position& pos) const
{
	if (pos.idx >= this->bytes.size())
		throw std::out_of_range("Translator::read<const Byte::position&>:: index out of range");

	return this->bytes.at(pos.idx);
}

const std::vector<Byte>& Translator::get() const
{
	return this->bytes;
}
