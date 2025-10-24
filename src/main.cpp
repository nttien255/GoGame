#include <bits/stdc++.h>
#include "info_board.h"
using namespace std;


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
        SDL_SetRenderDrawColor(renderer, 200, 160, 80, 255);
        SDL_RenderClear(renderer);

        // draw grid
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = 0; i < BOARD_SIZE; ++i) {
            int pos = MARGIN + i * CELL_SIZE;
            SDL_RenderDrawLine(renderer, MARGIN, pos, WINDOW_SIZE - MARGIN, pos);
            SDL_RenderDrawLine(renderer, pos, MARGIN, pos, WINDOW_SIZE - MARGIN);
        }

        // draw stones
        for (int r = 0; r < BOARD_SIZE; ++r) {
            for (int c = 0; c < BOARD_SIZE; ++c) {
                if (board[r][c] != EMPTY) {
                    int cx = MARGIN + c * CELL_SIZE;
                    int cy = MARGIN + r * CELL_SIZE;

                    if (board[r][c] == BLACK)
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    else
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                    for (int dy = -STONE_RADIUS; dy <= STONE_RADIUS; ++dy) {
                        for (int dx = -STONE_RADIUS; dx <= STONE_RADIUS; ++dx) {
                            if (dx * dx + dy * dy <= STONE_RADIUS * STONE_RADIUS)
                                SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
                        }
                    }
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}