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
};

extern deque<DataRecord> history;
extern int PosStatus;


inline void to_json(json& j, const DataRecord& dr){
    j = json{
        {"boardStatus", dr.boardStatus},
        {"player1", dr.player1},
        {"player2", dr.player2},
        {"blackTurn", dr.blackTurn}
    };
}

inline void from_json(const json& j, DataRecord& dr){
    j.at("boardStatus").get_to(dr.boardStatus);
    j.at("player1").get_to(dr.player1);
    j.at("player2").get_to(dr.player2);
    j.at("blackTurn").get_to(dr.blackTurn);
}


void Init_History();
void Undo_Move();
void Redo_Move();
void Pop_History();
void Push_History();
