#include "text.h"

typedef struct ui_text {
    char *message;
    rect_t hitbox;
    double timer;
    bool removed;
} ui_text_t;

ui_text_t *ui_text_init(char *message, rect_t hitbox, double lifetime) {
    ui_text_t *text = malloc(sizeof(ui_text_t));
    text->message = message;
    text->hitbox = hitbox;
    text->timer = lifetime;
    text->removed = false;
    return text;
}

char *ui_text_get_message(ui_text_t *text) {
    return text->message;
}

rect_t ui_text_get_hitbox(ui_text_t *text) {
    return text->hitbox;
}

double ui_text_get_timer(ui_text_t *text) {
    return text->timer;
}

void ui_text_tick(ui_text_t *text, double dt) {
    text->timer -= dt;
}

void ui_text_free(void *text) {
    free((ui_text_t *) text);
}

void ui_text_set_removed(ui_text_t *text, bool removed) {
    text->removed = removed;
}

bool ui_text_is_removed(ui_text_t *text) {
    return text->removed;
}
