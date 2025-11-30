#include "scoring.h"
#include "player.h"
#include "board.h"
#include "move.h"
#include "inside.h"
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;


int cnt;
bool check_black;
bool check_white;
queue<pair<int,int>> Q;
vector<vector<bool>> visited;

void add_score(Player &player, int score){
    player.kill += score;
}

void BFS_Score(int x, int y){
    Q.push({x,y});
    visited[x][y] = true;
    while(!Q.empty()){ 
        int x = Q.front().first;
        int y = Q.front().second;
        cnt++;
        Q.pop();
        for(int i=0; i<4; i++){
            int u = x + dx[i];
            int v = y + dy[i];
            if(Inside(u,v) && board[u][v] == BLACK) check_black = true;
            if(Inside(u,v) && board[u][v] == WHITE) check_white = true;
            if(Inside(u,v) && !visited[u][v] && board[u][v] == 0){
                visited[u][v] = true;
                Q.push({u,v});
            }
        }
    }
}

void calc_place_score(){
    visited.resize(BOARD_SIZE, vector<bool>(BOARD_SIZE, false));
    for(int i=0;i<BOARD_SIZE;i++)
        for(int j=0;j<BOARD_SIZE;j++)
            visited[i][j] = false;
    
    for(int i=0;i<BOARD_SIZE;i++){
        for(int j=0;j<BOARD_SIZE;j++){
            if(board[i][j] == 0 && !visited[i][j]){
                cnt = 0;
                check_black = false;
                check_white = false;
                BFS_Score(i,j);
                if(check_black && check_white) continue;
                else if(check_black) 
                    player1.Score += cnt * 10;
                else if(check_white)
                    player2.Score += cnt * 10;
                
            }
        }
    }
}

string Score_Player(Player &player){
    stringstream ss;
    ss << fixed << setprecision(1) << (player.Score + player.kill) / 10.0;
    return ss.str();
}

void Run_Score(){
    Reset_Player();
    calc_place_score();
}

