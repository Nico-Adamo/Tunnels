#ifndef __UI_COMPONENT_H__
#define __UI_COMPONENT_H__

#include "body.h"

typedef struct UI {
    sprite_t *sprite;
    rect_t hitbox;
    vector_t centroid;
    char *type;
    double scale;
    int cur_frame;
    double animation_timer;
} UI_t;

UI_t *UI_init(sprite_t *sprite, rect_t hitbox, char *type, double scale);

void UI_component_free(UI_t *UI);

SDL_Rect UI_get_shape(UI_t *UI);

rect_t UI_get_hitbox(UI_t *UI);

SDL_Texture *UI_get_texture(UI_t *UI);

vector_t UI_get_centroid(UI_t *UI);

double UI_get_scale(UI_t *UI);

char *UI_get_type(UI_t *UI);

void UI_set_centroid(UI_t *UI, vector_t x);

sprite_t *UI_get_sprite(UI_t *UI);

void UI_set_sprite(UI_t *UI, sprite_t *sprite);

void UI_tick(UI_t *UI, double dt);

#endif // #ifndef __UI_COMPONENT_H__
