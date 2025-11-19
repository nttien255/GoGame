#include <SDL2/SDL.h>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include "endgame_interface.h"
#include "text_utlish.h"
#include "player.h"
#include "WINDOW_PLAYING.h"
#include "scoring.h"
#include "board.h"

using namespace std;

void draw_endgame_interface(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, vector<Button*> &button_list) {
    SDL_Texture* cup_texture = IMG_LoadTexture(renderer, "../assets/honour.png");
    if (!cup_texture) {
        SDL_Log("Failed to load honour texture");
        return;
    }   
    int cup_size = 500;
    SDL_Rect cup_rect = {WINDOW_SIZE / 2 - cup_size / 2, WINDOW_SIZE / 2 - cup_size / 2, cup_size, cup_size};
    SDL_RenderCopy(renderer, cup_texture, NULL, &cup_rect);
    SDL_DestroyTexture(cup_texture);
    
    if (player1.Score + player1.kill > player2.Score + player2.kill) {
        SDL_Texture* winner_text = renderText(renderer, font, "Player 1", color);
        int w, h;
        SDL_QueryTexture(winner_text, NULL, NULL, &w, &h);
        w += 20;
        SDL_Rect text_rect = {WINDOW_SIZE / 2 - w / 2, WINDOW_SIZE / 2 - cup_size / 4 + 260, w, h};
        SDL_RenderCopy(renderer, winner_text, NULL, &text_rect);
        SDL_DestroyTexture(winner_text);
    } else if (player1.Score + player1.kill < player2.Score + player2.kill) {
        SDL_Texture* winner_text = renderText(renderer, font, "Player 2", color);
        int w, h;
        SDL_QueryTexture(winner_text, NULL, NULL, &w, &h);
        w += 20;
        SDL_Rect text_rect = {WINDOW_SIZE / 2 - w / 2, WINDOW_SIZE / 2 - cup_size / 4 + 260, w, h};
        SDL_RenderCopy(renderer, winner_text, NULL, &text_rect);
        SDL_DestroyTexture(winner_text);
    } else {
        SDL_Log("It's a tie!");
    }
    for (auto& button : button_list) {
        button->draw_button(renderer);
    }
}