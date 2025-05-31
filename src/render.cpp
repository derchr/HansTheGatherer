#include "render.hpp"

#include "definitions.hpp"
#include "physics.hpp"
#include "sprite.hpp"

#include <format>

RenderModule::RenderModule(flecs::world& world)
{
    world.system<SdlHandles>("RenderClear")
        .term_at(0)
        .singleton()
        .kind(flecs::PreStore)
        .each([](SdlHandles& sdl_handles) { SDL_RenderClear(sdl_handles.renderer); });

    flecs::entity render_present_phase =
        world.entity().add(flecs::Phase).depends_on(flecs::OnStore);

    world.system<SdlHandles>("RenderPresent")
        .term_at(0)
        .singleton()
        .kind(render_present_phase)
        .each([](SdlHandles& sdl_handles) { SDL_RenderPresent(sdl_handles.renderer); });

    world.system<SdlHandles const, Position const, Size const, Sprite const>("RenderSprites")
        .kind(flecs::OnStore)
        .term_at(0)
        .singleton()
        .each(
            [](SdlHandles const& sdl_handles,
               Position const& pos,
               Size const& size,
               Sprite const& sprite)
            {
                TextureAtlasLayout layout = sprite.texture->texture_atlas_layout;
                uint8_t row = sprite.texture_atlas_index / layout.columns;
                uint8_t column = sprite.texture_atlas_index % layout.columns;
                SDL_FRect srcrect{static_cast<float>(column * layout.width),
                                  static_cast<float>(row * layout.height),
                                  static_cast<float>(layout.width),
                                  static_cast<float>(layout.height)};

                SDL_FRect dstrect{static_cast<float>(pos.x),
                                  static_cast<float>(pos.y),
                                  static_cast<float>(size.w),
                                  static_cast<float>(size.h)};

                SDL_RenderTexture(
                    sdl_handles.renderer, sprite.texture->sdl_texture, &srcrect, &dstrect);
            });

    world.system<SdlHandles const, Game const, FontAssets const>("RenderScore")
        .kind(flecs::OnStore)
        .term_at(0)
        .singleton()
        .term_at(1)
        .singleton()
        .term_at(2)
        .singleton()
        .each(
            [](SdlHandles const& sdl_handles, Game const& game, FontAssets const& font_assets)
            {
                auto score_string = std::format("Score: {}\nTime: {}", game.score, game.time);
                auto text = TTF_CreateText(sdl_handles.text_engine,
                                           font_assets.default_font.font,
                                           score_string.c_str(),
                                           score_string.length());
                TTF_DrawRendererText(text, 0.0, 0.0);

                TTF_DestroyText(text);
            });
}
