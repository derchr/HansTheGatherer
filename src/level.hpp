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
            .set<Position>(Position{.x = 0, .y = 0})
            .set<Size>(Size{.w = 64, .h = 32})
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
                    if ((game.ticks % 100) == 0)
                    {
                        auto fruit =
                            it.world()
                                .entity()
                                .add<Fruit>()
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

        // world.system<WorldPosition const, Fruit>("DespawnFruits")
        //     .kind(flecs::OnValidate)
        //     .each(
        //         [](flecs::entity e, WorldPosition const& pos, Fruit)
        //         {
        //             if (pos.y >= WINDOW_HEIGHT)
        //                 e.destruct();
        //         });

        world.system<Game, Fruit, Collided>("CollectItem")
            .term_at(0)
            .singleton()
            .each([](flecs::entity e, Game& game, Fruit, Collided) { game.score += 10; });

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