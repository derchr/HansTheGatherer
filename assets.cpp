#include "assets.hpp"
#include "sdl_types.hpp"

#include <SDL3_image/SDL_image.h>
#include <cstdint>
#include <spdlog/spdlog.h>

static constexpr uint8_t BACKGROUND_DATA[] = {
#embed "jungle.jpg"
};

static constexpr uint8_t FRUITS_DATA[] = {
#embed "fruits.jpg"
};

void init_assets(flecs::world &world) {
  auto *renderer = world.get<SdlHandles>()->renderer;

  auto *background_iostream =
      SDL_IOFromConstMem(BACKGROUND_DATA, sizeof(BACKGROUND_DATA));
  SDL_Texture *background =
      IMG_LoadTexture_IO(renderer, background_iostream, false);
  if (background == nullptr) {
    spdlog::error("Failed to load SDL texture!\nCause: {}", SDL_GetError());
  }

  world.set<SpriteAssets>(SpriteAssets{.background = background});
}
