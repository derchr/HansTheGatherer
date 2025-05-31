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
static constexpr int WINDOW_HEIGHT = 240;

struct WorldPosition
{
    int x;
    int y;
};

struct Position
{
    int x;
    int y;
};

struct Velocity
{
    int x;
    int y;
};

struct Size
{
    int w;
    int h;
};

struct Game
{
    uint32_t ticks;
};

int main()
{
    spdlog::info("Initialize SDL...");

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        spdlog::critical("Failed to initialize SDL!\nCause: {}", SDL_GetError());
        std::terminate();
    }

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

    flecs::world world;
    world.set<Game>(Game{.ticks = 0});
    world.set<ButtonInput>(ButtonInput{});
    world.set<SdlHandles>(SdlHandles{.window = window, .renderer = renderer});
    init_assets(world);
    auto* texture_assets = world.get<TextureAssets>();

    world.entity("Background")
        .set<Position>(Position{.x = 0, .y = 0})
        .set<Sprite>(Sprite{.texture = &texture_assets->background, .texture_atlas_index = 0})
        .set<Size>(Size{.w = WINDOW_WIDTH, .h = WINDOW_HEIGHT});

    struct Fruit
    {
    };
    struct Basket
    {
    };
    struct CollisionBox
    {
    };

    world.system<Game>("IncrementTicks")
        .term_at(0)
        .singleton()
        .each([](Game& game_ticks) { game_ticks.ticks += 1; });

    auto basket =
        world.entity("Basket")
            .add<WorldPosition>()
            .set<Position>(Position{.x = WINDOW_WIDTH / 2 - 32, .y = WINDOW_HEIGHT - 32})
            .set<Sprite>(Sprite{.texture = &texture_assets->basket, .texture_atlas_index = 0})
            .set<Size>(Size{.w = 64, .h = 32})
            .add<Basket>();

    world.entity("CollisionBox")
        .child_of(basket)
        .add<WorldPosition>()
        .set<Position>(Position{.x = 0, .y = 0})
        .set<Size>(Size{.w = 64, .h = 32})
        .add<CollisionBox>();

    world.system<SdlHandles const, WorldPosition const, Size const, CollisionBox>("DrawBoxes")
        .term_at(0)
        .singleton()
        .each(
            [](SdlHandles const& sdl_handles,
               WorldPosition const& pos,
               Size const& size,
               CollisionBox)
            {
                SDL_FRect rect{static_cast<float>(pos.x),
                               static_cast<float>(pos.y),
                               static_cast<float>(size.w),
                               static_cast<float>(size.h)};
                SDL_SetRenderDrawColor(sdl_handles.renderer, 0, 0, 255, 255);
                SDL_RenderRect(sdl_handles.renderer, &rect);
            });

    world.system<Game const, TextureAssets const>("SpawnFruits")
        .term_at(0)
        .singleton()
        .term_at(1)
        .singleton()
        .each(
            [](flecs::iter& it, size_t index, Game const& game, TextureAssets const& texture_assets)
            {
                if ((game.ticks % 100) == 0)
                {
                    auto fruit =
                        it.world()
                            .entity()
                            .add<WorldPosition>()
                            .set<Position>(Position{
                                .x = static_cast<int>(game.ticks % WINDOW_WIDTH), .y = -16})
                            .set<Velocity>(Velocity{.x = 0, .y = 1})
                            .set<Sprite>(Sprite{.texture = &texture_assets.fruits,
                                                .texture_atlas_index =
                                                    static_cast<uint16_t>(game.ticks % 228)})
                            .set<Size>(Size{.w = 32, .h = 32});

                    it.world()
                        .entity("CollisionBox")
                        .child_of(fruit)
                        .add<WorldPosition>()
                        .set<Position>(Position{.x = 0, .y = 0})
                        .set<Size>(Size{.w = 32, .h = 32})
                        .add<CollisionBox>();
                }
            });

    world.system<WorldPosition, Position const>("PropagatePosition")
        .kind(flecs::PostUpdate)
        .each(
            [](flecs::entity e, WorldPosition& world_pos, Position const& local_pos)
            {
                if (e.parent() == flecs::entity::null())
                {
                    world_pos.x = local_pos.x;
                    world_pos.y = local_pos.y;
                }
                else
                {
                    auto parent_world_pos = e.parent().get<WorldPosition>();
                    world_pos.x = parent_world_pos->x + local_pos.x;
                    world_pos.y = parent_world_pos->y + local_pos.y;
                }
            });

    world.system<WorldPosition const, Size const, CollisionBox>("CollisionCheck")
        .each(
            [](flecs::iter& it,
               size_t index,
               WorldPosition const& world_pos,
               Size const& size,
               CollisionBox)
            {
                auto basket_box = it.world().lookup("Basket::CollisionBox");
                if (it.entity(index) == basket_box)
                    return;

                auto basket_box_pos = basket_box.get<WorldPosition>();
                auto basket_box_size = basket_box.get<Size>();

                if (basket_box_pos->x + basket_box_size->w >= world_pos.x &&
                    basket_box_pos->x <= world_pos.x + size.w &&
                    basket_box_pos->y + basket_box_size->h >= world_pos.y &&
                    basket_box_pos->y <= world_pos.y + size.h)
                {
                    spdlog::info("collision");
                }
            });
    world.system<SdlHandles const, Position const, Size const, Sprite const>("RenderSprites")
        .kind(flecs::PreUpdate)
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

    world.system<ButtonInput const, Position, Size const, Sprite const, Basket>("MoveBasket")
        .term_at(0)
        .singleton()
        .each(
            [](ButtonInput const& input,
               Position& pos,
               Size const& size,
               Sprite const& sprite,
               Basket)
            {
                if (input.pressed.contains(SDLK_LEFT))
                {
                    pos.x -= 5;
                }
                if (input.pressed.contains(SDLK_RIGHT))
                {
                    pos.x += 5;
                }

                pos.x = pos.x < 0 ? 0 : pos.x;
                pos.x = pos.x > WINDOW_WIDTH - size.w ? WINDOW_WIDTH - size.w : pos.x;
            });

    world.system<Position, Velocity const>("MoveSprites")
        .each(
            [](Position& pos, Velocity const& vel)
            {
                pos.x += vel.x;
                pos.y += vel.y;
            });

    auto* audio_assets = world.get<AudioAssets>();
    auto* stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio_assets->background_music.spec, NULL, NULL);
    if (!stream)
    {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_ResumeAudioStreamDevice(stream);

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

        // Game Logic
        if (SDL_GetAudioStreamQueued(stream) < (int)audio_assets->background_music.buffer_length)
        {
            SDL_PutAudioStreamData(stream,
                                   audio_assets->background_music.buffer,
                                   audio_assets->background_music.buffer_length);
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
