#pragma once

#include <set>

struct ButtonInput
{
    std::set<int> pressed;
    std::set<int> just_pressed;
    std::set<int> just_released;
};
