#include "assets.hpp"
#include "input.hpp"
#include "sdl_types.hpp"
#include "sprite.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <flecs.h>
#include <spdlog/spdlog.h>

static constexpr int WINDOW_WIDTH = 400;
static constexpr int WINDOW_HEIGHT = 280;

struct Position {
  int x;
  int y;
};

int main() {
  spdlog::info("Initialize SDL...");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    spdlog::critical("Failed to initialize SDL!\nCause: {}", SDL_GetError());
    std::terminate();
  }

  auto *window =
      SDL_CreateWindow("HansTheGatherer", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (window == nullptr) {
    spdlog::critical("Failed to create SDL window!\nCause: {}", SDL_GetError());
  }

  auto *renderer = SDL_CreateRenderer(window, nullptr);
  if (renderer == nullptr) {
    spdlog::critical("Failed to create SDL renderer!\nCause: {}",
                     SDL_GetError());
  }

  flecs::world world;
  world.set<ButtonInput>(ButtonInput{});
  world.set<SdlHandles>(SdlHandles{.window = window, .renderer = renderer});
  init_assets(world);
  auto *texture_assets = world.get<TextureAssets>();

  world.entity("Sprite1")
      .set<Position>(Position{.x = 0, .y = 0})
      .set<Sprite>(
          Sprite{.texture = &texture_assets->fruits, .texture_atlas_index = 0});

  world.entity("Sprite2")
      .set<Position>(Position{.x = 32, .y = 32})
      .set<Sprite>(
          Sprite{.texture = &texture_assets->fruits, .texture_atlas_index = 5});

  flecs::system sys =
      world
          .system<SdlHandles const, Position const, Sprite const>(
              "RenderSprites")
          .term_at(0)
          .singleton()
          .each([](flecs::entity e, SdlHandles const &sdl_handles,
                   Position const &pos, Sprite const &sprite) {
            TextureAtlasLayout layout = sprite.texture->texture_atlas_layout;
            uint8_t row = sprite.texture_atlas_index / layout.columns;
            uint8_t column = sprite.texture_atlas_index % layout.columns;
            SDL_FRect srcrect{static_cast<float>(column * layout.width),
                              static_cast<float>(row * layout.height),
                              static_cast<float>(layout.width),
                              static_cast<float>(layout.height)};

            SDL_FRect dstrect{static_cast<float>(pos.x),
                              static_cast<float>(pos.y),
                              static_cast<float>(layout.width),
                              static_cast<float>(layout.height)};

            SDL_RenderTexture(sdl_handles.renderer, sprite.texture->sdl_texture,
                              &srcrect, &dstrect);
          });

  bool exit_gameloop = false;
  while (!exit_gameloop) {
    auto *input = world.get_mut<ButtonInput>();

    // Clear just pressed/released
    input->just_pressed.clear();
    input->just_released.clear();

    // Input
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT:
        exit_gameloop = true;
        break;
      case SDL_EVENT_KEY_DOWN:
        if (event.key.key == SDLK_ESCAPE) {
          exit_gameloop = true;
        }
        if (input->pressed.insert(event.key.key).second) {
          input->just_pressed.insert(event.key.key);
        }
        break;
      case SDL_EVENT_KEY_UP:
        if (input->pressed.erase(event.key.key) != 0) {
          input->just_released.insert(event.key.key);
        }
        break;
      }
    }

    // Game Logic
    if (input->just_pressed.contains(SDLK_X))
      spdlog::info("X pressed!");

    if (input->just_released.contains(SDLK_X))
      spdlog::info("X released!");

    // Render
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture_assets->background.sdl_texture, nullptr,
                      nullptr);
    world.progress();
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
