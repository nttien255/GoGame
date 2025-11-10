#include "loadgame_interface.h"

int start_index = 0;
bool is_playing = false;
vector<string> list_file_shown;

void init_loadgame_interface(){
    start_index = 0;
    is_playing = false;
    list_file_shown.clear();
}

void draw_loadgame_interface(SDL_Renderer* renderer){
    for(int i=0; i< list_file_shown.size(); i++){
        Button button = Button(WINDOW_SIZE / 2 - BUTTON_WIDTH / 2,
                              (WINDOW_SIZE - LIST_LOAD_LENGTH) / 2 + i * BUTTON_HEIGHT * 3 / 2,
                               BUTTON_WIDTH, 
                               BUTTON_HEIGHT,
                               "../assets/bar.png",
                               renderer,
                               "bar" + to_string(i));
        
        TTF_Font* font = TTF_OpenFont("../assets/BAUHS93.ttf", 24);
        SDL_Color color = {0, 0, 0};
        SDL_Texture* text = renderText(renderer, font, list_file_shown[i], color);        
        int w, h;
        SDL_QueryTexture(text, NULL, NULL, &w, &h);
        SDL_Rect rect = {button.position.x + (button.position.h  - h) / 2,
                         button.position.y + (button.position.h - h) / 2,
                         w, h};
        button.draw_button(renderer);
        SDL_RenderCopy(renderer, text, NULL, &rect);
    }    
}

void load_handle_event(SDL_Event &e, vector<string> &save_files, bool blackTurn){
    SDL_Point mousePos = {-1, -1};

    if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
        mousePos = {e.button.x, e.button.y};
    }

    if(e.type == SDL_MOUSEWHEEL){
        if(e.wheel.y > 0 && start_index > 0){
            start_index--;
        }
        if(e.wheel.y < 0 && start_index < (int)save_files.size() - 1){
            start_index++;
        }
    }

    list_file_shown.clear();
    for(int i = 0; i< SIZE_LIST_SHOW; i++){
        if(start_index + i >= save_files.size()) break;
        list_file_shown.push_back(save_files[start_index + i]);
    }

    if(mousePos.x == -1 && mousePos.y == -1) return;
    
    for(int i = 0; i< list_file_shown.size(); i++){
        SDL_Rect buttonRect = {WINDOW_SIZE / 2 - BUTTON_WIDTH / 2,
                              (WINDOW_SIZE - LIST_LOAD_LENGTH) / 2 + i * BUTTON_HEIGHT * 3 / 2,
                               BUTTON_WIDTH, 
                               BUTTON_HEIGHT};
        if(SDL_PointInRect(&mousePos, &buttonRect) == SDL_TRUE){
            is_playing = true;
            LoadGame(blackTurn, list_file_shown[i]);           
            break;
        }
    }
}