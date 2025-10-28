#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

struct Button{
    SDL_Rect position;
    SDL_Texture* texture;
    string button_label;
    bool is_hover;

    Button(int x, int y, int w, int h, const string& path, SDL_Renderer* renderer, const string& label){
        position = {x, y, w, h};
        texture = IMG_LoadTexture(renderer, path.c_str());
        is_hovered = false;
        button_label = label;
        if (!texture){
            SDL_Log("Failed to load button texture: %s", SDL_GetError());
        }
    }   

    void handleEvent(SDL_Event &e){
        if (e.type == SDL_MOUSEMOTION){
            int x = e.motion.x;
            int y = e.motion.y;
            is_hover = (x >= position.x && x <= position.x + position.w && 
                        y >= position.y && y <= position.y + position.h);
        }
    }

    bool clicked(SDL_Event& e){
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
            int mx = e.button.x;
            int my = e.button.y;
            if (mx >= position.x && mx <= position.x + position.w &&
                my >= position.y && my <= position.y + position.h){
                return true;
            }
        }
        return false;
    }

    void draw_button(SDL_Renderer* renderer){
        if (is_hover) SDL_SetTextureAlphaMod(texture, 200); // Slightly transparent when hovered
        else SDL_SetTextureAlphaMod(texture, 255); // Fully opaque otherwise
        SDL_RendererCopy(renderer, texture, NULL, &position);
    }

    void destroy(){
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
};