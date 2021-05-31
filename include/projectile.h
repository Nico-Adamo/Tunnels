#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "body.h"
#include "game.h"

body_t *make_bullet(game_t *game, body_t *body, vector_t bullet_dir, int sprite_id, double bullet_speed);

body_t *make_bullet_pos(game_t *game, vector_t spawn_point, vector_t bullet_dir, int sprite_id, int atk, double bullet_speed);

#endif // #ifndef __PROJECTILE_H__
