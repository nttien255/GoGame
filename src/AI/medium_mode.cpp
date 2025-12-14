#include "medium_mode.h"
#include "skip.h"
#include "history.h"
#include "board.h"
#include "scoring.h"
#include "kill_enemy.h"
#include "valid.h"
#include "inside.h"
#include "player.h"
#include <random>
#include <stack>

using namespace std;

vector<pair<int, int>> validMoves;
vector<vector<Stone>> b[1];
vector<pair<int, int>> killedStones;
vector<pair<int, int>> possibleMoves;
Player p1, p2;
mt19937_64 rng;

bool maximum(int &a, int b){
    if(b > a){
        a = b;
        return true;
    }
    return false;
}

int point;
int initial_place_self;
int final_place_self;
int initial_place_enemy;
int final_place_enemy;

int calculateCaptures(int &r, int &c, bool &blackTurn){
    point = 0;

    // protect
    board[r][c] = blackTurn ? WHITE : BLACK;
    killedStones = kill_enemy_stones(r, c, blackTurn, 0);
    point += (int)killedStones.size() * (5 + 2);
    // cout << "step 1 capture score: " << point << endl;

    // kill enemy
    board = b[0];
    board[r][c] = blackTurn ? BLACK : WHITE;
    killedStones = kill_enemy_stones(r, c, !blackTurn, 0);
    point += (int)killedStones.size() * (3 + 2);
    // cout << "step 2 capture score: " << point << endl;
    
    // space score
    board = b[0];
    Run_Score();
    initial_place_self = (blackTurn ? player1.Score : player2.Score);
    initial_place_enemy = (blackTurn ? player2.Score : player1.Score);
    board[r][c] = blackTurn ? BLACK : WHITE;
    kill_enemy_stones(r, c, !blackTurn, 1);
    Run_Score();
    final_place_self = (blackTurn ? player1.Score : player2.Score);
    final_place_enemy = (blackTurn ? player2.Score : player1.Score);
    point += (final_place_self - initial_place_self) / 10 * 2;
    point += (initial_place_enemy - final_place_enemy) / 10 * 3;
    // cout << "step 3 capture score: " << initial_place_self << " " << final_place_self << ": " << player1.Score << " " << player2.Score << " " << point << endl;

    // adjacent enemy stones
    board = b[0];
    for(int i = 0; i < 4; i++){
        int u = r + dx[i];
        int v = c + dy[i];
        if(Inside(u,v) && board[u][v] == (blackTurn ? WHITE : BLACK)){
            point += 1;
        }
    }
    // cout << "step 4 capture score: " << point << endl;

    return point;
}


void medium_mode_move(bool &blackTurn, int &who_plays_first){    
    srand(time(0));
    rng.seed(rand());
    int bestRow = -1, bestCol = -1;
    int maxCaptures = -1e9;
    b[0] = board;
    p1 = player1;
    p2 = player2;

    possibleMoves.clear();
    for(int r = 0; r < BOARD_SIZE; r++){
        for(int c = 0; c < BOARD_SIZE; c++){
            if(valid(r, c, blackTurn) == false) continue;
            possibleMoves.push_back({r, c});
        }
    }
    shuffle(possibleMoves.begin(), possibleMoves.end(), rng);

    for(auto move : possibleMoves){
        int r = move.first;
        int c = move.second;
        board = b[0];
        player1 = p1;
        player2 = p2;

        int captures = calculateCaptures(r, c, blackTurn);
        if(maximum(maxCaptures, captures)){
            bestRow = r;
            bestCol = c;
        }
    }
    // cout << "Medium AI chooses: (" << bestRow << ", " << bestCol << ") with score " << maxCaptures << endl;   

    if(maxCaptures < 0){
        skip_turn(blackTurn, who_plays_first);
        return;
    }

    cnt_skips_turn = 0;
    Pop_History();

    board[bestRow][bestCol] = blackTurn ? BLACK : WHITE;
    who_plays_first = !(who_plays_first - 1) + 1;
    blackTurn = !blackTurn;

    kill_enemy_stones(bestRow, bestCol, blackTurn, 1);
    Run_Score();
    Push_History(blackTurn, who_plays_first, bestRow, bestCol);
}