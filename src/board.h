#pragma once

#include <vector>
#include <iostream>
#include <SDL2/SDL.h>

extern const int BOARD_SIZE;
extern const int CELL_SIZE;
extern const int MARGIN;
extern const int WINDOW_SIZE;
extern const int STONE_RADIUS;
extern const int CLICK_RADIUS;

enum Stone { EMPTY, BLACK, WHITE };
extern std::vector<std::vector<Stone>> board;

void init_board(int size);
int board_size();
void draw_board(int x, int y, bool blackTurn, SDL_Renderer* renderer, SDL_Texture* black_stone, SDL_Texture* white_stone);
