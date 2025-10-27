#pragma once
#include <vector>
#include "board.h"
#include "player.h"
#include <deque>
using namespace std;

struct DataRecord {
    vector<vector<Stone>> boardStatus;
    Player player1,player2;
    bool blackTurn;
};

extern deque<DataRecord> history;
extern int PosStatus;

void Init_History();
void Undo_Move();
void Redo_Move();
void Pop_History();
void Push_History();
