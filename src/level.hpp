#pragma once

#include "definitions.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "sprite.hpp"

#include <flecs.h>
#include <spdlog/spdlog.h>

struct Fruit
{
};

struct Spider
{
};

struct Item
{
};

struct Basket
{
};

struct LevelModule
{
    LevelModule(flecs::world& world)
    {
        auto const* texture_assets = world.get<TextureAssets>();

        world.component<Item>();
        world.component<Fruit>().is_a<Item>();
        world.component<Spider>().is_a<Item>();

        world.entity("Background")
            .set<Position>(Position{.x = 0, .y = 0})
            .set<Sprite>(Sprite{.texture = &texture_assets->background, .texture_atlas_index = 0})
            .set<Size>(Size{.w = WINDOW_WIDTH, .h = WINDOW_HEIGHT});

        auto basket =
            world.entity("Basket")
                .add<WorldPosition>()
                .set<Position>(Position{.x = WINDOW_WIDTH / 2 - 32, .y = WINDOW_HEIGHT - 32})
                .set<Sprite>(Sprite{.texture = &texture_assets->basket, .texture_atlas_index = 0})
                .set<Size>(Size{.w = 64, .h = 32})
                .add<Basket>();

        world.entity()
            .child_of(basket)
            .add<WorldPosition>()
            .set<Position>(Position{.x = 0, .y = 16})
            .set<Size>(Size{.w = 64, .h = 16})
            .add<CollisionBox>();

        world.system<Game, TextureAssets const>("SpawnFruits")
            .term_at(0)
            .singleton()
            .term_at(1)
            .singleton()
            .each(
                [](flecs::iter& it, size_t index, Game& game, TextureAssets const& texture_assets)
                {
                    std::bernoulli_distribution spider_dist(0.25);
                    bool spider = spider_dist(game.random_engine);

                    std::binomial_distribution<> velocity_dist(3, 0.5);
                    int vel = velocity_dist(game.random_engine) + 1;

                    if ((game.ticks % 50) == 0)
                    {
                        flecs::entity e;
                        std::uniform_int_distribution<> xpos_dist(32, WINDOW_WIDTH - 32);
                        int xpos = xpos_dist(game.random_engine);
                        if (!spider)
                        {
                            std::uniform_int_distribution<> index_dist(0, 228 - 1);
                            uint16_t index = index_dist(game.random_engine);

                            e = it.world()
                                    .entity()
                                    .add<Fruit>()
                                    .add<WorldPosition>()
                                    .set<Position>(Position{.x = xpos, .y = -16})
                                    .set<Velocity>(Velocity{.x = 0, .y = vel})
                                    .set<Sprite>(Sprite{.texture = &texture_assets.fruits,
                                                        .texture_atlas_index = index})
                                    .set<Size>(Size{.w = 32, .h = 32});
                        }
                        else
                        {
                            std::uniform_int_distribution<> index_dist(0, 8 - 1);
                            uint16_t index = index_dist(game.random_engine);

                            e = it.world()
                                    .entity()
                                    .add<Spider>()
                                    .add<WorldPosition>()
                                    .set<Position>(Position{.x = xpos, .y = -16})
                                    .set<Velocity>(Velocity{.x = 0, .y = vel})
                                    .set<Sprite>(Sprite{.texture = &texture_assets.spiders,
                                                        .texture_atlas_index = index})
                                    .set<Size>(Size{.w = 32, .h = 32});
                        }
                        it.world()
                            .entity("CollisionBox")
                            .child_of(e)
                            .add<WorldPosition>()
                            .set<Position>(Position{.x = 0, .y = 0})
                            .set<Size>(Size{.w = 32, .h = 32})
                            .add<CollisionBox>();
                    }
                });

        // world.system<WorldPosition const, Item>("DespawnItems")
        //     .kind(flecs::OnValidate)
        //     .each(
        //         [](flecs::entity e, WorldPosition const& pos, Item)
        //         {
        //             if (pos.y >= WINDOW_HEIGHT)
        //                 e.destruct();
        //         });

        world.system<Game, AudioStreams, AudioAssets, Position, Fruit, Collided>("CollectFruit")
            .term_at(0)
            .singleton()
            .term_at(1)
            .singleton()
            .term_at(2)
            .singleton()
            .each(
                [](flecs::entity e,
                   Game& game,
                   AudioStreams& audio_streams,
                   AudioAssets& audio_assets,
                   Position& pos,
                   Fruit,
                   Collided)
                {
                    game.score += 10;
                    pos.x += 1000;
                    // e.destruct();

                    SDL_PutAudioStreamData(audio_streams.sound_stream,
                                           audio_assets.pickup_sound.buffer,
                                           audio_assets.pickup_sound.buffer_length);
                });

        world.system<Game, AudioStreams, AudioAssets, Position, Spider, Collided>("CollectSpider")
            .term_at(0)
            .singleton()
            .term_at(1)
            .singleton()
            .term_at(2)
            .singleton()
            .each(
                [](flecs::entity e,
                   Game& game,
                   AudioStreams& audio_streams,
                   AudioAssets& audio_assets,
                   Position& pos,
                   Spider,
                   Collided)
                {
                    game.score -= 50;
                    pos.x += 1000;
                    // e.destruct();

                    SDL_PutAudioStreamData(audio_streams.sound_stream,
                                           audio_assets.hit_sound.buffer,
                                           audio_assets.hit_sound.buffer_length);
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
    }
};
