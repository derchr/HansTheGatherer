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

struct Basket
{
};

struct LevelModule
{
    LevelModule(flecs::world& world)
    {
        auto const* texture_assets = world.get<TextureAssets>();

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

        world.system<Game const, TextureAssets const>("SpawnFruits")
            .term_at(0)
            .singleton()
            .term_at(1)
            .singleton()
            .each(
                [](flecs::iter& it,
                   size_t index,
                   Game const& game,
                   TextureAssets const& texture_assets)
                {
                    bool spider = std::rand() % 3 == 0;
                    int vel = std::rand() % 3 + 1;

                    if ((game.ticks % 50) == 0)
                    {
                        flecs::entity e;
                        if (!spider)
                        {
                            e = it.world()
                                    .entity()
                                    .add<Fruit>()
                                    .add<WorldPosition>()
                                    .set<Position>(
                                        Position{.x = std::rand() % WINDOW_WIDTH, .y = -16})
                                    .set<Velocity>(Velocity{.x = 0, .y = vel})
                                    .set<Sprite>(
                                        Sprite{.texture = &texture_assets.fruits,
                                               .texture_atlas_index =
                                                   static_cast<uint16_t>(std::rand() % 228)})
                                    .set<Size>(Size{.w = 32, .h = 32});
                        }
                        else
                        {
                            e = it.world()
                                    .entity()
                                    .add<Spider>()
                                    .add<WorldPosition>()
                                    .set<Position>(
                                        Position{.x = std::rand() % WINDOW_WIDTH, .y = -16})
                                    .set<Velocity>(Velocity{.x = 0, .y = vel})
                                    .set<Sprite>(Sprite{.texture = &texture_assets.spiders,
                                                        .texture_atlas_index =
                                                            static_cast<uint16_t>(std::rand() % 8)})
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

        // world.system<WorldPosition const, Fruit>("DespawnFruits")
        //     .kind(flecs::OnValidate)
        //     .each(
        //         [](flecs::entity e, WorldPosition const& pos, Fruit)
        //         {
        //             if (pos.y >= WINDOW_HEIGHT)
        //                 e.destruct();
        //         });

        world.system<Game, Position, Fruit, Collided>("CollectFruit")
            .term_at(0)
            .singleton()
            .each(
                [](flecs::entity e, Game& game, Position& pos, Fruit, Collided)
                {
                    game.score += 10;
                    pos.x = 1000;
                });

        world.system<Game, Position, Spider, Collided>("CollectSpider")
            .term_at(0)
            .singleton()
            .each(
                [](flecs::entity e, Game& game, Position& pos, Spider, Collided)
                {
                    game.score -= 50;
                    pos.x = 1000;
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
