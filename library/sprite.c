#include "sprite.h"

typedef struct sprite {
    SDL_Rect shape; // Actual pixel shape, eg 16x16 or 16x32
    SDL_Rect collision_shape;
    SDL_Rect hitbox_shape;
    SDL_Texture *texture;
} sprite_t;

typedef struct sprite_info {
    SDL_Rect shape; // Actual pixel shape, eg 16x16 or 16x32
    SDL_Rect collision_shape;
    SDL_Rect hitbox_shape;
    const char *path;
} sprite_info_t;

const sprite_info_t SPRITE_INFOS[] = {
    // **** PLAYER RELATED *****
    // 0 - PLAYER
    {{0, 0, 16, 32},
    {3, 0, 12, 6},
    {3, 0, 9, 16},
    "assets/knight_f_idle_anim_f0.png"
    },
    // 1 - HORIZONTAL BULLET
    {{0, 0, 7, 4},
    {0, 0, 7, 4},
    {0, 0, 7, 4},
    "assets/bullet_horizontal_default.png"
    },
    // 2 - VERTICAL BULLET
    {{0, 0, 4, 7},
    {0, 0, 4, 7},
    {0, 0, 4, 7},
    "assets/bullet_vertical_default.png"
    },
};

sprite_t *sprite_init(const char *path, SDL_Rect shape, SDL_Rect collision_shape, SDL_Rect hitbox_shape) {
    sprite_t *sprite = malloc(sizeof(sprite_t));

    sprite->texture = sdl_load_texture(path);
    sprite->shape = shape;
    sprite->collision_shape = collision_shape;
    sprite->hitbox_shape = hitbox_shape;

    return sprite;
}

void sprite_register_sprites(game_t *game) {
    for(int i = 0; i< sizeof(SPRITE_INFOS) / sizeof(const char*); i++) {
        sprite_info_t sprite_info = SPRITE_INFOS[i];
        game_add_sprite(game, sprite_init(sprite_info.path, sprite_info.shape, sprite_info.collision_shape, sprite_info.hitbox_shape));
    }
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