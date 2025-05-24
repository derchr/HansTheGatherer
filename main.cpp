#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <flecs.h>
#include <spdlog/spdlog.h>

const uint8_t background_data[] = {
#embed "jungle.jpg"
};

static constexpr int WINDOW_WIDTH = 400;
static constexpr int WINDOW_HEIGHT = 280;

struct SpriteAssets {
  SDL_Texture *background;
};

struct SdlHandles {
  SDL_Window *window;
  SDL_Renderer *renderer;
};

auto init_assets(flecs::world &world) -> void {
  auto *renderer = world.get<SdlHandles>()->renderer;

  auto *background_iostream =
      SDL_IOFromConstMem(background_data, sizeof(background_data));
  SDL_Texture *background =
      IMG_LoadTexture_IO(renderer, background_iostream, false);
  if (background == nullptr) {
    spdlog::error("Failed to load SDL texture!\nCause: {}", SDL_GetError());
  }

  world.set<SpriteAssets>(SpriteAssets{.background = background});
}

auto main() -> int {
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
  world.set<SdlHandles>(SdlHandles{.window = window, .renderer = renderer});
  init_assets(world);

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

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, world.get<SpriteAssets>()->background, nullptr, nullptr);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
