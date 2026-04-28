#pragma once

#include "SearchCommand.h"

#include <concepts>
#include <filesystem>

#define CONCRETE_TRUE_BIT '1'
#define CONCRETE_FALSE_BIT '0'
#define ANY_BIT 'X'
#define SEPARATOR '_'
#define HEX_TRIGGER "0x"
#define BIN_TRIGGER "0b"
#define HIGH_BITS_SHIFT 4
#define LOW_BITS_SHIFT 0
#define NON_HEX_DIGIT_FLAG 0x00

namespace fs = std::filesystem;

template<std::integral T>
inline void setBit(T& byte, int idx) {
	byte |= 1 << (idx % (sizeof(T) * 8));
};

template<std::integral T>
inline void setBits(T& byte, int from, int to) {
	for (int i = from; i < to; i++) {
		byte |= 1 << (i % (sizeof(T) * 8));
	}
};

template<std::integral T>
inline void resetBit(T& byte, int idx) {
	byte &= ~(1 << (idx % (sizeof(T) * 8)));
};

template<std::integral T>
inline void resetBits(T& byte, int from, int to) {
	for (int i = from; i < to; i++) {
		byte &= ~(1 << (i % (sizeof(T) * 8)));
	}
};

template<std::integral T>
inline bool getBit(const T& byte, int idx) {
	return byte & (1 << idx % (sizeof(T) * 8));
}

inline uint8_t hexDigitValue(char ch) {
	if (ch >= '0' && ch <= '9') return ch - '0';
	if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
	if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;

	return NON_HEX_DIGIT_FLAG;
}

inline SearchCommand::Mode parseMode(const std::string& mode) {
	if (mode == "first-of") return SearchCommand::Mode::FirstOf;
	else if (mode == "last-of") return SearchCommand::Mode::LastOf;
	else if (mode == "all-of") return SearchCommand::Mode::AllOf;
	else throw std::invalid_argument("parseMode<const string&>:: undefined search mode");
}

inline bool isDirectoryInput(const fs::path& path) {
	return fs::exists(path) && fs::is_directory(path);
}
