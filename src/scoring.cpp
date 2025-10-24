#pragma once
#include "scoring.h"
#include "player.h"
#include "board.h"
#include <bits/stdc++.h>
using namespace std;


void calc_place_score(){
    int size = board.size();
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(board[i][j] == 0){
                BFS(i,j);
            }
        }
    }
}

void Run(){
    calc_place_score();

}

