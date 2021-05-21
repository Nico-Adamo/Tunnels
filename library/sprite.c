#include "sprite.h"
#include "sdl_wrapper.h"

typedef struct sprite {
    SDL_Rect shape; // Actual pixel shape, eg 16x16 or 16x32
    SDL_Rect collision_shape;
    SDL_Rect hitbox_shape;
    SDL_Texture *texture;
} sprite_t;

sprite_t *sprite_init(const char *path, SDL_Rect shape, SDL_Rect collision_shape, SDL_Rect hitbox_shape) {
    sprite_t *sprite = malloc(sizeof(sprite_t));

    sprite->texture = sdl_load_texture(path);
    sprite->shape = shape;
    sprite->collision_shape = collision_shape;
    sprite->hitbox_shape = hitbox_shape;

    return sprite;
}

// Getters
SDL_Rect sprite_get_shape(sprite_t *sprite) {
    return sprite->shape;
}

SDL_Rect sprite_get_collision_shape(sprite_t *sprite) {
    return sprite->collision_shape;
}

SDL_Rect sprite_get_hitbox_shape(sprite_t *sprite) {
    return sprite->hitbox_shape;
}

SDL_Texture *sprite_get_texture(sprite_t *sprite) {
    return sprite->texture;
}

// Setters
void sprite_set_shape(sprite_t *sprite, SDL_Rect shape) {
    sprite->shape = shape;
}

void sprite_set_collision_shape(sprite_t *sprite, SDL_Rect collision_shape) {
    sprite->collision_shape = collision_shape;
}

void sprite_set_hitbox_shape(sprite_t *sprite, SDL_Rect hitbox_shape) {
    sprite->hitbox_shape = hitbox_shape;
}

void sprite_set_texture(sprite_t *sprite, SDL_Texture *texture) {
    sprite->texture = texture;
}
