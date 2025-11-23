#include "home_interface.h"


void draw_home_interface(SDL_Renderer* renderer, vector<Button*>& button_list) {
    for (const auto& btn : button_list) {
        btn->draw_button(renderer);
    }
}