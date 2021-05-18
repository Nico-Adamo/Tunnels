#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "list.h"
#include "polygon.h"

/**
 * An enemy.
 * Stores enemy info (e.g., atk damange, health, position, name)
 */
typedef struct enemy enemy_t;

enemy_t *enemy_init(list_t *shape, double mass, double attack, double health, char *type);

void enemy_free(enemy_t *enemy);

double enemy_get_centroid(enemy_t *enemy);

double enemy_get_mass(enemy_t *enemy);

double enemy_get_dmg(enemy_t *enemy);

double enemy_get_health(enemy_t *enemy);

double enemy_get_type(enemy_t *enemy);

double enemy_get_velocity(enemy_t *enemy);

void enemy_set_velocity(enemy_t *enemy);

void enemy_set_centroid(enemy_t *enemy);

void enemy_set_rotation(enemy_t *enemy);




#endif