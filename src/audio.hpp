#pragma once

#include <SDL3/SDL.h>
#include <cstdint>
#include <flecs.h>

struct AudioAsset
{
    SDL_AudioSpec spec;
    uint8_t* buffer;
    uint32_t buffer_length;
};

struct AudioStreams
{
    SDL_AudioStream* music_stream;
    SDL_AudioStream* sound_stream;
};

struct AudioModule
{
    AudioModule(flecs::world& world);
};
