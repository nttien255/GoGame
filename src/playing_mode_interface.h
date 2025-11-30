#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "button.h"

using namespace std;

void draw_board_mode_interface(SDL_Renderer* renderer, std::vector<Button*> button_list);
void draw_playing_mode_interface(SDL_Renderer* renderer, vector<Button*> button_list);