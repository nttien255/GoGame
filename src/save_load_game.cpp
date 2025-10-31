#include "save_load_game.h"
#include "board.h"

filesystem::path currentPath;

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


int countFiles(filesystem::path &path){
    int cnt = 0;
    for(auto &entry : filesystem::directory_iterator(path)){
        if(entry.is_regular_file())
            ++cnt;
    }
    return cnt;
}

void SaveGame(bool &blackTurn, string filename){
    SaveDataGame currentState(board, player1, player2, blackTurn, history, PosStatus, cnt_skips_turn);

    if(!createFolders("saves")) return;
    // int numbers = files.size;
    // int numbers = countFiles(currentPath);
    
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

    string newPath = currentPath.u8string() + "/" + filename + ".json";  
    ifstream file(newPath);
    if(!file.is_open()){
        cerr << "Error opening file: " << newPath << endl;
        return;
    }

    json j;
    j = json::parse(file);
    file.close();

    currentState = j["currentState"].get<SaveDataGame>();
    currentState.Update(board, player1, player2, blackTurn, history, PosStatus, cnt_skips_turn);
}