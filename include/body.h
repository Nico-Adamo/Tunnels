#ifndef __BODY_H__
#define __BODY_H__

#include <stdbool.h>
#include "list.h"
#include "vector.h"
#include "sprite.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>

/**
 * Attack type of a body, if a body is a player or an enemy.
 */
enum attack_type{MELEE, RADIAL_SHOOTER, STATIC_SHOOTER, MELEE_SHOOTER};

/**
 * Type of a body.
 */
enum body_type{PLAYER, PLAYER_BULLET, ENEMY_BULLET, ENEMY, BOSS_NECROMANCER_WIZARD, BOSS_BIG_ZOMBIE, BOSS_OGRE, BOSS_BIG_DEMON};

// Go through stats_info (in enemy+) and update to include level + invulnerability_timer

/**
 * Stat information for a body.
 */
typedef struct stats_info {
    double experience;
    double health;
    double attack;
    double cooldown;
    double invulnerability_timer;
    int bullet_id;
    double speed;
    int level;
    enum attack_type atk_type;
} stats_info_t;

/**
 * Sprite information for a body.
 */
typedef struct body_sprite_info {
    int idle_sprite_id;
    int walking_anim_id;
    int hit_anim_id;
    int invulnerable_anim_id;
} body_sprite_info_t;

/**
 * A rigid body constrained to the plane.
 * Implemented as a sprite with a hitbox.
 * Bodies can accumulate forces and impulses during each tick.
 * Bodies can be animated.
 * Bodies have stats that save their properties.
 */
typedef struct body body_t;

/**
 * Generates a random number from min to max.
 *
 * @param min the minimum value
 * @param max the maximum value
 * @return a random double between min and max
 */
double rand_from(double min, double max);

/**
 * Allocates memory for a body with the given parameters.
 * The body is initially at rest.
 * Asserts that the mass is positive and that the required memory is allocated.
 *
 * @param sprite_ids sprite information for a body
 * @param sprite sprite of a body
 * @param bottom_left a vector that gives the bottom left of a body
 * @param mass the mass of the body (if INFINITY, stops the body from moving)
 * @param scale the scale of a body to its sprite
 * @param type type of a body
 * @param info stats information for a body
 * @return a pointer to the newly allocated body
 */
body_t *body_init_with_info(body_sprite_info_t sprite_ids, sprite_t *sprite, vector_t bottom_left, double mass, double scale, enum body_type type, stats_info_t info);

/**
 * Releases the memory allocated for a body.
 *
 * @param body a pointer to a body returned from body_init()
 */
void body_free(body_t *body);

/**
 * Gets the stat information of a body
 *
 * @param body a pointer to a body
 * @return stats information for a body
 */
stats_info_t body_get_stats_info(body_t *body);

/**
 * Sets the stat information of a body
 *
 * @param body a pointer to a body
 * @param info the new stats information for the body
 */
void body_set_stats_info(body_t *body, stats_info_t info);

/**
 * Gets the current sprite information for a body
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's sprite information
 */
body_sprite_info_t body_get_sprite_ids(body_t *body);

/**
 * Gets the current sprite ID of a body
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's sprite ID
 */
int body_get_cur_sprite_id(body_t *body);

/**
 * Sets the sprite ID of a body
 *
 * @param body a pointer to a body returned from body_init()
 * @param sprite_id the body's new sprite ID
 */
void body_set_sprite_id(body_t *body, int sprite_id);

/**
 * Gets the current hitbox shape of a body
 * Hitbox is the area in which the body will collide with a bullet
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's hitbox
 */
SDL_Rect body_get_hitbox_shape(body_t *body);

/**
 * Gets the current shape of the sprite image
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's draw shape
 */
SDL_Rect body_get_draw_shape(body_t *body);

/**
 * Gets the current collision shape of a body
 * Collision is the area at which a body will collide with the tiles
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's collision shape
 */
SDL_Rect body_get_collision_shape(body_t *body);

/**
 * Gets the entire hitbox of a body
 * Hitbox gives the current location and size of the sprite shape of a body
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's entire hitbox
 */
rect_t body_get_draw_hitbox(body_t *body);

/**
 * Gets the bullet hitbox of a body
 * Bullet hitbox gives the current location and size of the hitbox shape
 * 
 * @param body a pointer to a body returned from body_init()
 * @return the body's hitbox
 */
rect_t body_get_hitbox(body_t *body);

/**
 * Gets the current collision hitbox of a body
 * Collision hitbox gives the current location and size of the collision shape
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's collision hitbox
 */
rect_t body_get_collision_hitbox(body_t *body);

/**
 * Returns whether the body is flipped or not
 *
 * @param body a pointer to a body returned from body_init()
 * @return whether the body is flipped
 */
bool body_get_flipped(body_t *body);

/**
 * Gets the texture of the body
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's shoot cooldown
 */
SDL_Texture *body_get_texture(body_t *body);

/**
 * Gets the current sprite of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's sprite
 */
sprite_t *body_get_sprite(body_t *body);

/**
 * Sets the sprite of a body
 *
 * @param body a pointer to a body returned from body_init()
 * @param sprite the body's new sprite
 */
void body_set_sprite(body_t *body, sprite_t *sprite);

/**
 * Gets the current time on the invulnerability timer of a body
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's invulnerability timer
 */
double body_get_invulnerability_timer(body_t *body);

/**
 * Sets the invulnerability timer of a body
 *
 * @param body a pointer to a body returned from body_init()
 * @param invulnerability_timer the body's new invulnerability time
 */
void body_set_invulnerability_timer(body_t *body, double invulnerability_timer);

/**
 * Gets the current center of mass of a body.
 * While this could be calculated with polygon_centroid(), that becomes too slow
 * when this function is called thousands of times every tick.
 * Instead, the body should store its current centroid.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's center of mass
 */
vector_t body_get_centroid(body_t *body);

/**
 * Gets the current velocity of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's velocity vector
 */
vector_t body_get_velocity(body_t *body);

/**
 * Gets the mass of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the mass passed to body_init(), which must be greater than 0
 */
double body_get_mass(body_t *body);

/**
 * Gets the scale of a body's hitbox to its shape
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's scale
 */
double body_get_scale(body_t *body);

/**
 * Gets the type associated with a body.
 *
 * @param type a pointer to a type returned from body_init()
 * @return the info passed to body_init()
 */
enum body_type body_get_type(body_t *body);

/**
 * Translates a body to a new position.
 * The position is specified by the position of the body's center of mass.
 *
 * @param body a pointer to a body returned from body_init()
 * @param x the body's new centroid
 */
void body_set_centroid(body_t *body, vector_t x);

/**
 * Changes a body's velocity (the time-derivative of its position).
 *
 * @param body a pointer to a body returned from body_init()
 * @param v the body's new velocity
 */
void body_set_velocity(body_t *body, vector_t v);

/**
 * Gets the direction the body is currently facing
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's direction
 */
vector_t body_get_direction(body_t *body);

/**
 * Sets the direction of a body
 *
 * @param body a pointer to a body returned from body_init()
 * @param dir the body's new direction
 */
void body_set_direction(body_t *body, vector_t dir);

/**
 * Applies a force to a body over the current tick.
 * If multiple forces are applied in the same tick, they should be added.
 * Should not change the body's position or velocity; see body_tick().
 *
 * @param body a pointer to a body returned from body_init()
 * @param force the force vector to apply
 */
void body_add_force(body_t *body, vector_t force);

/**
 * Applies an impulse to a body.
 * An impulse causes an instantaneous change in velocity,
 * which is useful for modeling collisions.
 * If multiple impulses are applied in the same tick, they should be added.
 * Should not change the body's position or velocity; see body_tick().
 *
 * @param body a pointer to a body returned from body_init()
 * @param impulse the impulse vector to apply
 */
void body_add_impulse(body_t *body, vector_t impulse);

/**
 * Gets the current velocity of a body
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's velocity
 */
vector_t body_calculate_velocity(body_t *body, double dt);

/**
 * Updates the body after a given time interval has elapsed.
 * Sets acceleration and velocity according to the forces and impulses
 * applied to the body during the tick.
 * The body should be translated at the *average* of the velocities before
 * and after the tick.
 * Resets the forces and impulses accumulated on the body.
 *
 * @param body the body to tick
 * @param dt the number of seconds elapsed since the last tick
 */
void body_tick(body_t *body, double dt);

/**
 * Marks a body for removal--future calls to body_is_removed() will return true.
 * Does not free the body.
 * If the body is already marked for removal, does nothing.
 *
 * @param body the body to mark for removal
 */
void body_remove(body_t *body);

/**
 * Returns whether a body has been marked for removal.
 * This function returns false until body_remove() is called on the body,
 * and returns true afterwards.
 *
 * @param body the body to check
 * @return whether body_remove() has been called on the body
 */
bool body_is_removed(body_t *body);

/**
 * Gets the current time on the shoot cooldown of a body
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's shoot cooldown
 */
double body_get_shoot_cooldown(body_t *body);

/**
 * Sets the shoot cooldown of a body
 *
 * @param body a pointer to a body returned from body_init()
 * @param cooldown the body's new shoot cooldown
 */
void body_set_shoot_cooldown(body_t *body, double cooldown);

/**
 * Gets the current time on the hit timer of a body
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's hit timer
 */
double body_get_hit_timer(body_t *body);

/**
 * Sets the hit timer of a body
 *
 * @param body a pointer to a body returned from body_init()
 * @param timer the body's new hit time
 */
void body_set_hit_timer(body_t *body, double timer);

#endif // #ifndef __BODY_H__
