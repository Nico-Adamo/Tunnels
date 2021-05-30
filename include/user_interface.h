#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include "game.h"

extern const int MENU_SPRITE_ID;
extern const int PRESS_F_ID;
extern const int MURAL_ID;
extern const int EMPTY_HEART_ID;
extern const int HALF_HEART_ID;
extern const int FULL_HEART_ID;
extern const int COIN_FILLED_ID;
extern const int COIN_EMPTY_ID;

bool UI_handle_door_interaction(game_t *game, bool entered_area);

#endif // #ifndef __USER_INTERFACE_H__
