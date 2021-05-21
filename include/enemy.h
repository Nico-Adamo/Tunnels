#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "game.h"

const double min_cooldown;
const double max_cooldown;

double rand_from(double min, double max);

void handle_enemies(game_t *game, double dt);

#endif