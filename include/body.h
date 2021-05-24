#ifndef __BODY_H__
#define __BODY_H__

#include <stdbool.h>
#include "color.h"
#include "list.h"
#include "vector.h"
#include "sprite.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>

typedef struct stats_info {
    double experience;
    double health;
    double attack;
    double cooldown;
    int bullet_id;
} stats_info_t;

typedef struct body_sprite_info {
    int idle_sprite_id;
    int walking_anim_id;
    int hit_anim_id;
    int invulnerable_anim_id;
} body_sprite_info_t;

/**
 * A rigid body constrained to the plane.
 * Implemented as a polygon with uniform density.
 * Bodies can accumulate forces and impulses during each tick.
 * Angular physics (i.e. torques) are not currently implemented.
 */
typedef struct body body_t;

typedef struct body_sprite_info body_sprite_info_t;

/**
 * Initializes a body without any info.
 * Acts like body_init_with_info() where info and info_freer are NULL.
 */
body_t *body_init(body_sprite_info_t sprite_ids, sprite_t *sprite, vector_t bottom_left, double mass, double scale);

stats_info_t body_get_stats_info(body_t *body);

void body_set_stats_info(body_t *body, stats_info_t info);

/** EDIT THIS
 * Allocates memory for a body with the given parameters.
 * The body is initially at rest.
 * Asserts that the mass is positive and that the required memory is allocated.
 *
 * @param shape a list of vectors describing the initial shape of the body
 * @param mass the mass of the body (if INFINITY, stops the body from moving)
 * @param color the color of the body, used to draw it on the screen
 * @param info additional information to associate with the body,
 *   e.g. its type if the scene has multiple types of bodies
 * @param info_freer if non-NULL, a function call on the info to free it
 * @return a pointer to the newly allocated body
 */
body_t *body_init_with_info(body_sprite_info_t sprite_ids, sprite_t *sprite, vector_t bottom_left, double mass, double scale, char *type, stats_info_t info);
/**
 * Releases the memory allocated for a body.
 *
 * @param body a pointer to a body returned from body_init()
 */
void body_free(body_t *body);

/**
 * Gets the current shape of a body.
 * Returns a newly allocated vector list, which must be list_free()d.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the polygon describing the body's current position
 */
double rand_from(double min, double max);

int body_get_cur_sprite_id(body_t *body);
body_sprite_info_t body_get_sprite_ids(body_t *body);

void body_set_sprite_id(body_t *body, int sprite_id);
sprite_t *body_get_sprite(body_t *body);
void body_set_sprite(body_t *body, sprite_t *sprite);

double body_get_invulnerability_timer(body_t *body);

void body_set_invulnerability_timer(body_t *body, double invulnerability_timer);
void body_set_shoot_cooldown(body_t *body, double cooldown);
double body_get_shoot_cooldown(body_t *body);
double body_get_hit_timer(body_t *body);
void body_set_hit_timer(body_t *body, double timer);

SDL_Rect body_get_hitbox_shape(body_t *body);
SDL_Rect body_get_draw_shape(body_t *body);
SDL_Rect body_get_collision_shape(body_t *body);
rect_t body_get_draw_hitbox(body_t *body);
rect_t body_get_hitbox(body_t *body);
rect_t body_get_collision_hitbox(body_t *body);
SDL_Texture *body_get_texture(body_t *body);

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

vector_t body_get_direction(body_t *body);

void body_set_direction(body_t *body, vector_t dir);

/**
 * Gets the mass of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the mass passed to body_init(), which must be greater than 0
 */
double body_get_mass(body_t *body);

double body_get_scale(body_t *body);

/**
 * Gets the type associated with a body.
 *
 * @param type a pointer to a type returned from body_init()
 * @return the info passed to body_init()
 */
char *body_get_type(body_t *body);

bool body_get_flipped(body_t *body);

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

#endif // #ifndef __BODY_H__
