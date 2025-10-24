#pragma once
#include <SDL.h>
#include <vector>
#include "info_board.h"


void make_move(SDL_Event& e, std::vector<std::vector<Stone>>& board, bool& blackTurn);
