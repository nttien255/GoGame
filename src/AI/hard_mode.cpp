#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "hard_mode.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <thread>
#include <chrono>
#include "../skip.h"
#include "../history.h"
#include "../board.h"
#include "../scoring.h"
#include "../kill_enemy.h"

#ifndef _WIN32
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#endif

using namespace std;

// ============== KataGo Configuration ==============
static const char* KATAGO_DIR = "katago";

// ============== KataGo Process ==============
#ifdef _WIN32
static HANDLE g_katago_process = NULL;
static HANDLE g_stdin_write = NULL;
static HANDLE g_stdout_read = NULL;
#else
static pid_t g_katago_pid = -1;
static int g_stdin_fd = -1;
static int g_stdout_fd = -1;
#endif

static bool g_katago_running = false;
static int g_board_size = 19;


// ============== KataGo Process Management ==============
static bool startKataGo() {
    if (g_katago_running) return true;
    
#ifdef _WIN32
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    
    HANDLE stdin_read, stdout_write;
    if (!CreatePipe(&stdin_read, &g_stdin_write, &sa, 0)) return false;
    if (!CreatePipe(&g_stdout_read, &stdout_write, &sa, 0)) return false;
    
    SetHandleInformation(g_stdin_write, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(g_stdout_read, HANDLE_FLAG_INHERIT, 0);
    
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdInput = stdin_read;
    si.hStdOutput = stdout_write;
    si.hStdError = stdout_write;
    si.dwFlags |= STARTF_USESTDHANDLES;
    ZeroMemory(&pi, sizeof(pi));
    
    char fullPath[MAX_PATH];
    GetFullPathNameA(KATAGO_DIR, MAX_PATH, fullPath, NULL);
    
    string fullCmd = string(fullPath) + "\\katago.exe gtp -model " + fullPath + "\\model.bin.gz -config " + fullPath + "\\gtp.cfg";
    
    if (!CreateProcessA(NULL, (LPSTR)fullCmd.c_str(), NULL, NULL, TRUE, 
                        CREATE_NO_WINDOW, NULL, fullPath, &si, &pi)) {
        return false;
    }
    
    g_katago_process = pi.hProcess;
    CloseHandle(pi.hThread);
    CloseHandle(stdin_read);
    CloseHandle(stdout_write);
    
    g_katago_running = true;
    
    // Wait for KataGo to initialize
    char buffer[4096];
    DWORD bytesRead, bytesAvailable;
    string startupOutput;
    auto startTime = chrono::steady_clock::now();
    
    while (true) {
        DWORD exitCode;
        if (GetExitCodeProcess(g_katago_process, &exitCode) && exitCode != STILL_ACTIVE) {
            g_katago_running = false;
            return false;
        }
        
        if (PeekNamedPipe(g_stdout_read, NULL, 0, NULL, &bytesAvailable, NULL) && bytesAvailable > 0) {
            DWORD toRead = min(bytesAvailable, (DWORD)(sizeof(buffer) - 1));
            if (ReadFile(g_stdout_read, buffer, toRead, &bytesRead, NULL) && bytesRead > 0) {
                buffer[bytesRead] = '\0';
                startupOutput += buffer;
                if (startupOutput.find("GTP ready") != string::npos || 
                    startupOutput.find("main protocol loop") != string::npos) {
                    Sleep(100);
                    break;
                }
            }
        }
        
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(
            chrono::steady_clock::now() - startTime).count();
        if (elapsed > 30000) break;
        Sleep(50);
    }
#else
    int stdin_pipe[2], stdout_pipe[2];
    if (pipe(stdin_pipe) < 0 || pipe(stdout_pipe) < 0) return false;
    
    g_katago_pid = fork();
    if (g_katago_pid == 0) {
        dup2(stdin_pipe[0], STDIN_FILENO);
        dup2(stdout_pipe[1], STDOUT_FILENO);
        close(stdin_pipe[1]);
        close(stdout_pipe[0]);
        
        string katagoPath = string(KATAGO_DIR) + "/katago";
        string modelPath = string(KATAGO_DIR) + "/model.bin.gz";
        string configPath = string(KATAGO_DIR) + "/gtp.cfg";
        execlp(katagoPath.c_str(), "katago", "gtp", "-model", modelPath.c_str(), "-config", configPath.c_str(), NULL);
        exit(1);
    }
    close(stdin_pipe[0]);
    close(stdout_pipe[1]);
    g_stdin_fd = stdin_pipe[1];
    g_stdout_fd = stdout_pipe[0];
    g_katago_running = true;
    this_thread::sleep_for(chrono::milliseconds(3000));
#endif
    
    return true;
}


static string sendGtpCommand(const string& cmd) {
    if (!g_katago_running) return "";
    
#ifdef _WIN32
    DWORD exitCode;
    if (GetExitCodeProcess(g_katago_process, &exitCode) && exitCode != STILL_ACTIVE) {
        g_katago_running = false;
        return "";
    }
    
    string command = cmd + "\n";
    DWORD written;
    if (!WriteFile(g_stdin_write, command.c_str(), (DWORD)command.length(), &written, NULL)) {
        return "";
    }
    FlushFileBuffers(g_stdin_write);
    
    string response;
    char buffer[4096];
    DWORD bytesRead, bytesAvailable;
    auto startTime = chrono::steady_clock::now();
    
    while (true) {
        if (GetExitCodeProcess(g_katago_process, &exitCode) && exitCode != STILL_ACTIVE) {
            g_katago_running = false;
            break;
        }
        
        if (!PeekNamedPipe(g_stdout_read, NULL, 0, NULL, &bytesAvailable, NULL)) break;
        
        if (bytesAvailable > 0) {
            DWORD toRead = min(bytesAvailable, (DWORD)(sizeof(buffer) - 1));
            if (ReadFile(g_stdout_read, buffer, toRead, &bytesRead, NULL) && bytesRead > 0) {
                buffer[bytesRead] = '\0';
                response += buffer;
                
                if (response.find("\n\n") != string::npos || 
                    response.find("\r\n\r\n") != string::npos) break;
                
                size_t eqPos = response.find("=");
                if (eqPos != string::npos && response.find('\n', eqPos) != string::npos) {
                    Sleep(100);
                    continue;
                }
            }
        } else {
            auto elapsed = chrono::duration_cast<chrono::milliseconds>(
                chrono::steady_clock::now() - startTime).count();
            if (elapsed > 60000) break;
            Sleep(10);
        }
    }
    return response;
#else
    string command = cmd + "\n";
    write(g_stdin_fd, command.c_str(), command.length());
    
    string response;
    char buffer[4096];
    int flags = fcntl(g_stdout_fd, F_GETFL, 0);
    fcntl(g_stdout_fd, F_SETFL, flags | O_NONBLOCK);
    
    auto startTime = chrono::steady_clock::now();
    while (true) {
        ssize_t n = read(g_stdout_fd, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            response += buffer;
            if (response.find("\n\n") != string::npos) break;
        } else if (n < 0 && errno == EAGAIN) {
            auto elapsed = chrono::duration_cast<chrono::milliseconds>(
                chrono::steady_clock::now() - startTime).count();
            if (elapsed > 30000) break;
            usleep(10000);
        } else break;
    }
    fcntl(g_stdout_fd, F_SETFL, flags);
    return response;
#endif
}


// ============== GTP Move Conversion ==============
static string coordToGtp(int row, int col, int boardSize) {
    char colChar = 'A' + col;
    if (colChar >= 'I') colChar++;
    int gtpRow = boardSize - row;
    stringstream ss;
    ss << colChar << gtpRow;
    return ss.str();
}

static pair<int, int> gtpToCoord(const string& gtp, int boardSize) {
    if (gtp.empty() || gtp == "pass" || gtp == "PASS") return {-1, -1};
    if (gtp == "resign" || gtp == "RESIGN") return {-2, -2};
    
    char colChar = toupper(gtp[0]);
    int col = colChar - 'A';
    if (colChar > 'I') col--;
    int gtpRow = stoi(gtp.substr(1));
    int row = boardSize - gtpRow;
    return {row, col};
}

// ============== KataGoAI Class ==============
KataGoAI::KataGoAI() : size(0) {}
KataGoAI::~KataGoAI() {}

void KataGoAI::init(int board_size) {
    size = board_size;
    g_board_size = board_size;
    
    if (startKataGo()) {
        sendGtpCommand("boardsize " + to_string(board_size));
        sendGtpCommand("clear_board");
        sendGtpCommand("komi 6.5");
    }
}

void KataGoAI::cleanup() {}

void KataGoAI::playMove(int r, int c, int color) {
    if (!g_katago_running) return;
    string gtpColor = (color == 1) ? "B" : "W";
    string gtpCoord = coordToGtp(r, c, g_board_size);
    sendGtpCommand("play " + gtpColor + " " + gtpCoord);
}

void KataGoAI::undoMove() {
    if (!g_katago_running) return;
    sendGtpCommand("undo");
}

void KataGoAI::redoMoves(int count) {
    if (!g_katago_running) return;
    
    bool currentBlackTurn = !history[PosStatus].blackTurn;  
    
    for (int i = 0; i < count && PosStatus + 1 + i < (int)history.size(); i++) {
        int nextPos = PosStatus + 1 + i;
        int row = history[nextPos].lastMoveRow;
        int col = history[nextPos].lastMoveCol;
        
        if (row >= 0 && col >= 0) {
            playMove(row, col, currentBlackTurn ? 1 : 2);
        }

        currentBlackTurn = !currentBlackTurn;
    }
}

void KataGoAI::syncBoard(vector<vector<Stone>>& game_board, bool blackTurn) {
    if (!g_katago_running) return;
    
    sendGtpCommand("clear_board");
    
    for (int i = 0; i < g_board_size; i++) {
        for (int j = 0; j < g_board_size; j++) {
            if (game_board[i][j] == BLACK) {
                string gtpCoord = coordToGtp(i, j, g_board_size);
                sendGtpCommand("play B " + gtpCoord);
            }
        }
    }
    for (int i = 0; i < g_board_size; i++) {
        for (int j = 0; j < g_board_size; j++) {
            if (game_board[i][j] == WHITE) {
                string gtpCoord = coordToGtp(i, j, g_board_size);
                sendGtpCommand("play W " + gtpCoord);
            }
        }
    }
}

pair<int, int> KataGoAI::generateMove(int aicolor) {
    if (!g_katago_running) return {-1, -1};
    
    string gtpColor = (aicolor == 1) ? "B" : "W";
    string response = sendGtpCommand("genmove " + gtpColor);
    
    size_t eqPos = response.find('=');
    if (eqPos == string::npos) return {-1, -1};
    
    string move = response.substr(eqPos + 1);
    size_t start = move.find_first_not_of(" \t\n\r");
    size_t end = move.find_last_not_of(" \t\n\r");
    if (start != string::npos && end != string::npos) {
        move = move.substr(start, end - start + 1);
    }
    
    return gtpToCoord(move, g_board_size);
}


// ============== Main Interface ==============
static pair<int, int> findRandomValidMove() {
    vector<pair<int, int>> validMoves;
    for (int i = 0; i < g_board_size; i++) {
        for (int j = 0; j < g_board_size; j++) {
            if (board[i][j] == EMPTY) {
                validMoves.push_back({i, j});
            }
        }
    }
    if (validMoves.empty()) return {-1, -1};
    return validMoves[rand() % validMoves.size()];
}

void hard_mode_move(bool &blackTurn, int &who_plays_first, KataGoAI &katago) {
    int aiColor = blackTurn ? 1 : 2;
    
    pair<int, int> move = katago.generateMove(aiColor);
    
    // Fallback to random if KataGo fails
    if (move.first < 0) {
        move = findRandomValidMove();
        if (move.first < 0) {
            skip_turn(blackTurn, who_plays_first);
            return;
        }
    }
    
    int bestRow = move.first;
    int bestCol = move.second;
    
    // Validate move
    if (bestRow < 0 || bestRow >= g_board_size || bestCol < 0 || bestCol >= g_board_size ||
        board[bestRow][bestCol] != EMPTY) {
        move = findRandomValidMove();
        if (move.first < 0) {
            skip_turn(blackTurn, who_plays_first);
            return;
        }
        bestRow = move.first;
        bestCol = move.second;
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
