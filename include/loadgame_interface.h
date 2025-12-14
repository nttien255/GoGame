#pragma once
#include <iostream>
#include <string>
#include "save_load_game.h"
#include "game.h"
#include "button.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "text_utlish.h"

const int BUTTON_WIDTH = 528;
const int BUTTON_HEIGHT = 128;
const int LIST_LOAD_LENGTH = 704;
const int SIZE_LIST_SHOW = 4;

using namespace std;


extern int start_index;

void init_loadgame_interface();
void draw_loadgame_interface(SDL_Renderer* renderer, vector<string> &save_files, vector<Button*>& loadgame_button_list, Button &back_button, TTF_Font* font, SDL_Color color);
void load_handle_event(SDL_Renderer* renderer, SDL_Event &e, vector<string> &save_files, vector<Button*>& loadgame_button_list);