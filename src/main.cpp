#include "assets.hpp"
#include "definitions.hpp"
#include "input.hpp"
#include "level.hpp"
#include "physics.hpp"
#include "render.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <entt/entt.hpp>

void increment_ticks(entt::registry& registry)
{
    auto& game = registry.ctx().get<Game>();
    // auto& translate_system = registry.ctx().get<TranslateSystem>();

    game.ticks += 1;

    if (game.ticks % 60 == 0)
    {
        game.time = std::max(0, game.time - 1);
    }

    // if (game.time == 0)
    //     translate_system.translate_system.disable();
}

int main()
{
    // spdlog::info("Initialize SDL...");

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        // spdlog::critical("Failed to initialize SDL!\nCause: {}", SDL_GetError());
        std::terminate();
    }

    TTF_Init();

    auto* window = SDL_CreateWindow("HansTheGatherer", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == nullptr)
    {
        // spdlog::critical("Failed to create SDL window!\nCause: {}", SDL_GetError());
    }

    auto* renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr)
    {
        // spdlog::critical("Failed to create SDL renderer!\nCause: {}", SDL_GetError());
    }

    auto* text_engine = TTF_CreateRendererTextEngine(renderer);

    entt::registry registry;

    registry.ctx().emplace<Game>(Game{.ticks = 0, .time = 60, .score = 0, .random_engine = {}});
    registry.ctx().emplace<ButtonInput>(ButtonInput{});
    auto sdl_handles = registry.ctx().emplace<SdlHandles>(
        SdlHandles{.window = window, .renderer = renderer, .text_engine = text_engine});

    AssetModule asset_module(registry);
    AudioModule audio_module(registry);
    RenderModule render_module(registry);
    PhysicsModule physics_module(registry);
    LevelModule level_module(registry);

    bool exit_gameloop = false;
    while (!exit_gameloop)
    {
        auto* input = &registry.ctx().get<ButtonInput>();

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

        AudioModule::FeedAudioStreams(registry);

        if (registry.ctx().get<Game>().time != 0)
        {
            increment_ticks(registry);
            LevelModule::MoveBasket(registry);
            LevelModule::SpawnFruits(registry);
            LevelModule::CollectFruit(registry);
            LevelModule::CollectSpider(registry);
            // LevelModule::DespawnItems(registry);
        }

        PhysicsModule::TranslatePhysicsObject(registry);
        PhysicsModule::PropagatePosition(registry);
        PhysicsModule::RemoveCollisionMarker(registry);
        PhysicsModule::CollisionCheck(registry);

        SDL_RenderClear(sdl_handles.renderer);
        RenderModule::RenderSprites(registry);
        RenderModule::RenderScore(registry);
        SDL_RenderPresent(sdl_handles.renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
