#include "tile.h"
#include "sdl_wrapper.h"

tile_info_t *tile_info_init(const char *path, SDL_Rect shape) {
    tile_info_t *tile_info = malloc(sizeof(tile_info));
    
    tile_info->texture = sdl_load_texture(path);
    tile_info->shape = shape;

    return tile_info;
}

tile_t *tile_init(tile_info_t *tile_info, rect_t hitbox) {
    tile_t *tile = malloc(sizeof(tile));

    tile->info = tile_info;
    tile->hitbox = hitbox;

    return tile;
}

void tile_info_free(void *tile_info) {
    SDL_DestroyTexture(((tile_info_t *) tile_info)->texture);
    free((tile_info_t *) tile_info);
}

void tile_free(void *tile) {
    free((tile_t *) tile);
}

void tile_draw(tile_t *tile) {
    sdl_draw_texture(tile->info->texture, tile->info->shape, tile->hitbox, false);
}