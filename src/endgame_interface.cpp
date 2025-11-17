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
    double player1_score = player1.Score + player1.kill;
    double player2_score = player2.Score + player2.kill;
    string tmp = (player1_score > player2_score ? "Player 1" : "Player 2");
    string winner_player = "WINNER: " + tmp;
    SDL_Texture* winner_text = renderText(renderer, font, winner_player, color);
    int w, h;
    SDL_QueryTexture(winner_text, NULL, NULL, &w, &h);
    SDL_Rect dst = { (WINDOW_SIZE - w) / 2, (WINDOW_SIZE - h) / 2, w, h };
    SDL_RenderCopy(renderer, winner_text, NULL, &dst);
    SDL_DestroyTexture(winner_text);

    for (auto& button : button_list) {
        button->draw_button(renderer);
    }
}