#include "level.h"

const double MAX_WIDTH = 1024;
const double MAX_HEIGHT = 512;

const double HALF_HEART_HEALTH = 5;
const double PLAYER_HEALTH = 100;
const double HEART_PADDING = 4;
const size_t ROOMS_PER_LEVEL = 5;
const size_t TOTAL_LEVELS = 3;

const size_t VICTORY_ID = 69; // nice

char *level_variants[] = {
    "a_full.txt",
    "b_full.txt",
    "c_full.txt"
};

const char* LEVELS[] = {
    "assets/levels/room_01",
    "assets/levels/room_02",
    "assets/levels/room_03",
    "assets/levels/room_04",
    "assets/levels/room_05",
    "assets/levels/room_06",
    "assets/levels/room_07",
    "assets/levels/room_08",
    "assets/levels/room_09",
    "assets/levels/room_10",
    "assets/levels/room_11",
    "assets/levels/room_12",
    "assets/levels/room_13",
    "assets/levels/room_14",
    "assets/levels/room_17"
};

const char* BOSS_LEVELS[] = {
    "assets/levels/boss_room_01_orc_full.txt",
    "assets/levels/boss_room_01_zombie_full.txt",
    "assets/levels/boss_room_02_demon_full.txt",
    "assets/levels/boss_room_02_necromancer_full.txt"
};

const char* POST_BOSS_LEVELS[] = {
    "assets/levels/post_boss_room_level_1_full.txt",
    "assets/levels/post_boss_room_level_2_full.txt",
    "assets/levels/post_boss_room_level_3_full.txt"
};

const char* MUSIC[] = {
    "assets/sounds/music_1.wav",
    "assets/sounds/music_2.wav",
    "assets/sounds/music_3.wav",
    "assets/sounds/music_4.wav",
    "assets/sounds/music_boss.wav",
    "assets/sounds/music_mural.wav"
};

const char* MURALS[] = {
    "ATK_MURAL",
    "SPD_MURAL",
    "INV_MURAL",
    "CD_MURAL",
    "HP_MURAL"
};

const body_sprite_info_t PLAYER_SPRITE_INFO = {
    .idle_sprite_id = 0,
    .walking_anim_id = 1,
    .hit_anim_id = -1,
    .invulnerable_anim_id = 2
};

const double PLAYER_MASS = 100;
const double PLAYER_SCALE = 4;
const double PLAYER_EXP = 100;
const double PLAYER_EXP_MULTIP = 1.5;

const char *PLAYER_HEART = "PLAYER HEART";
const char *PLAYER_COIN = "PLAYER COIN";
const char *HEALTH_TEXT = "HEALTH";
const char *EXPERIENCE_TEXT = "EXPERIENCE";
const char *START_TEXT = "START";
const char *VICTORY_TEXT = "VICTORY SCREEN";
const char *LEVEL_UP_TEXT = "LEVEL_UP";
const char *MURAL_TEXT = "MURAL";

const double HEART_SIZE = 32;
const double HEART_SCALE = 2;
const double COIN_SIZE = 16;
const int HEART_BUFFER = 60;
const int COIN_BUFFER = 88 / 2;
const int COIN_NUM = 10;
const double HEALTH_LENGTH = 56;
const double HEALTH_SCALE = 1;
const double EXP_LENGTH = 83 / 2;
const double EXP_SCALE = 0.5;
const double START_SCREEN_SCALE = 2;
const double VICTORY_SCREEN_SCALE = 1;
const double MURAL_SCALE = 1;
const double BUFFER_DIST = 40;
const double LEVEL_UP_SCALE = 0.5;
const int LEVEL_NUM = 15;

const double ATTACK_POWER_UP = 5;
const double SPEED_POWER_UP = 50;
const double INV_POWER_UP = 1.5;
const double CD_POWER_UP = 0.8;

char ** levels_shuffled;
char ** boss_levels_shuffled;

char **shuffle_str_array(const char *arr[], int arr_size) {
    char **arr_shuffled = malloc(arr_size * sizeof(char *));
    for (int i = 0; i < arr_size; i++) {
        arr_shuffled[i] = malloc((strlen(arr[i]) + 1) * sizeof(char));
        arr_shuffled[i] = arr[i];
    }

    for (int i = 0; i < arr_size; i++) {
        char *temp = arr_shuffled[i];
        int randomIndex = rand() % arr_size;
        arr_shuffled[i] = arr_shuffled[randomIndex];
        arr_shuffled[randomIndex] = temp;
    }
    return arr_shuffled;
}

void shuffle_levels() {
    levels_shuffled = shuffle_str_array(LEVELS, sizeof(LEVELS) / sizeof(char*));
    boss_levels_shuffled = shuffle_str_array(BOSS_LEVELS, sizeof(BOSS_LEVELS) / sizeof(char*));
}

body_t *make_player(game_t *game, double x, double y, enum body_type type, stats_info_t info) {
    vector_t bottom_left = {x, y};
    return body_init_with_info(PLAYER_SPRITE_INFO, game_get_sprite(game, PLAYER_SPRITE_INFO.idle_sprite_id), bottom_left, PLAYER_MASS, PLAYER_SCALE, type, info);
}

UI_t *make_heart(double x, double y, sprite_t *sprite, char *type) {
    rect_t hitbox = (rect_t) {x, y, HEART_SIZE, HEART_SIZE};
    return UI_init(sprite, hitbox, type, HEART_SCALE);
}

list_t *get_player_hearts(scene_t *scene) {
    list_t *UIs = scene_get_UI_components(scene);
    list_t *hearts = list_init(list_size(UIs), NULL);
    for (size_t i = 0; i < list_size(UIs); i++) {
        if (strcmp(UI_get_type(list_get(UIs, i)), PLAYER_HEART) == 0) {
            list_add(hearts, list_get(UIs, i));
        }
    }
    return hearts;
}

UI_t *make_coin(double x, double y, sprite_t *sprite, char *type) {
    rect_t hitbox = (rect_t) {x, y, COIN_SIZE, COIN_SIZE};
    return UI_init(sprite, hitbox, type, HEART_SCALE);
}

list_t *get_player_coins(scene_t *scene) {
    list_t *UIs = scene_get_UI_components(scene);
    list_t *coins = list_init(list_size(UIs), NULL);
    for (size_t i = 0; i < list_size(UIs); i++) {
        if (strcmp(UI_get_type(list_get(UIs, i)), PLAYER_COIN) == 0) {
            list_add(coins, list_get(UIs, i));
        }
    }
    return coins;
}

scene_t *scene_reset(game_t *game) {

    // Initialize Sprite/Player
    scene_t *scene = scene_init();

    double exp;
    int max_exp;

    int heart_num = PLAYER_HEALTH / (HALF_HEART_HEALTH * 2);

    if (list_size(get_player_hearts(game_get_current_scene(game))) != 0) {
        body_t *player = game_get_player(game);
        exp = body_get_stats_info(player).experience;
        max_exp = round(PLAYER_EXP * pow(PLAYER_EXP_MULTIP, body_get_stats_info(player).level - 1));
        heart_num = list_size(get_player_hearts(game_get_current_scene(game)));
    }
    else {
        exp = 0;
        max_exp = PLAYER_EXP;
    }


    // Initialize Hearts
    for (int i = 0; i < heart_num; i++) {
        UI_t *heart = make_heart(HEART_PADDING + HEART_BUFFER + HEART_SIZE * i, MAX_HEIGHT - HEART_SIZE - HEART_PADDING, game_get_sprite(game, FULL_HEART_ID), PLAYER_HEART);
        scene_add_UI_component(scene, heart);
    }
    sprite_t *health = game_get_sprite(game, HEALTH);
    UI_t *health_text = UI_init(health, (rect_t) {HEART_PADDING, MAX_HEIGHT - HEART_SIZE - HEART_PADDING, HEALTH_LENGTH, HEART_SIZE}, HEALTH_TEXT, HEALTH_SCALE);
    scene_add_UI_component(scene, health_text);


    // Initialize Coins
    int coin_exp = max_exp / COIN_NUM;
    int coin_filled = floor(exp / coin_exp);
    for (int i = 0; i < coin_filled; i++) {
        UI_t *coin = make_coin(HEART_PADDING + COIN_BUFFER + COIN_SIZE * i, MAX_HEIGHT - COIN_SIZE - HEART_SIZE - HEART_PADDING * 2, game_get_sprite(game, COIN_FILLED_ID), PLAYER_COIN);
        scene_add_UI_component(scene, coin);
    }

    for (int i = coin_filled; i < COIN_NUM; i++) {
        UI_t *coin = make_coin(HEART_PADDING + COIN_BUFFER + COIN_SIZE * i, MAX_HEIGHT - COIN_SIZE - HEART_SIZE - HEART_PADDING * 2, game_get_sprite(game, COIN_EMPTY_ID), PLAYER_COIN);
        scene_add_UI_component(scene, coin);
    }

    sprite_t *experience = game_get_sprite(game, EXPERIENCE);
    UI_t *experience_text = UI_init(experience, (rect_t) {HEART_PADDING, MAX_HEIGHT - COIN_SIZE - HEART_SIZE - HEART_PADDING * 2, EXP_LENGTH, COIN_SIZE}, EXPERIENCE_TEXT, EXP_SCALE);
    scene_add_UI_component(scene, experience_text);
    return scene;
}


scene_t *make_title(game_t *game) {
    scene_t *scene = scene_init();

    rect_t hitbox = (rect_t) {0, 0, MAX_WIDTH, MAX_HEIGHT};
    UI_t *start_screen = UI_init(game_get_sprite(game, MENU_SPRITE_ID), hitbox, START_TEXT, START_SCREEN_SCALE);
    scene_add_UI_component(scene, start_screen);
    scene_set_is_menu(scene, true);
    return scene;

}

void random_room_music() {
    int room_music = rand() % 4;
    if(room_music == 0) music_play(MUSIC[0]);
    if(room_music == 1) music_play(MUSIC[1]);
    if(room_music == 2) music_play(MUSIC[2]);
    if(room_music == 3) music_play(MUSIC[3]);
}

void make_room(game_t *game) {
    body_t *player = game_get_player(game);
    scene_t *scene_new = scene_reset(game);
    stats_info_t player_info = body_get_stats_info(player);
    body_t *player_new = make_player(game, 0, 0, PLAYER, player_info);
    game_reset_tile_interactors(game);
    game_set_player(game, player_new);
    scene_free(game_get_current_scene(game));
    game_set_current_scene(game, scene_new);
    scene_add_body(scene_new, player_new);
    char *path = list_get(game_get_dungeon(game), game_get_room(game));
    map_load(game, path);
    create_tile_collision(game_get_current_scene(game), game_get_player(game));
}


void make_level(game_t *game, int level) {
    random_room_music();
    game_set_room(game, 0);
    game_reset_dungeon(game);
    for(int i=level*ROOMS_PER_LEVEL; i < level*ROOMS_PER_LEVEL+ROOMS_PER_LEVEL; i++) {
        char *level = levels_shuffled[i];
        char *level_full_path = malloc(sizeof(level) + LEVEL_NUM * sizeof(char *));
        strcpy(level_full_path, level);
        strcat(level_full_path, level_variants[rand() % 3]);
        game_add_room(game, level_full_path);
    }
    game_add_room(game, boss_levels_shuffled[level]);
    game_add_room(game, POST_BOSS_LEVELS[level]);
}

void game_end_room(game_t *game) {
    if(scene_check_objective(game_get_current_scene(game))) {
        game_next_room(game);
        make_room(game);
    }
    if(game_get_room(game) == ROOMS_PER_LEVEL) { // On the boss
        music_play(MUSIC[4]);
    } else if(game_get_room(game) == ROOMS_PER_LEVEL + 1) {
        random_room_music();
    }
}

void game_end_level(game_t *game) {
    game_set_level(game, game_get_level(game) + 1);

    if (game_get_level(game) >= TOTAL_LEVELS) {
        UI_t *victory = UI_init(game_get_sprite(game, VICTORY_ID), (rect_t) {0, 0, MAX_WIDTH, MAX_HEIGHT}, VICTORY_TEXT, VICTORY_SCREEN_SCALE);
        scene_add_UI_component(game_get_current_scene(game), victory);
    } else {
        scale_enemies();
        make_level(game, game_get_level(game));
        make_room(game);
    }
}

void handle_mural_buffs(char *type, game_t *game){
    scene_t *scene = game_get_current_scene(game);
    body_t *player = game_get_player(game);
    stats_info_t player_info = body_get_stats_info(player);
    sprite_t *powerup_sprite;
    if (strcmp(type, MURALS[0]) == 0) {
        player_info.attack += ATTACK_POWER_UP;
        powerup_sprite = game_get_sprite(game, ATK_POWERUP);
    } else if (strcmp(type, MURALS[1]) == 0) {
        player_info.speed += SPEED_POWER_UP;
        powerup_sprite = game_get_sprite(game, SPD_POWERUP);
    } else if (strcmp(type, MURALS[2]) == 0) {
        player_info.invulnerability_timer *= INV_POWER_UP;
        powerup_sprite = game_get_sprite(game, INV_POWERUP);
    } else if (strcmp(type, MURALS[3]) == 0) {
        player_info.cooldown *= CD_POWER_UP;
        powerup_sprite = game_get_sprite(game, CD_POWERUP);
    } else if (strcmp(type, MURALS[4]) == 0) {
        list_t *hearts = get_player_hearts(scene);
        size_t num_hearts = list_size(hearts);
        UI_t *heart = make_heart(HEART_PADDING + HEART_SIZE * (num_hearts) + HEART_BUFFER, MAX_HEIGHT - HEART_SIZE - HEART_PADDING, game_get_sprite(game, EMPTY_HEART_ID), PLAYER_HEART);
        scene_add_UI_component(scene, heart);
        list_free(hearts);
        powerup_sprite = game_get_sprite(game, 53);
    }
    rect_t player_hitbox = body_get_hitbox(player);

    player_info.health = list_size(get_player_hearts(game_get_current_scene(game))) * (HALF_HEART_HEALTH * 2);
    player_info.level++;

    SDL_Rect sprite_size = sprite_get_shape(powerup_sprite, 1);
    UI_t *component = UI_init(powerup_sprite, (rect_t) {MAX_WIDTH / 2 - sprite_size.w / 4, MAX_HEIGHT / 2 + player_hitbox.h / 2 + BUFFER_DIST, sprite_size.w / 2, sprite_size.h / 2}, LEVEL_UP_TEXT, LEVEL_UP_SCALE);
    scene_add_UI_component(scene, component);

    body_set_stats_info(player, player_info);
}


void game_random_mural(game_t *game) {
    int mural_type = rand() % 5;
    int mural_id;
    game_set_paused(game, true);
    music_play(MUSIC[5]);
    switch (mural_type) {
        case 0:
            mural_id = ATK_MURAL_ID;
            handle_mural_buffs(MURALS[0], game);
            break;
        case 1:
            mural_id = SPD_MURAL_ID;
            handle_mural_buffs(MURALS[1], game);
            break;
        case 2:
            mural_id = INV_MURAL_ID;
            handle_mural_buffs(MURALS[2], game);
            break;
        case 3:
            mural_id = CD_MURAL_ID;
            handle_mural_buffs(MURALS[3], game);
            break;
        case 4:
            mural_id = HP_MURAL_ID;
            handle_mural_buffs(MURALS[4], game);
            break;
    }
    UI_t *mural = UI_init(game_get_sprite(game, mural_id), (rect_t) {0,0, MAX_WIDTH, MAX_HEIGHT}, MURAL_TEXT, MURAL_SCALE);
    scene_add_UI_component(game_get_current_scene(game), mural);
}


