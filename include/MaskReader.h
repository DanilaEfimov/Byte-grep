#pragma once

#include "Pattern.h"
#include "common.h"

#include <string>

class MaskReader
{
public:

	static Pattern readHex(const std::string& pattern);

	static Pattern readBin(const std::string& pattern);

	static void normalize(std::string& pattern);

	static bool validate(const std::string& pattern);

	static Pattern readMask(const std::string& pattern);
};

