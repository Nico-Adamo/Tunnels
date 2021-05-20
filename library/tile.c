#include "tile.h"
#include "sdl_wrapper.h"

tile_t *tile_init(tile_info_t *tile_info, rect_t hitbox) {
    tile_t *tile = malloc(sizeof(tile_t));

    tile->info = tile_info;
    tile->hitbox = hitbox;

    return tile;
}

tile_info_t *tile_info_init(const char *path, SDL_Rect shape) {
    tile_info_t *tile_info = malloc(sizeof(tile_info_t));

    tile_info->texture = sdl_load_texture(path);
    tile_info->shape = shape;

    return tile_info;
}

tile_info_t *tile_collider_info_init(SDL_Rect shape) {
        tile_info_t *tile_info = malloc(sizeof(tile_info_t));

        tile_info->texture = NULL;
        tile_info->shape = shape;

        return tile_info;
}

void tile_info_free(void *tile_info) {
    SDL_DestroyTexture(((tile_info_t *) tile_info)->texture);
    free((tile_info_t *) tile_info);
}

void tile_draw(tile_t *tile) {
    sdl_draw_texture(tile->info->texture, tile->info->shape, tile->hitbox, false);
}

tile_info_t *tile_get_info(tile_t *tile) {
    return tile->info;
}

rect_t tile_get_hitbox(tile_t *tile) {
    return tile->hitbox;
}

void tile_free(void *tile) {
    free((tile_t *) tile);
}
