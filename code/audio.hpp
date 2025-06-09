#pragma once

#include <cstdint>
#include <entt/entt.hpp>

struct AudioAsset
{
    uint8_t const* buffer;
    uint32_t buffer_length;
};

struct MonoChannel
{
    int id_mono;
};

struct StereoChannel
{
    int id_left;
    int id_right;
};

struct AudioStreams
{
    StereoChannel music_stream;
    MonoChannel sound_stream;
};

struct AudioModule
{
    AudioModule(entt::registry& registry);
};
