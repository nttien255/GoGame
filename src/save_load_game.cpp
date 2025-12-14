#include "save_load_game.h"
#include "board.h"

filesystem::path currentPath;
vector<string> vectorFileJson;

string getTime(){
    auto now = chrono::system_clock::now();
    time_t now_t = chrono::system_clock::to_time_t(now);
    tm* realTime = localtime(&now_t);

    stringstream ss;
    ss << put_time(realTime, "%Y-%m-%d_%H-%M-%S");
    return ss.str();
}

void deleteFile(string namefolder, string pattern) {
    

    try{
        currentPath = filesystem::current_path();
        currentPath = currentPath.parent_path() / namefolder;

        for (auto &entry : filesystem::directory_iterator(currentPath)) {
            if (entry.is_regular_file()) {
                string filename = entry.path().filename().string();

                if (filename.find(pattern) != string::npos) {                    
                    filesystem::remove(entry.path());
                }
            }
        }
    } catch (const filesystem::filesystem_error& e) {
        std::cerr << "Loi he thong: " << e.what() << std::endl;
        return;
    }
}


bool createFolders(string namefolder){
    try {
        currentPath = filesystem::current_path();
        currentPath = currentPath.parent_path() / namefolder;        
        filesystem::create_directories(currentPath);
        return true;
    } 
    catch (const filesystem::filesystem_error& e) {
        cerr << "Error creating directories: " << e.what() << endl;
        return false;
    }
}


void SaveGame(bool &blackTurn, int &who_plays_first, AIState &ai_state){
    SaveDataGame currentState(board, player1, player2, blackTurn, who_plays_first, history, PosStatus, cnt_skips_turn, BOARD_SIZE, CELL_SIZE, STONE_RADIUS, CLICK_RADIUS, MARGIN, ai_state);
    vectorFileJson = allFileLoadGame();
    int stt = 0;
    string string_stt = "";
    if(!vectorFileJson.empty()) stt = (vectorFileJson[0][25]-'0') * 10 + (vectorFileJson[0][26]-'0') + 1;
    stt %= 20;
    // cout << "stt: " << stt << endl;

    string filename = currentState.Time + "_Game_";
    if(stt < 10 ) string_stt += "0", string_stt += char('0' + stt);
    else string_stt += char('0' + stt / 10), string_stt += char('0' + stt % 10);
    filename += string_stt;


    if(ai_state == AIState::NONE) filename += "_0_";
    if(ai_state == AIState::EASY_PLAY) filename += "_1_";
    if(ai_state == AIState::MEDIUM_PLAY) filename += "_2_";
    if(ai_state == AIState::HARD_PLAY) filename += "_3_";
    filename += (BOARD_SIZE == 13 ? "13" : "19"); 

    
    // cout << filename << endl;
    deleteFile("saves", "Game_" + string_stt);
    string newPath = currentPath.u8string() + "/" + filename + ".json";  

    ofstream file(newPath);
    json j;
    j["currentState"] = currentState;
    file << j.dump(4) << endl;
    file.close();
}

void LoadGame(bool &blackTurn, int &who_plays_first, AIState &ai_state, string filename){
    SaveDataGame currentState;
    if(!createFolders("saves")) return;
    string Path = currentPath.u8string() + "/" + filename + ".json";
    ifstream file(Path);
    if(!file.is_open()){
        cerr << "Error opening file: " << Path << endl;
        return;
    }

    json j;
    j = json::parse(file);
    file.close();

    currentState = j["currentState"].get<SaveDataGame>();
    currentState.Update(board, player1, player2, blackTurn, who_plays_first, history, PosStatus, cnt_skips_turn, BOARD_SIZE, CELL_SIZE, STONE_RADIUS, CLICK_RADIUS, MARGIN, ai_state);
}


vector<string> allFileLoadGame(){
    vectorFileJson.clear();
    if(!createFolders("saves")) return vectorFileJson;
    for(auto &entry : filesystem::directory_iterator(currentPath)){
        if(entry.is_regular_file()){
            vectorFileJson.push_back(entry.path().stem().string());
        }
    }
    sort(vectorFileJson.begin(), vectorFileJson.end(), greater<string>());
    return vectorFileJson;
}




