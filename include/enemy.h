#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "game.h"
#include "sprite.h"

enum enemy_type{TINY_ZOMBIE, GOBLIN, IMP, SKELET, MUDDY, SWAMPY, ZOMBIE, ICE_ZOMBIE, MASKED_ORC, ORC_WARRIOR, ORC_SHAMAN, NECROMANCER, WOGUL, CHORT, WIZARD, NECROMANCER_WIZARD, BIG_ZOMBIE, OGRE, BIG_DEMON};

const double min_cooldown;
const double max_cooldown;

double rand_from(double min, double max);

void handle_enemies(game_t *game, double dt);

body_sprite_info_t enemy_get_sprite_info(enum enemy_type type);
stats_info_t enemy_get_stats(enum enemy_type type);
#endif
