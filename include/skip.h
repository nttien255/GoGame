#pragma once

extern int cnt_skips_turn;

void init_skip();
void skip_turn(bool& blackTurn, int &who_plays_first);