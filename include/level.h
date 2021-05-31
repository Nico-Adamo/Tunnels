#include "game.h"
#include "sdl_wrapper.h"
#include "enemy.h"
#include "map.h"
#include "forces.h"

extern const char *SPRITE;
// const char *SPRITE_PATH = "assets/knight_f_idle_anim_f0.png";

extern const double MAX_WIDTH;
extern const double MAX_HEIGHT;

extern const double HALF_HEART_HEALTH;
extern const double PLAYER_HEALTH;
extern const double HEART_PADDING;

void shuffle_levels();

UI_t *make_heart(double x, double y, sprite_t *sprite, char *type);

list_t *get_player_hearts(scene_t *scene);

body_t *make_player(game_t *game, double x, double y, enum body_type type, stats_info_t info);

UI_t *make_coin(double x, double y, sprite_t *sprite, char *type);

list_t *get_player_coins(scene_t *scene);

body_t *make_enemy(game_t *game, double x, double y, enum enemy_type type);

scene_t *scene_reset(game_t *game);

scene_t *make_title(game_t *game);

void make_room(game_t *game);

void make_level(game_t *game, int level);

void game_end_room(game_t *game);

void game_end_level(game_t *game);

void game_random_mural(game_t *game);
