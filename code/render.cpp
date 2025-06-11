#include "render.hpp"

#include "definitions.hpp"
#include "level.hpp"
#include "physics.hpp"
#include "sprite.hpp"

#include <Hall/Hall.h>

#include <format>

RenderModule::RenderModule(entt::registry& registry)
{
}

void RenderModule::RenderSprites(entt::registry& registry)
{
    auto const& game = registry.ctx().get<Game>();
    auto sprites_view =
        registry.view<Position const, Size const, Sprite const>(entt::exclude<Background>);
    auto background_view = registry.view<Position const, Size const, Sprite const, Background>();

    auto render_sprite =
        [](entt::entity entity, Position const& pos, Size const& size, Sprite const& sprite)
    {
        TextureAtlasLayout layout = sprite.texture->texture_atlas_layout;
        uint8_t row = sprite.texture_atlas_index / layout.columns;
        uint8_t column = sprite.texture_atlas_index % layout.columns;

        Hall::Draw(sprite.texture->data,
                   column * layout.width,
                   row * layout.height,
                   pos.x,
                   pos.y,
                   layout.width,
                   layout.height,
                   sprite.texture->data_length);
    };

    background_view.each(render_sprite);
    sprites_view.each(render_sprite);
}

void RenderModule::RenderScore(entt::registry& registry)
{
    // auto const& sdl_handles = registry.ctx().get<SdlHandles>();
    auto const& game = registry.ctx().get<Game>();
    // auto const& font_assets = registry.ctx().get<FontAssets>();

    // auto score_string = std::format("Score: {}\nTime: {}", game.score, game.time);
    // auto text = TTF_CreateText(sdl_handles.text_engine,
    //                            font_assets.default_font.font,
    //                            score_string.c_str(),
    //                            score_string.length());
    // TTF_DrawRendererText(text, 0.0, 0.0);
    // TTF_DestroyText(text);
}