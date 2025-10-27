#include <iostream>
#include "history.h"
using namespace std;

deque<DataRecord> history;
int PosStatus;

void Init_History(){
    history.clear();
    history.push_back({board, player1, player2, true});
    PosStatus = 0;
}




void Undo_Move(){
    if(history.size() <= 1) return;
    PosStatus -=2;
    board = history[PosStatus].boardStatus;
    player1 = history[PosStatus].player1;
    player2 = history[PosStatus].player2;
}

void  Redo_Move(){
    if(PosStatus + 2 >= history.size()) return;
    PosStatus += 2;
    board = history[PosStatus].boardStatus;
    player1 = history[PosStatus].player1;
    player2 = history[PosStatus].player2;
}
void Pop_History(){
    while(history.size() > PosStatus + 1){
        history.pop_back();
    }
}

void Push_History(){
    PosStatus++;
    history.push_back({board, player1, player2, (PosStatus % 2 == 0)});
}