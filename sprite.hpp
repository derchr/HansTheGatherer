#pragma once

#include "assets.hpp"
#include "util.hpp"

#include <SDL3/SDL.h>
#include <cstdint>
#include <optional>

struct Sprite {
  Texture const *texture;
  uint16_t texture_atlas_index;
  std::optional<Vec2> custom_size;
};
