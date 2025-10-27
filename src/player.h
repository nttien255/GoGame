#pragma once
#include <iostream>

class Player{
    public:
        int Score;
        int kill;
};

extern Player player1, player2;

void Reset_Player();
void Init_Player();
