#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "board.h"
#include "move.h"
#include "valid.h"
#include "kill_enemy.h"
#include "scoring.h"
#include <cassert>


bool make_move(SDL_Event& e, std::vector<std::vector<Stone>>& board, bool& blackTurn) {
    // if (e.type != SDL_MOUSEBUTTONDOWN || e.button.button != SDL_BUTTON_LEFT) return;

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
    if (bestRow != -1 && valid(bestRow, bestCol, blackTurn)) { // thêm điều kiện valid(bestRow, bestCol, blackTurn)
        board[bestRow][bestCol] = blackTurn ? BLACK : WHITE;
        blackTurn = !blackTurn;
        kill_enemy_stones(bestRow, bestCol, blackTurn);
        Run_Score();
        return 1;
    }
    return 0;
}
