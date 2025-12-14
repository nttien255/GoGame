#pragma once
#include <vector>
#include "board.h"
#include "player.h"
#include <deque>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

struct DataRecord {
    vector<vector<Stone>> boardStatus;
    Player player1,player2;
    bool blackTurn;
    int who_plays_first;
    int lastMoveRow;    // -1 if pass/skip
    int lastMoveCol;    // -1 if pass/skip
};

extern deque<DataRecord> history;
extern int PosStatus;
extern int cnt;

inline void to_json(json& j, const DataRecord& dr){
    j = json{
        {"boardStatus", dr.boardStatus},
        {"player1", dr.player1},
        {"player2", dr.player2},
        {"blackTurn", dr.blackTurn},
        {"who_plays_first", dr.who_plays_first},
        {"lastMoveRow", dr.lastMoveRow},
        {"lastMoveCol", dr.lastMoveCol}
    };
}

inline void from_json(const json& j, DataRecord& dr){
    j.at("boardStatus").get_to(dr.boardStatus);
    j.at("player1").get_to(dr.player1);
    j.at("player2").get_to(dr.player2);
    j.at("blackTurn").get_to(dr.blackTurn);
    j.at("who_plays_first").get_to(dr.who_plays_first);
    dr.lastMoveRow = j.value("lastMoveRow", -1);
    dr.lastMoveCol = j.value("lastMoveCol", -1);
}


void Init_History(bool &blackTurn, int &who_plays_first);
void Undo_Move(bool &blackturn, int &who_plays_first);
void Redo_Move(bool &blackturn, int &who_plays_first);
void Pop_History();
void Push_History(bool &blackTurn, int &who_plays_first, int lastRow = -1, int lastCol = -1);
