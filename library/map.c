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
    "assets/wall_inner_corner_l_top_right.png", // 12,
    "assets/wall_corner_bottom_left.png", // 13
    "assets/wall_corner_bottom_right.png", // 14
    "assets/wall_corner_front_left.png", // 15
    "assets/wall_corner_front_right.png", // 16
    "assets/wall_side_top_left.png", // 17
    "assets/wall_side_top_right.png", // 18
    "assets/floor_2.png", // 19
    "assets/floor_3.png", // 20
    "assets/floor_4.png", // 21
    "assets/floor_5.png", // 22
    "assets/floor_6.png", // 23
    "assets/floor_7.png", // 24
    "assets/floor_8.png", // 25
    "assets/floor_ladder.png", // 26
    "assets/door_top_left.png", // 27
    "assets/door_top_m_left.png", // 28
    "assets/door_top_m_right.png", // 29
    "assets/door_top_right.png", // 30
    "assets/door_bottom_left.png", // 31
    "assets/door_bottom_m_left.png", // 32
    "assets/door_bottom_m_right.png", // 33
    "assets/door_bottom_right.png", // 34
    "assets/door_cap_left.png", // 35
    "assets/door_cap_right.png", // 36
    "assets/wall_mid_mural.png", // 37
    "assets/wall_banner_red.png", // 38
    "assets/wall_banner_yellow.png", // 39
    "assets/wall_banner_green.png", // 40
    "assets/wall_banner_blue.png", // 41
    "assets/wall_banner_black.png", // 42
    "assets/wall_column_top.png", // 43
    "assets/wall_column_mid.png", // 44
    "assets/wall_column_base.png", // 45
    "assets/column_base.png", // 46
    "assets/wall_hole_1.png", // 47
    "assets/wall_hole_2.png", // 48
    "assets/wall_goo.png", // 49
    "assets/wall_goo_base.png", // 50
    "assets/skull_floor_1.png", // 51
    "assets/skull_floor_2.png", // 52
    "assets/skull_floor_3.png", // 53
    "assets/skull_floor_4.png", // 54
    "assets/skull_floor_5.png", // 55
    "assets/skull_floor_6.png", // 56
    "assets/skull_floor_7.png", // 57
    "assets/skull_floor_8.png", // 58
    "assets/eyes_wall_hole_1.png", // 59
    "assets/eyes_wall_hole_2.png", // 60
};

SDL_Rect COLLIDER_TILES[]= {
    {0, 0, 16, 16}, // 0 - Full
    {0, 0, 4, 16}, // 1 - Left quarter
    {12, 0, 4, 16}, // 2 - Right quarter
    {0, 0, 16, 8}, // 3 - Bottom half
    {12, 0, 4, 8} // 4 - Right quarter, half height
};

tile_info_t *COLLIDER_TILE_INFOS[5];

const int TILE_SIZE = 16;

void map_register_tiles(game_t *game) {
    for(int i = 0; i< sizeof(TILE_PATHS) / sizeof(const char*); i++) {
        game_add_tile_info(game, tile_info_init(TILE_PATHS[i], (SDL_Rect) {0, 0, TILE_SIZE, TILE_SIZE}));
    }
}

void map_register_collider_tiles() {
    for (size_t i = 0; i < sizeof(COLLIDER_TILE_INFOS) / sizeof(tile_info_t *); i++) {
        // COLLIDER_TILE_INFOS[i] = tile_collider_info_init(COLLIDER_TILES[i]);
        COLLIDER_TILE_INFOS[i] = tile_info_init("assets/collider_tile_temp.png", COLLIDER_TILES[i]);
    }
}

tile_info_t *map_get_collider_tile_info(size_t tile_id) {
    return COLLIDER_TILE_INFOS[tile_id];
}

void add_collider_tile(scene_t *scene, int tile_id, int x, int y, double game_scale) {
    tile_info_t *tile_info = map_get_collider_tile_info(tile_id);
    tile_t *tile = tile_init(tile_info, (rect_t) {game_scale*(x*TILE_SIZE + tile_info->shape.x), game_scale*(y*TILE_SIZE + tile_info->shape.y), game_scale*tile_info->shape.w, game_scale*tile_info->shape.h});
    scene_add_collider_tile(scene, tile);
}

void map_load_file(game_t *game, FILE *file, size_t x_tiles, size_t y_tiles, uint8_t tile_type) {
    scene_t *scene = game_get_current_scene(game);
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
                    if(*tile_id_buffer == 32 || *tile_id_buffer == 33) {
                        tile_interactor_t *interactor = tile_interactor_init((rect_t) {game_scale*x*TILE_SIZE, game_scale*(y-1)*TILE_SIZE, game_scale*TILE_SIZE, game_scale*TILE_SIZE}, game_end_level);
                        game_add_tile_interactor(game, interactor);
                    }
                } else if(tile_type == 1) {
                    tile_info_t *tile_info = game_get_tile_info(game, *tile_id_buffer);
                    tile_t *tile = tile_init(tile_info, (rect_t) {game_scale*x*TILE_SIZE, game_scale*y*TILE_SIZE, game_scale*TILE_SIZE, game_scale*TILE_SIZE});
                    scene_add_wall_tile(scene, tile);

                } else if(tile_type == 2) {
                    // (Ugly hard code) Checks for corner overlap collisions; ID represents combination of tiles
                    if (*tile_id_buffer == 31) {
                        add_collider_tile(scene, 3, x, y, game_scale);
                        add_collider_tile(scene, 1, x, y, game_scale);
                    } else if (*tile_id_buffer == 32) {
                        add_collider_tile(scene, 2, x, y, game_scale);
                        add_collider_tile(scene, 3, x, y, game_scale);
                    } else {
                        add_collider_tile(scene, *tile_id_buffer, x, y, game_scale);
                    }
                }
            }
        }

        fscanf(file, "\n");
    }
    free(tile_id_buffer);
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
