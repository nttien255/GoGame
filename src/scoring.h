#pragma once
#include <iostream>
#include "player.h"
#include <string>

using namespace std;

extern int cnt;
extern bool check_black;
extern bool check_white;
extern vector<vector<bool>> visited;


void BFS_Score(int x, int y);
void calc_place_score();
void Run_Score();
string Score_Player(Player &player);
