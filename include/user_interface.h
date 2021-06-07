#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include "game.h"

extern const int MENU_SPRITE_ID;
extern const int PRESS_F_ID;
extern const int ATK_MURAL_ID;
extern const int SPD_MURAL_ID;
extern const int HP_MURAL_ID;
extern const int CD_MURAL_ID;
extern const int INV_MURAL_ID;
extern const int EMPTY_HEART_ID;
extern const int HALF_HEART_ID;
extern const int FULL_HEART_ID;
extern const int COIN_FILLED_ID;
extern const int COIN_EMPTY_ID;
extern const int EXPERIENCE;
extern const int HEALTH;
extern const int HP_POWERUP;
extern const int ATK_POWERUP;
extern const int CD_POWERUP;
extern const int SPD_POWERUP;
extern const int INV_POWERUP;

/**
 * Handles door interactions, showing the "PRESS F" UI component
 * as long as the player is in the door interactor's area.
 *
 * @param game a pointer to a game
 * @param entered_area whether the play was in the area of the door
 * @return whether the player is still in the area of the door
 */
bool UI_handle_door_interaction(game_t *game, bool entered_area);

#endif // #ifndef __USER_INTERFACE_H__
