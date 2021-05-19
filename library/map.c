#include "map.h"

void map_load(game_t *game, const char *path, size_t x_tiles, size_t y_tiles) {
    scene_t *scene = game->current_scene;
    FILE *file = fopen(path, "r");
    int *tile_id_buffer = malloc(sizeof(int));

    for (size_t y = 0; y < y_tiles; y++) {
        for (size_t x = 0; x < x_tiles; x++) {
            fscanf(file, "%d,", tile_id_buffer);

            tile_info_t *tile_info = game_get_tile_info(game, tile_id_buffer);
            tile_t *tile = tile_init(tile_info, (rect_t) {x*tile_info->shape.w, y*tile_info->shape.h});
            scene_add_tile(scene, tile);
        }
        fscanf(file, "\n");
    }
}

//TODO: multiple layers