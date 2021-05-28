#include "text.h"

typedef struct ui_text {
    char *message;
    vector_t bottom_left;
    double timer;
    bool removed;
} ui_text_t;

ui_text_t *ui_text_init(char *message, vector_t bottom_left, double lifetime) {
    ui_text_t *text = malloc(sizeof(ui_text_t));
    text->message = message;
    text->bottom_left = bottom_left;
    text->timer = lifetime;
    text->removed = false;
    return text;
}

char *ui_text_get_message(ui_text_t *text) {
    return text->message;
}

void ui_text_set_message(ui_text_t *text, char *message) {
    text->message = message;
}

vector_t ui_text_get_bottom_left(ui_text_t *text) {
    return text->bottom_left;
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
