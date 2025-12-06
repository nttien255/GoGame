#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "board.h"
#include "move.h"
#include "valid.h"
#include "kill_enemy.h"
#include "scoring.h"
#include "history.h"
#include "skip.h"
#include "WINDOW_PLAYING.h"
#include "AI/hard_mode.h"

bool make_move(SDL_Event& e, std::vector<std::vector<Stone>>& board, bool& blackTurn, int &who_plays_first, KataGoAI &katago, AIState &ai_state, ShortSound place_stone_sound) {
    int x = e.button.x;
    int y = e.button.y;

    int bestRow = -1, bestCol = -1;
    double minDist = CLICK_RADIUS;

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            int cx = MARGIN + r * CELL_SIZE;
            int cy = MARGIN + c * CELL_SIZE;
            double dist = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
            if (dist < minDist) {
                minDist = dist;
                bestRow = r;
                bestCol = c;
            }
        }
    }

    if (bestRow != -1 && valid(bestRow, bestCol, blackTurn)) { 
        place_stone_sound.play();
        cnt_skips_turn = 0;
        Pop_History();
        
        int moveColor = blackTurn ? 1 : 2;
        board[bestRow][bestCol] = blackTurn ? BLACK : WHITE;
        if(ai_state == AIState::HARD_PLAY)
            katago.playMove(bestRow, bestCol, moveColor);
        blackTurn = !blackTurn;
        who_plays_first = !(who_plays_first - 1) + 1;

        kill_enemy_stones(bestRow, bestCol, blackTurn, 1);
        Run_Score();
        Push_History(blackTurn, who_plays_first, bestRow, bestCol);
        return 1;
    }
    
    return 0;
}
