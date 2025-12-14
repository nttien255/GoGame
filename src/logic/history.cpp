#include <iostream>
#include "history.h"
#include "game.h"
using namespace std;

deque<DataRecord> history;
int PosStatus;

void Init_History(bool &blackTurn, int &who_plays_first){
    history.clear();
    history.push_back({board, player1, player2, blackTurn, who_plays_first, -1, -1});
    PosStatus = 0;
}

void Undo_Move(bool &blackturn, int &who_plays_first){
    if(PosStatus < 1) return;
    PosStatus -=1;
    board = history[PosStatus].boardStatus;
    player1 = history[PosStatus].player1;
    player2 = history[PosStatus].player2;
    blackturn = history[PosStatus].blackTurn;
    who_plays_first = history[PosStatus].who_plays_first;
}

void Redo_Move(bool &blackturn, int &who_plays_first){
    if(PosStatus + 1 >= history.size()) return;
    PosStatus += 1;
    board = history[PosStatus].boardStatus;
    player1 = history[PosStatus].player1;
    player2 = history[PosStatus].player2;
    blackturn = history[PosStatus].blackTurn;
    who_plays_first = history[PosStatus].who_plays_first;
}

void Pop_History(){
    while(history.size() > PosStatus + 1){
        history.pop_back();
    }
}

// lastRow, lastCol = -1 means pass/skip
void Push_History(bool &blackTurn, int &who_plays_first, int lastRow, int lastCol){
    PosStatus++;
    history.push_back({board, player1, player2, blackTurn, who_plays_first, lastRow, lastCol});
}