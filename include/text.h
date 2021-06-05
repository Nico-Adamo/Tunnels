#ifndef __TEXT_H__
#define __TEXT_H__

#include "body.h"
#include <stdbool.h>

typedef struct ui_text ui_text_t;

enum text_type{OBJECTIVE_TEXT, EXP_TEXT};

/**
 * Allocates memory for and initializes a UI text.
 *
 * @param message the message for the text
 * @param bottom_left the bottom left coordinate vector for the text
 * @param lifetime how long the text will display
 * @param type the type of text
 * @return the initialized UI text
 */
ui_text_t *ui_text_init(char *message, vector_t bottom_left, double lifetime, enum text_type type);

/**
 * Returns the message for a UI text.
 *
 * @param text a pointer to a UI text
 * @return the message for the UI text
 */
char *ui_text_get_message(ui_text_t *text);

/**
 * Sets the message for a UI text.
 *
 * @param text a pointer to a UI text
 * @param message the new message
 */
void ui_text_set_message(ui_text_t *text, char *message);

/**
 * Returns the type of a UI text.
 *
 * @param text a pointer to a UI text
 * @return the enum type for the text
 */
enum text_type ui_text_get_type(ui_text_t *text);

/**
 * Returns the bottom left coordinate vector for a UI text.
 *
 * @param text a pointer to a UI text
 * @return a vector representing the bottom left of the UI text
 */
vector_t ui_text_get_bottom_left(ui_text_t *text);

/**
 * Releases the memory allocated for a UI text.
 *
 * @param text a pointer to a UI text
 */
void ui_text_free(void *text);

/**
 * Returns the timer (time remaining) for a UI text.
 *
 * @param text a pointer to a UI text
 * @return the time remaining (timer) for the UI text
 */
double ui_text_get_timer(ui_text_t *text);

/**
 * Sets whether or not a UI text is marked for removal.
 *
 * @param text a pointer to a UI text
 * @param removed whether the UI text is marked for removal or not
 */
void ui_text_set_removed(ui_text_t *text, bool removed);

/**
 * Returns whether or not a UI text has been marked for removal.
 *
 * @param text a pointer to a UI text
 * @return whether or not the UI text has been marked for removal
 */
bool ui_text_is_removed(ui_text_t *text);

/**
 * Updates a UI text after a given time has elapsed.
 * (Decrements the timer (time remaining) for the text by dt.)
 *
 * @param text a pointer to a UI text
 * @param dt the number of seconds elapsed since the last tick
 */
void ui_text_tick(ui_text_t *text, double dt);

#endif
