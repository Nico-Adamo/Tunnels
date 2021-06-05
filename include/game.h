#ifndef __GAME_H__
#define __GAME_H__

#include "list.h"
#include "scene.h"
#include "sprite.h"

/**
 * A game.
 * Stores all components of a current game (e.g., scene, player, sprites, levels)
 */
typedef struct game game_t;

/**
 * A function that can be called on a tile interactor
 * to handle its respective interaction.
 */
typedef void (*tile_interaction)(game_t *game);

/**
 * Specifies the type of a tile interactor
 */
enum tile_interactor_type{PRESS_F, MURAL};

/**
 * A tile interactor with a given area, specific interaction, and type.
 */
typedef struct tile_interactor {
    rect_t area;
    tile_interaction interaction;
    enum tile_interactor_type type;
} tile_interactor_t;

/**
 * Initializes a game with the specified scale.
 *
 * @param scale the scale of the game
 * @return the created game
 */
game_t *game_init(double scale);

/**
 * Releases the memory allocated for a game.
 *
 * @param game a pointer to the game to be freed
 */
void game_free(void *game);

/**
 * Adds provided tile info to a game.
 *
 * @param game a pointer to a game
 * @param tile_info the tile info to be added
 */
void game_add_tile_info(game_t *game, tile_info_t *tile_info);

/**
 * Allocates memory for a tile interactor with specified paramters.
 *
 * @param area the area (rect_t) for the interactor
 * @param interaction the interaction assossiated with the interactor
 * @param type the type of the interactor
 * @return the created tile interactor
 */
tile_interactor_t *tile_interactor_init(rect_t area, tile_interaction interaction, enum tile_interactor_type type);

/**
 * Adds a tile interactor to a game.
 *
 * @param game a pointer to a game
 * @param interactor the tile interactor to be added
 */
void game_add_tile_interactor(game_t *game, tile_interactor_t *interactor);

/**
 * Returns all tile interactors in a game.
 *
 * @param game a pointer to a game
 * @return a list of all tile interactors in the game
 */
list_t *game_get_tile_interactors(game_t *game);

/**
 * Resets the tile interactors in a game.
 *
 * @param game a pointer to a game
 */
void game_reset_tile_interactors(game_t *game);

/**
 * Adds a sprite to a game.
 *
 * @param game a pointer to a game
 * @param sprite the sprite to be added
 */
void game_add_sprite(game_t *game, sprite_t *sprite);

/**
 * Returns the current scene of a game.
 *
 * @param game a pointer to a game
 * @return the current scene of the game
 */
scene_t *game_get_current_scene(game_t *game);

/**
 * Sets the current scene of a game.
 *
 * @param game a pointer to a game
 * @param scene the new current scene for the game
 */
void game_set_current_scene(game_t *game, scene_t *scene);

/**
 * Returns the scale of a game.
 *
 * @param game a pointer to a game
 * @return the (double) scale of the game
 */
double game_get_scale(game_t *game);

/**
 * Returns the player of a game.
 *
 * @param game a pointer to a game
 * @return the player in the specified game
 */
body_t *game_get_player(game_t *game);

/**
 * Sets the player of a game.
 *
 * @param game a pointer to a game
 * @param player the new player (body_t) in the game
 */
void game_set_player(game_t *game, body_t *player);

/**
 * Returns the tile info for a specified tile id in a game.
 *
 * @param game a pointer to a game
 * @param tile_id the id of the desired tile info
 * @return the tile info for the specified id in the game
 */
tile_info_t *game_get_tile_info(game_t *game, size_t tile_id);

/**
 * Returns the sprite for a specified sprite id in a game.
 *
 * @param game a pointer to a game
 * @param sprite_id the id of the desired sprite
 * @return the sprite that corresponds to the specified id in the game
 */
sprite_t *game_get_sprite(game_t *game, size_t sprite_id);

/**
 * Registers all sprites to a game.
 *
 * @param game a pointer to a game.
 */
void game_register_sprites(game_t *game);

/**
 * Resets the dungeon in a game.
 *
 * @param game a pointer to a game
 */
void game_reset_dungeon(game_t *game);

/**
 * Adds a room to the current game (i.e., dungeon)
 *
 * @param game a pointer to a game
 * @param path the file path to the new room
 */
void game_add_room(game_t *game, const char *path);

/**
 * Returns the current dungeon of a game
 *
 * @param game a pointer to a game
 * @return a list representing the current dungeon of the specified game
 */
list_t *game_get_dungeon(game_t *game);

/**
 * Returns the current room of a game.
 *
 * @param game a pointer to a game
 */
size_t game_get_room(game_t *game);

/**
 * Progresses a game to the next room.
 *
 * @param game a pointer to a game
 */
void game_next_room(game_t *game);

/**
 * Sets the current room of a game.
 *
 * @param game a pointer to a game
 * @param room the new current room
 */
void game_set_room(game_t *game, int room);

/**
 * Gets the current level of a game.
 *
 * @param game a pointer to a game
 * @return the current level
 */
size_t game_get_level(game_t *game);

/**
 * Sets the current level of a game.
 *
 * @param game a pointer to a game
 * @param level the new current level
 */
void game_set_level(game_t *game, size_t level);

/**
 * Returns whether or not a game is paused.
 *
 * @param game a pointer to a game
 * @return whether the game is paused or not (boolean)
 */
bool game_is_paused(game_t *game);

/**
 * Set whether or not a game is paused.
 *
 * @param game a pointer to a game
 * @param paused whether or not the game is paused
 */
void game_set_paused(game_t *game, bool paused);

/**
 * Returns the size of the current dungeon.
 *
 * @param game a pointer to a game
 * @return the size of the current dungeon in the game
 */
size_t game_get_dungeon_size(game_t *game);

#endif // #ifndef __GAME_H__
