#include "assets.hpp"
#include "audio.hpp"
#include "sdl_types.hpp"

#include <SDL3_image/SDL_image.h>
#include <cstdint>
#include <spdlog/spdlog.h>

static constexpr uint8_t BACKGROUND_DATA[] = {
#embed "assets/images/jungle.jpg"
};

static constexpr uint8_t FRUITS_DATA[] = {
#embed "assets/images/fruits.jpg"
};

static constexpr uint8_t BACKGROUND_MUSIC_DATA[] = {
#embed "assets/sounds/JamaicanSunrise.wav"
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

AudioAsset load_audio(uint8_t const *data, size_t size) {
  AudioAsset audio_asset;

  auto *iostream = SDL_IOFromConstMem(data, size);
  bool res = SDL_LoadWAV_IO(iostream, false, &audio_asset.spec,
                            &audio_asset.buffer, &audio_asset.buffer_length);
  if (!res) {
    spdlog::error("Failed to load audio file!\nCause: {}", SDL_GetError());
  }
  return audio_asset;
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

  auto background_music =
      load_audio(BACKGROUND_MUSIC_DATA, sizeof(BACKGROUND_MUSIC_DATA));
  world.set<AudioAssets>(AudioAssets{.background_music = background_music});
}
