#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

static constexpr int WINDOW_WIDTH = 400;
static constexpr int WINDOW_HEIGHT = 240;

struct SdlHandles
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_TextEngine* text_engine;
};

struct Game
{
    uint32_t ticks;
    uint32_t time;
    uint32_t score;
};
