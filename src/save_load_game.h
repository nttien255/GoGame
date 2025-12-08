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
    int BOARD_SIZE, CELL_SIZE, STONE_RADIUS, CLICK_RADIUS, MARGIN;
    string Time;

    SaveDataGame() {}
    SaveDataGame(vector<vector<Stone>> b, Player p1, Player p2, bool bt, deque<DataRecord> h, int ps, int cst, int board_size, int cell_size, int stone_radius, int click_radius, int margin, string t = getTime())
        : board(b), player1(p1), player2(p2), blackTurn(bt), history(h), PosStatus(ps), cnt_skips_turn(cst), BOARD_SIZE(board_size), CELL_SIZE(cell_size), STONE_RADIUS(stone_radius), CLICK_RADIUS(click_radius), MARGIN(margin), Time(t) {}
    void Update(vector<vector<Stone>> &b, Player &p1, Player &p2, bool &bt, deque<DataRecord> &h, int &ps, int &cst, int &board_size, int &cell_size, int &stone_radius, int &click_radius, int &margin){
        b = board;
        p1 = player1;
        p2 = player2;
        bt = blackTurn;
        h = history;
        ps = PosStatus;
        cst = cnt_skips_turn;
        board_size = BOARD_SIZE;
        cell_size = CELL_SIZE;
        stone_radius = STONE_RADIUS;
        click_radius = CLICK_RADIUS;
        margin = MARGIN;
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
        {"BOARD_SIZE", SDG.BOARD_SIZE},
        {"CELL_SIZE", SDG.CELL_SIZE},
        {"STONE_RADIUS", SDG.STONE_RADIUS},
        {"CLICK_RADIUS", SDG.CLICK_RADIUS},
        {"MARGIN", SDG.MARGIN},
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
    j.at("BOARD_SIZE").get_to(SDG.BOARD_SIZE);
    j.at("CELL_SIZE").get_to(SDG.CELL_SIZE);
    j.at("STONE_RADIUS").get_to(SDG.STONE_RADIUS);
    j.at("CLICK_RADIUS").get_to(SDG.CLICK_RADIUS);
    j.at("MARGIN").get_to(SDG.MARGIN);
    j.at("Time").get_to(SDG.Time);
}

bool createFolders(std::string namefolder);
// int countFiles(filesystem::path &path);
void SaveGame(bool &blackTurn);
void LoadGame(bool &blackTurn, string filename);
vector<string> allFileLoadGame();






