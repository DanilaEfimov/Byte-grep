#include "MaskReader.h"
#include "common.h"

#include <stdexcept>
#include <algorithm>
#include <iostream>

bool is_non_digit_char(char ch) {
    return ch == SEPARATOR || ch == ANY_BIT;
}

bool is_binary_syntax(char ch) {
    return is_non_digit_char(ch)
        || ch == CONCRETE_FALSE_BIT
        || ch == CONCRETE_TRUE_BIT;
}

bool is_hex_syntax(char ch) {
    return is_non_digit_char(ch)
        || std::isxdigit(static_cast<unsigned char>(ch));
}

bool is_hex_syntax(const std::string& pattern) {
    if (!pattern.starts_with(HEX_TRIGGER))
        return false;

    for (size_t i = std::strlen(HEX_TRIGGER); i < pattern.size(); i++) {
        if (!is_hex_syntax(pattern[i]))
            return false;
    }

    return true;
}

bool is_binary_syntax(const std::string& pattern) {
    if (!pattern.starts_with(BIN_TRIGGER))
        return false;

    for (size_t i = std::strlen(BIN_TRIGGER); i < pattern.size(); i++) {
        if (!is_binary_syntax(pattern[i]))
            return false;
    }

    return true;
}

bool valid_char(char ch, bool hex = false) {
    if (hex)
        return is_hex_syntax(ch);
    else
        return is_binary_syntax(ch);
}

bool MaskReader::validate(const std::string& pattern)
{
    return (is_hex_syntax(pattern) || is_binary_syntax(pattern)) && !pattern.empty();
}

Pattern MaskReader::readHex(const std::string& pattern)
{
    size_t size = pattern.size();
    size_t byte_count = (size + 1) / 2;

    Pattern mask;
    mask.pattern.resize(byte_count);
    auto& bytes = mask.pattern;

    using byte_t = Pattern::byte_t;

    for (size_t i = 0; i < size; i += 2) {
        size_t byte_idx = i / 2;
        bytes[byte_idx] = Pattern::PatternByte(0x00, 0xFF);
        auto& byte = bytes[byte_idx];

        char hi = pattern[i];
        char lo = (i + 1 < size) ? pattern[i + 1] : ANY_BIT;

        if (hi == ANY_BIT)
            byte.mask &= 0x0F;

        if (lo == ANY_BIT)
            byte.mask &= 0xF0;

        byte_t high = hexDigitValue(hi) << HIGH_BITS_SHIFT;
        byte_t low = hexDigitValue(lo)  << LOW_BITS_SHIFT;

        byte.value |= high | low;

    }
    
    return mask;
}

Pattern MaskReader::readBin(const std::string& pattern) {
    size_t size = pattern.size();
    size_t byte_count = (size + 7) / 8;

    Pattern mask;
    mask.pattern.resize(byte_count);
    auto& bytes = mask.pattern;
    
    using byte_t = Pattern::byte_t;

    for (size_t i = 0; i < byte_count; i++) {
        bytes[i] = Pattern::PatternByte(0x00, 0xFF);
        auto& byte = bytes[i];

        for (size_t j = 0; j < 8; j++) {
            int idx = i * 8 + j;
            char val = idx < size ? pattern[idx] : ANY_BIT;

            if (val == CONCRETE_TRUE_BIT)
                setBit<byte_t>(byte.value, j);
            else if (val == ANY_BIT)
                resetBit<byte_t>(byte.mask, j);
        }
    }

    return mask;
}

void MaskReader::normalize(std::string& pattern)
{
    std::erase(pattern, SEPARATOR);
    
    if (is_hex_syntax(pattern))
        if (pattern.starts_with(HEX_TRIGGER))
            pattern.erase(0, strlen(HEX_TRIGGER));

    if (is_binary_syntax(pattern))
        if (pattern.starts_with(BIN_TRIGGER))
            pattern.erase(0, strlen(BIN_TRIGGER));
}

Pattern MaskReader::readMask(const std::string& pattern)
{
    if (pattern.empty())
        throw std::invalid_argument("MaskReader::readMask<const string&>:: pattern can not be empty");

    if (!MaskReader::validate(pattern))
        throw std::invalid_argument("MaskReader::readMask<const string&>:: invalid pattern format");

    // normalize block
    std::string cp = pattern;
    bool isHex = is_hex_syntax(cp);
    MaskReader::normalize(cp);

    if (isHex)
        return MaskReader::readHex(cp);
    else
        return MaskReader::readBin(cp);
}
