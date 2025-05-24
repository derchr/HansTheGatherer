#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>
#include <flecs.h>
#include <spdlog/spdlog.h>

const uint8_t image_data[] = {
#embed "main.cpp"
};

auto main() -> int {
  spdlog::info("Initialize SDL...");
  bool sdl_success = SDL_Init(SDL_INIT_VIDEO);
  if (!sdl_success) {
    spdlog::critical("Failed to initialize SDL!\nCause: {}", SDL_GetError());
    std::terminate();
  }

  auto *sdl_window = SDL_CreateWindow("HansTheGatherer", 400, 280, 0);
  if (sdl_window == nullptr) {
    spdlog::critical("Failed to create SDL window!\nCause: {}", SDL_GetError());
  }

  auto *sdl_renderer = SDL_CreateRenderer(sdl_window, nullptr);
  if (sdl_renderer == nullptr) {
    spdlog::critical("Failed to create SDL renderer!\nCause: {}",
                     SDL_GetError());
  }

  flecs::world world;

  bool exit_gameloop = false;
  while (!exit_gameloop) {
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
        break;
      }
    }
  }

  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();

  return 0;
}
