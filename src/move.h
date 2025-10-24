#ifndef MOVE_H
#define MOVE_H
#pragma once
#include <SDL.h>
#include <vector>
#include "board.h"

extern const int BOARD_SIZE;
extern const int CELL_SIZE;
extern const int MARGIN;
extern const int WINDOW_SIZE;
extern const int STONE_RADIUS;
extern const int CLICK_RADIUS;

void make_move(SDL_Event& e, std::vector<std::vector<Stone>>& board, bool& blackTurn);

#endif