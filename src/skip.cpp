#include<iostream>
#include "skip.h"
#include "history.h"
using namespace std;

int cnt_skips_turn = 0;

void init_skip(){
    cnt_skips_turn = 0;
}

void skip_turn(bool& blackTurn) {
    Pop_History();
    blackTurn = !blackTurn;
    cnt_skips_turn++;
    Push_History();
}