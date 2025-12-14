#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Player{
    public:
        int Score;
        int kill;
};

extern Player player1, player2;

inline void to_json(json& j, const Player& p){
    j = json{
        {"Score", p.Score},
        {"kill", p.kill}
    };
}

inline void from_json(const json& j, Player& p){
    j.at("Score").get_to(p.Score);
    j.at("kill").get_to(p.kill);
}


void Reset_Player();
void Init_Player();
