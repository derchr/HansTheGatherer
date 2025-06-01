#include "audio.hpp"
#include "assets.hpp"

AudioModule::AudioModule(flecs::world& world)
{
    auto* audio_assets = world.get<AudioAssets>();
    auto* music_stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio_assets->background_music.spec, NULL, NULL);
    auto* sound_stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio_assets->pickup_sound.spec, NULL, NULL);

    SDL_ResumeAudioStreamDevice(music_stream);
    SDL_ResumeAudioStreamDevice(sound_stream);

    world.set<AudioStreams>(
        AudioStreams{.music_stream = music_stream, .sound_stream = sound_stream});

    world.system<AudioStreams, AudioAssets>("FeedAudioStreams")
        .term_at(0)
        .singleton()
        .term_at(1)
        .singleton()
        .each(
            [](AudioStreams& audio_streams, AudioAssets& audio_assets)
            {
                if (SDL_GetAudioStreamQueued(audio_streams.music_stream) <
                    static_cast<int>(audio_assets.background_music.buffer_length))
                {
                    SDL_PutAudioStreamData(audio_streams.music_stream,
                                           audio_assets.background_music.buffer,
                                           audio_assets.background_music.buffer_length);
                }
            });
}
