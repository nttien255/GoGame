cmake_minimum_required(VERSION 3.16)

if(POLICY CMP0169)
    cmake_policy(SET CMP0169 OLD)
endif()

project(GoGame)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- PHẦN 1: TÌM CÁC GÓI THƯ VIỆN ---
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# --- PHẦN 2: TẠO FILE THỰC THI ---
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
    src/AI/easy_mode.cpp
    src/AI/hard_mode.cpp
    src/choose_mode_interface.cpp
    src/AI/medium_mode.cpp
)

# --- PHẦN 3: LIÊN KẾT CÁC THƯ VIỆN ---
target_link_libraries(GoGame PRIVATE 
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image-static
    SDL2_mixer::SDL2_mixer-static
    SDL2_ttf::SDL2_ttf-static
    nlohmann_json::nlohmann_json
)


# --- PHẦN 4: TẢI KATAGO KHI BUILD ---
set(KATAGO_DIR "${CMAKE_BINARY_DIR}/katago")
set(KATAGO_EXE "${KATAGO_DIR}/katago.exe")
set(KATAGO_MODEL "${KATAGO_DIR}/model.bin.gz")
set(KATAGO_ZIP "${KATAGO_DIR}/katago.zip")

set(KATAGO_EXE_URL "https://github.com/lightvector/KataGo/releases/download/v1.15.3/katago-v1.15.3-eigenavx2-windows-x64.zip")
# Model hosted on katagotraining.org (direct link, no redirect)
set(KATAGO_MODEL_URL "https://media.katagotraining.org/uploaded/networks/models/kata1/kata1-b18c384nbt-s7709731328-d3715293823.bin.gz")

# Create katago directory
file(MAKE_DIRECTORY ${KATAGO_DIR})

# Download and extract KataGo executable
if(NOT EXISTS ${KATAGO_EXE})
    message(STATUS "Downloading KataGo executable...")
    file(DOWNLOAD ${KATAGO_EXE_URL} ${KATAGO_ZIP} 
         SHOW_PROGRESS 
         STATUS DOWNLOAD_STATUS
         TLS_VERIFY ON)
    list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
    if(STATUS_CODE EQUAL 0)
        message(STATUS "Extracting KataGo...")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xzf ${KATAGO_ZIP}
            WORKING_DIRECTORY ${KATAGO_DIR}
        )
        file(GLOB_RECURSE KATAGO_FOUND "${KATAGO_DIR}/**/katago.exe")
        if(KATAGO_FOUND)
            list(GET KATAGO_FOUND 0 KATAGO_FOUND_PATH)
            get_filename_component(KATAGO_SRC_DIR ${KATAGO_FOUND_PATH} DIRECTORY)
            file(GLOB KATAGO_FILES "${KATAGO_SRC_DIR}/*")
            foreach(F ${KATAGO_FILES})
                get_filename_component(FNAME ${F} NAME)
                if(NOT EXISTS "${KATAGO_DIR}/${FNAME}")
                    file(RENAME ${F} "${KATAGO_DIR}/${FNAME}")
                endif()
            endforeach()
        endif()
    else()
        message(WARNING "Failed to download KataGo executable")
    endif()
endif()

# Download KataGo model using FetchContent (handles redirects)
include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

if(NOT EXISTS ${KATAGO_MODEL})
    message(STATUS "Downloading KataGo model...")
    FetchContent_Declare(
        katago_model
        URL ${KATAGO_MODEL_URL}
        DOWNLOAD_DIR ${KATAGO_DIR}
        DOWNLOAD_NO_EXTRACT TRUE
        TLS_VERIFY OFF
    )
    FetchContent_Populate(katago_model)
    
    # Find and rename the downloaded file
    file(GLOB DOWNLOADED_MODEL "${KATAGO_DIR}/kata1*.bin.gz")
    if(DOWNLOADED_MODEL)
        list(GET DOWNLOADED_MODEL 0 MODEL_FILE)
        if(NOT "${MODEL_FILE}" STREQUAL "${KATAGO_MODEL}")
            file(RENAME "${MODEL_FILE}" "${KATAGO_MODEL}")
        endif()
        message(STATUS "KataGo model ready!")
    else()
        # Check in _deps folder
        file(GLOB_RECURSE DOWNLOADED_MODEL "${CMAKE_BINARY_DIR}/_deps/katago_model*/*.bin.gz")
        if(DOWNLOADED_MODEL)
            list(GET DOWNLOADED_MODEL 0 MODEL_FILE)
            file(COPY "${MODEL_FILE}" DESTINATION "${KATAGO_DIR}")
            get_filename_component(MODEL_FILENAME "${MODEL_FILE}" NAME)
            file(RENAME "${KATAGO_DIR}/${MODEL_FILENAME}" "${KATAGO_MODEL}")
            message(STATUS "KataGo model ready!")
        endif()
    endif()
endif()

# Create GTP config file
set(KATAGO_CONFIG "${KATAGO_DIR}/gtp.cfg")
file(WRITE ${KATAGO_CONFIG} 
"# KataGo GTP Config - Optimized for strength in 3 seconds

# Logs
logDir = logs
logAllGTPCommunication = false
logSearchInfo = false
logToStderr = false

# Rules
rules = japanese

# Search limits - max strength in 3 seconds
maxTime = 3.0
numSearchThreads = 8
ponderingEnabled = false

# Neural net cache - larger cache = less recalculation
nnCacheSizePowerOfTwo = 23

# Resignation
allowResignation = false
resignThreshold = -0.99
resignConsecTurns = 10

# Play strongest move (no randomness)
chosenMoveTemperature = 0.0
chosenMoveTemperatureEarly = 0.0
chosenMoveSubtract = 0
chosenMovePrune = 1

# Search quality optimizations
cpuctExploration = 1.0
cpuctExplorationLog = 0.45
fpuReductionMax = 0.2
useGraphSearch = true
useNoisePruning = true

# Root optimizations - sample symmetries for better eval
rootNumSymmetriesToSample = 8
rootSymmetryPruning = true

# Disable randomization
nnRandomize = false

# Use LCB for more accurate move selection
useLcbForSelection = true
lcbStdevs = 5.0
minVisitPropForLCB = 0.15
")

# Copy katago folder to output directory after build
add_custom_command(TARGET GoGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${KATAGO_DIR} $<TARGET_FILE_DIR:GoGame>/katago
    COMMENT "Copying KataGo to output directory"
)