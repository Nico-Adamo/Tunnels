#include "game.h"

const size_t NUM_TILES = 40; //TODO: update
const size_t NUM_SPRITES = 40;

typedef struct game {
    list_t *tile_infos;
    list_t *sprites;
    list_t *tile_interactors;
    scene_t *current_scene;
    double scale;
    body_t *player;
    list_t *dungeon;
    size_t cur_room;
    size_t cur_level;
    bool paused;
} game_t;

typedef struct sprite_info {
    SDL_Rect shape; // Actual pixel shape, eg 16x16 or 16x32
    SDL_Rect collision_shape;
    SDL_Rect hitbox_shape;
    const char *path;
    bool is_animated;
    double animation_speed;
    size_t animation_frames;
} sprite_info_t;

const sprite_info_t SPRITE_INFOS[] = {
    // **** PLAYER RELATED *****
    // 0 - PLAYER
    {{0, 0, 16, 28},
    {3, 0, 12, 6},
    {3, 0, 9, 16},
    "assets/knight_f_idle_anim_full.png",
    true, .125, 4
    },
    // 1 - RUNNING PLAYER
    {{0, 0, 16, 28},
    {3, 0, 12, 6},
    {3, 0, 9, 16},
    "assets/knight_f_run_anim_full.png",
    true, .1, 4
    },
    // 2 - HIT PLAYER
    {{0, 0, 16, 28},
    {3, 0, 12, 6},
    {3, 0, 9, 16},
    "assets/knight_f_hit_anim_full.png",
    true, .25, 2
    },
    // 3 - PLAYER BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/player_bullet.png",
    false, 0, 0
    },
    // 4 - TINY ZOMBIE IDLE
    {{0, 0, 16, 16},
    {5, 0, 9, 10},
    {5, 0, 9, 10},
    "assets/tiny_zombie_idle_anim_full.png",
    true, .25, 4
    },
    // 5 - TINY ZOMBIE RUN
    {{0, 0, 16, 16},
    {5, 0, 9, 10},
    {5, 0, 9, 10},
    "assets/tiny_zombie_run_anim_full.png",
    true, .25, 4
    },
    // 6 - GOBLIN IDLE
    {{0, 0, 16, 16},
    {5, 0, 9, 10},
    {5, 0, 9, 10},
    "assets/goblin_idle_anim_full.png",
    true, .25, 4
    },
    // 7 - GOBLIN RUN
    {{0, 0, 16, 16},
    {5, 0, 9, 10},
    {5, 0, 9, 10},
    "assets/goblin_run_anim_full.png",
    true, .25, 4
    },
    // 8 - IMP IDLE
    {{0, 0, 16, 16},
    {5, 0, 9, 10},
    {5, 0, 9, 10},
    "assets/imp_idle_anim_full.png",
    true, .25, 4
    },
    // 9 - IMP RUN
    {{0, 0, 16, 16},
    {4, 0, 9, 10},
    {4, 0, 9, 10},
    "assets/imp_run_anim_full.png",
    true, .25, 4
    },
    // 10 - SKELET IDLE
    {{0, 0, 16, 16},
    {4, 0, 10, 14},
    {4, 0, 10, 14},
    "assets/skelet_idle_anim_full.png",
    true, .25, 4
    },
    // 11 - SKELET RUN
    {{0, 0, 16, 16},
    {4, 0, 10, 14},
    {4, 0, 10, 14},
    "assets/skelet_run_anim_full.png",
    true, .25, 4
    },
    // 12 - MUDDY IDLE/RUN
    {{0, 0, 16, 16},
    {0, 0, 16, 13},
    {0, 0, 16, 13},
    "assets/muddy_anim_full.png",
    true, .25, 4
    },
    // 13 - SWAMPY IDLE/RUN
    {{0, 0, 16, 16},
    {4, 0, 16, 13},
    {4, 0, 16, 13},
    "assets/swampy_anim_full.png",
    true, .25, 4
    },
    // 14 - ZOMBIE IDLE/RUN
    {{0, 0, 16, 16},
    {5, 0, 8, 16},
    {5, 0, 8, 16},
    "assets/zombie_anim_full.png",
    true, .25, 4
    },
    // 15 - ICE_ZOMBIE IDLE/RUN
    {{0, 0, 16, 16},
    {5, 0, 8, 16},
    {5, 0, 8, 16},
    "assets/ice_zombie_anim_full.png",
    true, .25, 4
    },
    // 16 - MASKED_ORC IDLE
    {{0, 0, 16, 20},
    {4, 0, 12, 16},
    {4, 0, 12, 16},
    "assets/masked_orc_idle_anim_full.png",
    true, .25, 4
    },
    // 17 - MASKED_ORC RUN
    {{0, 0, 16, 20},
    {4, 0, 12, 16},
    {4, 0, 12, 16},
    "assets/masked_orc_run_anim_full.png",
    true, .25, 4
    },
    // 18 - ORC_WARRIOR IDLE
    {{0, 0, 16, 20},
    {5, 0, 11, 16},
    {5, 0, 11, 16},
    "assets/orc_warrior_idle_anim_full.png",
    true, .25, 4
    },
    // 19 - ORC_WARRIOR RUN
    {{0, 0, 16, 20},
    {5, 0, 11, 16},
    {5, 0, 11, 16},
    "assets/orc_warrior_run_anim_full.png",
    true, .25, 4
    },
    // 20 - ORC_SHAMAN IDLE
    {{0, 0, 16, 20},
    {5, 0, 11, 16},
    {5, 0, 11, 16},
    "assets/orc_shaman_idle_anim_full.png",
    true, .25, 4
    },
    // 21 - ORC_SHAMAN RUN
    {{0, 0, 16, 20},
    {5, 0, 11, 16},
    {5, 0, 11, 16},
    "assets/orc_shaman_run_anim_full.png",
    true, .25, 4
    },
    // 22 - NECROMANCER IDLE/RUN
    {{0, 0, 16, 20},
    {2, 0, 13, 16},
    {2, 0, 13, 16},
    "assets/necromancer_anim_full.png",
    true, .25, 4
    },
    // 23 - WOGUL IDLE
    {{0, 0, 16, 24},
    {4, 0, 10, 16},
    {4, 0, 10, 16},
    "assets/wogul_idle_anim_full.png",
    true, .25, 4
    },
    // 24 - WOGUL RUN
    {{0, 0, 16, 24},
    {4, 0, 10, 16},
    {4, 0, 10, 16},
    "assets/wogul_run_anim_full.png",
    true, .25, 4
    },
    // 25 - CHORT IDLE
    {{0, 0, 16, 24},
    {4, 0, 10, 18},
    {4, 0, 10, 18},
    "assets/chort_idle_anim_full.png",
    true, .25, 4
    },
    // 26 - CHORT RUN
    {{0, 0, 16, 24},
    {4, 0, 10, 18},
    {4, 0, 10, 18},
    "assets/chort_run_anim_full.png",
    true, .25, 4
    },
    // 27 - WIZARD IDLE
    {{0, 0, 16, 28},
    {4, 0, 11, 20},
    {4, 0, 11, 20},
    "assets/wizzard_m_idle_anim_full.png",
    true, .25, 4
    },
    // 28 - WIZARD RUN
    {{0, 0, 16, 28},
    {4, 0, 11, 18},
    {4, 0, 11, 18},
    "assets/wizzard_m_run_anim_full.png",
    true, .25, 4
    },
    // 29 - NECROMANCER_WIZARD IDLE
    {{0, 0, 16, 28},
    {4, 0, 11, 18},
    {4, 0, 11, 18},
    "assets/necromancer_wizard_run_anim_full.png",
    true, .25, 4
    },
    // 30 - NECROMANCER_WIZARD RUN
    {{0, 0, 16, 28},
    {4, 0, 11, 18},
    {4, 0, 11, 18},
    "assets/necromancer_wizard_run_anim_full.png",
    true, .25, 4
    },
    // 31 - BIG_ZOMBIE IDLE
    {{0, 0, 32, 36},
    {8, 0, 18, 27},
    {8, 0, 18, 27},
    "assets/big_zombie_idle_anim_full.png",
    true, .25, 4
    },
    // 32 - BIG_ZOMBIE RUN
    {{0, 0, 32, 36},
    {8, 0, 18, 27},
    {8, 0, 18, 27},
    "assets/big_zombie_run_anim_full.png",
    true, .25, 4
    },
    // 33 - OGRE IDLE
    {{0, 0, 32, 36},
    {8, 0, 21, 26},
    {8, 0, 21, 26},
    "assets/ogre_idle_anim_full.png",
    true, .25, 4
    },
    // 34 - OGRE RUN
    {{0, 0, 32, 36},
    {8, 0, 21, 26},
    {8, 0, 21, 26},
    "assets/ogre_run_anim_full.png",
    true, .25, 4
    },
    // 35 - BIG_DEMON IDLE
    {{0, 0, 32, 36},
    {6, 0, 22, 30},
    {6, 0, 22, 30},
    "assets/big_demon_idle_anim_full.png",
    true, .25, 4
    },
    // 36 - BIG_DEMON RUN
    {{0, 0, 32, 36},
    {6, 0, 22, 30},
    {6, 0, 22, 30},
    "assets/big_demon_run_anim_full.png",
    true, .25, 4
    },
    // 37 - DEMON BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/demon_bullet.png",
    false, 0, 0
    },
    // 38 - ICE ZOMBIE BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/ice_zombie_bullet.png",
    false, 0, 0
    },
    // 39 - MUDDY BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/muddy_bullet.png",
    false, 0, 0
    },
    // 40 - NECROMANCER BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/necromancer_bullet.png",
    false, 0, 0
    },
    // 41 - NECROMANCER WIZARD BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/necromancer_wizard_bullet.png",
    false, 0, 0
    },
    // 42 - ORC BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/orc_bullet.png",
    false, 0, 0
    },
    // 43 - ORC SHAMAN BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/orc_shaman_bullet.png",
    false, 0, 0
    },
    // 44 - SKELET BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/skelet_bullet.png",
    false, 0, 0
    },
    // 45 - SWAMPY BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/swampy_bullet.png",
    false, 0, 0
    },
    // 46 - ZOMBIE BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/zombie_bullet.png",
    false, 0, 0
    },
    // 47 - WIZARD BULLET
    {{0, 0, 7, 7},
    {0, 0, 7, 7},
    {0, 0, 7, 7},
    "assets/wizzard_bullet.png",
    false, 0, 0
    },
    // 48 - START SCREEN
    {{0, 0, 2000, 1000},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    "assets/ui/game_start_screen_full.png",
    true, 0.1, 4
    },
    // 49 - PRESS_F UI
    {{0, 0, 600, 230},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    "assets/ui/press_f_full2.png",
    true, 0.6, 2
    },
    // 50 - EMPTY HEART
    {{0, 0, 16, 16},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    "assets/ui/ui_heart_empty.png",
    false, 0, 0
    },
    // 51 - HALF HEART
    {{0, 0, 16, 16},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    "assets/ui/ui_heart_half.png",
    false, 0, 0
    },
    // 52 - HALF HEART
    {{0, 0, 16, 16},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    "assets/ui/ui_heart_full.png",
    false, 0, 0
    },
    // 53 – HEALTH POWERUP
    {{0, 0, 259, 181},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    "assets/ui/health_powerup.png",
    false, 0, 0
    },
    // 54 – ATTACK POWERUP
    {{0, 0, 259, 181},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    "assets/ui/attack_powerup.png",
    false, 0, 0
    },
    // 55 – BULLET COOLDOWN POWERUP
    {{0, 0, 423, 181},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    "assets/ui/cooldown_powerup.png",
    false, 0, 0
    },
    // 56 – SPEED POWERUP
    {{0, 0, 259, 181},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    "assets/ui/speed_powerup.png",
    false, 0, 0
    },
    // 57 – INVULNERABILITY POWERUP
    {{0, 0, 391, 181},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    "assets/ui/invulnerability_powerup.png",
    false, 0, 0
    },
    // 58 – BLACKER MURAL
    {{0, 0, 1976, 988},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    "assets/ui/blacker_mural.png",
    false, 0, 0
    },
};

game_t *game_init(double scale) {
    game_t *game = malloc(sizeof(game_t));

    game->current_scene = NULL;
    game->tile_infos = list_init(NUM_TILES, tile_info_free);
    game->tile_interactors = list_init(2, free); // TODO: Magic number
    game->sprites = list_init(NUM_SPRITES, tile_info_free);
    game->scale = scale;
    game->player = NULL;
    game->dungeon = list_init(5, free); // Todo: magic number
    game->cur_room = 0;
    game->cur_level = 0;
    game->paused = false;

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

tile_interactor_t *tile_interactor_init(rect_t area, tile_interaction interaction, enum tile_interactor_type type) {
    tile_interactor_t *tile_interactor = malloc(sizeof(tile_interactor_t));
    tile_interactor->area = area;
    tile_interactor->interaction = interaction;
    tile_interactor->type = type;
    return tile_interactor;
}

void game_add_tile_interactor(game_t *game, tile_interactor_t *interactor) {
    list_add(game->tile_interactors, interactor);
}

list_t *game_get_tile_interactors(game_t *game) {
    return game->tile_interactors;
}

void game_reset_tile_interactors(game_t *game) {
    game->tile_interactors = list_init(2, free); // Todo: magic number
}

list_t *game_get_dungeon(game_t *game) {
    return game->dungeon;
}

void game_reset_dungeon(game_t *game) {
    game->dungeon = list_init(5, free); // Todo: magic number
}

void game_add_room(game_t *game, const char *path) {
    list_add(game->dungeon, path);
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
        game_add_sprite(game, sprite_init(sprite_info.path, sprite_info.shape, sprite_info.collision_shape, sprite_info.hitbox_shape, sprite_info.is_animated, sprite_info.animation_speed, sprite_info.animation_frames));
    }
}

size_t game_get_room(game_t *game) {
    return game->cur_room;
}

void game_next_room(game_t *game) {
    game->cur_room++;
}

void game_set_room(game_t *game, int room) {
    game->cur_room = room;
}

bool game_is_paused(game_t *game) {
    return game->paused;
}

void game_set_paused(game_t *game, bool paused) {
    game->paused = paused;
}
