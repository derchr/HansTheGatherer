#include "assets.hpp"
#include "sdl_types.hpp"
#include "input.hpp"

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
    SDL_RenderTexture(renderer, world.get<SpriteAssets>()->background, nullptr,
                      nullptr);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
