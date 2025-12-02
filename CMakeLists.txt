cmake_minimum_required(VERSION 3.16)

project(GoGame)

# Đặt chuẩn C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Đặt chuẩn C
set(CMAKE_C_STANDARD 99) 
set(CMAKE_C_STANDARD_REQUIRED True)


# --- PHẦN 1: TÌM CÁC GÓI THƯ VIỆN ---
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(nlohmann_json REQUIRED)

# # --- PHẦN 2: TẢI VỀ VÀ THÊM THƯ VIỆN PACHI ---
include(FetchContent)
FetchContent_Declare(
  pachi
  GIT_REPOSITORY https://github.com/pasky/pachi.git
  GIT_TAG master
)
FetchContent_MakeAvailable(pachi)

file(WRITE "${pachi_SOURCE_DIR}/build.h" "#define BUILD_INFO \"SDL_Build\"\n")
file(GLOB_RECURSE PACHI_SOURCES "${pachi_SOURCE_DIR}/*.c")

list(FILTER PACHI_SOURCES EXCLUDE REGEX "main\\.c$")
list(FILTER PACHI_SOURCES EXCLUDE REGEX "gtp\\.c$")
list(FILTER PACHI_SOURCES EXCLUDE REGEX "pachi\\.c$")

list(FILTER PACHI_SOURCES EXCLUDE REGEX "/dcnn/")
list(FILTER PACHI_SOURCES EXCLUDE REGEX "/tools/")
list(FILTER PACHI_SOURCES EXCLUDE REGEX "slave\\.c$")
list(FILTER PACHI_SOURCES EXCLUDE REGEX "/distributed/")
list(FILTER PACHI_SOURCES EXCLUDE REGEX "/josekifix/")
list(FILTER PACHI_SOURCES EXCLUDE REGEX "/joseki/")
list(FILTER PACHI_SOURCES EXCLUDE REGEX "/tests/")
list(FILTER PACHI_SOURCES EXCLUDE REGEX "benchmark\\.c$")

list(FILTER PACHI_SOURCES EXCLUDE REGEX "fifo\\.c$")   
list(FILTER PACHI_SOURCES EXCLUDE REGEX "spipe\\.c$")  
list(FILTER PACHI_SOURCES EXCLUDE REGEX "network\\.c$")
list(FILTER PACHI_SOURCES EXCLUDE REGEX "plugins\\.c$")
list(FILTER PACHI_SOURCES EXCLUDE REGEX "/plugins/")

# TẠO THƯ VIỆN
add_library(pachi_lib STATIC ${PACHI_SOURCES})


# --- PHẦN 3: TẠO FILE THỰC THI ---
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
        src/menu_interface.cpp
    )
# (Nếu main.cpp ở ngang hàng)
# add_executable(GoGame main.cpp)

# --- PHẦN 4: LIÊN KẾT CÁC THƯ VIỆN VÀO GAME ---

# THÊM ĐƯỜNG DẪN HEADER
target_include_directories(pachi_lib PUBLIC 
    "${pachi_SOURCE_DIR}"             # Thư mục gốc
    "${pachi_SOURCE_DIR}/board"     # Thư mục board
    "${pachi_SOURCE_DIR}/uct"       # Thư mục uct
    "${pachi_SOURCE_DIR}/tactics"   # Thư mục tactics
    "${pachi_SOURCE_DIR}/pattern"   # Thư mục pattern
    "${pachi_SOURCE_DIR}/playout"   # Thư mục playout
)
# Thêm flag để cho phép code Pachi (C cũ) biên dịch được mà không quá khắt khe
target_compile_options(pachi_lib PRIVATE -w)
target_compile_definitions(pachi_lib PRIVATE -DWIN32 -D_WINDOWS)

# Cho phép project include header của Pachi
target_include_directories(GoGame PRIVATE 
    "${pachi_SOURCE_DIR}"
)

target_link_libraries(GoGame PRIVATE 
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image-static
    SDL2_mixer::SDL2_mixer-static
    SDL2_ttf::SDL2_ttf-static
    nlohmann_json::nlohmann_json
    pachi_lib
)

