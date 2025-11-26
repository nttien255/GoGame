#include <iostream>
#include <vector>
#include "playing_mode_interface.h"
#include "button.h"
#include "board.h"

using namespace std;

void draw_playing_mode_interface(SDL_Renderer* renderer, vector<Button*> button_list){
    for (const auto &btn : button_list)
        btn->draw_button(renderer);
}

