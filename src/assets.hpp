#pragma once

#include "audio.hpp"

#include <SDL3/SDL.h>
#include <flecs.h>

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
    Texture basket;
};

struct AudioAssets
{
    AudioAsset background_music;
};

struct AssetModule
{
    AssetModule(flecs::world& world);
};
