cmake_minimum_required(VERSION 3.16)

project(GoGame)

# Đặt chuẩn C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- PHẦN 1: TÌM CÁC GÓI THƯ VIỆN ---
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(nlohmann_json REQUIRED)

# --- PHẦN 2: TẠO FILE THỰC THI ---
# (Nếu main.cpp ở thư mục 'src')
add_executable(GoGame
        src/main.cpp 
        src/move.cpp 
        src/board.cpp
        src/player.cpp
        src/scoring.cpp
        src/valid.cpp
        src/kill_enemy.cpp
        src/inside.cpp
        src/history.cpp
        src/WINDOW_PLAYING.cpp
        src/playing_interface.cpp
        src/skip.cpp
        src/check_game_state.cpp
        src/save_load_game.cpp
        src/loadgame_interface.cpp
        src/endgame_interface.cpp
        src/home_interface.cpp
        src/playing_mode_interface.cpp
    )
# (Nếu main.cpp ở ngang hàng)
# add_executable(GoGame main.cpp)

# --- PHẦN 3: LIÊN KẾT CÁC THƯ VIỆN VÀO GAME ---
target_link_libraries(GoGame PRIVATE 
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image-static
    SDL2_mixer::SDL2_mixer-static
    SDL2_ttf::SDL2_ttf-static
    nlohmann_json::nlohmann_json
)

