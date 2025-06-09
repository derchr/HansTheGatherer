#pragma once

#include <cstdint>
#include <set>

enum Key : uint8_t
{
    Left,
    Right
};

struct ButtonInput
{
    std::set<Key> pressed;
};
