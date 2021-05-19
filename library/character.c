#include "character.h"

typedef struct stats {
    char *type;
    double attack_damage;
    double health;
    double experience;
} stats_t;

/* TO DOS
Movement & Shooting controls
- on_key function - WASD to move (translations, rotation),
esc to menu (later), F to interact (later), spacebar to shoot (body_get_rotation)
- no acceleration for now (maybe add it as a feature later)

Collision
- edit collisions to work with hitboxes (rect_t):
no separating axes, bounding boxes should be enough for rectangles

One enemy targeting player
- create an enemy
- make it so that a body can target another body (possibly with gravity - make sure player isn't affected)
- shoot with a randomized time interval between 1-3 seconds

Health & Damage
- player/enemy health depletes upon collision by damage
- possibly write a new collision function (damage_collision)
- increase experience for player when an enemy is destroyed
(later - write it so it increases but nothing happens)

Projectile system
- create bullet type
- spawn bullets for enemies and player (when spacebar is hit)
- destroy bullets when it hits something
- typedef bullet info if we have bouncing bullets so we know how many times it collided
*/
