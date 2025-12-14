#include "easy_mode.h"
#include "board.h"
#include "valid.h"
#include "kill_enemy.h"
#include "scoring.h"
#include "history.h"
#include "skip.h"
#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;





void easy_mode_move(bool &blackTurn, int &who_plays_first){
    vector<pair<int,int>> possible_moves;
    for(int i=0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if(valid(i,j,blackTurn)){
                possible_moves.push_back({i,j});
            }
        }
    }
    if(possible_moves.size() == 0){
        skip_turn(blackTurn, who_plays_first);
        return;
    }
    int idx = rand() % possible_moves.size();
    int bestRow = possible_moves[idx].first;   
    int bestCol = possible_moves[idx].second;
    
    cnt_skips_turn = 0;
    Pop_History();
    
    board[bestRow][bestCol] = blackTurn ? BLACK : WHITE;
    blackTurn = !blackTurn;
    who_plays_first = !(who_plays_first - 1) + 1;
    
    kill_enemy_stones(bestRow, bestCol, blackTurn, 1);
    Run_Score();
    Push_History(blackTurn, who_plays_first);
}