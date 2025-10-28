#pragma once
#include <SDL.h>

enum class GameState {
    MENU,
    PLAYING,
    END_GAME
};
int RUN_PLAYING(SDL_Window* window, SDL_Renderer* renderer);