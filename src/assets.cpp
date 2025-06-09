#include "assets.hpp"
#include "audio.hpp"

#include <cstdint>

static constexpr uint8_t BACKGROUND_DATA[] = {
#embed "../assets/images/jungle.bmp"
};

static constexpr uint8_t FRUITS_DATA[] = {
#embed "../assets/images/fruits.bmp"
};

static constexpr uint8_t SPIDERS_DATA[] = {
#embed "../assets/images/spiders.bmp"
};

static constexpr uint8_t BASKET_DATA[] = {
#embed "../assets/images/basket.bmp"
};

static constexpr uint8_t BACKGROUND_MUSIC_DATA[] = {
#embed "../assets/sounds/JamaicanSunrise.wav"
};

static constexpr uint8_t PICKUP_SOUND_DATA[] = {
#embed "../assets/sounds/pickup.wav"
};

static constexpr uint8_t HIT_SOUND_DATA[] = {
#embed "../assets/sounds/hit.wav"
};

static constexpr uint8_t DEFAULT_FONT_DATA[] = {
#embed "../assets/fonts/OpenTTD-Sans.ttf"
};

// SDL_Texture* load_texture(uint8_t const* data, size_t size, SDL_Renderer* renderer)
// {
//     auto* iostream = SDL_IOFromConstMem(data, size);
//     SDL_Surface* surface = SDL_LoadBMP_IO(iostream, false);
//     if (surface == nullptr)
//     {
//         // spdlog::error("Failed to load SDL surface!\nCause: {}", SDL_GetError());
//     }

//     SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
//     if (texture == nullptr)
//     {
//         // spdlog::error("Failed to create texture from surface!\nCause: {}", SDL_GetError());
//     }

//     return texture;
// }

// AudioAsset load_audio(uint8_t const* data, size_t size)
// {
//     AudioAsset audio_asset;

//     auto* iostream = SDL_IOFromConstMem(data, size);
//     bool res = SDL_LoadWAV_IO(
//         iostream, false, &audio_asset.spec, &audio_asset.buffer, &audio_asset.buffer_length);
//     if (!res)
//     {
//         // spdlog::error("Failed to load audio file!\nCause: {}", SDL_GetError());
//     }
//     return audio_asset;
// }

// FontAsset load_font(uint8_t const* data, size_t size)
// {
//     FontAsset font_asset;

//     auto* iostream = SDL_IOFromConstMem(data, size);
//     auto* ttf = TTF_OpenFontIO(iostream, false, 20);

//     font_asset.font = ttf;

//     return font_asset;
// }

AssetModule::AssetModule(entt::registry& registry)
{
    TextureAtlasLayout background_layout{.width = 400, .height = 240, .rows = 1, .columns = 1};
    TextureAtlasLayout fruits_layout{.width = 32, .height = 32, .rows = 6, .columns = 38};
    TextureAtlasLayout spiders_layout{.width = 32, .height = 32, .rows = 2, .columns = 4};
    TextureAtlasLayout basket_layout{.width = 64, .height = 32, .rows = 1, .columns = 1};
    Texture background{.data = BACKGROUND_DATA,
                       .data_length = sizeof(BACKGROUND_DATA),
                       .texture_atlas_layout = background_layout};
    Texture fruits{.data = FRUITS_DATA,
                   .data_length = sizeof(FRUITS_DATA),
                   .texture_atlas_layout = fruits_layout};
    Texture spiders{.data = SPIDERS_DATA,
                    .data_length = sizeof(SPIDERS_DATA),
                    .texture_atlas_layout = spiders_layout};
    Texture basket{.data = BASKET_DATA,
                   .data_length = sizeof(BASKET_DATA),
                   .texture_atlas_layout = basket_layout};
                   
    registry.ctx().emplace<TextureAssets>(TextureAssets{
        .background = background, .fruits = fruits, .spiders = spiders, .basket = basket});

    // auto background_music = load_audio(BACKGROUND_MUSIC_DATA, sizeof(BACKGROUND_MUSIC_DATA));
    // auto pickup_sound = load_audio(PICKUP_SOUND_DATA, sizeof(PICKUP_SOUND_DATA));
    // auto hit_sound = load_audio(HIT_SOUND_DATA, sizeof(HIT_SOUND_DATA));
    registry.ctx().emplace<AudioAssets>(AudioAssets{
        .background_music = {.buffer = BACKGROUND_MUSIC_DATA,
                             .buffer_length = sizeof(BACKGROUND_MUSIC_DATA)},
        .pickup_sound = {.buffer = PICKUP_SOUND_DATA, .buffer_length = sizeof(PICKUP_SOUND_DATA)},
        .hit_sound = {.buffer = HIT_SOUND_DATA, .buffer_length = sizeof(HIT_SOUND_DATA)}});
    // registry.ctx().emplace<AudioAssets>(AudioAssets{.background_music = background_music,
    //                                                 .pickup_sound = pickup_sound,
    //                                                 .hit_sound = hit_sound});

    // auto font = load_font(DEFAULT_FONT_DATA, sizeof(DEFAULT_FONT_DATA));
    // registry.ctx().emplace<FontAssets>(FontAssets{.default_font = font});
}
