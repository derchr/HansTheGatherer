#include "assets.hpp"
#include "definitions.hpp"
#include "input.hpp"
#include "level.hpp"
#include "physics.hpp"
#include "render.hpp"

#include <SDL3/SDL.h>
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
    world.import <RenderModule>();
    world.import <PhysicsModule>();
    world.import <LevelModule>();

    world.system<Game, TranslateSystem>("IncrementTicks")
        .term_at(0)
        .singleton()
        .term_at(1)
        .singleton()
        .each(
            [](Game& game, TranslateSystem& translate_system)
            {
                game.ticks += 1;

                if (game.ticks % 60 == 0)
                {
                    game.time = std::max(0, game.time - 1);
                }

                if (game.time == 0)
                    translate_system.translate_system.disable();
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

        world.progress();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
