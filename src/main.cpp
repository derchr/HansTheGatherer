#include "assets.hpp"
#include "definitions.hpp"
#include "input.hpp"
#include "level.hpp"
#include "physics.hpp"
#include "sprite.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <flecs.h>
#include <spdlog/spdlog.h>

int main()
{
    spdlog::info("Initialize SDL...");

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        spdlog::critical("Failed to initialize SDL!\nCause: {}", SDL_GetError());
        std::terminate();
    }

    TTF_Init();

    auto* window = SDL_CreateWindow("HansTheGatherer", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == nullptr)
    {
        spdlog::critical("Failed to create SDL window!\nCause: {}", SDL_GetError());
    }

    auto* renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr)
    {
        spdlog::critical("Failed to create SDL renderer!\nCause: {}", SDL_GetError());
    }

    auto* text_engine = TTF_CreateRendererTextEngine(renderer);

    flecs::world world;

    world.set<Game>(Game{.ticks = 0, .time = 60, .score = 0, .random_engine = {}});
    world.set<ButtonInput>(ButtonInput{});
    world.set<SdlHandles>(
        SdlHandles{.window = window, .renderer = renderer, .text_engine = text_engine});

    world.import <AssetModule>();
    world.import <AudioModule>();
    world.import <PhysicsModule>();
    world.import <LevelModule>();

    world.system<Game>("IncrementTicks")
        .term_at(0)
        .singleton()
        .each(
            [](Game& game)
            {
                game.ticks += 1;

                if (game.ticks % 60 == 0)
                {
                    game.time--;
                }
            });

    world.system<SdlHandles const, Position const, Size const, Sprite const>("RenderSprites")
        .kind(flecs::OnStore)
        .term_at(0)
        .singleton()
        .each(
            [](SdlHandles const& sdl_handles,
               Position const& pos,
               Size const& size,
               Sprite const& sprite)
            {
                TextureAtlasLayout layout = sprite.texture->texture_atlas_layout;
                uint8_t row = sprite.texture_atlas_index / layout.columns;
                uint8_t column = sprite.texture_atlas_index % layout.columns;
                SDL_FRect srcrect{static_cast<float>(column * layout.width),
                                  static_cast<float>(row * layout.height),
                                  static_cast<float>(layout.width),
                                  static_cast<float>(layout.height)};

                SDL_FRect dstrect{static_cast<float>(pos.x),
                                  static_cast<float>(pos.y),
                                  static_cast<float>(size.w),
                                  static_cast<float>(size.h)};

                SDL_RenderTexture(
                    sdl_handles.renderer, sprite.texture->sdl_texture, &srcrect, &dstrect);
            });

    world.system<SdlHandles const, Game const, FontAssets const>("RenderScore")
        .kind(flecs::OnStore)
        .term_at(0)
        .singleton()
        .term_at(1)
        .singleton()
        .term_at(2)
        .singleton()
        .each(
            [](SdlHandles const& sdl_handles, Game const& game, FontAssets const& font_assets)
            {
                auto score_string = std::format("Score: {}\nTime: {}", game.score, game.time);
                auto text = TTF_CreateText(sdl_handles.text_engine,
                                           font_assets.default_font.font,
                                           score_string.c_str(),
                                           score_string.length());
                TTF_DrawRendererText(text, 0.0, 0.0);

                TTF_DestroyText(text);
            });

    bool exit_gameloop = false;
    while (!exit_gameloop)
    {
        auto* input = world.get_mut<ButtonInput>();

        // Clear just pressed/released
        input->just_pressed.clear();
        input->just_released.clear();

        // Input
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                exit_gameloop = true;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE)
                {
                    exit_gameloop = true;
                }
                if (input->pressed.insert(event.key.key).second)
                {
                    input->just_pressed.insert(event.key.key);
                }
                break;
            case SDL_EVENT_KEY_UP:
                if (input->pressed.erase(event.key.key) != 0)
                {
                    input->just_released.insert(event.key.key);
                }
                break;
            }
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
