#pragma once
#include <cstdint>

constexpr uint64_t fnv1a(const char* s, uint64_t h = 1469598103934665603ULL) {
    return *s ? fnv1a(s + 1, (h ^ (uint8_t)*s) * 1099511628211ULL) : h;
}
#define H(x) fnv1a(x)
