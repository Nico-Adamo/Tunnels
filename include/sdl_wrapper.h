#ifndef __SDL_WRAPPER_H__
#define __SDL_WRAPPER_H__

#include <stdbool.h>
#include "list.h"
#include "game.h"
#include "vector.h"
#include "user_interface.h"
#include "text.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Values passed to a key handler when the given arrow key is pressed
typedef enum {
    LEFT_ARROW = 1,
    UP_ARROW = 2,
    RIGHT_ARROW = 3,
    DOWN_ARROW = 4,
    ESCAPE = 5
} arrow_key_t;

/**
 * The possible types of key events.
 * Enum types in C are much more primitive than in Java; this is equivalent to:
 * typedef unsigned int KeyEventType;
 * #define KEY_PRESSED 0
 * #define KEY_RELEASED 1
 */
typedef enum {
    KEY_PRESSED,
    KEY_RELEASED
} key_event_type_t;

/**
 * A keypress handler.
 * When a key is pressed or released, the handler is passed its char value.
 * Most keys are passed as their char value, e.g. 'a', '1', or '\r'.
 * Arrow keys have the special values listed above.
 *
 * @param key a character indicating which key was pressed
 * @param type the type of key event (KEY_PRESSED or KEY_RELEASED)
 * @param held_time if a press event, the time the key has been held in seconds
 * @param scene a scene containing all relevant bodies
 */
typedef void (*key_handler_t)(char key, key_event_type_t type, double held_time, game_t *game);

/**
 * Initializes the SDL window and renderer.
 * Must be called once before any of the other SDL functions.
 *
 * @param min the x and y coordinates of the bottom left of the scene
 * @param max the x and y coordinates of the top right of the scene
 */
void sdl_init(vector_t min, vector_t max);

/**
 * Processes all SDL events and returns whether the window has been closed.
 * This function must be called in order to handle keypresses.
 *
 * @param scene a scene containing all relevant bodies
 * @return true if the window was closed, false otherwise
 */
bool sdl_is_done(scene_t *scene);

/**
 * Loads and returns a specified texture.
 *
 * @param path the file path to the texture to load
 * @return the loaded texture
 */
SDL_Texture *sdl_load_texture(const char *path);

/**
 * Draws a texture.
 *
 * @param texture the texture to draw
 * @param source the source rectangle
 * @param destination the destination for the texture (where it is loaded)
 * @param flipped whether or not the texture is flipped
 */
void sdl_draw_texture(SDL_Texture *texture, SDL_Rect source, rect_t destination, bool flipped);

/**
 * Sets the camera for the screen.
 *
 * @param cam the new position for the camera
 */
void sdl_set_camera(vector_t cam);

/**
 * Returns whether a current coordinate is on-screen in relation to
 * the window width/height and the camera position.
 *
 * @param x the x-coodinate
 * @param y the y-coordinate
 * @return if the coordinate is on-screen
 */
bool sdl_is_onscreen(double x, double y);

/**
 * Clears the screen. Should be called before drawing polygons in each frame.
 */
void sdl_clear(void);

/**
 * Displays the rendered frame on the SDL window.
 * Must be called after drawing the polygons in order to show them.
 */
void sdl_show(void);

/**
 * Draws all bodies in a scene.
 * This internally calls sdl_clear(), sdl_draw_polygon(), and sdl_show(),
 * so those functions should not be called directly.
 *
 * @param scene the scene to draw
 */
void sdl_render_scene(scene_t *scene);

/**
 * Renders a given game, including tilemap rendering, sprite rendering,
 * and text rendering.
 * Interally references sdl_draw_texture(), sdl_clear(), sdl_render_tilemap(),
 * and sdl_show(), which should not be called directly.
 *
 * @param game the game to render
 */
void sdl_render_game(game_t *game);

/**
 * Registers a function to be called every time a key is pressed.
 * Overwrites any existing handler.
 *
 * Example:
 * ```
 * void on_key(char key, key_event_type_t type, double held_time) {
 *     if (type == KEY_PRESSED) {
 *         switch (key) {
 *             case 'a':
 *                 printf("A pressed\n");
 *                 break;
 *             case UP_ARROW:
 *                 printf("UP pressed\n");
 *                 break;
 *         }
 *     }
 * }
 * int main(void) {
 *     sdl_on_key(on_key);
 *     while (!sdl_is_done());
 * }
 * ```
 *
 * @param handler the function to call with each key press
 */
void sdl_on_key(key_handler_t handler);

/**
 * Gets the amount of time that has passed since the last time
 * this function was called, in seconds.
 *
 * @return the number of seconds that have elapsed
 */
double time_since_last_tick(void);

#endif // #ifndef __SDL_WRAPPER_H__
