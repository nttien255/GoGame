#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "board.h"
#include "move.h"
#include "history.h"
#include "button.h"
#include "WINDOW_PLAYING.h"
#include "playing_interface.h"
#include "skip.h"
#include "check_game_state.h"

using namespace std;

int BOARD_SIZE = 13;
int CELL_SIZE = 63;
int STONE_RADIUS = 20;
int CLICK_RADIUS = 22; 
const int BOARD_LENGTH = 72 * 7; // 72 = lcm(8, 18)
const int MARGIN = 148;
const int WINDOW_SIZE = 800;



int RUN_PLAYING(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_SetWindowSize(window, WINDOW_SIZE, WINDOW_SIZE);
    SDL_Texture* black_stone = IMG_LoadTexture(renderer, "../assets/black.png");
    SDL_Texture* white_stone = IMG_LoadTexture(renderer, "../assets/white.png");
    TTF_Font* font = TTF_OpenFont("../assets/BAUHS93.ttf", 24);
    SDL_Color color = {0, 0, 0};

    if (!black_stone) {
        SDL_Log("Failed to load black stone images");
        return -1;
    }
    if (!white_stone) {
        SDL_Log("Failed to load white stone images");
        return -1;
    }

    Button undo_button(10, WINDOW_SIZE - 60, 60, 40, "../assets/undo.png", renderer, "Undo");
    Button redo_button(80, WINDOW_SIZE - 60, 60, 40, "../assets/redo.png", renderer, "Redo");
    Button pass_button(WINDOW_SIZE / 2 - 40, WINDOW_SIZE - 65, 120, 42, "../assets/pass.png", renderer, "Pass");
    SDL_Texture* player1_score = nullptr;
    SDL_Texture* player2_score = nullptr;

    init_board(BOARD_SIZE, BOARD_SIZE, CELL_SIZE, STONE_RADIUS, CLICK_RADIUS, BOARD_LENGTH);
    Init_History();
    Init_Player();
    bool running = true;
    bool blackTurn = true;
    int hoverRow = -1, hoverCol = -1;
    GameState current_state = GameState::PLAYING;
    vector<Button*> button_list = {
        &undo_button, 
        &redo_button, 
        &pass_button
    };

    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT){
                running = false;
                return 0;
            }
            
            if (current_state == GameState::PLAYING) {
                board_handle_event(e, hoverRow, hoverCol);
                undo_button.handleEvent(e);
                redo_button.handleEvent(e);
                pass_button.handleEvent(e);

                if (undo_button.clicked(e)) {
                    Undo_Move();
                }
                if (redo_button.clicked(e)) {
                    Redo_Move();
                }
                if (pass_button.clicked(e)) {
                    skip_turn(blackTurn);
                }

                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    if (hoverRow != -1 && board[hoverRow][hoverCol] == EMPTY){
                        make_move(e, board, blackTurn);
                    }
                }
            }
        }
        // draw board background
        // current_state = check_game_state();

        current_state = check_game_state();
        if (current_state == GameState::PLAYING){
            SDL_SetRenderDrawColor(renderer, 200, 160, 80, 255);
            SDL_RenderClear(renderer);

            draw_interface(renderer, black_stone, white_stone, hoverRow, hoverCol, blackTurn, button_list, player1_score, player2_score, font, color);

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }

        if (current_state == GameState::END_GAME) {
            // Handle end game state (not implemented yet)
            // ???
        }
    }

    return 0;
}