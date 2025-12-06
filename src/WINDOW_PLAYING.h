#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

enum class GameState {
    HOME,
    MENU,
    PLAYING,
    SETTINGS_PLAYING,
    LOAD_GAME,
    END_GAME,
    GAME_MODE,
    BOARD_MODE,
    OPTION,
    CHOOSE_MODE,
    NONE
};

enum class AIState {
    NONE,
    EASY_PLAY,
    MEDIUM_PLAY,
    HARD_PLAY
};

struct LongSound{
    Mix_Music* music;
    int volume;
    bool muted;
    LongSound(Mix_Music* _music, int _volume, bool _muted) : music(_music), volume(_volume), muted(_muted) {}

    void switchMute(){
        muted = !muted;
        if (muted){
            Mix_VolumeMusic(0);
        } else {
            Mix_VolumeMusic(volume * 20);
        }
    }

    void adjustVolume(int x){
        volume += x;
        if(volume < 0) volume = 0;
        if(volume > 5) volume = 5;
        if(music != nullptr && !muted){
            Mix_VolumeMusic(volume * 20);
        }
    }

    void play(){
        Mix_VolumeMusic(volume * 20);
        Mix_PlayMusic(music, -1);
    }

    void MixFree(){
        if(music != nullptr){
            Mix_FreeMusic(music);
            music = nullptr;
        }
    }
};

struct ShortSound{
    Mix_Chunk* chunk;
    int volume;
    bool muted;

    ShortSound(Mix_Chunk* _chunk, int _volume, bool _muted) : chunk(_chunk), volume(_volume), muted(_muted){}
    
    void adjustVolume(int x){
        volume += x;
        if(volume < 0) volume = 0;
        if(volume > 5) volume = 5;
    }

    void switchMute(){
        muted = !muted;
    }

    void play(){
        if (muted) return;
        Mix_VolumeChunk(chunk, volume * 20);
        Mix_PlayChannel(-1, chunk, 0);
    }

    void MixFree(){
        if(chunk != nullptr){
            Mix_FreeChunk(chunk);
            chunk = nullptr;
        }
    }
};

int RUN_PLAYING(SDL_Window* window, SDL_Renderer* renderer);