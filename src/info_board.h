#pragma one
#include<bits/stdc++.h>

enum Stone { EMPTY, BLACK, WHITE };
extern std::vector<std::vector<Stone>> board;

void init_board(int size);
int board_size();