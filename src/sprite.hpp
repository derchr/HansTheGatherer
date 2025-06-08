#pragma once

#include "assets.hpp"

#include <cstdint>

struct Sprite
{
    Texture const* texture;
    uint16_t texture_atlas_index;
};
