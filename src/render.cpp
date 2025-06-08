#include "render.hpp"

#include "definitions.hpp"
#include "level.hpp"
#include "physics.hpp"
#include "sprite.hpp"

#include <format>

RenderModule::RenderModule(entt::registry& registry)
{
}

void RenderModule::RenderSprites(entt::registry& registry)
{
    // auto const& sdl_handles = registry.ctx().get<SdlHandles>();
    auto const& game = registry.ctx().get<Game>();
    auto sprites_view =
        registry.view<Position const, Size const, Sprite const>(entt::exclude<Background>);
    auto background_view = registry.view<Position const, Size const, Sprite const, Background>();

    for (auto [entity, pos, size, sprite] : background_view.each())
    {
        TextureAtlasLayout layout = sprite.texture->texture_atlas_layout;
        uint8_t row = sprite.texture_atlas_index / layout.columns;
        uint8_t column = sprite.texture_atlas_index % layout.columns;
        // SDL_FRect srcrect{static_cast<float>(column * layout.width),
        //                   static_cast<float>(row * layout.height),
        //                   static_cast<float>(layout.width),
        //                   static_cast<float>(layout.height)};

        // SDL_FRect dstrect{static_cast<float>(pos.x),
        //                   static_cast<float>(pos.y),
        //                   static_cast<float>(size.w),
        //                   static_cast<float>(size.h)};

        // SDL_RenderTexture(sdl_handles.renderer, sprite.texture->sdl_texture, &srcrect, &dstrect);
    }

    for (auto [entity, pos, size, sprite] : sprites_view.each())
    {
        TextureAtlasLayout layout = sprite.texture->texture_atlas_layout;
        uint8_t row = sprite.texture_atlas_index / layout.columns;
        uint8_t column = sprite.texture_atlas_index % layout.columns;
        // SDL_FRect srcrect{static_cast<float>(column * layout.width),
        //                   static_cast<float>(row * layout.height),
        //                   static_cast<float>(layout.width),
        //                   static_cast<float>(layout.height)};

        // SDL_FRect dstrect{static_cast<float>(pos.x),
        //                   static_cast<float>(pos.y),
        //                   static_cast<float>(size.w),
        //                   static_cast<float>(size.h)};

        // SDL_RenderTexture(sdl_handles.renderer, sprite.texture->sdl_texture, &srcrect, &dstrect);
    }
}

void RenderModule::RenderScore(entt::registry& registry)
{
    // auto const& sdl_handles = registry.ctx().get<SdlHandles>();
    auto const& game = registry.ctx().get<Game>();
    // auto const& font_assets = registry.ctx().get<FontAssets>();

    auto score_string = std::format("Score: {}\nTime: {}", game.score, game.time);
    // auto text = TTF_CreateText(sdl_handles.text_engine,
    //                            font_assets.default_font.font,
    //                            score_string.c_str(),
    //                            score_string.length());
    // TTF_DrawRendererText(text, 0.0, 0.0);
    // TTF_DestroyText(text);
}