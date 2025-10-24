#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "board.h"
#include "move.h"

const int BOARD_SIZE = 19;
const int CELL_SIZE = 30;
const int MARGIN = 40;
const int WINDOW_SIZE = MARGIN * 2 + CELL_SIZE * (BOARD_SIZE - 1);
const int STONE_RADIUS = 10;
const int CLICK_RADIUS = 12; 

void make_move(SDL_Event& e, std::vector<std::vector<Stone>>& board, bool& blackTurn) {
    if (e.type != SDL_MOUSEBUTTONDOWN || e.button.button != SDL_BUTTON_LEFT)
        return;

    int x = e.button.x;
    int y = e.button.y;

    int bestRow = -1, bestCol = -1;
    double minDist = CLICK_RADIUS;

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            int cx = MARGIN + c * CELL_SIZE;
            int cy = MARGIN + r * CELL_SIZE;
            double dist = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
            if (dist < minDist) {
                minDist = dist;
                bestRow = r;
                bestCol = c;
            }
        }
    }

    if (bestRow != -1 && board[bestRow][bestCol] == EMPTY) { // thêm điều kiện valid(bestRow, bestCol)
        board[bestRow][bestCol] = blackTurn ? BLACK : WHITE;
        blackTurn = !blackTurn;
        // thêm hàm kill_enemy_stones(bestRow, bestCol);
    }
}
