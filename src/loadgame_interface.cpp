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
        Button* button = loadgame_button_list[i];
        button->draw_button(renderer);
        int w, h;
        SDL_Texture* text = renderText(renderer, font, button->button_label, color);
        SDL_QueryTexture(text, NULL, NULL, &w, &h);
        SDL_Rect rect = {button->position.x + (button->position.h  - h) / 2,
                         button->position.y + (button->position.h - h) / 2,
                         w, h};
        SDL_RenderCopy(renderer, text, NULL, &rect);
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

    
    for(int i = 0; i< SIZE_LIST_SHOW; i++){
        if(start_index + i >= save_files.size()) break;
        loadgame_button_list[i] = new Button(WINDOW_SIZE / 2 - BUTTON_WIDTH / 2,
                                             (WINDOW_SIZE - LIST_LOAD_LENGTH) / 2 + i * BUTTON_HEIGHT * 3 / 2,
                                             BUTTON_WIDTH, 
                                             BUTTON_HEIGHT,
                                             "../assets/bar.png",
                                             renderer,
                                             save_files[start_index + i]);
    }
}