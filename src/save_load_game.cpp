#include "save_load_game.h"
#include "board.h"

filesystem::path currentPath;


string getTime(){
    auto now = chrono::system_clock::now();
    time_t now_t = chrono::system_clock::to_time_t(now);
    tm* realTime = localtime(&now_t);

    stringstream ss;
    ss << put_time(realTime, "%Y-%m-%d_%H-%M-%S");
    return ss.str();
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


void SaveGame(bool &blackTurn){
    SaveDataGame currentState(board, player1, player2, blackTurn, history, PosStatus, cnt_skips_turn, BOARD_SIZE, CELL_SIZE, STONE_RADIUS, CLICK_RADIUS, MARGIN);
    string filename = currentState.Time;
    if(!createFolders("saves")) return;

    cout << filename << endl;
    string newPath = currentPath.u8string() + "/" + filename + ".json";  

    ofstream file(newPath);
    json j;
    j["currentState"] = currentState;
    file << j.dump(4) << endl;
    file.close();
}

void LoadGame(bool &blackTurn, string filename){
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
    currentState.Update(board, player1, player2, blackTurn, history, PosStatus, cnt_skips_turn, BOARD_SIZE, CELL_SIZE, STONE_RADIUS, CLICK_RADIUS, MARGIN);
}


vector<string> allFileLoadGame(){
    vector<string> vectorFileJson;
    if(!createFolders("saves")) return vectorFileJson;
    for(auto &entry : filesystem::directory_iterator(currentPath)){
        if(entry.is_regular_file()){
            vectorFileJson.push_back(entry.path().stem().string());
        }
    }
    return vectorFileJson;
}




