#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "board.h"
#include "move.h"
#include "history.h"
#include "WINDOW_PLAYING.h"
using namespace std;

const int BOARD_SIZE = 5;
const int CELL_SIZE = 30;
const int MARGIN = 40;
const int WINDOW_SIZE = MARGIN * 2 + CELL_SIZE * (BOARD_SIZE - 1);
const int STONE_RADIUS = 10;
const int CLICK_RADIUS = 12; 


int RUN_PLAYING(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_SetWindowSize(window, WINDOW_SIZE, WINDOW_SIZE);
    SDL_Texture* black_stone = IMG_LoadTexture(renderer, "../assets/black.png");
    SDL_Texture* white_stone = IMG_LoadTexture(renderer, "../assets/white.png");
    
    if (!black_stone) {
        SDL_Log("Failed to load black stone images");
        return -1;
    }
    if (!white_stone) {
        SDL_Log("Failed to load white stone images");
        return -1;
    }

    init_board(BOARD_SIZE);
    Init_History();
    bool running = true;
    bool blackTurn = true;
    int hoverRow = -1, hoverCol = -1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT){
                running = false;
                return 0;
            }
            if (e.type == SDL_MOUSEMOTION) {
                int x = e.motion.x;
                int y = e.motion.y;

                int bestRow = -1, bestCol = -1;
                double minDist = CLICK_RADIUS;

                for (int r = 0; r < BOARD_SIZE; ++r) {
                    for (int c = 0; c < BOARD_SIZE; ++c) {
                        int cx = MARGIN + r * CELL_SIZE;
                        int cy = MARGIN + c * CELL_SIZE;
                        double dist = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
                        if (dist < minDist) {
                            minDist = dist;
                            bestRow = r;
                            bestCol = c;
                        }
                    }
                }

                if (bestRow != -1 && board[bestRow][bestCol] == EMPTY) {
                    hoverRow = bestRow;
                    hoverCol = bestCol;
                } else {
                    hoverRow = hoverCol = -1;
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                if (hoverRow != -1 && board[hoverRow][hoverCol] == EMPTY) {
                    if (make_move(e, board, blackTurn)){
                        hoverRow = -1;
                        hoverCol = -1;
                    }
                }
            }
        }
        // draw board background
        draw_board(hoverRow, hoverCol, blackTurn, renderer, black_stone, white_stone);
    }

    return 0;
}