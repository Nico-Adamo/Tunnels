#include "sprite.h"
#include "sdl_wrapper.h"

typedef struct sprite {
    SDL_Rect shape; // Actual pixel shape, eg 16x16 or 16x32
    SDL_Rect collision_shape;
    SDL_Rect hitbox_shape;
    SDL_Texture *texture;
    bool is_animated;
    double animation_speed;
    size_t animation_frames;
} sprite_t;

sprite_t *sprite_init(const char *path, SDL_Rect shape, SDL_Rect collision_shape, SDL_Rect hitbox_shape,  bool is_animated, double animation_speed, size_t animation_frames) {
    sprite_t *sprite = malloc(sizeof(sprite_t));

    sprite->texture = sdl_load_texture(path);
    sprite->shape = shape;
    sprite->collision_shape = collision_shape;
    sprite->hitbox_shape = hitbox_shape;
    sprite->animation_speed = animation_speed;
    sprite->animation_frames = animation_frames;
    sprite->is_animated = is_animated;

    return sprite;
}

// Getters
SDL_Rect sprite_get_shape(sprite_t *sprite, size_t cur_frame) {
    if (!sprite->is_animated) return sprite->shape;
    SDL_Rect curr_draw_shape = {
        sprite->shape.x + cur_frame * sprite->shape.w,
        sprite->shape.y,
        sprite->shape.w,
        sprite->shape.h
    };
    // printf("%d, x: %d, y: %d || ", sprite->current_frame, curr_draw_shape.x, curr_draw_shape.y);
    // printf("w: %d, h: %d\n", sprite->shape.w, sprite->shape.h);
    return curr_draw_shape;
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

double sprite_get_animation_speed(sprite_t *sprite) {
    return sprite->animation_speed;
}

size_t sprite_get_animation_frames(sprite_t *sprite) {
    return sprite->animation_frames;
}


bool sprite_is_animated(sprite_t *sprite) {
    return sprite->is_animated;
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

void sprite_set_animation_speed(sprite_t *sprite, double animation_speed) {
    sprite->animation_speed = animation_speed;
}

void sprite_set_animation_frames(sprite_t *sprite, size_t animation_frames) {
    sprite->animation_frames = animation_frames;
}
