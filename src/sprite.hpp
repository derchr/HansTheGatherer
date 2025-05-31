#pragma once

#include "assets.hpp"

#include <SDL3/SDL.h>
#include <cstdint>

struct Sprite
{
    Texture const* texture;
    uint16_t texture_atlas_index;
};
