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

struct Velocity {
  int x;
  int y;
};

struct GameTicks {
  uint32_t ticks;
};

int main() {
  spdlog::info("Initialize SDL...");

  SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
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
  world.set<GameTicks>(GameTicks{0});
  world.set<ButtonInput>(ButtonInput{});
  world.set<SdlHandles>(SdlHandles{.window = window, .renderer = renderer});
  init_assets(world);
  auto *texture_assets = world.get<TextureAssets>();

  world.entity("Background")
      .set<Position>(Position{.x = 0, .y = 0})
      .set<Sprite>(
          Sprite{.texture = &texture_assets->background,
                 .texture_atlas_index = 0,
                 .custom_size = Vec2{.x = WINDOW_WIDTH, .y = WINDOW_HEIGHT}});

  struct Fruit {};
  struct Basket {};

  world.system<GameTicks>("IncrementTicks")
      .term_at(0)
      .singleton()
      .each([](GameTicks &game_ticks) { game_ticks.ticks += 1; });

  world.entity("Basket")
      .set<Position>(
          Position{.x = WINDOW_WIDTH / 2 - 32, .y = WINDOW_HEIGHT - 32})
      .set<Sprite>(Sprite{.texture = &texture_assets->fruits,
                          .texture_atlas_index = 212,
                          .custom_size = Vec2{.x = 64, .y = 16}})
      .add<Basket>();

  world.system<GameTicks const, TextureAssets const>("SpawnFruits")
      .term_at(0)
      .singleton()
      .term_at(1)
      .singleton()
      .run([](flecs::iter &it) {
        while (it.next()) {
          auto game_ticks = it.field<GameTicks const>(0);
          auto texture_assets = it.field<TextureAssets const>(1);

          if ((game_ticks->ticks % 100) == 0) {
            it.world()
                .entity()
                .set<Position>(Position{
                    .x = static_cast<int>(game_ticks->ticks % WINDOW_WIDTH),
                    .y = -16})
                .set<Velocity>(Velocity{.x = 0, .y = 1})
                .set<Sprite>(
                    Sprite{.texture = &texture_assets->fruits,
                           .texture_atlas_index =
                               static_cast<uint16_t>(game_ticks->ticks % 228),
                           .custom_size = Vec2{.x = 32, .y = 32}});
          }
        }
      });

  world.system<SdlHandles const, Position const, Sprite const>("RenderSprites")
      .term_at(0)
      .singleton()
      .each([](SdlHandles const &sdl_handles, Position const &pos,
               Sprite const &sprite) {
        TextureAtlasLayout layout = sprite.texture->texture_atlas_layout;
        uint8_t row = sprite.texture_atlas_index / layout.columns;
        uint8_t column = sprite.texture_atlas_index % layout.columns;
        SDL_FRect srcrect{static_cast<float>(column * layout.width),
                          static_cast<float>(row * layout.height),
                          static_cast<float>(layout.width),
                          static_cast<float>(layout.height)};

        Vec2 size = sprite.custom_size.value_or(
            {.x = layout.width, .y = layout.height});

        SDL_FRect dstrect{static_cast<float>(pos.x), static_cast<float>(pos.y),
                          static_cast<float>(size.x),
                          static_cast<float>(size.y)};

        SDL_RenderTexture(sdl_handles.renderer, sprite.texture->sdl_texture,
                          &srcrect, &dstrect);
      });

  world
      .system<ButtonInput const, Position, Sprite const, Basket>(
          "SetSpriteVelocity")
      .term_at(0)
      .singleton()
      .each([](ButtonInput const &input, Position &pos, Sprite const &sprite,
               Basket) {
        if (input.pressed.contains(SDLK_LEFT)) {
          pos.x -= 5;
        }
        if (input.pressed.contains(SDLK_RIGHT)) {
          pos.x += 5;
        }
      });

  world.system<Position, Velocity const>("MoveSprites")
      .each([](Position &pos, Velocity const &vel) {
        pos.x += vel.x;
        pos.y += vel.y;
      });

    auto *audio_assets = world.get<AudioAssets>();
    auto *stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio_assets->background_music.spec, NULL, NULL);
    if (!stream) {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_ResumeAudioStreamDevice(stream);

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
    if (SDL_GetAudioStreamQueued(stream) < (int)audio_assets->background_music.buffer_length) {
        SDL_PutAudioStreamData(stream, audio_assets->background_music.buffer, audio_assets->background_music.buffer_length);
    }

    // Render
    SDL_RenderClear(renderer);
    world.progress();
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
