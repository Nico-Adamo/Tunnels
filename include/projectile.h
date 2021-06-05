#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "body.h"
#include "game.h"

/**
 * Initializes a bullet with respect to a body.
 *
 * @param game a pointer to a game
 * @param body to body associated with the bullet
 * @param bullet_dir the direction of the bullet
 * @param sprite_id the sprite to display
 * @param bullet_speed the speed for the bullet
 * @return a pointer to the created bullet
 */
body_t *make_bullet(game_t *game, body_t *body, vector_t bullet_dir, int sprite_id, double bullet_speed);

/**
 * Initializes a bullet given a position & attack damage.
 *
 * @param game a pointer to a game
 * @param spawn_point the bottom left corner of the bullet
 * @param bullet_dir the direction of the bullet
 * @param sprite_id the sprite to display
 * @param atk the attack damange for the bullet
 * @param bullet_speed the speed for the bullet
 * @return a pointer to the created bullet
 */
body_t *make_bullet_pos(game_t *game, vector_t spawn_point, vector_t bullet_dir, int sprite_id, int atk, double bullet_speed);

#endif // #ifndef __PROJECTILE_H__
