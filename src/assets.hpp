#pragma once

#include "audio.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <entt/entt.hpp>

struct TextureAtlasLayout
{
    uint16_t width;
    uint16_t height;
    uint8_t rows;
    uint8_t columns;
};

struct Texture
{
    SDL_Texture* sdl_texture;
    TextureAtlasLayout texture_atlas_layout;
};

struct TextureAssets
{
    Texture background;
    Texture fruits;
    Texture spiders;
    Texture basket;
};

struct AudioAssets
{
    AudioAsset background_music;
    AudioAsset pickup_sound;
    AudioAsset hit_sound;
};

struct FontAsset
{
    TTF_Font* font;
};

struct FontAssets
{
    FontAsset default_font;
};

struct AssetModule
{
    AssetModule(entt::registry& registry);
};
