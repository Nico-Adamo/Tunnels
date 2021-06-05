#ifndef __UI_COMPONENT_H__
#define __UI_COMPONENT_H__

#include "body.h"

/**
 * A visual component that appears above the bodies and tiles.
 * Implemented as a sprite.
 * Doesn't interact with other components in the scene.
 */
typedef struct UI {
    sprite_t *sprite;
    rect_t hitbox;
    vector_t centroid;
    char *type;
    double scale;
    int cur_frame;
    double animation_timer;
} UI_t;

/**
 * Allocates memory for a UI component with the given parameters.
 *
 * @param sprite sprite of a UI component
 * @param hitbox the position and size of a hitbox
 * @param type type of a UI component
 * @param scale scale of a UI component to its sprite image
 * @return a pointer to the newly allocated body
 */
UI_t *UI_init(sprite_t *sprite, rect_t hitbox, char *type, double scale);

/**
 * Releases the memory allocated for a UI component.
 * 
 * @param body a pointer to a UI component returned from UI_init()
 */
void UI_component_free(UI_t *UI);

/**
 * Returns the shape and size of a UI component
 * 
 * @param body a pointer to a UI component
 * @return shape and size of a UI component
 */
SDL_Rect UI_get_shape(UI_t *UI);

/**
 * Returns the location and size of a UI component
 * 
 * @param body a pointer to a UI component
 * @return location and size of a UI component
 */
rect_t UI_get_hitbox(UI_t *UI);

/**
 * Returns the sprite texture of a UI component
 * 
 * @param body a pointer to a UI component
 * @return sprite texture of a UI component
 */
SDL_Texture *UI_get_texture(UI_t *UI);

/**
 * Returns the centroid of a UI component
 * 
 * @param body a pointer to a UI component
 * @return centroid of a UI component
 */
vector_t UI_get_centroid(UI_t *UI);

/**
 * Sets the centroid of a UI component
 * 
 * @param body a pointer to a UI component
 * @param x the new centroid for the UI component
 */
void UI_set_centroid(UI_t *UI, vector_t x);

/**
 * Returns the scale of a UI component
 * Scale gives the ratio of the sprite image size to hitbox size
 * 
 * @param body a pointer to a UI component
 * @return shape and size of a UI component
 */
double UI_get_scale(UI_t *UI);

/**
 * Returns the type of a UI component
 * 
 * @param body a pointer to a UI component
 * @return type of a UI component
 */
char *UI_get_type(UI_t *UI);

/**
 * Returns the sprite of a UI component
 * 
 * @param body a pointer to a UI component
 * @return sprite of a UI component
 */
sprite_t *UI_get_sprite(UI_t *UI);

/**
 * Sets the sprite of a UI component
 * 
 * @param body a pointer to a UI component
 * @param sprite the new sprite for the UI component
 */
void UI_set_sprite(UI_t *UI, sprite_t *sprite);

/**
 * If the sprite is animated, pdates the UI component after a given time 
 * interval has elapsed.
 *
 * @param UI the UI component to tick
 * @param dt the number of seconds elapsed since the last tick
 */
void UI_tick(UI_t *UI, double dt);

#endif // #ifndef __UI_COMPONENT_H__
