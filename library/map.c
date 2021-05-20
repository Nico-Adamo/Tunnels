#include "map.h"
#include <stdint.h>

const char *TILE_PATHS[] = {
    "assets/floor_1.png", // 0
    "assets/wall_mid.png", // 1
    "assets/wall_left.png", // 2
    "assets/wall_right.png", // 3
    "assets/wall_top_mid.png", // 4
    "assets/wall_inner_corner_mid_left.png", // 5
    "assets/wall_inner_corner_mid_right.png", // 6
    "assets/wall_inner_corner_t_top_left.png", // 7
    "assets/wall_inner_corner_t_top_right.png", // 8
    "assets/wall_side_mid_left.png", // 9
    "assets/wall_side_mid_right.png", // 10
    "assets/wall_inner_corner_l_top_left.png", // 11
    "assets/wall_inner_corner_l_top_right.png" // 12
};

SDL_Rect COLLIDER_TILES[]= {
    {0, 0, 16, 16}, // 0
    {0, 0, 4, 16}, // 1
    {12, 0, 4, 16}, // 2
    {0, 0, 16, 8} // 3
};

tile_info_t *COLLIDER_TILE_INFOS[4];

const int TILE_SIZE = 16;

void map_register_tiles(game_t *game) {
    for(int i = 0; i< sizeof(TILE_PATHS) / sizeof(const char*); i++) {
        game_add_tile_info(game, tile_info_init(TILE_PATHS[i], (SDL_Rect) {0, 0, TILE_SIZE, TILE_SIZE}));
    }
}

void map_register_collider_tiles() {
    for (size_t i = 0; i < sizeof(COLLIDER_TILE_INFOS) / sizeof(tile_info_t *); i++) {
        //COLLIDER_TILE_INFOS[i] = tile_collider_info_init(COLLIDER_TILES[i]);
        COLLIDER_TILE_INFOS[i] = tile_info_init("assets/collider_tile_temp.png", COLLIDER_TILES[i]);
    }    
}

tile_info_t *map_get_collider_tile_info(size_t tile_id) {
    return COLLIDER_TILE_INFOS[tile_id];
}

void map_load_file(game_t *game, FILE *file, size_t x_tiles, size_t y_tiles, uint8_t tile_type) {
    scene_t *scene = game->current_scene;
    int *tile_id_buffer = malloc(sizeof(int));
    double game_scale = game_get_scale(game);
    for (int y = y_tiles - 1; y >= 0; y--) {
        for (int x = 0; x < x_tiles; x++) {
            fscanf(file, "%d,", tile_id_buffer);

            if(*tile_id_buffer >= 0) {
                if(tile_type == 0) {
                    tile_info_t *tile_info = game_get_tile_info(game, *tile_id_buffer);
                    tile_t *tile = tile_init(tile_info, (rect_t) {game_scale*x*TILE_SIZE, game_scale*y*TILE_SIZE, game_scale*TILE_SIZE, game_scale*TILE_SIZE});
                    scene_add_floor_tile(scene, tile);
                
                } else if(tile_type == 1) {
                    tile_info_t *tile_info = game_get_tile_info(game, *tile_id_buffer);
                    tile_t *tile = tile_init(tile_info, (rect_t) {game_scale*x*TILE_SIZE, game_scale*y*TILE_SIZE, game_scale*TILE_SIZE, game_scale*TILE_SIZE});
                    scene_add_wall_tile(scene, tile);
                
                } else if(tile_type == 2) {
                    tile_info_t *tile_info = map_get_collider_tile_info(*tile_id_buffer);
                    tile_t *tile = tile_init(tile_info, (rect_t) {game_scale*(x*TILE_SIZE + tile_info->shape.x), game_scale*(y*TILE_SIZE + tile_info->shape.y), game_scale*tile_info->shape.w, game_scale*tile_info->shape.h});
                    scene_add_collider_tile(scene, tile);
                }
            }
        }

        fscanf(file, "\n");
    }

}
void map_load(game_t *game, const char *path, size_t x_tiles, size_t y_tiles) {
    FILE *file = fopen(path, "r");
    map_load_file(game, file, x_tiles, y_tiles, 0);
    fscanf(file,"\n");
    map_load_file(game, file, x_tiles, y_tiles, 1);
    fscanf(file,"\n");
    map_load_file(game, file, x_tiles, y_tiles, 2);
    fclose(file);
}

//TODO: collider layersw
