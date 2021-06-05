#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

/**
 * A sprite.
 * Contains all details relevant to a given sprite (shape, collision shape,
 * hitbox shape, texture, and animation details).
 */
typedef struct sprite sprite_t;

/**
 * Allocates memory for and initializes a sprite.
 *
 * @param path the file path for the sprite's texture
 * @param shape the sprite's overall shape
 * @param collision_shape the sprite's collision shape
 * @param hitbox_shape the sprite's hitbox shape
 * @param is_animated whether or not the sprite is animated
 * @param animation_speed the speed of the sprite's animation
 * @param animation_frames how many animation frames a sprite has
 * @return a pointer to the created sprite
 */
sprite_t *sprite_init(const char *path, SDL_Rect shape, SDL_Rect collision_shape, SDL_Rect hitbox_shape, bool is_animated, double animation_speed, size_t animation_frames);

// Getters

/**
 * Returns the shape of a sprite, with respect to animations when necessary.
 *
 * @param sprite a pointer to a sprite
 * @param cur_frame the current animation frame
 * @return the shape of the sprite at the current frame
 */
SDL_Rect sprite_get_shape(sprite_t *sprite, size_t cur_frame);

/**
 * Returns the collision shape of a sprite.
 *
 * @param sprite a pointer to a sprite
 * @return the collision shape of the sprite
 */
SDL_Rect sprite_get_collision_shape(sprite_t *sprite);

/**
 * Returns the hitbox shape of a sprite.
 *
 * @param sprite a pointer to a sprite
 * @return the hitbox shape of the sprite
 */
SDL_Rect sprite_get_hitbox_shape(sprite_t *sprite);

/**
 * Returns the texture of a sprite.
 *
 * @param sprite a pointer to a sprite
 * @return the sprite's texture
 */
SDL_Texture *sprite_get_texture(sprite_t *sprite);

/**
 * Returns the animation speed of a sprite
 *
 * @param sprite a pointer to a sprite
 * @return the animation speed of the sprite (double)
 */
double sprite_get_animation_speed(sprite_t *sprite);

/**
 * Returns the number of animation frames a sprite has.
 *
 * @param sprite a pointer to a sprite
 * @return the number of animation frames the sprite has
 */
size_t sprite_get_animation_frames(sprite_t *sprite);

/**
 * Returns whether or not a sprite is animated.
 *
 * @param sprite a pointer to a sprite
 * @return whether or not the sprite is animated
 */
bool sprite_is_animated(sprite_t *sprite);

// Setters

/**
 * Sets the shape of a sprite.
 *
 * @param sprite a pointer to a sprite
 * @param shape the new shape
 */
void sprite_set_shape(sprite_t *sprite, SDL_Rect shape);

/**
 * Sets the collision shape of a sprite.
 *
 * @param sprite a pointer to a sprite
 * @param collision_shape the new collision shape
 */
void sprite_set_collision_shape(sprite_t *sprite, SDL_Rect collision_shape);

/**
 * Sets the hitbox shape of a sprite.
 *
 * @param sprite a pointer to a sprite
 * @param hitbox_shape the new hitbox shape
 */
void sprite_set_hitbox_shape(sprite_t *sprite, SDL_Rect hitbox_shape);

/**
 * Sets the texture of a sprite.
 *
 * @param sprite a pointer to a sprite
 * @param texture the new texture
 */
void sprite_set_texture(sprite_t *sprite, SDL_Texture *texture);

/**
 * Sets the animation speed of a sprite.
 *
 * @param sprite a pointer to a sprite
 * @param animation_speed the new animation speed
 */
void sprite_set_animation_speed(sprite_t *sprite, double animation_speed);

/**
 * Sets the number of animation frames a sprite has.
 *
 * @param sprite a pointer to a sprite
 * @param animation_frames the new number of animation frames
 */
void sprite_set_animation_frames(sprite_t *sprite, size_t animation_frames);

/**
 * Releases the memory allocated for a sprite.
 *
 * @param sprite a pointer to the sprite to be freed
 */
void sprite_free(void *sprite);

#endif // #ifndef __SPRITE_H__
