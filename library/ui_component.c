#include "ui_component.h"

UI_t *UI_init(sprite_t *sprite, rect_t hitbox, char *type, double scale) {
    UI_t *UI = malloc(sizeof(UI_t));

    UI->sprite = sprite;
    UI->hitbox = hitbox;
    UI->type = type;
    UI->scale = scale;
    UI->cur_frame = 0;
    UI->animation_timer = 0;
    UI->centroid.x = hitbox.x + hitbox.w / 2;
    UI->centroid.y = hitbox.y + hitbox.h / 2;
    return UI;
}

// Texture??
void UI_component_free(UI_t *UI) {
    free(UI);
}

SDL_Rect UI_get_shape(UI_t *UI) {
    return sprite_get_shape(UI->sprite, UI->cur_frame);
}

rect_t UI_get_hitbox(UI_t *UI) {
    return UI->hitbox;
}

SDL_Texture *UI_get_texture(UI_t *UI) {
    return sprite_get_texture(UI->sprite);
}

vector_t UI_get_centroid(UI_t *UI) {
    return UI->centroid;
}

void UI_set_centroid(UI_t *UI, vector_t x) {
    vector_t translation = vec_subtract(x, UI->centroid);
    UI->centroid = x;
    UI->hitbox.x += translation.x;
    UI->hitbox.y += translation.y;
}

double UI_get_scale(UI_t *UI) {
    return UI->scale;
}

char *UI_get_type(UI_t *UI) {
    return UI->type;
}


sprite_t *UI_get_sprite(UI_t *UI) {
    return UI->sprite;
}

void UI_set_sprite(UI_t *UI, sprite_t *sprite) {
    UI->sprite = sprite;
}

void UI_tick(UI_t *UI, double dt) {
    if (sprite_is_animated(UI->sprite)) {
        if(UI->cur_frame >= sprite_get_animation_frames(UI->sprite)) {
            UI->cur_frame = 0;
        }
        UI->animation_timer += dt;
        if (UI->animation_timer >= sprite_get_animation_speed(UI->sprite)) {
            UI->cur_frame = (UI->cur_frame + 1) % sprite_get_animation_frames(UI->sprite);
            UI->animation_timer = 0;
        }
    }
}
