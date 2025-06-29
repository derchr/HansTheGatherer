#pragma once

#include <random>

static constexpr int WINDOW_WIDTH = 400;
static constexpr int WINDOW_HEIGHT = 240;

// struct SdlHandles
// {
//     SDL_Window* window;
//     SDL_Renderer* renderer;
//     TTF_TextEngine* text_engine;
// };

struct Game
{
    uint32_t ticks;
    int32_t time;
    int32_t score;
    std::default_random_engine random_engine;
};
