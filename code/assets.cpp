#include "assets.hpp"
#include "audio.hpp"

#include <cstdint>
#include "bitmaps/basket.h"
#include "bitmaps/fruits.h"
#include "bitmaps/jungle.h"
#include "bitmaps/spiders.h"

AssetModule::AssetModule(entt::registry& registry)
{
    TextureAtlasLayout background_layout{.width = 400, .height = 240, .rows = 1, .columns = 1};
    TextureAtlasLayout fruits_layout{.width = 32, .height = 32, .rows = 6, .columns = 38};
    TextureAtlasLayout spiders_layout{.width = 32, .height = 32, .rows = 2, .columns = 4};
    TextureAtlasLayout basket_layout{.width = 64, .height = 32, .rows = 1, .columns = 1};
    Texture background{.data = jungle_data,
                       .width = JUNGLE_WIDTH,
                       .height = JUNGLE_HEIGHT,
                       .texture_atlas_layout = background_layout};
    Texture fruits{.data = fruits_data,
                   .width = FRUITS_WIDTH,
                   .height = FRUITS_HEIGHT,
                   .texture_atlas_layout = fruits_layout};
    Texture spiders{.data = spiders_data,
                    .width = SPIDERS_WIDTH,
                    .height = SPIDERS_HEIGHT,
                    .texture_atlas_layout = spiders_layout};
    Texture basket{.data = basket_data,
                    .width = BASKET_WIDTH,
                    .height = BASKET_HEIGHT,
                   .texture_atlas_layout = basket_layout};

    registry.ctx().emplace<TextureAssets>(TextureAssets{
        .background = background, .fruits = fruits, .spiders = spiders, .basket = basket});

    // AudioAsset background_music{.buffer = BACKGROUND_MUSIC_DATA,
    //                             .buffer_length = sizeof(BACKGROUND_MUSIC_DATA)};
    // AudioAsset pickup_sound{.buffer = PICKUP_SOUND_DATA,
    //                         .buffer_length = sizeof(PICKUP_SOUND_DATA)};
    // AudioAsset hit_sound{.buffer = HIT_SOUND_DATA, .buffer_length = sizeof(HIT_SOUND_DATA)};
    // registry.ctx().emplace<AudioAssets>(AudioAssets{.background_music = background_music,
    //                                                 .pickup_sound = pickup_sound,
    //                                                 .hit_sound = hit_sound});

    // auto font = load_font(DEFAULT_FONT_DATA, sizeof(DEFAULT_FONT_DATA));
    // registry.ctx().emplace<FontAssets>(FontAssets{.default_font = font});
}
