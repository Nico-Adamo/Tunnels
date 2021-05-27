#ifndef __TEXT_H__
#define __TEXT_H__

#include "body.h"
#include <stdbool.h>

typedef struct ui_text ui_text_t;

ui_text_t *ui_text_init(char *message, rect_t hitbox, double lifetime);
char *ui_text_get_message(ui_text_t *text);
rect_t ui_text_get_hitbox(ui_text_t *text);
void ui_text_free(void *text);
double ui_text_get_timer(ui_text_t *text);
void ui_text_set_removed(ui_text_t *text, bool removed);
bool ui_text_is_removed(ui_text_t *text);

#endif
