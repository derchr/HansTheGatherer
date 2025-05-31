#pragma once

#include <entt/entt.hpp>

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

struct Parent
{
    entt::entity parent;
};

struct Children
{
    std::vector<entt::entity> children;
};

struct CollisionBox
{
};

struct Collided
{
};

struct PhysicsModule
{
    PhysicsModule(entt::registry& registry);

    static void TranslatePhysicsObject(entt::registry& registry);
    static void PropagatePosition(entt::registry& registry);
    static void RemoveCollisionMarker(entt::registry& registry);
    static void CollisionCheck(entt::registry& registry);
};
