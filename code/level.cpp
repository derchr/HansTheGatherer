#include "level.hpp"
#include "input.hpp"

#include <Hall/Hall.h>

LevelModule::LevelModule(entt::registry& registry)
{
    auto const* texture_assets = &registry.ctx().get<TextureAssets>();
    // auto const* audio_steams = &registry.ctx().get<AudioStreams>();
    // auto const* audio_assets = &registry.ctx().get<AudioAssets>();

    // Hall::SetupStereo(audio_steams->music_stream.id_left,
    //                   audio_steams->music_stream.id_right,
    //                   audio_assets->background_music.buffer,
    //                   audio_assets->background_music.buffer_length / (2 * 2),
    //                   0,
    //                   audio_assets->background_music.buffer_length / (2 * 2));
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
    registry.emplace<Sprite>(basket,
                             Sprite{.texture = &texture_assets->basket, .texture_atlas_index = 0});
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

void LevelModule::MoveBasket(entt::registry& registry)
{
    auto const& input = registry.ctx().get<ButtonInput>();
    auto basket_view = registry.view<Position, Size const, Sprite const, Basket>();

    for (auto [entity, pos, size, sprite] : basket_view.each())
    {
        if (input.pressed.find(Key::Left) != input.pressed.end())
        {
            pos.x -= 5;
        }
        if (input.pressed.find(Key::Right) != input.pressed.end())
        {
            pos.x += 5;
        }

        pos.x = pos.x < 0 ? 0 : pos.x;
        pos.x = pos.x > WINDOW_WIDTH - size.w ? WINDOW_WIDTH - size.w : pos.x;
    }
}

void LevelModule::SpawnFruits(entt::registry& registry)
{
    auto& game = registry.ctx().get<Game>();
    auto const& texture_assets = registry.ctx().get<TextureAssets>();

    std::bernoulli_distribution spider_dist(0.25);
    bool spider = spider_dist(game.random_engine);

    std::binomial_distribution<> velocity_dist(3, 0.5);
    int vel = velocity_dist(game.random_engine) + 1;

    if ((game.ticks % 50) == 0)
    {
        auto item = registry.create();
        std::uniform_int_distribution<> xpos_dist(32, WINDOW_WIDTH - 32);
        int xpos = xpos_dist(game.random_engine);
        if (!spider)
        {
            std::uniform_int_distribution<> index_dist(0, 228 - 1);
            uint16_t index = index_dist(game.random_engine);

            registry.emplace<Fruit>(item);
            registry.emplace<WorldPosition>(item);
            registry.emplace<Position>(item, Position{.x = xpos, .y = -16});
            registry.emplace<Velocity>(item, Velocity{.x = 0, .y = vel});
            registry.emplace<Sprite>(
                item, Sprite{.texture = &texture_assets.fruits, .texture_atlas_index = index});
            registry.emplace<Size>(item, Size{.w = 32, .h = 32});
        }
        else
        {
            std::uniform_int_distribution<> index_dist(0, 8 - 1);
            uint16_t index = index_dist(game.random_engine);

            registry.emplace<Spider>(item);
            registry.emplace<WorldPosition>(item);
            registry.emplace<Position>(item, Position{.x = xpos, .y = -16});
            registry.emplace<Velocity>(item, Velocity{.x = 0, .y = vel});
            registry.emplace<Sprite>(
                item, Sprite{.texture = &texture_assets.spiders, .texture_atlas_index = index});
            registry.emplace<Size>(item, Size{.w = 32, .h = 32});
        }

        auto collision_box = registry.create();
        registry.emplace<Parent>(collision_box, item);
        registry.emplace<WorldPosition>(collision_box);
        registry.emplace<Position>(collision_box, Position{.x = 0, .y = 0});
        registry.emplace<Size>(collision_box, Size{.w = 32, .h = 32});
        registry.emplace<CollisionBox>(collision_box, item);
    }
}

void LevelModule::CollectFruit(entt::registry& registry)
{
    auto& game = registry.ctx().get<Game>();
    // auto const& audio_streams = registry.ctx().get<AudioStreams>();
    // auto const& audio_assets = registry.ctx().get<AudioAssets>();

    auto view = registry.view<Position, Fruit, Collided>();
    for (auto [entity, pos] : view.each())
    {
        game.score += 10;
        pos.x += 1000;
        // registry.destroy(entity);

        // SDL_PutAudioStreamData(audio_streams.sound_stream,
        //                        audio_assets.pickup_sound.buffer,
        //                        audio_assets.pickup_sound.buffer_length);
    }
}

void LevelModule::CollectSpider(entt::registry& registry)
{
    auto& game = registry.ctx().get<Game>();
    // auto const& audio_streams = registry.ctx().get<AudioStreams>();
    // auto const& audio_assets = registry.ctx().get<AudioAssets>();

    auto view = registry.view<Position, Spider, Collided>();
    for (auto [entity, pos] : view.each())
    {
        game.score -= 50;
        pos.x += 1000;
        // registry.destroy(entity);

        // SDL_PutAudioStreamData(audio_streams.sound_stream,
        //                        audio_assets.hit_sound.buffer,
        //                        audio_assets.hit_sound.buffer_length);
    };
}

void LevelModule::DespawnItems(entt::registry& registry)
{
    auto view = registry.view<WorldPosition const, Item>();
    for (auto [entity, pos] : view.each())
    {
        if (pos.y >= WINDOW_HEIGHT)
            registry.destroy(entity);
    }
}
