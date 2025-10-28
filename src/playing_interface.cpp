#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "board.h"
#include "button.h"
#include "playing_interface.h"

using namespace std;

void draw_interface(SDL_Renderer* renderer, SDL_Texture* black_stone, SDL_Texture* white_stone, int hoverRow, int hoverCol, bool blackTurn, vector<Button*> button_list) {
    draw_board(hoverRow, hoverCol, blackTurn, renderer, black_stone, white_stone);
    for (auto button : button_list) {
        button->draw_button(renderer);
    }
}