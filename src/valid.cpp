//TH1: dat khong hop le
//TH2: het khi
//TH3: vong lap
#include "valid.h"
#include "board.h"
#include "inside.h"
#include "kill_enemy.h"
#include "history.h"
#include <iostream>
using namespace std;


bool TH1(int &x, int &y, Stone &player){
    if(board[x][y] != EMPTY) return false;
    return true;
}
bool TH2(int &x, int &y, Stone &player){
    //check free adjacent
    if(Inside(x-1,y) && board[x-1][y] == EMPTY) return true;
    if(Inside(x+1,y) && board[x+1][y] == EMPTY) return true;
    if(Inside(x,y-1) && board[x][y-1] == EMPTY) return true;
    if(Inside(x,y+1) && board[x][y+1] == EMPTY) return true;
    //check kill enemy stones
    for(int i=0; i<4; i++){
        int nx = x + dx[i];
        int ny = y + dy[i];
        if(!Inside(nx,ny)) continue;    
        board[x][y] = player;
        bool check_kill = check_kill_enemy_stones(nx,ny, (player == BLACK ? WHITE : BLACK));
        board[x][y] = EMPTY;
        if(check_kill) return true;
    }
    //check self kill
    board[x][y] = player;
    bool check_kill = check_kill_enemy_stones(x,y, player);
    board[x][y] = EMPTY;
    if(check_kill) return false;
    else return true;
}

bool TH3(int &x, int &y, Stone &player){
    board[x][y] = player;
    vector<pair<int,int>> killed_stones = kill_enemy_stones(x, y, (player == BLACK ? false : true), 0);
    for(int i = PosStatus - 1; i >= 0; i -= 2){
        if(history[i].boardStatus == board){
            board[x][y] = EMPTY;
            for (auto [x, y] : killed_stones)
                board[x][y] = (player == BLACK ? WHITE : BLACK);
            return false;
        }
    }
    board[x][y] = EMPTY;
    for (auto [x, y] : killed_stones)
        board[x][y] = (player == BLACK ? WHITE : BLACK);
    return true;
}

bool valid(int x, int y, bool blackTurn){
    Stone player = blackTurn ? BLACK : WHITE;
    if(!TH1(x,y,player)) return false;
    if(!TH2(x,y,player)) return false;
    if(!TH3(x,y,player)) return false;
    return true;
}