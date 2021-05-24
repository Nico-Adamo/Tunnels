#include "game.h"
#include "sdl_wrapper.h"
#include "enemy.h"
#include "map.h"
#include "forces.h"

extern const char *SPRITE;
// const char *SPRITE_PATH = "assets/knight_f_idle_anim_f0.png";
extern const char *FULL_HEART;
extern const char *HALF_HEART;
extern const char *EMPTY_HEART;
extern const char *START;

extern const double MAX_WIDTH;
extern const double MAX_HEIGHT;

extern const double HALF_HEART_HEALTH;
extern const double PLAYER_HEALTH;
extern const double HEART_PADDING;

UI_t *make_heart(double x, double y, char *texture_name, char *type);

list_t *get_player_hearts(scene_t *scene);

body_t *make_player(game_t *game, double x, double y, char *type, stats_info_t info);

body_t *make_enemy(game_t *game, double x, double y, enum enemy_type type);

scene_t *scene_reset(game_t *game);

scene_t *make_title(game_t *game);

void make_level(game_t *game);

void game_end_level(game_t *game);
