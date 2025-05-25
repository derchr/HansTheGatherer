#pragma once

#include <SDL3/SDL.h>
#include <cstdint>

struct AudioAsset {
  SDL_AudioSpec spec;
  uint8_t *buffer;
  uint32_t buffer_length;
};
