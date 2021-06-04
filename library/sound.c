#include "sound.h"

void sound_play(const char *path) {
    Mix_Chunk *waves = Mix_LoadWAV(path);
    Mix_PlayChannel(-1, waves, 0);
}

void music_play(const char *path) {
    if(Mix_PlayingMusic()) {
        Mix_FadeOutMusic(200);
    }
    Mix_Music *music = Mix_LoadMUS(path);
    int ret = Mix_FadeInMusic(music, -1, 500); // TODO: magic number
}
