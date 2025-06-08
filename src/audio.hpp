#pragma once

#include <cstdint>
#include <entt/entt.hpp>

struct AudioAsset
{
    // SDL_AudioSpec spec;
    uint8_t const* buffer;
    uint32_t buffer_length;
};

// struct AudioStreams
// {
//     SDL_AudioStream* music_stream;
//     SDL_AudioStream* sound_stream;
// };

struct AudioModule
{
    AudioModule(entt::registry& registry);
    // static void FeedAudioStreams(entt::registry& registry);
};
