#pragma once
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "text_utlish.h"
#include "button.h"

void draw_confirm_interface(SDL_Renderer* renderer, std::vector<Button*>& button_list, TTF_Font* font, SDL_Color color, const std::string& message);
void draw_options_interface(SDL_Renderer* renderer, std::vector<Button*>& button_list, int music_volume, int sound_effect_volume, TTF_Font* font, SDL_Color color, SDL_Texture* current_background);
void draw_menu_interface(SDL_Renderer* renderer, std::vector<Button*>& button_list);