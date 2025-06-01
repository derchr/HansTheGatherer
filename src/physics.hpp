#pragma once

#include <flecs.h>

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

struct CollisionBox
{
};

struct Collided
{
};

struct TranslateSystem
{
    flecs::system translate_system;
};

struct PhysicsModule
{
    PhysicsModule(flecs::world& world);
};
