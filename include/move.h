#pragma once
#include <SDL.h>
#include <vector>
#include "board.h"
#include "game.h"
#include "hard_mode.h"

bool make_move(SDL_Event& e, std::vector<std::vector<Stone>>& board, bool& blackTurn, int &who_plays_first, KataGoAI &katago, AIState &ai_state, ShortSound place_stone_sound);

