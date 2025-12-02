#include "loadgame_interface.h"

int start_index = 0;
vector<string> list_file_shown;

void init_loadgame_interface(){
    start_index = 0;
    list_file_shown.clear();
}

void draw_loadgame_interface(SDL_Renderer* renderer, vector<string> &save_files, vector<Button*>& loadgame_button_list, Button &back_button, TTF_Font* font, SDL_Color color){
    for(int i = 0 ; i < SIZE_LIST_SHOW; i++){ 
        if(start_index + i >= save_files.size()) break;
        loadgame_button_list[i]->draw_button(renderer);
        int w, h;
        SDL_Texture* text = renderText(renderer, font, save_files[i + start_index], color);
        SDL_QueryTexture(text, NULL, NULL, &w, &h);
        SDL_Rect rect = {loadgame_button_list[i]->position.x + (loadgame_button_list[i]->position.h  - h) / 2,
                         loadgame_button_list[i]->position.y + (loadgame_button_list[i]->position.h - h) / 2,
                         w, h};
        SDL_RenderCopy(renderer, text, NULL, &rect);
        SDL_DestroyTexture(text);
    }
    back_button.draw_button(renderer);
}

void load_handle_event(SDL_Renderer* renderer, SDL_Event &e, vector<string> &save_files, vector<Button*>& loadgame_button_list){
    if(e.type == SDL_MOUSEWHEEL){
        if(e.wheel.y > 0 && start_index > 0){
            start_index--;
        }
        if(e.wheel.y < 0 && start_index < (int)save_files.size() - 1){
            start_index++;
        }
    }
}