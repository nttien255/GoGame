#pragma one
#ifndef INFO_BOARD_H
#define INFO_BOARD_H

#include <vector>
#include <iostream>
#include <SDL2/SDL.h>

enum Stone { EMPTY, BLACK, WHITE };
extern std::vector<std::vector<Stone>> board;

void init_board(int size);
int board_size();
void draw_board(SDL_Renderer* renderer);

#endif