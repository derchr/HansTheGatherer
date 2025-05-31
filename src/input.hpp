#pragma once

#include <SDL3/SDL.h>
#include <set>

struct ButtonInput
{
    std::set<SDL_Keycode> pressed;
    std::set<SDL_Keycode> just_pressed;
    std::set<SDL_Keycode> just_released;
};
