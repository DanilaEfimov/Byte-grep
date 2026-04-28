#include "Selector.h"

Selector::indx_t Selector::first_of(const std::vector<Byte>& bytes, const Pattern& pattern)
{
    indx_t pos = Selector::npos;

    size_t value_size = bytes.size();
    size_t pattern_size = pattern.pattern.size();

    if (value_size < pattern_size)
        return Selector::npos;

    for (size_t i = 0; i < value_size; i++) {
        auto slice = std::vector<Byte>(bytes.begin() + i, bytes.end());
        if (pattern.matchs(slice))
            return i;
    }

    return pos;
}

Selector::indx_t Selector::first_of(const std::vector<Byte>& bytes, const Pattern::PatternByte& pb)
{
    indx_t pos = Selector::npos;

    for (size_t i = 0; i < bytes.size(); i++) {
        if (Pattern::matchs(bytes[i], pb))
            return i;
    }

    return pos;
}

Selector::indx_t Selector::last_of(const std::vector<Byte>& bytes, const Pattern& pattern)
{
    indx_t pos = Selector::npos;

    size_t value_size = bytes.size();
    size_t pattern_size = pattern.pattern.size();

    if (value_size < pattern_size)
        return Selector::npos;

    for (size_t i = 0; i < value_size; i++) {
        auto slice = std::vector<Byte>(bytes.begin() + i, bytes.end());
        if (pattern.matchs(slice))
            pos = i;
    }

    return pos;
}

Selector::indx_t Selector::last_of(const std::vector<Byte>& bytes, const Pattern::PatternByte& pb)
{
    indx_t pos = Selector::npos;

    for (size_t i = 0; i < bytes.size(); i++) {
        if (Pattern::matchs(bytes[i], pb))
            pos = i;
    }

    return pos;
}

std::vector<Selector::indx_t> Selector::all_of(const std::vector<Byte>& bytes, const Pattern& pattern)
{
    std::vector<indx_t> pos;

    size_t value_size = bytes.size();
    size_t pattern_size = pattern.pattern.size();

    if (value_size < pattern_size)
        return {};

    for (size_t i = 0; i < value_size; i++) {
        auto slice = std::vector<Byte>(bytes.begin() + i, bytes.end());
        if (pattern.matchs(slice))
            pos.push_back(i);
    }

    return pos;
}

std::vector<Selector::indx_t> Selector::all_of(const std::vector<Byte>& bytes, const Pattern::PatternByte& pb)
{
    std::vector<indx_t> pos;

    for (size_t i = 0; i < bytes.size(); i++) {
        if (Pattern::matchs(bytes[i], pb))
            pos.push_back(i);
    }

    return pos;
}
