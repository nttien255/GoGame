#include "player.h"
#include <bits/stdc++.h>
using namespace std;

Player player1,player2;


void Reset_Player(){
    player1.Score = 0;
    player2.Score = 65;
}

void Init_Player(){
    player1.Score = 0;
    player2.Score = 65;
    player1.kill = 0;
    player2.kill = 0;
}

