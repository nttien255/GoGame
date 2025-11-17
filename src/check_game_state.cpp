#include <iostream>
#include "check_game_state.h"
#include "skip.h"
#include "board.h"
#include "loadgame_interface.h"
#include "WINDOW_PLAYING.h"

bool check_two_skips() {
    return cnt_skips_turn >= 2;
}

bool check_full_board() {
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board[r][c] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}
bool check_end_game() {
    if(check_two_skips()) return true;
    if(check_full_board()) return true;
    return false;
}


GameState check_game_state() {
    if (check_end_game()) 
        return GameState::END_GAME;
    return GameState::PLAYING;
}