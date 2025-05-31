#pragma once

#include <SDL3/SDL.h>

static constexpr int WINDOW_WIDTH = 400;
static constexpr int WINDOW_HEIGHT = 240;

struct SdlHandles
{
    SDL_Window* window;
    SDL_Renderer* renderer;
};

struct Game
{
    uint32_t ticks;
};
