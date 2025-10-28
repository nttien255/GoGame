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

void init_board(int size){
    board.resize(size, vector<Stone>(size, EMPTY));
}

int board_size(){
    return board.size();
}

void draw_board(int hoverRow, int hoverCol, bool blackTurn, SDL_Renderer* renderer, SDL_Texture* black_stone, SDL_Texture* white_stone) {
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
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
}