#ifndef __SCENE_H__
#define __SCENE_H__

#include "body.h"
#include "list.h"
#include "tile.h"
#include "ui_component.h"
#include "text.h"
#include <stdbool.h>

/**
 * A collection of bodies and force creators.
 * The scene automatically resizes to store
 * arbitrarily many bodies and force creators.
 */
typedef struct scene scene_t;

/**
 * All possible room types.
 */
typedef enum {KILL, SURVIVE, NAVIGATE, BOSS, POST_BOSS} room_type_t;

/**
 * A function which adds some forces or impulses to bodies,
 * e.g. from collisions, gravity, or spring forces.
 * Takes in an auxiliary value that can store parameters or state.
 */
typedef void (*force_creator_t)(void *aux, double dt);

/**
 * Allocates memory for an empty scene.
 * Makes a reasonable guess of the number of bodies to allocate space for.
 * Asserts that the required memory is successfully allocated.
 *
 * @return the new scene
 */
scene_t *scene_init(void);

/**
 * Releases memory allocated for a given scene
 * and all the bodies and force creators it contains.
 *
 * @param scene a pointer to a scene returned from scene_init()
 */
void scene_free(scene_t *scene);

/**
 * Gets the number of bodies in a given scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @return the number of bodies added with scene_add_body()
 */
size_t scene_bodies(scene_t *scene);

/**
 * Sets whether or not the current scene is the menu.
 *
 * @param scene a pointer to a scene
 * @param is_menu whether the scene is the menu or not
 */
void scene_set_is_menu(scene_t *scene, bool is_menu);

/**
 * Returns whether or not the current scene is the menu.
 *
 * @param scene a pointer toa scene
 * @return whether the scene is the menu or not (boolean)
 */
bool scene_is_menu(scene_t *scene);

/**
 * Gets the body at a given index in a scene.
 * Asserts that the index is valid.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param index the index of the body in the scene (starting at 0)
 * @return a pointer to the body at the given index
 */
body_t *scene_get_body(scene_t *scene, size_t index);

/**
 * Adds a body to a scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param body a pointer to the body to add to the scene
 */
void scene_add_body(scene_t *scene, body_t *body);

/**
 * @deprecated Use body_remove() instead
 *
 * Removes and frees the body at a given index from a scene.
 * Asserts that the index is valid.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param index the index of the body in the scene (starting at 0)
 */
void scene_remove_body(scene_t *scene, size_t index);

/**
 * @deprecated Use scene_add_bodies_force_creator() instead
 * so the scene knows which bodies the force creator depends on
 */
void scene_add_force_creator(
    scene_t *scene,
    force_creator_t forcer,
    void *aux,
    free_func_t freer
);

/**
 * Adds a force creator to a scene,
 * to be invoked every time scene_tick() is called.
 * The auxiliary value is passed to the force creator each time it is called.
 * The force creator is registered with a list of bodies it applies to,
 * so it can be removed when any one of the bodies is removed.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param forcer a force creator function
 * @param aux an auxiliary value to pass to forcer when it is called
 * @param bodies the list of bodies affected by the force creator.
 *   The force creator will be removed if any of these bodies are removed.
 *   This list does not own the bodies, so its freer should be NULL.
 * @param freer if non-NULL, a function to call in order to free aux
 */
void scene_add_bodies_force_creator(
    scene_t *scene,
    force_creator_t forcer,
    void *aux,
    list_t *bodies,
    free_func_t freer
);

/**
 * Adds a floor tile to a scene.
 *
 * @param scene a pointer to a scene
 * @param tile a pointer to the tile to add to the scene
 */
void scene_add_floor_tile(scene_t *scene, tile_t *tile);

/**
 * Adds a well tile to a scene.
 *
 * @param scene a pointer to a scene
 * @param tile a pointer to the tile to add to the scene
 */
void scene_add_wall_tile(scene_t *scene, tile_t *tile);

/**
 * Adds a collider tile to a scene.
 *
 * @param scene a pointer to a scene
 * @param tile a pointer to the tile to add to the scene
 */
void scene_add_collider_tile(scene_t *scene, tile_t *tile);

/**
 * Adds a UI component to a scene.
 *
 * @param scene a pointer to a scene
 * @param UI a pointer to the UI component to add to the scene
 */
void scene_add_UI_component(scene_t *scene, UI_t *UI);

/**
 * Adds UI text to a scene.
 *
 * @param scene a pointer to a scene
 * @param UI a pointer to the UI text to add to the scene
 */
void scene_add_UI_text(scene_t *scene, ui_text_t *text);

/**
 * Sets the room type of a scene and adds corresponding objective text.
 *
 * @param scene a pointer to a scene
 * @param room_type the type to be set for the scene
 */
void scene_set_room_type(scene_t *scene, room_type_t room_type);

/**
 * Sets the unlock time for a scene.
 * Called when necessary for specific time-based rooms.
 *
 * @param scene a pointer to a scene
 * @param unlock_time the time before the scene is unlocked
 */
void scene_set_unlock_time(scene_t *scene, double unlock_time);

/**
 * Returns a list of the floor tiles for a scene.
 *
 * @param scene a pointer to a scene
 * @return a list of the floor tiles in the specified scene
 */
list_t *scene_get_floor_tiles(scene_t *scene);

/**
 * Returns a list of the wall tiles for a scene.
 *
 * @param scene a pointer to a scene
 * @return a list of the wall tiles in the specified scene
 */
list_t *scene_get_wall_tiles(scene_t *scene);

/**
 * Returns a list of the collider tiles for a scene.
 *
 * @param scene a pointer to a scene
 * @return a list of the collider tiles in the specified scene
 */
list_t *scene_get_collider_tiles(scene_t *scene);

/**
 * Returns a list of the UI components for a scene.
 *
 * @param scene a pointer to a scene
 * @return a list of the UI components in the specified scene
 */
list_t *scene_get_UI_components(scene_t *scene);

/**
 * Returns a list of the UI texts for a scene.
 *
 * @param scene a pointer to a scene
 * @return a list of the UI texts in the specified scene
 */
list_t *scene_get_UI_texts(scene_t *scene);

/**
 * Returns the room type for a given scene.
 *
 * @param scene a pointer to a scene
 * @return the room type for the specified scene
 */
room_type_t scene_get_room_type(scene_t *scene);

/**
 * Returns the unlock time for a given scene.
 *
 * @param scene a pointer to a scene
 * @return the unlock time for the specified scene
 */
double scene_get_unlock_time(scene_t *scene);

/**
 * Returns whether or not the objective for a scene (room) has been met.
 *
 * @param scene a pointer to a scene
 * @return whether or not the objective has been met
 */
bool scene_check_objective(scene_t *scene);

/**
 * Sorts a list of bodies in a scene by depth.
 *
 * @param scene a pointer to a scene
 */
void scene_sort_by_depth(scene_t *scene);

/**
 * Returns a list of enemies in a scene.
 *
 * @param scene a pointer to a scene
 * @return a list of enemies in a scene
 */
list_t *scene_get_enemies(scene_t *scene);

/**
 * Executes a tick of a given scene over a small time interval.
 * This requires executing all the force creators
 * and then ticking each body (see body_tick()).
 * If any bodies are marked for removal, they should be removed from the scene
 * and freed, along with any force creators acting on them.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param dt the time elapsed since the last tick, in seconds
 */
void scene_tick(scene_t *scene, double dt);

#endif // #ifndef __SCENE_H__
