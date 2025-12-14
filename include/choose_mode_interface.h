#pragma once
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "button.h"

void draw_choose_mode_interface(SDL_Renderer* renderer, vector<Button*>& choose_mode_button_list, Button &back_button);
