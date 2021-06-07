#include "user_interface.h"
#include "collision.h"
#include "ui_component.h"
#include "level.h"

const int MENU_SPRITE_ID = 48;
const int PRESS_F_ID = 49;
const int EMPTY_HEART_ID = 50;
const int HALF_HEART_ID = 51;
const int FULL_HEART_ID = 52;
const int HP_MURAL_ID = 64;
const int ATK_MURAL_ID = 65;
const int SPD_MURAL_ID = 66;
const int CD_MURAL_ID = 67;
const int INV_MURAL_ID = 68;
const int COIN_FILLED_ID = 59;
const int COIN_EMPTY_ID = 60;
const int EXPERIENCE = 61;
const int HEALTH = 62;
const int HP_POWERUP = 53;
const int ATK_POWERUP = 54;
const int CD_POWERUP = 55;
const int SPD_POWERUP = 56;
const int INV_POWERUP = 57;

const double BUFFER = 40;
const double BOTTOM_LEFT_X = 512 - 125/2;
const double BOTTOM_LEFT_Y = 256;
const double HITBOX_WIDTH = 120;
const double HITBOX_HEIGHT = 46;
const double SHAPE_WIDTH = 600;
const double SHAPE_HEIGHT = 230;
const double SCALE = 0.2;

bool UI_handle_door_interaction(game_t *game, bool entered_area) {
    bool still_in_area = false;
    list_t *interactors = game_get_tile_interactors(game);
    body_t *player_current = game_get_player(game);
    scene_t *scene = game_get_current_scene(game);
    for(size_t i = 0; i<list_size(interactors); i++) {
        tile_interactor_t *interactor = list_get(interactors, i);
        if (find_collision(interactor->area, body_get_hitbox(player_current)).collided) {
            if(entered_area == false) {
                SDL_Rect shape = (SDL_Rect) {0, 0, SHAPE_WIDTH, SHAPE_HEIGHT};
                rect_t player_hitbox = body_get_draw_hitbox(player_current);
                double buffer_dist = BUFFER;
                rect_t hitbox = (rect_t) {BOTTOM_LEFT_X, BOTTOM_LEFT_Y + player_hitbox.h/2 + buffer_dist, HITBOX_WIDTH, HITBOX_HEIGHT};
                UI_t *press_F = UI_init(game_get_sprite(game, PRESS_F_ID), hitbox, "PRESS_F", SCALE);
                scene_add_UI_component(scene, press_F);
            }
            still_in_area = true;
        }
    }
    return still_in_area;
}
