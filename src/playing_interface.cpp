#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "player.h"
#include "board.h"
#include "button.h"
#include "playing_interface.h"
#include "scoring.h"
#include "text_utlish.h"

using namespace std;

void draw_interface(SDL_Renderer* renderer, SDL_Texture* black_stone, SDL_Texture* white_stone, 
    int hoverRow, int hoverCol, bool blackTurn, 
    vector<Button*> button_list, 
    SDL_Texture* player1_score, SDL_Texture* player2_score, TTF_Font* font, SDL_Color color) {
    draw_board(hoverRow, hoverCol, blackTurn, renderer, black_stone, white_stone);
    for (auto button : button_list) {
        button->draw_button(renderer);
    }
    // Draw player scores
    string score_text1 = "Player 1 Score: " + to_string(Score_Player(player1));
    string score_text2 = "Player 2 Score: " + to_string(Score_Player(player2));
    SDL_DestroyTexture(player1_score);
    SDL_DestroyTexture(player2_score);
    player1_score = renderText(renderer, font, score_text1, color);
    player2_score = renderText(renderer, font, score_text2, color);
    int w1, h1;
    int w2, h2;
    SDL_QueryTexture(player1_score, NULL, NULL, &w1, &h1);
    SDL_QueryTexture(player2_score, NULL, NULL, &w2, &h2);
    SDL_Rect dst1 = {10, 10, w1, h1};
    SDL_Rect dst2 = {WINDOW_SIZE - w2 - 10, 10, w2, h2};
    SDL_RenderCopy(renderer, player1_score, NULL, &dst1);
    SDL_RenderCopy(renderer, player2_score, NULL, &dst2);
}