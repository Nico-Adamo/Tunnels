#ifndef __SOUND_H__
#define __SOUND_H__

#include <SDL2/SDL_mixer.h>

/**
 * Plays a specified sound.
 *
 * @param path the file path to the sound to play
 */
void sound_play(const char *path);

/**
 * Plays the specified music.
 * If other music is playing, stops it and then begins to play the new music.
 *
 * @param path the file path to the music to play
 */
void music_play(const char *path);

#endif
