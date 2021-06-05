#ifndef __TILE_H__
#define __TILE_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include "body.h"

/**
 * Tile information for a tile.
 * Contains tile texture and shape.
 */
typedef struct tile_info {
    SDL_Texture *texture;
    SDL_Rect shape;
} tile_info_t;

/**
 * A tile.
 * Contains tile info and hitbox.
 */
typedef struct tile {
    tile_info_t *info;
    rect_t hitbox;
} tile_t;

/**
 * Allocates memory for and initializes a tile.
 *
 * @param tile_info the tile information for the tile
 * @param hitbox the tile's hitbox
 * @return the initialized tile
 */
tile_t *tile_init(tile_info_t *tile_info, rect_t hitbox);

/**
 * Releases the memory allocated for a given tile.
 *
 * @param tile a pointer to the tile to be freed
 */
void tile_free(void *tile);

/**
 * Allocates memory for and initalizes a tile info.
 *
 * @param path the file path to the tile's texture
 * @param shape the tile's shape
 * @return the initialized tile info
 */
tile_info_t *tile_info_init(const char *path, SDL_Rect shape);

/**
 * Allocatoes memory for and initializes a tile info for a tile collider.
 *
 * @param shape the tile collider's shape
 * @return the initialized tile info
 */
tile_info_t *tile_collider_info_init(SDL_Rect shape);

/**
 * Releases the memory allocated for a given tile info.
 *
 * @param tile_info a pointer to the tile info to be freed
 */
void tile_info_free(void *tile_info);

/**
 * Draws a tile using sdl_draw_texture()
 *
 * @param tile a pointer to the tile to be drawn
 */
void tile_draw(tile_t *tile);

/**
 * Returns the tile info associated with a tile.
 *
 * @param tile a pointer to a tile
 * @return the tile info for the tile
 */
tile_info_t *tile_get_info(tile_t *tile);

/**
 * Returns the hitbox associated with a tile.
 *
 * @param tile a pointer to a tile
 * @return the hitbox (rect_t) for the tile
 */
rect_t tile_get_hitbox(tile_t *tile);

#endif // #ifndef __TILE_H__
