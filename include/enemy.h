#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "game.h"
#include "sprite.h"
#include <stdbool.h>

/**
 * Specifies all enemy types.
 */
enum enemy_type{TINY_ZOMBIE, GOBLIN, IMP, SKELET, MUDDY, SWAMPY, ZOMBIE, ICE_ZOMBIE, MASKED_ORC, ORC_WARRIOR, ORC_SHAMAN, NECROMANCER, WOGUL, CHORT, WIZARD, NECROMANCER_WIZARD, BIG_ZOMBIE, OGRE, BIG_DEMON};

/**
 * Creates a new enemy of a given type at the specified coordinates.
 *
 * @param game the current game
 * @param x bottom left x coordinate
 * @param y bottom left y coordinate
 * @param type the enemy type
 * @return a body_t representing the created enemy
 */
body_t *make_enemy(game_t *game, double x, double y, enum enemy_type type);

/**
 * Handles the enemies in a given game over time dt.
 * (Handles both boss and non-boss behavior.)
 *
 * @param game the current game
 * @param dt duration
 */
void handle_enemies(game_t *game, double dt);

/**
 * Scales enemy experience and health.
 * Called as users progress to new levels.
 */
void scale_enemies();

/**
 * Returns the sprite info for a specified enemy.
 *
 * @param type an enemy type
 * @return the sprite info for the given enemy type
 */
body_sprite_info_t enemy_get_sprite_info(enum enemy_type type);

/**
 * Returns the stat info for a specified enemy.
 *
 * @param type an enemy type
 * @return the stat info for the given enemy type
 */
stats_info_t enemy_get_stats(enum enemy_type type);
#endif
