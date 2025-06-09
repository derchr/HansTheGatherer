#pragma once

#include <entt/entt.hpp>

struct RenderModule
{
    RenderModule(entt::registry& registry);

    static void RenderSprites(entt::registry& registry);
    static void RenderScore(entt::registry& registry);
};
