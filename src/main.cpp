#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "board.h"
#include "move.h"

using namespace std;

const int BOARD_SIZE = 5;
const int CELL_SIZE = 30;
const int MARGIN = 40;
const int WINDOW_SIZE = MARGIN * 2 + CELL_SIZE * (BOARD_SIZE - 1);
const int STONE_RADIUS = 10;
const int CLICK_RADIUS = 12; 
int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return -1;

    SDL_Window* window = SDL_CreateWindow(
        "Go Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_SIZE, WINDOW_SIZE,
        SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    init_board(BOARD_SIZE);
    bool running = true;
    bool blackTurn = true;
    SDL_Event e;

    while (running) {
       while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            make_move(e, board, blackTurn);
        }
        // draw board background
        draw_board(renderer);
       
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}