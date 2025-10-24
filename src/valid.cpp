//TH1: dat khong hop le
//TH2: het khi
//TH3: vong lap
#include "valid.h"
#include "board.h"
#include "inside.h"
#include "kill_enemy.h"
#include <iostream>
using namespace std;


bool TH1(int &x, int &y, Stone &player){
    if(board[x][y] != EMPTY) return false;
    return true;
}
bool TH2(int &x, int &y, Stone &player){
    if(Inside(x-1,y) && (board[x-1][y] == player || board[x-1][y] == EMPTY)) return true;
    if(Inside(x+1,y) && (board[x+1][y] == player || board[x+1][y] == EMPTY)) return true;
    if(Inside(x,y-1) && (board[x][y-1] == player || board[x][y-1] == EMPTY)) return true;
    if(Inside(x,y+1) && (board[x][y+1] == player || board[x][y+1] == EMPTY)) return true;
    for(int i=0; i<4; i++){
        int nx = x + dx[i];
        int ny = y + dy[i];
        board[x][y] = player;
        bool check_kill = check_kill_enemy_stones(nx,ny, (player == BLACK ? WHITE : BLACK));
        board[x][y] = EMPTY;
        if(Inside(nx,ny) && check_kill){
            return true;
        }
    }
    return false;
}

bool TH3(int &x, int &y, Stone &player){

    return true;
}



bool valid(int x, int y, bool blackTurn){
    Stone player = blackTurn ? BLACK : WHITE;
    if(!TH1(x,y,player)) return false;
    if(!TH2(x,y,player)) return false;
    if(!TH3(x,y,player)) return false;
    return true;
}