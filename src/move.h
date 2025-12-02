#pragma once
#include <SDL.h>
#include <vector>
#include "board.h"
#include "WINDOW_PLAYING.h"

bool make_move(SDL_Event& e, std::vector<std::vector<Stone>>& board, bool& blackTurn, ShortSound place_stone_sound);

