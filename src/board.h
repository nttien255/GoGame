#pragma once

#include <vector>
#include <iostream>
#include <SDL2/SDL.h>

extern int BOARD_SIZE;
extern int CELL_SIZE;
extern int STONE_RADIUS;
extern int CLICK_RADIUS;
extern int MARGIN;
extern const int WINDOW_SIZE;

enum Stone { EMPTY, BLACK, WHITE };
extern std::vector<std::vector<Stone>> board;

void init_board(int size, int &BOARD_SIZE, int &CELL_SIZE, int &STONE_RADIUS, int &CLICK_RADIUS, int &MARGIN, int BOARD_LENGTH);
int board_size();
void draw_board(int x, int y, bool blackTurn, SDL_Renderer* renderer, SDL_Texture* black_stone, SDL_Texture* white_stone);
void board_handle_event(SDL_Event& e, int& hoverRow, int& hoverCol);