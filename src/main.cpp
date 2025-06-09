#include "assets.hpp"
#include "definitions.hpp"
#include "input.hpp"
#include "level.hpp"
#include "physics.hpp"
#include "render.hpp"

#include <Hall/Hall.h>
#include <entt/entt.hpp>

void increment_ticks(entt::registry& registry)
{
    auto& game = registry.ctx().get<Game>();
    game.ticks += 1;

    if (game.ticks % 60 == 0 && game.time > 0)
    {
        game.time--;
    }
}

int main()
{
    entt::registry registry;

    registry.ctx().emplace<Game>(Game{.ticks = 0, .time = 60, .score = 0, .random_engine = {}});
    registry.ctx().emplace<ButtonInput>(ButtonInput{});

    AssetModule asset_module(registry);
    AudioModule audio_module(registry);
    RenderModule render_module(registry);
    PhysicsModule physics_module(registry);
    LevelModule level_module(registry);

    bool exit_gameloop = false;
    while (!exit_gameloop)
    {
        // Input
        auto* input = &registry.ctx().get<ButtonInput>();
        if (Hall::GetLeft(0))
            input->pressed.insert(Key::Left);
        else
            input->pressed.erase(Key::Left);

        if (Hall::GetRight(0))
            input->pressed.insert(Key::Right);
        else
            input->pressed.erase(Key::Right);

        if (registry.ctx().get<Game>().time != 0)
        {
            increment_ticks(registry);
            LevelModule::MoveBasket(registry);
            LevelModule::SpawnFruits(registry);
            LevelModule::CollectFruit(registry);
            LevelModule::CollectSpider(registry);
            // LevelModule::DespawnItems(registry);
        }

        PhysicsModule::TranslatePhysicsObject(registry);
        PhysicsModule::PropagatePosition(registry);
        PhysicsModule::RemoveCollisionMarker(registry);
        PhysicsModule::CollisionCheck(registry);

        Hall::Clear(0);
        RenderModule::RenderSprites(registry);
        RenderModule::RenderScore(registry);
        Hall::SetCommandSwapBuffers();
        Hall::GetVSync();
    }

    return 0;
}
