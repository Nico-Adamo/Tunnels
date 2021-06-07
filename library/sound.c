#include "sound.h"

const int LOOPS = -1;
const int MS = 500;

void sound_play(const char *path) {
    Mix_Chunk *waves = Mix_LoadWAV(path);
    Mix_PlayChannel(-1, waves, 0);
}

void music_play(const char *path) {
    if(Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
    Mix_Music *music = Mix_LoadMUS(path);
    int ret = Mix_FadeInMusic(music, LOOPS, MS); // TODO: magic number
}
