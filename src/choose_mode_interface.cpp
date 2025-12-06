#include "choose_mode_interface.h"

void draw_choose_mode_interface(SDL_Renderer* renderer, vector<Button*>& choose_mode_button_list, Button &back_button){
    for(auto btn: choose_mode_button_list)
        btn->draw_button(renderer);
}