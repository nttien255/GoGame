#pragma once
#include <utility>
#include <vector>
#include "board.h"

using namespace std;


class KataGoAI {
public:
    KataGoAI();
    ~KataGoAI();

    void init(int board_size);
    void cleanup();
    void playMove(int r, int c, int color);
    void undoMove();
    void redoMoves(int count);  // Redo moves using history
    void syncBoard(vector<vector<Stone>>& game_board, bool blackTurn);
    pair<int, int> generateMove(int aicolor);
    
private:
    int size;
};

void hard_mode_move(bool &blackTurn, int &who_plays_first, KataGoAI &katago);
