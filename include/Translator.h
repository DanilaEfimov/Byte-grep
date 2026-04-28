// file text -> bytes translator

#pragma once

#include "Byte.h"

#include <fstream>
#include <vector>

class Translator
{
	std::vector<Byte> bytes;

	void translate(std::fstream& in);

public:
	Translator(std::fstream& in);
	~Translator() = default;

	Byte read(const Byte::position& pos) const;
	const std::vector<Byte>& get() const;
};
