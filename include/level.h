#include "game.h"
#include "sdl_wrapper.h"
#include "enemy.h"
#include "map.h"
#include "forces.h"
#include "sound.h"

extern const char *SPRITE;

extern const double MAX_WIDTH;
extern const double MAX_HEIGHT;

extern const double HALF_HEART_HEALTH;
extern const double PLAYER_HEALTH;
extern const double HEART_PADDING;

const char *MURAL_TEXT = "MURAL";

/**
 * Shuffles the array of levels.
 */
void shuffle_levels();

/**
 * Initializes a heart UI element.
 *
 * @param x the bottom left x-cordinate
 * @param y the bottom left y-coordinate
 * @param sprite the sprite to display
 * @param type the type of heart
 * @return the created UI element (heart)
 */
UI_t *make_heart(double x, double y, sprite_t *sprite, char *type);

/**
 * Returns a list of all player hearts in a given scene.
 *
 * @param scene a pointer to a scene
 * @return a list of all player hearts in the scene
 */
list_t *get_player_hearts(scene_t *scene);

/**
 * Initializes a player.
 *
 * @param game a pointer to a game
 * @param x the bottom left x-coordiante
 * @param y the bottom left y-coordinate
 * @param type the player type
 * @param info the stats info for the player
 * @return a pointer to the created player
 */
body_t *make_player(game_t *game, double x, double y, enum body_type type, stats_info_t info);

/**
 * Initializes an (experience) coin.
 *
 * @param x the bottom left x-cordinate
 * @param y the bottom left y-coordinate
 * @param sprite the sprite to display
 * @param type the type of coin
 * @return the created UI element (coin)
 */
UI_t *make_coin(double x, double y, sprite_t *sprite, char *type);

/**
 * Returns a list of all player coins in a given scene.
 *
 * @param scene a pointer to a scene
 * @return a list of all player coins in the scene
 */
list_t *get_player_coins(scene_t *scene);

/**
 * Resets the scene of a game.
 *
 * @param game a pointer to a game
 * @return the reset scene
 */
scene_t *scene_reset(game_t *game);

/**
 * Adds the title/start screen to a game.
 *
 * @param game a pointer to a game
 * @return a pointer to the scene with the title screen
 */
scene_t *make_title(game_t *game);

/**
 * Selects and plays random music for a given room.
 */
void random_room_music();

/**
 * Creates a room (loads the specified map, creates relevant collisions, etc.)
 *
 * @param game a pointer to a game
 */
void make_room(game_t *game);

/**
 * Creates a level in a game by selecting rooms, boss room, music, etc.
 *
 * @param game a pointer to a game
 * @param level the level to create
 */
void make_level(game_t *game, int level);

/**
 * A tile interaction to be called at the end of a room (door).
 * Proceeds to the next room if objectives have been met and handles
 * boss and post-boss related music.
 *
 * @param game a pointer to a game
 */
void game_end_room(game_t *game);

/**
 * A tile interaction to be called at the end of a level.
 * Progresses the game to the next level and handles scaling and/or victory.
 */
void game_end_level(game_t *game);

/**
 * A tile interaction to be called when murals are interacted with.
 * Selects a random mural, displays it, and handles the related buffs.
 *
 * @param game a pointer to a game
 */
void game_random_mural(game_t *game);
