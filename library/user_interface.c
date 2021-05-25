#include "user_interface.h"
#include "collision.h"
#include "ui_component.h"
#include "level.h"

const int MENU_SPRITE_ID = 48;
const int PRESS_F_ID = 49;
const int EMPTY_HEART_ID = 50;
const int HALF_HEART_ID = 51;
const int FULL_HEART_ID = 52;

bool UI_handle_door_interaction(game_t *game, bool entered_area) {
    bool still_in_area = false;
    list_t *interactors = game_get_tile_interactors(game);
    body_t *player_current = game_get_player(game);
    scene_t *scene = game_get_current_scene(game);
    for(size_t i = 0; i<list_size(interactors); i++) {
        tile_interactor_t *interactor = list_get(interactors, i);
        if (find_collision(interactor->area, body_get_hitbox(player_current)).collided) {
            if(entered_area == false) {
                SDL_Rect shape = (SDL_Rect) {0, 0, 600, 230};
                rect_t player_hitbox = body_get_draw_hitbox(player_current);
                rect_t hitbox = (rect_t) {MAX_WIDTH - 125, MAX_HEIGHT - 50, 120, 46};
                UI_t *press_F = UI_init(game_get_sprite(game, PRESS_F_ID), hitbox, "PRESS_F", 0.1);
                scene_add_UI_component(scene, press_F);
            }
            still_in_area = true;
        }
    }
    return still_in_area;
}
