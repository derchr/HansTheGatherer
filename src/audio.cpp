#include "audio.hpp"
#include "assets.hpp"

AudioModule::AudioModule(entt::registry& registry)
{
    auto const& audio_assets = registry.ctx().get<AudioAssets>();
    auto* music_stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio_assets.background_music.spec, NULL, NULL);
    auto* sound_stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio_assets.pickup_sound.spec, NULL, NULL);

    SDL_ResumeAudioStreamDevice(music_stream);
    SDL_ResumeAudioStreamDevice(sound_stream);

    registry.ctx().emplace<AudioStreams>(
        AudioStreams{.music_stream = music_stream, .sound_stream = sound_stream});
}

void AudioModule::FeedAudioStreams(entt::registry& registry)
{
    auto audio_streams = registry.ctx().get<AudioStreams>();
    auto audio_assets = registry.ctx().get<AudioAssets>();

    if (SDL_GetAudioStreamQueued(audio_streams.music_stream) <
        static_cast<int>(audio_assets.background_music.buffer_length))
    {
        SDL_PutAudioStreamData(audio_streams.music_stream,
                               audio_assets.background_music.buffer,
                               audio_assets.background_music.buffer_length);
    }
}
