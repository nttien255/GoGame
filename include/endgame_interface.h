#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "button.h"
#include "player.h"

void draw_endgame_interface(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, vector<Button*> &button_list, SDL_Texture* black_stone, SDL_Texture* white_stone);