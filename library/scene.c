#include <stdlib.h>
#include "scene.h"
#include <assert.h>
#include "body.h"

const size_t INIT_NUM_BODIES = 10;
const size_t INIT_NUM_FC = 5;
const size_t INIT_NUM_TILES = 100;
const size_t INIT_NUM_UIs = 100;

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

    return scene;
}


size_t scene_bodies(scene_t *scene) {
    return list_size(scene->bodies);
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


void scene_tick(scene_t *scene, double dt) {
    for(size_t i = 0; i < list_size(scene->force_creators); i++) {
        force_aux_t *force_func = list_get(scene->force_creators, i);
        bool removed = false;
        for (size_t j = 0; j < list_size(force_func->bodies); j++) {
            if (body_is_removed(list_get(force_func->bodies, j))) {
                force_func->removed = true;
            }
        }
    }
   for(size_t i = 0; i < list_size(scene->enemies); i++) {
        if (body_is_removed(list_get(scene->enemies, i))) {
            list_remove(scene->enemies, i);
        }
    }

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

    //printf("Initial vel x: %f\n", body_get_velocity(scene_get_body(scene,0)).x);

    for(size_t i = 0; i < list_size(scene->force_creators); i++) {
        force_aux_t *force_func = list_get(scene->force_creators, i);
        force_func->forcer(force_func->aux, dt);
    }
    //printf("after vel x: %f\n", body_get_velocity(scene_get_body(scene,0)).x);

    for(size_t i = 0; i < list_size(scene->bodies); i++) {
        body_tick(list_get(scene->bodies, i), dt);
    }
}
