#ifndef __TILE_H__
#define __TILE_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include "body.h"

typedef struct tile_info {
    SDL_Texture *texture;
    SDL_Rect shape;
} tile_info_t;

typedef struct tile {
    tile_info_t *info;
    rect_t hitbox;
} tile_t;


tile_t *tile_init(tile_info_t *tile_info, rect_t hitbox);

void tile_free(void *tile);

tile_info_t *tile_info_init(const char *path, SDL_Rect shape);

void tile_info_free(void *tile_info);

void tile_draw(tile_t *tile);

tile_info_t *tile_get_info(tile_t *tile);

rect_t tile_get_hitbox(tile_t *tile);

#endif // #ifndef __TILE_H__
