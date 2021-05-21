#include "user_interface.h"

UI_t *UI_init(SDL_Rect shape, rect_t hitbox, SDL_Texture *texture, char *type, double scale) {
    UI_t *UI = malloc(sizeof(UI_t));

    UI->shape = shape;
    UI->hitbox = hitbox;
    UI->texture = texture;
    UI->type = type;
    UI->scale = scale;

    UI->centroid.x = hitbox.x + hitbox.w / 2;
    UI->centroid.y = hitbox.y + hitbox.h / 2;
    return UI;
}

// Texture??
void UI_component_free(UI_t *UI) {
    free(UI->type);
    free(UI);
}

SDL_Rect UI_get_shape(UI_t *UI) {
    return UI->shape;
}

rect_t UI_get_hitbox(UI_t *UI) {
    return UI->hitbox;
}

SDL_Texture *UI_get_texture(UI_t *UI) {
    return UI->texture;
}

vector_t UI_get_centroid(UI_t *UI) {
    return UI->centroid;
}

double UI_get_scale(UI_t *UI) {
    return UI->scale;
}

char *UI_get_type(UI_t *UI) {
    return UI->type;
}

void UI_set_centroid(UI_t *UI, vector_t x) {
    vector_t translation = vec_subtract(x, UI->centroid);
    UI->centroid = x;
    UI->hitbox.x += translation.x;
    UI->hitbox.y += translation.y;
}