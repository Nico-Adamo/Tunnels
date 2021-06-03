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
    "assets/door_top_m_left_boss.png", // 61
    "assets/door_top_m_right_boss.png", // 62
    "assets/door_bottom_m_left_boss.png", // 63
    "assets/door_bottom_m_right_boss.png" // 64
};

SDL_Rect COLLIDER_TILES[]= {
    {0, 0, 16, 16}, // 0 - Full
    {0, 0, 4, 16}, // 1 - Left quarter
    {12, 0, 4, 16}, // 2 - Right quarter
    {0, 0, 16, 8}, // 3 - Bottom half
    {12, 0, 4, 8}, // 4 - Right quarter, half height
    {0, 0, 4, 8} // 5 - Left quarter, half height
};

tile_info_t *COLLIDER_TILE_INFOS[6];

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
                    int tile_id = *tile_id_buffer;
                    if(game_get_room(game) == game_get_dungeon_size(game) - 3) {
                        if(tile_id == 28) tile_id = 61;
                        if(tile_id == 29) tile_id = 62;
                        if(tile_id == 32) tile_id = 63;
                        if(tile_id == 33) tile_id = 64;
                    }
                    tile_info_t *tile_info = game_get_tile_info(game, tile_id);
                    tile_t *tile = tile_init(tile_info, (rect_t) {game_scale*x*TILE_SIZE, game_scale*y*TILE_SIZE, game_scale*TILE_SIZE, game_scale*TILE_SIZE});
                    scene_add_floor_tile(scene, tile);

                    if(*tile_id_buffer == 32 || *tile_id_buffer == 33) { // Door tile IDS
                        tile_interactor_t *interactor = tile_interactor_init((rect_t) {game_scale*x*TILE_SIZE, game_scale*(y-1)*TILE_SIZE, game_scale*TILE_SIZE, game_scale*TILE_SIZE}, game_end_room, PRESS_F);
                        game_add_tile_interactor(game, interactor);

                    } else if(*tile_id_buffer == 37) { // Mural
                        tile_interactor_t *interactor = tile_interactor_init((rect_t) {game_scale*x*TILE_SIZE, game_scale*(y-1)*TILE_SIZE, game_scale*TILE_SIZE, game_scale*TILE_SIZE}, game_random_mural, MURAL);
                        game_add_tile_interactor(game, interactor);
                    } else if (*tile_id_buffer == 26) { // Ladder
                       tile_interactor_t *interactor = tile_interactor_init((rect_t) {game_scale*x*TILE_SIZE, game_scale*(y)*TILE_SIZE, game_scale*TILE_SIZE, game_scale*TILE_SIZE}, game_end_level, PRESS_F);
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
                } else if(tile_type == 3) {
                    if(*tile_id_buffer == 100) {
                        body_set_centroid(game_get_player(game), (vector_t) {game_scale*x*TILE_SIZE, game_scale*y*TILE_SIZE});
                    } else {
                        int enemy_id;
                        if(*tile_id_buffer == 19) {
                            enemy_id = rand() % 4; // Random small enemy
                        } else if(*tile_id_buffer == 20) {
                            enemy_id = rand() % 6 + 4; // Random medium enemy
                        } else if(*tile_id_buffer == 21) {
                            enemy_id = rand() % 5 + 10; // Random large enemy
                        } else {
                            enemy_id = *tile_id_buffer;
                        }
                        body_t *enemy = make_enemy(game, game_scale*x*TILE_SIZE, game_scale*y*TILE_SIZE, enemy_id);
                        scene_add_body(scene, enemy);
                        create_tile_collision(game_get_current_scene(game), enemy);
                        create_enemy_collision(game_get_current_scene(game), enemy, game_get_player(game));
                    }
                }
            }
        }

        fscanf(file, "\n");
    }
    free(tile_id_buffer);
}

void map_load(game_t *game, const char *path) {
    scene_t *scene = game_get_current_scene(game);

    int *x_tiles = malloc(sizeof(int));
    int *y_tiles = malloc(sizeof(int));

    int *room_type = malloc(sizeof(int));
    double *unlock_time = malloc(sizeof(double));

    FILE *file = fopen(path, "r");

    fscanf(file, "x: %d\n", x_tiles);
    fscanf(file, "y: %d\n", y_tiles);

    fscanf(file, "room type: %d\n", room_type);
    fscanf(file, "unlock time: %lf\n\n", unlock_time);

    scene_set_room_type(scene, (room_type_t) *room_type);
    scene_set_unlock_time(scene, *unlock_time);

    map_load_file(game, file, *x_tiles, *y_tiles, 0);
    fscanf(file,"\n");
    map_load_file(game, file, *x_tiles, *y_tiles, 1);
    fscanf(file,"\n");
    map_load_file(game, file, *x_tiles, *y_tiles, 2);
    fscanf(file,"\n");
    map_load_file(game, file, *x_tiles, *y_tiles, 3);
    fclose(file);

    free(x_tiles);
    free(y_tiles);
    free(room_type);
    free(unlock_time);
}
