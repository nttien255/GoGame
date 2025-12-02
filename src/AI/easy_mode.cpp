#include "easy_mode.h"





void easy_mode_move(bool &blackTurn){
    vector<pair<int,int>> possible_moves;
    for(int i=0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if(valid(i,j,blackTurn)){
                possible_moves.push_back({i,j});
            }
        }
    }
    if(possible_moves.size() == 0){
        skip_turn(blackTurn);
        return;
    }
    int idx = rand() % possible_moves.size();
    int bestRow = possible_moves[idx].first;   
    int bestCol = possible_moves[idx].second;
    
    cnt_skips_turn = 0;
    Pop_History();
    
    board[bestRow][bestCol] = blackTurn ? BLACK : WHITE;
    blackTurn = !blackTurn;

    kill_enemy_stones(bestRow, bestCol, blackTurn, 1);
    Run_Score();
    Push_History();
}