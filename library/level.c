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

// const char* BOSS_LEVELS[] = {
//     "assets/levels/boss_room_01_orc_full.txt"
// };

const char* POST_BOSS_LEVELS[] = {
    "assets/levels/post_boss_room_level_1_full.txt",
    "assets/levels/post_boss_room_level_2_full.txt",
    "assets/levels/post_boss_room_level_3_full.txt"
};

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
    // for(size_t i = 0; i<6; i++) {
    //     printf("Level: %s\n", levels_shuffled[i]);
    // }
    boss_levels_shuffled = shuffle_str_array(BOSS_LEVELS, sizeof(BOSS_LEVELS) / sizeof(char*));
}

body_t *make_player(game_t *game, double x, double y, enum body_type type, stats_info_t info) {
    vector_t bottom_left = {x, y};
    body_sprite_info_t player_sprite_info = {
        .idle_sprite_id = 0,
        .walking_anim_id = 1,
        .hit_anim_id = -1,
        .invulnerable_anim_id = 2
    };
    // First argument: Sprite size (x,y are always 0)
    // Second argument: Bottom left corner of sprite, and size (we should eventually change to scale factor rather than specifying explicit width and height)
    return body_init_with_info(player_sprite_info, game_get_sprite(game, player_sprite_info.idle_sprite_id), bottom_left, 100, 4, type, info);
}

UI_t *make_heart(double x, double y, sprite_t *sprite, char *type) {
    rect_t hitbox = (rect_t) {x, y, 32, 32};
    return UI_init(sprite, hitbox, type, 2);
}

list_t *get_player_hearts(scene_t *scene) {
    list_t *UIs = scene_get_UI_components(scene);
    list_t *hearts = list_init(list_size(UIs), NULL);
    for (size_t i = 0; i < list_size(UIs); i++) {
        if (strcmp(UI_get_type(list_get(UIs, i)), "PLAYER_HEART") == 0) {
            list_add(hearts, list_get(UIs, i));
        }
    }
    return hearts;
}

UI_t *make_coin(double x, double y, sprite_t *sprite, char *type) {
    rect_t hitbox = (rect_t) {x, y, 16, 16};
    return UI_init(sprite, hitbox, type, 2);
}

list_t *get_player_coins(scene_t *scene) {
    list_t *UIs = scene_get_UI_components(scene);
    list_t *coins = list_init(list_size(UIs), NULL);
    for (size_t i = 0; i < list_size(UIs); i++) {
        if (strcmp(UI_get_type(list_get(UIs, i)), "PLAYER_COIN") == 0) {
            list_add(coins, list_get(UIs, i));
        }
    }
    return coins;
}

scene_t *scene_reset(game_t *game) {

    // Initialize Sprite/Player
    scene_t *scene = scene_init();

    // Initialize Potential Enemy
    // for(int i=0; i<3; i++) {
    //     body_t *temp_enemy = make_enemy(game, 400+100*i, 200 + rand_from(-50,50), "ENEMY", enemy_info);
    //     scene_add_body(scene, temp_enemy);
    // }

    double exp;
    int max_exp;

    int heart_num = PLAYER_HEALTH / (HALF_HEART_HEALTH * 2);

    if (list_size(get_player_hearts(game_get_current_scene(game))) != 0) {
        body_t *player = game_get_player(game);
        exp = body_get_stats_info(player).experience;
        max_exp = round(100 * pow(1.5, body_get_stats_info(player).level - 1)); // Magic numbers (fix)
        heart_num = list_size(get_player_hearts(game_get_current_scene(game)));
    }
    else {
        exp = 0; // Magic numbers (fix)
        max_exp = 100;
    }


    // Initialize Demo Heart
    double length_heart = 60;
    for (int i = 0; i < heart_num; i++) {
        UI_t *heart = make_heart(HEART_PADDING + length_heart + 32 * i, MAX_HEIGHT - 32 - HEART_PADDING, game_get_sprite(game, FULL_HEART_ID), "PLAYER_HEART");
        scene_add_UI_component(scene, heart);
    }
    sprite_t *health = game_get_sprite(game, HEALTH);
    UI_t *health_text = UI_init(health, (rect_t) {HEART_PADDING, MAX_HEIGHT - 32 - HEART_PADDING, 56, 32}, "HEALTH", 1);
    scene_add_UI_component(scene, health_text);


    // Initialize Demo Coins
    double length_coin = 88; // Magic number
    int coin_exp = max_exp / 10;
    int coin_filled = floor(exp / coin_exp);
    for (int i = 0; i < coin_filled; i++) {
        UI_t *coin = make_coin(HEART_PADDING + length_coin / 2 + 16 * i, MAX_HEIGHT - 48 - HEART_PADDING * 2, game_get_sprite(game, COIN_FILLED_ID), "PLAYER_COIN");
        scene_add_UI_component(scene, coin);
    }

    for (int i = coin_filled; i < 10; i++) {
        UI_t *coin = make_coin(HEART_PADDING + length_coin / 2 + 16 * i, MAX_HEIGHT - 48 - HEART_PADDING * 2, game_get_sprite(game, COIN_EMPTY_ID), "PLAYER_COIN");
        scene_add_UI_component(scene, coin);
    }

    sprite_t *experience = game_get_sprite(game, EXPERIENCE);
    UI_t *experience_text = UI_init(experience, (rect_t) {HEART_PADDING, MAX_HEIGHT - 48 - HEART_PADDING * 2, 83 / 2, 16}, "EXPERIENCE", 0.5);
    scene_add_UI_component(scene, experience_text);

    //scene_add_UI_text(scene, ui_text_init("Test",  (rect_t) {0, 0, 100, 100}, 5));

    return scene;
}


scene_t *make_title(game_t *game) {
    scene_t *scene = scene_init();

    rect_t hitbox = (rect_t) {0, 0, 1024, 512};
    UI_t *start_screen = UI_init(game_get_sprite(game, MENU_SPRITE_ID), hitbox, "START", 2);
    scene_add_UI_component(scene, start_screen);
    scene_set_is_menu(scene, true);
    return scene;

}

void random_room_music() {
    int room_music = rand() % 4;
    if(room_music == 0) music_play("assets/sounds/music_1.wav");
    if(room_music == 1) music_play("assets/sounds/music_2.wav");
    if(room_music == 2) music_play("assets/sounds/music_3.wav");
    if(room_music == 3) music_play("assets/sounds/music_4.wav");
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
        char *level_full_path = malloc(sizeof(level) + 15 * sizeof(char *));
        strcpy(level_full_path, level);
        strcat(level_full_path, level_variants[rand() % 3]);
        printf("Level: %s\n", level_full_path);
        game_add_room(game, level_full_path);
    }
    printf("%s\n", boss_levels_shuffled[level]);
    game_add_room(game, boss_levels_shuffled[level]);
    game_add_room(game, POST_BOSS_LEVELS[level]);
}

void game_end_room(game_t *game) {
    if(scene_check_objective(game_get_current_scene(game))) {
        game_next_room(game);
        make_room(game);
    }
    if(game_get_room(game) == ROOMS_PER_LEVEL) { // On the boss
        music_play("assets/sounds/music_boss.wav");
    } else if(game_get_room(game) == ROOMS_PER_LEVEL + 1) {
        random_room_music();
    }
}

void game_end_level(game_t *game) {
    game_set_level(game, game_get_level(game) + 1);

    if (game_get_level(game) == TOTAL_LEVELS) {
        UI_t *victory = UI_init(game_get_sprite(game, VICTORY_ID), (rect_t) {0,0, 1024, 512}, "VICTORY_SCREEN", 1);
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
    ui_text_t *text;
    printf("Hey, I'm here\n");
    if (strcmp(type, "HP_MURAL") == 0) {
        printf("HP\n");
        player_info.health = list_size(get_player_hearts(game_get_current_scene(game))) * 10;
        text = ui_text_init(" Health restored", (vector_t) {HEART_PADDING, HEART_PADDING}, 3, OBJECTIVE_TEXT);
    } else if (strcmp(type, "ATK_MURAL") == 0) {
        printf("ATK\n");
        player_info.attack += 5;
        text = ui_text_init(" +5 Attack", (vector_t) {HEART_PADDING, HEART_PADDING}, 3, OBJECTIVE_TEXT);
    } else if (strcmp(type, "SPD_MURAL") == 0) {
        printf("SPD\n");
        player_info.speed += 50;
        text = ui_text_init(" +50 Speed", (vector_t) {HEART_PADDING, HEART_PADDING}, 3, OBJECTIVE_TEXT);
    } else if (strcmp(type, "INV_MURAL") == 0) {
        printf("INV\n");
        player_info.invulnerability_timer *= 1.5;
        text = ui_text_init(" 1.5x Invulnerabiltiy", (vector_t) {HEART_PADDING, HEART_PADDING}, 3, OBJECTIVE_TEXT);
    } else if (strcmp(type, "CD_MURAL") == 0) {
        printf("CD\n");
        player_info.cooldown *= .9;
        text = ui_text_init(" .9x Bullet Cooldown", (vector_t) {HEART_PADDING, HEART_PADDING}, 3, OBJECTIVE_TEXT);
    }
    scene_add_UI_text(game_get_current_scene(game), text);
    body_set_stats_info(player, player_info);
}


void game_random_mural(game_t *game) {
    int mural_type = rand() % 5;
    int mural_id;
    game_set_paused(game, true);
    music_play("assets/sounds/music_mural.wav");
    printf("MURAL TYPE: %d", mural_type);
    switch (mural_type) {
        case 0:
            mural_id = HP_MURAL_ID;
            handle_mural_buffs("HP_MURAL", game);
            break;
        case 1:
            mural_id = ATK_MURAL_ID;
            handle_mural_buffs("ATK_MURAL", game);
            break;
        case 2:
            mural_id = SPD_MURAL_ID;
            handle_mural_buffs("SPD_MURAL", game);
            break;
        case 3:
            mural_id = INV_MURAL_ID;
            handle_mural_buffs("INV_MURAL", game);
            break;
        case 4:
            mural_id = CD_MURAL_ID;
            handle_mural_buffs("CD_MURAL", game);
            break;
    }
    UI_t *mural = UI_init(game_get_sprite(game, mural_id), (rect_t) {0,0, 1024, 512}, "MURAL", 1);
    scene_add_UI_component(game_get_current_scene(game), mural);
}


