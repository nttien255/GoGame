#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "board.h"
#include "WINDOW_PLAYING.h"

using namespace std;

const int WINDOW_SIZE_MENU = 700;
const int PLAYING_BUTTON_SIZE_WIDTH = 100;
const int PLAYING_BUTTON_SIZE_HIGHT = 20;


enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window* window = SDL_CreateWindow(
        "Go Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_SIZE_MENU, WINDOW_SIZE_MENU,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    GameState currentState = GameState::MENU;
    bool running = true;
    SDL_Event e;

    while (running) {
        // RUN_PLAYING(window, renderer);
        // break;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;

            if (e.type == SDL_MOUSEMOTION) {
                int x = e.motion.x;
                int y = e.motion.y;
                bool playButtonRect = (x >= WINDOW_SIZE / 2 - 100 && x <= WINDOW_SIZE / 2 + 100 &&
                                       y >= WINDOW_SIZE / 2 - 25 && y <= WINDOW_SIZE / 2 + 25);
            }
        }

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}