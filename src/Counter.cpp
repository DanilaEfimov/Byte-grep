#include "Counter.h"

size_t Counter::count(const std::vector<Byte>& bytes, const Pattern& pattern)
{
    size_t res = 0;
    size_t bytes_size = bytes.size();
    size_t pattern_size = pattern.pattern.size();

    for (size_t i = 0; i + pattern_size <= bytes_size; i++) {
        if (pattern.matchs(bytes, i))
            res++;
    }

    return res;
}

size_t Counter::count(const std::vector<Byte>& bytes, const Pattern::PatternByte& pb)
{
    size_t res = 0;

    for (const auto& byte : bytes) {
        if(Pattern::matchs(byte, pb))
            res++;
    }

    return res;
}
