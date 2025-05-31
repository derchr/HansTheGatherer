#include "physics.hpp"
#include "level.hpp"

#include <spdlog/spdlog.h>

void PhysicsModule::TranslatePhysicsObject(entt::registry& registry)
{
    auto view = registry.view<Position, Velocity const>();

    for (auto [entity, pos, vel] : view.each())
    {
        pos.x += vel.x;
        pos.y += vel.y;
    }
}

void PhysicsModule::PropagatePosition(entt::registry& registry)
{
    auto root_transform_view = registry.view<Position const, WorldPosition>(entt::exclude<Parent>);
    auto transform_view = registry.view<Position const, WorldPosition, Parent const>();

    for (auto [entity, pos, world_pos] : root_transform_view.each())
    {
        world_pos.x = pos.x;
        world_pos.y = pos.y;
    }

    for (auto [entity, pos, world_pos, parent] : transform_view.each())
    {
        auto parent_pos = registry.get<WorldPosition const>(parent.parent);

        world_pos.x = parent_pos.x + pos.x;
        world_pos.y = parent_pos.y + pos.y;
    }
}

void PhysicsModule::RemoveCollisionMarker(entt::registry& registry)
{
    auto view = registry.view<Collided const>();
    for (auto [entity] : view.each())
    {
        registry.remove<Collided>(entity);
    }
}
void PhysicsModule::CollisionCheck(entt::registry& registry)
{
    auto view = registry.view<WorldPosition const, Size const, Parent const, CollisionBox>(
        entt::exclude<BasketCollisionBox>);
    auto basket_cb_view = registry.view<WorldPosition const, Size const, BasketCollisionBox>();

    for (auto [e, world_pos, size, parent] : view.each())
    {
        auto fruit = parent.parent;

        for (auto [basket, basket_world_pos, basket_size] : basket_cb_view.each())
        {
            if (basket_world_pos.x + basket_size.w >= world_pos.x &&
                basket_world_pos.x <= world_pos.x + size.w &&
                basket_world_pos.y + basket_size.h >= world_pos.y &&
                basket_world_pos.y <= world_pos.y + size.h)
            {
                registry.emplace<Collided>(fruit);
            }
        }
    }
}

PhysicsModule::PhysicsModule(entt::registry& registry)
{
}