#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "board.h"
#include "move.h"
#include "valid.h"

using namespace std;
int Size;
pair<int,int> tmp;
vector<vector<Stone>> board;

void init_board(int size, int &BOARD_SIZE, int &CELL_SIZE, int &STONE_RADIUS, int &CLICK_RADIUS, int BOARD_LENGTH) {
    board.resize(size, vector<Stone>(size, EMPTY));
    BOARD_SIZE = size;
    CELL_SIZE = BOARD_LENGTH / (size - 1);
    if (size == 19) {
        STONE_RADIUS = 15;
        CLICK_RADIUS = 17;
    } else if (size == 13) {
        STONE_RADIUS = 20;
        CLICK_RADIUS = 22;
    } else if (size == 9) {
        STONE_RADIUS = 27;
        CLICK_RADIUS = 29;
    }
}

int board_size(){
    return board.size();
}

void draw_board(int hoverRow, int hoverCol, bool blackTurn, SDL_Renderer* renderer, SDL_Texture* black_stone, SDL_Texture* white_stone) {
    
    // draw grid
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        int pos = MARGIN + i * CELL_SIZE;
        SDL_RenderDrawLine(renderer, MARGIN, pos, MARGIN + (BOARD_SIZE - 1) * CELL_SIZE, pos);
        SDL_RenderDrawLine(renderer, pos, MARGIN, pos, MARGIN + (BOARD_SIZE - 1) * CELL_SIZE);
    }

    // draw stones
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board[r][c] != EMPTY) {
                int cx = MARGIN + r * CELL_SIZE;
                int cy = MARGIN + c * CELL_SIZE;

                SDL_Rect rect = {cx - STONE_RADIUS, cy - STONE_RADIUS, STONE_RADIUS * 2, STONE_RADIUS * 2};
                SDL_Texture* stone_texture = (board[r][c] == BLACK) ? black_stone : white_stone;
                SDL_RenderCopy(renderer, stone_texture, NULL, &rect);
            }
        }
    }

    if (hoverRow != -1 && valid(hoverRow, hoverCol, blackTurn)){
        int cx = MARGIN + hoverRow * CELL_SIZE;
        int cy = MARGIN + hoverCol * CELL_SIZE;
        SDL_Rect rect = {cx - STONE_RADIUS, cy - STONE_RADIUS, STONE_RADIUS * 2, STONE_RADIUS * 2};
        SDL_Texture* stone_texture = blackTurn ? black_stone : white_stone;
        SDL_SetTextureAlphaMod(stone_texture, 128);
        SDL_RenderCopy(renderer, stone_texture, NULL, &rect);
        SDL_SetTextureAlphaMod(stone_texture, 255);
    }
}

void board_handle_event(SDL_Event& e, int& hoverRow, int& hoverCol) {
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
            hoverRow = -1;
            hoverCol = -1;
        }
    }
}