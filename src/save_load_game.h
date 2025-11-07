#pragma once
#include <vector>
#include <string>
#include <deque>
#include <iostream>
#include "history.h"
#include "player.h"
#include "board.h"
#include "skip.h"
#include <chrono>
#include <iomanip>
#include <sstream>

#include <filesystem> 
#include <fstream>
#include <nlohmann/json.hpp>

#include <SDL2/SDL.h>

using namespace std;
using json = nlohmann::json;

extern filesystem::path currentPath;

string getTime();


struct SaveDataGame{
    vector<vector<Stone>> board;
    Player player1, player2;
    bool blackTurn;
    
    deque<DataRecord> history;
    int PosStatus;

    int cnt_skips_turn;

    string Time;

    SaveDataGame() {}
    SaveDataGame(vector<vector<Stone>> b, Player p1, Player p2, bool bt, deque<DataRecord> h, int ps, int cst, string t = getTime())
        : board(b), player1(p1), player2(p2), blackTurn(bt), history(h), PosStatus(ps), cnt_skips_turn(cst), Time(t) {}
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
        {"cnt_skips_turn", SDG.cnt_skips_turn},
        {"Time", SDG.Time}

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
    j.at("Time").get_to(SDG.Time);
}

bool createFolders(std::string namefolder);
// int countFiles(filesystem::path &path);
void SaveGame(bool &blackTurn);
void LoadGame(bool &blackTurn, string filename);
vector<string> allFileLoadGame();






