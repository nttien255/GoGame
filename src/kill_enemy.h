#pragma once
#include "board.h"
#include <vector>
#include <iostream>

using namespace std;

bool check_kill_enemy_stones(int x, int y, Stone enemy);
vector<pair<int,int>> kill_enemy_stones(int x, int y, bool blackTurn, bool scoring_mode);
