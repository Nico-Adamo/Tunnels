#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include "body.h"

typedef struct UI {
    SDL_Rect shape;
    rect_t hitbox;
    SDL_Texture *texture;
    vector_t centroid;
    char *type;
    double scale;
} UI_t;

UI_t *UI_init(SDL_Rect shape, rect_t hitbox, SDL_Texture *texture, char *type, double scale);

void UI_component_free(UI_t *UI);

SDL_Rect UI_get_shape(UI_t *UI);

rect_t UI_get_hitbox(UI_t *UI);

SDL_Texture *UI_get_texture(UI_t *UI);

vector_t UI_get_centroid(UI_t *UI);

double UI_get_scale(UI_t *UI);

char *UI_get_type(UI_t *UI);

void UI_set_centroid(UI_t *UI, vector_t x);

#endif // #ifndef __USER_INTERFACE_H__