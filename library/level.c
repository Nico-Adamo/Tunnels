#include "level.h"

const double MAX_WIDTH = 1024;
const double MAX_HEIGHT = 512;

const double HALF_HEART_HEALTH = 5;
const double PLAYER_HEALTH = 100;
const double HEART_PADDING = 4;

char *level_variants[] = {
    "a_full.txt",
    "b_full.txt",
    "c_full.txt"
};

body_t *make_player(game_t *game, double x, double y, char *type, stats_info_t info) {
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

body_t *make_enemy(game_t *game, double x, double y, enum enemy_type type) {
    vector_t bottom_left = {x, y};
    stats_info_t info = enemy_get_stats(type);
    body_sprite_info_t enemy_sprite_info = enemy_get_sprite_info(type);
    body_t *enemy = body_init_with_info(enemy_sprite_info, game_get_sprite(game, enemy_sprite_info.idle_sprite_id), bottom_left, 100, 4, "ENEMY", info);
    return enemy;
}

UI_t *make_heart(double x, double y, sprite_t *sprite, char *type) {
    SDL_Rect shape = (SDL_Rect) {0, 0, 16, 16};
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

scene_t *scene_reset(game_t *game) {

    // Initialize Sprite/Player
    scene_t *scene = scene_init();

    // Initialize Potential Enemy
    // for(int i=0; i<3; i++) {
    //     body_t *temp_enemy = make_enemy(game, 400+100*i, 200 + rand_from(-50,50), "ENEMY", enemy_info);
    //     scene_add_body(scene, temp_enemy);
    // }

    // Initialize Demo Heart
    int heart_num = PLAYER_HEALTH / (HALF_HEART_HEALTH * 2);
    for (int i = 0; i < heart_num; i++) {
        UI_t *heart = make_heart(HEART_PADDING + 32 * i, MAX_HEIGHT - 32 - HEART_PADDING, game_get_sprite(game, FULL_HEART_ID), "PLAYER_HEART");
        scene_add_UI_component(scene, heart);
    }

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
void make_room(game_t *game){
    body_t *player = game_get_player(game);
    scene_t *scene_new = scene_reset(game);
    stats_info_t player_info = body_get_stats_info(player);
    body_t *player_new = make_player(game, 0, 0, "PLAYER", player_info);
    game_reset_tile_interactors(game);
    game_set_player(game, player_new);
    game_set_current_scene(game, scene_new);
    scene_add_body(scene_new, player_new);
    char *path = list_get(game_get_dungeon(game), game_get_room(game));
    map_load(game, path);
    create_tile_collision(game_get_current_scene(game), game_get_player(game));
}

void make_level(game_t *game, int level, char** levels_shuffled, char **boss_levels_shuffled, char **post_boss_levels) {
    game_set_room(game, 0);
    game_reset_dungeon(game);
    for(int i=level*2; i < level*2+2; i++) { // TODO: magic number
        char *level = levels_shuffled[i];
        char *level_full_path = malloc(sizeof(level) + 15 * sizeof(char *));
        strcpy(level_full_path, level);
        strcat(level_full_path, level_variants[rand() % 3]);
        printf("Level: %s\n", level_full_path);
        game_add_room(game, level_full_path);
    }
    game_add_room(game, boss_levels_shuffled[level]);
    game_add_room(game, post_boss_levels[level]);
}

void game_end_room(game_t *game) {
    if(scene_check_objective(game_get_current_scene(game))) {
        game_next_room(game);
        make_room(game);
    }
}

void game_random_mural(game_t *game) {
    game_set_paused(game, true);
    UI_t *mural = UI_init(game_get_sprite(game, MURAL_ID), (rect_t) {0,0, 1024, 512}, "MURAL", 1);
    scene_add_UI_component(game_get_current_scene(game), mural);
}
