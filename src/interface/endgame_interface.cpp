#include <SDL2/SDL.h>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include "endgame_interface.h"
#include "text_utlish.h"
#include "player.h"
#include "game.h"
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
    
    SDL_Texture* player1_score = nullptr;
    SDL_Texture* player2_score = nullptr;
    string score_text1 = "Player 1 Score: " + Score_Player(player1);
    string score_text2 = "Player 2 Score: " + Score_Player(player2);
    player1_score = renderText(renderer, font, score_text1, color);
    player2_score = renderText(renderer, font, score_text2, color);
    int w1, h1;
    int w2, h2;
    SDL_QueryTexture(player1_score, NULL, NULL, &w1, &h1);
    SDL_QueryTexture(player2_score, NULL, NULL, &w2, &h2);
    SDL_Rect dst1 = {15, 70, w1, h1};
    SDL_Rect dst2 = {WINDOW_SIZE - w2 - 15, 70, w2, h2};
    SDL_RenderCopy(renderer, player1_score, NULL, &dst1);
    SDL_RenderCopy(renderer, player2_score, NULL, &dst2);    
    
    SDL_DestroyTexture(player1_score);
    SDL_DestroyTexture(player2_score);
    SDL_DestroyTexture(cup_texture);
}