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
#include "save_load_game.h"
#include "loadgame_interface.h"
#include "endgame_interface.h"

using namespace std;

int BOARD_SIZE = 19;
int CELL_SIZE = 63;
int STONE_RADIUS = 20;
int CLICK_RADIUS = 22; 

int MARGIN = 123;
const int BOARD_LENGTH = 72 * 7; // 72 = lcm(8, 18)
const int WINDOW_SIZE = 750;

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
    Button menu_button(5, 5, 120, 40, "../assets/menu.png", renderer, "Menu");

    SDL_Texture* player1_score = nullptr;
    SDL_Texture* player2_score = nullptr;

    init_board(BOARD_SIZE, BOARD_SIZE, CELL_SIZE, STONE_RADIUS, CLICK_RADIUS, MARGIN, BOARD_LENGTH);
    Init_History();
    Init_Player();
    bool running = true;
    bool blackTurn = true;
    int hoverRow = -1, hoverCol = -1;
    GameState current_state = GameState::LOAD_GAME;
    GameState next_state = current_state;
    
    vector<Button*> playing_button_list = {
        &undo_button, 
        &redo_button, 
        &pass_button,
        &menu_button
    };
    
    vector<Button*> endgame_button_list = {
        &menu_button
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
                for (auto btn : playing_button_list) {
                    btn->handleEvent(e);
                }

                if (undo_button.clicked(e)) {
                    Undo_Move();
                    SaveGame(blackTurn);
                }
                if (redo_button.clicked(e)) {
                    Redo_Move();
                }
                if (pass_button.clicked(e)) {
                    skip_turn(blackTurn);
                }
                if (menu_button.clicked(e)) {
                    // Handle menu button click (not implemented yet)
                    current_state = GameState::LOAD_GAME;
                }

                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    if (hoverRow != -1 && board[hoverRow][hoverCol] == EMPTY){
                        make_move(e, board, blackTurn);
                    }
                }
            }

            if(current_state == GameState::LOAD_GAME){
                vector<string> save_files = allFileLoadGame();
                load_handle_event(e, save_files, blackTurn);
            }

            if (current_state == GameState::END_GAME) {
                for (auto btn : endgame_button_list) {
                    btn->handleEvent(e);
                }
                if (menu_button.clicked(e)) {
                    current_state = GameState::LOAD_GAME;
                }
            }
        }
        // draw board background
        // current_state = check_game_state();

        if (current_state == GameState::PLAYING){
            SDL_SetRenderDrawColor(renderer, 200, 160, 80, 255);
            SDL_RenderClear(renderer);

            draw_playing_interface(renderer, black_stone, white_stone, hoverRow, hoverCol, blackTurn, playing_button_list, player1_score, player2_score, font, color);

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
        if (current_state == GameState::LOAD_GAME){
            SDL_SetRenderDrawColor(renderer, 200, 160, 80, 255);
            SDL_RenderClear(renderer);

            draw_loadgame_interface(renderer);

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
            // draw_list_load_game(rendere);
        }
        if (current_state == GameState::END_GAME) {
            // Handle end game state (not implemented yet)
            // draw yellow screen like board background
            SDL_SetRenderDrawColor(renderer, 200, 160, 80, 255);
            SDL_RenderClear(renderer);

            draw_endgame_interface(renderer, font, color, endgame_button_list);

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
        next_state = check_game_state(); // check cái này nha ông

        if(next_state != GameState::NONE && next_state != current_state){
            cout << "STATE IS CHANGED \n ";
            current_state = next_state;
            if(current_state == GameState::LOAD_GAME){
                init_loadgame_interface();
            }
        }
    }

    return 0;
}