#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "button.h"

void draw_options_interface(SDL_Renderer* renderer, std::vector<Button*>& button_list, int music_volume, int sound_effect_volume){
    for (const auto &btn : button_list){
        btn->draw_button(renderer);
        int offset = 100;
        if (btn->button_label == "Music") {
            for (int i = 1; i <= 5; ++i){
                if (i <= music_volume){
                    // màu đen
                    SDL_Rect filled_rect = {offset + btn->position.x + i * 22, btn->position.y + 15, 20, 10};
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &filled_rect);
                } else {
                    // Màu xám
                    SDL_Rect empty_rect = {offset + btn->position.x + i * 22, btn->position.y + 15, 20, 10};
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                    SDL_RenderFillRect(renderer, &empty_rect);
                }
            }
        }
        if (btn->button_label == "Sound Effect") {
            for (int i = 1; i <= 5; ++i){
                if (i <= sound_effect_volume){
                    // Màu đen
                    SDL_Rect filled_rect = {offset + btn->position.x + i * 22, btn->position.y + 15, 20, 10};
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &filled_rect);
                } else {
                    // Màu xám
                    SDL_Rect empty_rect = {offset + btn->position.x + i * 22, btn->position.y + 15, 20, 10};
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                    SDL_RenderFillRect(renderer, &empty_rect);
                }
            }
        }
    }
}

void draw_menu_interface(SDL_Renderer* renderer, std::vector<Button*>& button_list){
    for (const auto &btn : button_list)
        btn->draw_button(renderer);
}