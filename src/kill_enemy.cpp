#include "kill_enemy.h"
#include "board.h"
#include "Player.h"
#include "inside.h"
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;
vector<pair<int,int>> stones_to_kill;

bool check_kill_enemy_stones(int x, int y, Stone enemy){
    if(board[x][y] != enemy) return false;

    stones_to_kill.clear();
    queue<pair<int,int>> Q;
    vector<vector<bool>> visited( BOARD_SIZE,vector<bool>( BOARD_SIZE,false));
    Q.push({x, y});
    visited[x][y] = true;
    bool check_kill = true;

    while(!Q.empty()){ 
        int u = Q.front().first;
        int v = Q.front().second;
        Q.pop();
        stones_to_kill.push_back({u,v});
        for(int j=0; j<4; j++){
            int nx = u + dx[j];
            int ny = v + dy[j];
            if(Inside(nx,ny) && board[nx][ny] == EMPTY){
                check_kill = false;
            }
            if(Inside(nx,ny) && !visited[nx][ny] && board[nx][ny] == enemy){
                visited[nx][ny] = true;
                Q.push({nx,ny});
            }
        }
    }
    return check_kill;
}


vector<pair<int,int>> kill_enemy_stones(int x, int y, bool blackTurn, bool scoring_mode){
    vector<pair<int,int>> killed_stones;
    Stone enemy = blackTurn ? BLACK : WHITE;
    
    for(int i=0; i<4; i++) if(Inside(x + dx[i], y + dy[i])) {
        if(check_kill_enemy_stones(x + dx[i], y + dy[i], enemy)) {
            for(auto [x,y] : stones_to_kill)
                killed_stones.push_back({x,y});
        }
    }
    sort(killed_stones.begin(), killed_stones.end());
    killed_stones.erase(unique(killed_stones.begin(), killed_stones.end()), killed_stones.end());
    for(auto [x,y] : killed_stones) board[x][y] = EMPTY;
    if (enemy == WHITE) player1.kill += scoring_mode * (int)killed_stones.size() * 10;
    else player2.kill += scoring_mode * (int)killed_stones.size() * 10;
    return killed_stones;
}