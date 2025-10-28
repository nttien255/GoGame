#pragma once
#include <SDL.h>
#include <vector>
#include "board.h"

bool make_move(SDL_Event& e, std::vector<std::vector<Stone>>& board, bool& blackTurn);

