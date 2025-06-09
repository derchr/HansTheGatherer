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
    LevelModule(entt::registry& registry);

    static void MoveBasket(entt::registry& registry);
    static void SpawnFruits(entt::registry& registry);
    static void CollectFruit(entt::registry &registry);
    static void CollectSpider(entt::registry &registry);
    static void DespawnItems(entt::registry &registry);
};
