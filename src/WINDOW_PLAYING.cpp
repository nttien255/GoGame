#include <vector>
#include <iostream>
#include <stack>
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
#include "home_interface.h"


using namespace std;

int BOARD_SIZE = 19;
int CELL_SIZE = 63;
int STONE_RADIUS = 20;
int CLICK_RADIUS = 22; 

int MARGIN = 123;
const int BOARD_LENGTH = 72 * 7; // 72 = lcm(8, 18)
const int WINDOW_SIZE = 750;

void init_game(bool &blackTurn) {
    init_board(BOARD_SIZE, BOARD_SIZE, CELL_SIZE, STONE_RADIUS, CLICK_RADIUS, MARGIN, BOARD_LENGTH);
    Init_History();
    Init_Player();
    init_skip();
    blackTurn = true;
}

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
    Button start_button(WINDOW_SIZE / 2 - 250 / 2, 187, 250, 75, "../assets/start.png", renderer, "Start Game");
    Button loadgame_button(WINDOW_SIZE / 2 - 250 / 2, 287, 250, 75, "../assets/loadgame.png", renderer, "Load Game");
    Button options_button(WINDOW_SIZE / 2 - 250 / 2, 387, 250, 75, "../assets/options.png", renderer, "Options");
    Button exit_button(WINDOW_SIZE / 2 - 250 / 2, 487, 250, 75, "../assets/exit.png", renderer, "Exit");
    Button back_button(10, 10, 60, 60, "../assets/back.png", renderer, "Back");
    Button home_button(5, 5, 120, 60, "../assets/home.png", renderer, "Exit");
    SDL_Texture* player1_score = nullptr;
    SDL_Texture* player2_score = nullptr;

    init_board(BOARD_SIZE, BOARD_SIZE, CELL_SIZE, STONE_RADIUS, CLICK_RADIUS, MARGIN, BOARD_LENGTH);
    bool running = true;
    bool blackTurn = true;
    int hoverRow = -1, hoverCol = -1;

    Init_Player();
    Init_History();

    GameState current_state = GameState::HOME;
    GameState next_state = current_state;
    
    init_game(blackTurn);
    stack<GameState> stackState;


    vector<Button*> playing_button_list = {
        &undo_button,
        &redo_button, 
        &pass_button,
        &menu_button
    };
    
    vector<Button*> endgame_button_list = {
        &home_button
    };

    vector<Button*> loadgame_button_list(SIZE_LIST_SHOW);

    vector<Button*> home_button_list{
        &start_button,
        &loadgame_button,
        &options_button,
        &exit_button
    };

    SDL_Event e;
    SDL_Surface* loadedSurface = IMG_Load("../assets/background.png");
    if (loadedSurface == nullptr) {
        std::cerr << "Lỗi khi load ảnh: " << IMG_GetError() << std::endl;
        // Có thể return hoặc xử lý lỗi khác ở đây
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT){
                running = false;
                return 0;
            }
            
            if (current_state == GameState::HOME) {
                for (auto btn : home_button_list) {
                    btn->handleEvent(e);
                }

                if (start_button.clicked(e)) {
                    // stackState.push(current_state);
                    current_state = GameState::PLAYING;
                    init_game(blackTurn);
                    break;
                }
                if (loadgame_button.clicked(e)) {
                    stackState.push(current_state);
                    current_state = GameState::LOAD_GAME;
                    init_loadgame_interface();
                    break;
                }

                if (exit_button.clicked(e)) {
                    running = false;
                    current_state = GameState::NONE;
                    break;
                }
            }
            
            if (current_state == GameState::PLAYING) {
                board_handle_event(e, hoverRow, hoverCol);
                for (auto btn : playing_button_list) {
                    btn->handleEvent(e);
                }

                if (undo_button.clicked(e)) {
                    Undo_Move(blackTurn);
                    // SaveGame(blackTurn);
                }
                if (redo_button.clicked(e)) {
                    Redo_Move(blackTurn);
                }
                if (pass_button.clicked(e)) {
                    skip_turn(blackTurn);
                }
                if (menu_button.clicked(e)) {
                    stackState.push(current_state);
                    current_state = GameState::LOAD_GAME;
                    break;
                }

                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    if (hoverRow != -1 && board[hoverRow][hoverCol] == EMPTY){
                        make_move(e, board, blackTurn);
                    }
                }
            }

            if(current_state == GameState::LOAD_GAME){
                vector<string> save_files = allFileLoadGame();
                load_handle_event(renderer, e, save_files, loadgame_button_list);
                for(auto btn : loadgame_button_list){
                    btn->handleEvent(e);
                }
                back_button.handleEvent(e);



                for(int i = 0; i < SIZE_LIST_SHOW; i++){
                    Button* btn = loadgame_button_list[i];
                    if(btn->clicked(e)){
                        LoadGame(blackTurn, btn->button_label);
                        current_state = GameState::PLAYING;
                        break;
                    }
                }
                if(back_button.clicked(e)){
                    current_state = stackState.top();
                    stackState.pop();
                }                
            }

            if (current_state == GameState::END_GAME) {
                for (auto btn : endgame_button_list) {
                    btn->handleEvent(e);
                }
                if (menu_button.clicked(e)) {
                    current_state = GameState::HOME;
                }
            }
        }
        // draw board background
        // cu
        // rrent_state = check_game_state();
        // cout << (int)current_state << endl;
        if (current_state == GameState::HOME) {
            SDL_Rect destRect;
            destRect.x = 0;
            destRect.y = 0;
            destRect.w = WINDOW_SIZE;  // Chiều rộng đích = Chiều rộng cửa sổ
            destRect.h = WINDOW_SIZE; // Chiều cao đích = Chiều cao cửa sổ
            SDL_RenderCopy(renderer, backgroundTexture, NULL, &destRect);
            SDL_SetRenderDrawColor(renderer, 200, 160, 80, 200);
            // SDL_RenderClear(renderer);
            SDL_RenderFillRect(renderer, &destRect);


            draw_home_interface(renderer, home_button_list);

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }

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

            vector<string> save_files = allFileLoadGame();
            load_handle_event(renderer, e, save_files, loadgame_button_list);
            draw_loadgame_interface(renderer, save_files, loadgame_button_list, back_button, font, color);

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
            
            // SDL_SetRenderDrawColor(renderer, 200, 160, 80, 255);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }

        next_state = check_game_state();

        if(current_state == GameState::PLAYING){
            if(next_state == GameState::LOAD_GAME){
                init_loadgame_interface();
            }
            current_state = next_state;
        }
    }

    return 0;
}