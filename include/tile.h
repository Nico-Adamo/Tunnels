#ifndef __TILE_H__
#define __TILE_H__

#include "body.h"

typedef struct tile_info {
    SDL_Texture *texture;
    SDL_Rect shape;
} tile_info_t;

typedef struct tile {
    tile_info_t *info;
    rect_t hitbox;
} tile_t;

tile_info_t *tile_info_init(const char *path, SDL_Rect shape);

tile_t *tile_init(tile_info_t *tile_info, rect_t hitbox);

void tile_info_free(void *tile_info);

void tile_free(void *tile);

void tile_draw(tile_t *tile);


#endif // #ifndef __TILE_H__