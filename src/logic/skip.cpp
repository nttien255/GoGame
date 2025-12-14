#include<iostream>
#include "skip.h"
#include "history.h"
using namespace std;

int cnt_skips_turn = 0;

void init_skip(){
    cnt_skips_turn = 0;
}

void skip_turn(bool& blackTurn, int &who_plays_first){ 
    Pop_History();
    blackTurn = !blackTurn;
    who_plays_first = !(who_plays_first - 1) + 1;
    cnt_skips_turn++;
    Push_History(blackTurn, who_plays_first, -1, -1);  // -1,-1 = pass
}