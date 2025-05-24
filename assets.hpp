#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>

struct SpriteAssets {
  SDL_Texture *background;
  SDL_Texture *fruits;
};

void init_assets(flecs::world &world);
