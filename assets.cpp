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

SDL_Texture *load_texture(uint8_t const *data, size_t size,
                          SDL_Renderer *renderer) {
  auto *iostream = SDL_IOFromConstMem(data, size);
  SDL_Texture *texture = IMG_LoadTexture_IO(renderer, iostream, false);
  if (texture == nullptr) {
    spdlog::error("Failed to load SDL texture!\nCause: {}", SDL_GetError());
  }

  return texture;
}

void init_assets(flecs::world &world) {
  auto *renderer = world.get<SdlHandles>()->renderer;

  auto *background =
      load_texture(BACKGROUND_DATA, sizeof(BACKGROUND_DATA), renderer);
  TextureAtlasLayout background_layout = {
      .width = 866, .height = 510, .rows = 1, .columns = 1};

  auto *fruits = load_texture(FRUITS_DATA, sizeof(FRUITS_DATA), renderer);
  TextureAtlasLayout fruits_layout = {
      .width = 16, .height = 16, .rows = 6, .columns = 38};

  world.set<TextureAssets>(TextureAssets{
      .background = Texture{.sdl_texture = background,
                            .texture_atlas_layout = background_layout},
      .fruits = Texture{.sdl_texture = fruits,
                        .texture_atlas_layout = fruits_layout}});
}
