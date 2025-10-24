#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "board.h"
#include "move.h"

using namespace std;
int Size;

vector<vector<Stone>> board;

void init_board(int size){
    board.resize(size, vector<Stone>(size, EMPTY));
}

int board_size(){
    return board.size();
}

void draw_board(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 200, 160, 80, 255);
    SDL_RenderClear(renderer);
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