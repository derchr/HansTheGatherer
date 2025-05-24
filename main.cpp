#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <flecs.h>
#include <spdlog/spdlog.h>

const uint8_t image_data[] = {
#embed "jungle.jpg"
};

static constexpr int WINDOW_WIDTH = 400;
static constexpr int WINDOW_HEIGHT = 280;

auto main() -> int {
  spdlog::info("Initialize SDL...");
  bool sdl_success = SDL_Init(SDL_INIT_VIDEO);
  if (!sdl_success) {
    spdlog::critical("Failed to initialize SDL!\nCause: {}", SDL_GetError());
    std::terminate();
  }

  auto *sdl_window =
      SDL_CreateWindow("HansTheGatherer", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (sdl_window == nullptr) {
    spdlog::critical("Failed to create SDL window!\nCause: {}", SDL_GetError());
  }

  auto *sdl_renderer = SDL_CreateRenderer(sdl_window, nullptr);
  if (sdl_renderer == nullptr) {
    spdlog::critical("Failed to create SDL renderer!\nCause: {}",
                     SDL_GetError());
  }

  flecs::world world;

  auto *image_iostream = SDL_IOFromConstMem(image_data, sizeof(image_data));
  SDL_Texture *img = IMG_LoadTexture_IO(sdl_renderer, image_iostream, false);
  if (img == nullptr) {
    spdlog::error("Failed to load SDL texture!\nCause: {}", SDL_GetError());
  }

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

    SDL_RenderClear(sdl_renderer);
    SDL_RenderTexture(sdl_renderer, img, nullptr, nullptr);
    SDL_RenderPresent(sdl_renderer);
  }

  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();

  return 0;
}
