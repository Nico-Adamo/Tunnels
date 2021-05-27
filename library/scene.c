#include <stdlib.h>
#include "scene.h"
#include <assert.h>
#include "body.h"

const size_t INIT_NUM_BODIES = 10;
const size_t INIT_NUM_FC = 5;
const size_t INIT_NUM_TILES = 100;
const size_t INIT_NUM_UIs = 100;
const double INIT_LEVEL_EXP = 100;
const double LEVEL_EXP_FACTOR = 1.5;

const double TEXT_COOLDOWN = 5;

double level_up_buffs[5] = {/*HEALTH*/ 10, /*ATTACK*/ 10, /*COOLDOWN*/ .8, /*SPEED*/ 50, /*INVULNERABILITY*/ 1.5};

typedef struct force_aux {
    force_creator_t forcer;
    void *aux;
    free_func_t freer;
    list_t *bodies;
    bool removed;
} force_aux_t;

typedef struct scene {
    list_t *bodies;
    list_t *enemies;
    list_t *force_creators;
    list_t *floor_tiles;
    list_t *wall_tiles;
    list_t *collider_tiles;
    list_t *UI_components;
    list_t *UI_texts;
    bool is_menu;
    room_type_t room_type;
    double unlock_time;
    int last_second;
} scene_t;

void force_aux_free(force_aux_t *force_aux) {
    if (force_aux->freer != NULL) {
        force_aux->freer(force_aux->aux);
    }
    list_free(force_aux->bodies);
    free(force_aux);
}

void scene_free(scene_t *scene) {
    list_free(scene->bodies);
    list_free(scene->enemies);
    list_free(scene->floor_tiles);
    list_free(scene->wall_tiles);
    list_free(scene->collider_tiles);
    list_free(scene->force_creators);
    list_free(scene->UI_components);
    list_free(scene->UI_texts);
    free(scene);
}

scene_t *scene_init(void) {
    scene_t *scene = malloc(sizeof(scene_t));
    assert(scene != NULL);

    scene->bodies = list_init(INIT_NUM_BODIES, body_free);
    scene->enemies = list_init(INIT_NUM_BODIES, NULL);

    scene->force_creators = list_init(INIT_NUM_FC, force_aux_free);

    scene->floor_tiles = list_init(INIT_NUM_TILES, tile_free);
    scene->wall_tiles = list_init(INIT_NUM_TILES, tile_free);
    scene->collider_tiles = list_init(INIT_NUM_TILES, tile_free);
    scene->UI_components = list_init(INIT_NUM_UIs, UI_component_free);
    scene->UI_texts = list_init(INIT_NUM_UIs, ui_text_free);
    scene->is_menu = false;
    return scene;
}


size_t scene_bodies(scene_t *scene) {
    return list_size(scene->bodies);
}

void scene_set_is_menu(scene_t *scene, bool is_menu) {
    scene->is_menu = is_menu;
}

bool scene_is_menu(scene_t *scene) {
    return scene->is_menu;
}

body_t *scene_get_body(scene_t *scene, size_t index) {
    assert(index < list_size(scene->bodies) && index >= 0);
    return (body_t *) list_get(scene->bodies, index);
}

void scene_add_body(scene_t *scene, body_t *body) {
    list_add(scene->bodies, body);
    if(strcmp(body_get_type(body), "ENEMY") == 0) list_add(scene->enemies, body);
}

int depth_comparator(void *body1, void *body2) {
    body_t *body1_c = (body_t *) body1;
    body_t *body2_c = (body_t *) body2;
    if(body_get_centroid(body1_c).y < body_get_centroid(body2_c).y) return 1;
    if(body_get_centroid(body1_c).y > body_get_centroid(body2_c).y) return -1;
    return 0;
}

void scene_sort_by_depth(scene_t *scene) {
    list_sort(scene->bodies, depth_comparator);
}

void scene_remove_body(scene_t *scene, size_t index) {
    assert(index < list_size(scene->bodies) && index >= 0);
    list_remove(scene->bodies, index);
}

void scene_add_force_creator(scene_t *scene, force_creator_t forcer, void *aux, free_func_t freer) {
    scene_add_bodies_force_creator(scene, forcer, aux, NULL, freer);
}

void scene_add_bodies_force_creator(scene_t *scene, force_creator_t forcer, void *aux, list_t *bodies, free_func_t freer) {
    force_aux_t *force_func = malloc(sizeof(force_aux_t));
    force_func->forcer = forcer;
    force_func->aux = aux;
    force_func->freer = freer;
    force_func->bodies = bodies;
    force_func->removed = false;
    list_add(scene->force_creators, force_func);
}

void scene_add_floor_tile(scene_t *scene, tile_t *tile) {
    list_add(scene->floor_tiles, tile);
}

void scene_add_wall_tile(scene_t *scene, tile_t *tile) {
    list_add(scene->wall_tiles, tile);
}

void scene_add_collider_tile(scene_t *scene, tile_t *tile) {
    list_add(scene->collider_tiles, tile);
}

void scene_add_UI_component(scene_t *scene, UI_t *UI) {
    list_add(scene->UI_components, UI);
}

void scene_add_UI_text(scene_t *scene, ui_text_t *text) {
    list_add(scene->UI_texts, text);
}

void scene_set_room_type(scene_t *scene, room_type_t room_type) {
    scene->room_type = room_type;
    if (room_type == KILL) {
        scene_add_UI_text(scene, ui_text_init(" Objective: Kill all enemies", (vector_t) {0, 0}, TEXT_COOLDOWN));
    } else if (room_type == NAVIGATE) {
        scene_add_UI_text(scene, ui_text_init(" Objective: Get to the door", (vector_t) {0, 0}, TEXT_COOLDOWN));
    }
}

void scene_set_unlock_time(scene_t *scene, double unlock_time) {
    scene->unlock_time = unlock_time;
    scene->last_second = (int) unlock_time + 1;
    if (unlock_time > 0) {
        scene_add_UI_text(scene, ui_text_init(" Unlock time:", (vector_t) {0, 0}, unlock_time));
    }
}


bool scene_check_objective(scene_t *scene) {
    if (scene->room_type == NAVIGATE) {
        return true;
    } if (scene->room_type == KILL) {
        return (list_size(scene->enemies) == 0);
    } if (scene->room_type == SURVIVE) {
        return scene->unlock_time <= 0;
    }
    printf("Room with no type"); //TODO changeto assert/throw somewhere
    return false;
}


list_t *scene_get_floor_tiles(scene_t *scene) {
    return scene->floor_tiles;
}

list_t *scene_get_wall_tiles(scene_t *scene) {
    return scene->wall_tiles;
}

list_t *scene_get_collider_tiles(scene_t *scene) {
    return scene->collider_tiles;
}

list_t *scene_get_enemies(scene_t *scene) {
    return scene->enemies;
}

list_t *scene_get_UI_components(scene_t *scene) {
    return scene->UI_components;
}

list_t *scene_get_UI_texts(scene_t *scene) {
    return scene->UI_texts;
}

room_type_t scene_get_room_type(scene_t *scene) {
    return scene->room_type;
}

double scene_get_unlock_time(scene_t *scene) {
    return scene->unlock_time;
}

void scene_tick(scene_t *scene, double dt) {
    // TODO reorganize pls

    if (scene->unlock_time > 0) {
        scene->unlock_time -= dt;
    }
    if (scene->last_second - scene->unlock_time >= 1 && scene->unlock_time > 0) {
        scene->last_second -= 1;

        int length = snprintf( NULL, 0, "%d", scene->last_second); //TODO: understand
        char* timer_string = malloc( length + 1 );
        snprintf(timer_string, length + 1, "%d", scene->last_second);
        printf(timer_string);

        scene_add_UI_text(scene, ui_text_init(timer_string, (vector_t) {256, 0}, 1));
    }

    for(size_t i = 0; i < list_size(scene->force_creators); i++) {
        force_aux_t *force_func = list_get(scene->force_creators, i);
        bool removed = false;
        for (size_t j = 0; j < list_size(force_func->bodies); j++) {
            if (body_is_removed(list_get(force_func->bodies, j))) {
                force_func->removed = true;
            }
        }
    }

    body_t *player;
    for(size_t i = 0; i < list_size(scene->bodies); i++) {
        if (strcmp(body_get_type(list_get(scene->bodies, i)), "PLAYER") == 0) {
            player = list_get(scene->bodies, i);
            break;
        }
    }

    stats_info_t player_stats = body_get_stats_info(player);
    for(size_t i = 0; i < list_size(scene->enemies); i++) {
        if (body_is_removed(list_get(scene->enemies, i))) {
            double exp = body_get_stats_info(list_get(scene->enemies, i)).experience;
            player_stats.experience += exp;

            //printf("Experience: %f\n", player_stats.experience);
            list_remove(scene->enemies, i);

            if (scene->room_type == KILL) {
                scene_add_UI_text(scene, ui_text_init(" Enemies remaining:", (vector_t) {0, 0}, 1.25));
                int length = snprintf( NULL, 0, "%d", list_size(scene->enemies)); //TODO: understand
                char* enem_string = malloc( length + 1 );
                snprintf(enem_string, length + 1, "%d", list_size(scene->enemies));
                printf(enem_string);

                scene_add_UI_text(scene, ui_text_init(enem_string, (vector_t) {400, 0}, 1.25));
            }
        }
    }

    //int level_up_exp = 10; // this one is for quick testing
    int level_up_exp = round(INIT_LEVEL_EXP * pow(LEVEL_EXP_FACTOR, player_stats.level - 1));
    if (player_stats.experience >= level_up_exp) {
        printf("LEVEL UP\n");
        player_stats.experience -= level_up_exp;
        player_stats.level++;
        int buff = floor(rand_from(0, 4.9)); // TODO: Magic Numbers?
        //printf("Before Stats: Health- %f, Attack- %f, Cooldown- %f, Speed- %f\n", player_stats.health, player_stats.attack, player_stats.cooldown, player_stats.speed);
        switch (buff) {
            case 0:
                player_stats.health += level_up_buffs[0];
                break;
            case 1:
                player_stats.attack += level_up_buffs[1];
                break;
            case 2:
                player_stats.cooldown *= level_up_buffs[2];
                break;
            case 3:
                player_stats.speed += level_up_buffs[3];
                break;
            case 4:
                player_stats.invulnerability_timer *= level_up_buffs[4];
                break;
        }
        //printf("Before Stats: Health- %f, Attack- %f, Cooldown- %f, Speed- %f\n", player_stats.health, player_stats.attack, player_stats.cooldown, player_stats.speed);
    }

    body_set_stats_info(player, player_stats);

    for(size_t i = 0; i < list_size(scene->bodies); i++) {
        if (body_is_removed(list_get(scene->bodies, i))) {
            body_free(list_remove(scene->bodies, i));
        }
    }

    int idx = 0;
    while (idx < list_size(scene->force_creators)) {
        force_aux_t *force_func = list_get(scene->force_creators, idx);
        if (force_func->removed) {
            force_aux_t *remove = list_remove(scene->force_creators, idx);
            force_aux_free(remove);
            idx--;
        }
        idx++;
    }

    for(size_t i = 0; i < list_size(scene->force_creators); i++) {
        force_aux_t *force_func = list_get(scene->force_creators, i);
        force_func->forcer(force_func->aux, dt);
    }

    for(size_t i = 0; i < list_size(scene->bodies); i++) {
        body_tick(list_get(scene->bodies, i), dt);
    }

    for(size_t i = 0; i< list_size(scene->UI_components); i++) {
        UI_tick(list_get(scene->UI_components, i), dt);
    }

    for(size_t i = 0; i< list_size(scene->UI_texts); i++) {
        ui_text_t *text = list_get(scene->UI_texts, i);
        ui_text_tick(text, dt);
        if(ui_text_get_timer(text) <= 0) {
            ui_text_set_removed(text, true);
        }
    }

    idx = 0;
    while (idx < list_size(scene->UI_texts)) {
        ui_text_t *text = list_get(scene->UI_texts, idx);
        if (ui_text_is_removed(text)) {
            ui_text_t *remove = list_remove(scene->UI_texts, idx);
            ui_text_free(remove);
            idx--;
        }
        idx++;
    }
}
