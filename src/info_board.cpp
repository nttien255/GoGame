#pragma once
#include "info_board.h"
#include<bits/stdc++.h>
using namespace std;
int Size;

vector<vector<Stone>> board;

void init_board(int size){
    board.resize(size, vector<Stone>(size, EMPTY));
}

int board_size(){
    return board.size();
}