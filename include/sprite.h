#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>

typedef struct sprite sprite_t;

sprite_t *sprite_init(const char *path, SDL_Rect shape, SDL_Rect collision_shape, SDL_Rect hitbox_shape);

// Getters

SDL_Rect sprite_get_shape(sprite_t *sprite);

SDL_Rect sprite_get_collision_shape(sprite_t *sprite);

SDL_Rect sprite_get_hitbox_shape(sprite_t *sprite);

SDL_Texture *sprite_get_texture(sprite_t *sprite);

// Setters

void sprite_set_shape(sprite_t *sprite, SDL_Rect shape);

void sprite_set_collision_shape(sprite_t *sprite, SDL_Rect collision_shape);

void sprite_set_hitbox_shape(sprite_t *sprite, SDL_Rect hitbox_shape);

void sprite_set_texture(sprite_t *sprite, SDL_Texture *texture);

#endif // #ifndef __SPRITE_H__
