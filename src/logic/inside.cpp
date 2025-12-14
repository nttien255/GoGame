#include<iostream>
#include "board.h"
using namespace std;
int dx[4] = {1,-1,0,0};
int dy[4] = {0,0,1,-1};
bool Inside(int x, int y){
    return (x>=0 && x< BOARD_SIZE && y>=0 && y< BOARD_SIZE);
}