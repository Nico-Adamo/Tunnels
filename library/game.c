#include "game.h"

const size_t NUM_TILES = 40; //TODO: update
const size_t NUM_SPRITES = 40;

typedef struct game {
    list_t *tile_infos;
    list_t *sprites;
    scene_t *current_scene;
    double scale;
    body_t *player;
} game_t;

typedef struct sprite_info {
    SDL_Rect shape; // Actual pixel shape, eg 16x16 or 16x32
    SDL_Rect collision_shape;
    SDL_Rect hitbox_shape;
    const char *path;
} sprite_info_t;

const sprite_info_t SPRITE_INFOS[] = {
    // **** PLAYER RELATED *****
    // 0 - PLAYER
    {{0, 0, 16, 32},
    {3, 0, 12, 6},
    {3, 0, 9, 16},
    "assets/knight_f_idle_anim_f0.png"
    },
    // 1 - HORIZONTAL BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/circle_bullet_test_3.png"
    },
    // 2 - ENEMY  BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/circle_bullet_test_2.png"
    },

};

game_t *game_init(double scale) {
    game_t *game = malloc(sizeof(game_t));

    game->current_scene = NULL;
    game->tile_infos = list_init(NUM_TILES, tile_info_free);
    game->sprites = list_init(NUM_SPRITES, tile_info_free);
    game->scale = scale;
    game->player = NULL;

    return game;
}

void game_free(void *game) {
    game_t *game_c = (game_t *) game;
    list_free(game_c->tile_infos);
    scene_free(game_c->current_scene);
    free(game_c);
}

void game_add_tile_info(game_t *game, tile_info_t *tile_info) {
    list_add(game->tile_infos, tile_info);
}

void game_add_sprite(game_t *game, sprite_t *sprite) {
    list_add(game->sprites, sprite);
}

scene_t *game_get_current_scene(game_t *game) {
    return game->current_scene;
}

void game_set_current_scene(game_t *game, scene_t *scene) {
    game->current_scene = scene;
}

double game_get_scale(game_t *game) {
    return game->scale;
}

body_t *game_get_player(game_t *game) {
    return game->player;
}

void game_set_player(game_t *game, body_t *player) {
    game->player = player;
}

tile_info_t *game_get_tile_info(game_t *game, size_t tile_id) {
    return list_get(game->tile_infos, tile_id);
}

sprite_t *game_get_sprite(game_t *game, size_t sprite_id) {
    return list_get(game->sprites, sprite_id);
}

void game_register_sprites(game_t *game) {
    for(int i = 0; i< sizeof(SPRITE_INFOS) / sizeof(sprite_info_t); i++) {
        sprite_info_t sprite_info = SPRITE_INFOS[i];
        game_add_sprite(game, sprite_init(sprite_info.path, sprite_info.shape, sprite_info.collision_shape, sprite_info.hitbox_shape));
    }
}
