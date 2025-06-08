#pragma once

#include "definitions.hpp"
#include "physics.hpp"
#include "sprite.hpp"

#include <entt/entt.hpp>

struct Background
{
};

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

struct BasketCollisionBox
{
};

struct LevelModule
{
    LevelModule(entt::registry& registry)
    {
        auto const* texture_assets = &registry.ctx().get<TextureAssets>();

        auto background = registry.create();
        registry.emplace<Position>(background, Position{.x = 0, .y = 0});
        registry.emplace<Sprite>(
            background, Sprite{.texture = &texture_assets->background, .texture_atlas_index = 0});
        registry.emplace<Size>(background, Size{.w = WINDOW_WIDTH, .h = WINDOW_HEIGHT});
        registry.emplace<Background>(background);

        auto basket = registry.create();
        registry.emplace<WorldPosition>(basket);
        registry.emplace<Position>(basket,
                                   Position{.x = WINDOW_WIDTH / 2 - 32, .y = WINDOW_HEIGHT - 32});
        registry.emplace<Sprite>(
            basket, Sprite{.texture = &texture_assets->basket, .texture_atlas_index = 0});
        registry.emplace<Size>(basket, Size{.w = 64, .h = 32});
        registry.emplace<Basket>(basket);

        auto basket_cb = registry.create();
        registry.emplace<WorldPosition>(basket_cb);
        registry.emplace<Position>(basket_cb, Position{.x = 0, .y = 16});
        registry.emplace<Size>(basket_cb, Size{.w = 64, .h = 16});
        registry.emplace<CollisionBox>(basket_cb);
        registry.emplace<Parent>(basket_cb, basket);
        registry.emplace<BasketCollisionBox>(basket_cb);
    }

    static void MoveBasket(entt::registry& registry);
    static void SpawnFruits(entt::registry& registry);
    static void CollectFruit(entt::registry &registry);
    static void CollectSpider(entt::registry &registry);
    static void DespawnItems(entt::registry &registry);
};
