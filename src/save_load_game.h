#pragma once
#include <vector>
#include <string>
#include <deque>
#include <iostream>
#include "history.h"
#include "player.h"
#include "board.h"
#include "skip.h"

#include <filesystem> 
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

struct SaveDataGame{
    vector<vector<Stone>> board;
    Player player1, player2;
    bool blackTurn;
    
    deque<DataRecord> history;
    int PosStatus;

    int cnt_skips_turn;
    SaveDataGame() {}
    SaveDataGame(vector<vector<Stone>> b, Player p1, Player p2, bool bt, deque<DataRecord> h, int ps, int cst)
        : board(b), player1(p1), player2(p2), blackTurn(bt), history(h), PosStatus(ps), cnt_skips_turn(cst) {}
    void Update(vector<vector<Stone>> &b, Player &p1, Player &p2, bool &bt, deque<DataRecord> &h, int &ps, int &cst){
        b = board;
        p1 = player1;
        p2 = player2;
        bt = blackTurn;
        h = history;
        ps = PosStatus;
        cst = cnt_skips_turn;
    }
};


inline void to_json(json& j, const SaveDataGame& SDG){
    j = json{
        {"board", SDG.board},
        {"player1", SDG.player1},
        {"player2", SDG.player2},
        {"blackTurn", SDG.blackTurn},
        {"history", SDG.history},
        {"PosStatus", SDG.PosStatus},
        {"cnt_skips_turn", SDG.cnt_skips_turn}

    };
}

inline void from_json(const json& j, SaveDataGame& SDG){
    j.at("board").get_to(SDG.board);
    j.at("player1").get_to(SDG.player1);
    j.at("player2").get_to(SDG.player2);
    j.at("blackTurn").get_to(SDG.blackTurn);
    j.at("history").get_to(SDG.history);
    j.at("PosStatus").get_to(SDG.PosStatus);
    j.at("cnt_skips_turn").get_to(SDG.cnt_skips_turn);
}

bool createFolders(std::string namefolder);
int countFiles(filesystem::path &path);
void SaveGame(bool &blackTurn, string filename);
void LoadGame(bool &blackTurn, string filename);