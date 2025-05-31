#include "physics.hpp"
#include "definitions.hpp"
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

    // world.system<WorldPosition, Position const>("PropagatePosition")
    //     .kind(flecs::PostUpdate)
    //     .each(
    //         [](flecs::entity e, WorldPosition& world_pos, Position const& local_pos)
    //         {
    //             if (e.parent() == flecs::entity::null())
    //             {
    //                 world_pos.x = local_pos.x;
    //                 world_pos.y = local_pos.y;
    //             }
    //             else
    //             {
    //                 auto parent_world_pos = e.parent().get<WorldPosition>();
    //                 world_pos.x = parent_world_pos->x + local_pos.x;
    //                 world_pos.y = parent_world_pos->y + local_pos.y;
    //             }
    //         });

    world.system<SdlHandles const, WorldPosition const, Size const, CollisionBox>("DrawBoxes")
        .term_at(0)
        .singleton()
        .each(
            [](SdlHandles const& sdl_handles,
               WorldPosition const& pos,
               Size const& size,
               CollisionBox)
            {
                SDL_FRect rect{static_cast<float>(pos.x),
                               static_cast<float>(pos.y),
                               static_cast<float>(size.w),
                               static_cast<float>(size.h)};
                SDL_SetRenderDrawColor(sdl_handles.renderer, 0, 0, 255, 255);
                SDL_RenderRect(sdl_handles.renderer, &rect);
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

                basket_query.first().children(
                    [world_pos, size](flecs::entity basket_child)
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
                            spdlog::info("collision");
                        }
                    });
            });
}