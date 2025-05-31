#include "physics.hpp"
#include "level.hpp"

#include <spdlog/spdlog.h>

PhysicsModule::PhysicsModule(flecs::world& world)
{
    world.system<Position, Velocity const>("TranslatePhysicsObject")
        .each(
            [](Position& pos, Velocity const& vel)
            {
                pos.x += vel.x;
                pos.y += vel.y;
            });

    // Introduce phase that runs after OnUpdate but before OnValidate
    flecs::entity propagate_phase = world.entity().add(flecs::Phase).depends_on(flecs::OnUpdate);

    world.system<WorldPosition, Position const>("PropagatePosition")
        .kind(propagate_phase)
        .each(
            [](flecs::entity e, WorldPosition& world_pos, Position const& local_pos)
            {
                if (e.parent() != flecs::entity::null() && e.parent().has<WorldPosition>())
                    return;

                world_pos.x = local_pos.x;
                world_pos.y = local_pos.y;

                std::function<void(flecs::entity, WorldPosition const&)> propagate_to_children;
                propagate_to_children = [&](flecs::entity parent, WorldPosition const& parent_pos)
                {
                    parent.children(
                        [=](flecs::entity child)
                        {
                            auto local_pos = child.get<Position>();
                            auto world_pos = child.get_mut<WorldPosition>();

                            world_pos->x = parent_pos.x + local_pos->x;
                            world_pos->y = parent_pos.y + local_pos->y;

                            propagate_to_children(child, *world_pos);
                        });
                };

                propagate_to_children(e, world_pos);
            });

    auto basket_query = world.query<Basket>();

    world.system<WorldPosition const, Size const, CollisionBox>("CollisionCheck")
        .kind(flecs::OnValidate)
        .each(
            [basket_query](
                flecs::entity e, WorldPosition const& world_pos, Size const& size, CollisionBox)
            {
                if (e.parent().has<Basket>())
                    return;

                auto fruit = e.parent();
                auto basket = basket_query.first();
                basket.children(
                    [fruit, world_pos, size](flecs::entity basket_child)
                    {
                        if (!basket_child.has<CollisionBox>())
                            return;

                        auto basket_child_pos = basket_child.get<WorldPosition>();
                        auto basket_child_size = basket_child.get<Size>();

                        if (basket_child_pos->x + basket_child_size->w >= world_pos.x &&
                            basket_child_pos->x <= world_pos.x + size.w &&
                            basket_child_pos->y + basket_child_size->h >= world_pos.y &&
                            basket_child_pos->y <= world_pos.y + size.h)
                        {
                            fruit.add<Collided>();
                        }
                    });
            });
}