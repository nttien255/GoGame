#pragma once
#include <SDL.h>

enum class GameState {
    HOME,
    MENU,
    PLAYING,
    SETTINGS_PLAYING,
    LOAD_GAME,
    END_GAME,
    NONE
};
int RUN_PLAYING(SDL_Window* window, SDL_Renderer* renderer);