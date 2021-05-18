#include <stdlib.h>
#include "scene.h"
#include <assert.h>
#include "body.h"

const size_t INIT_NUM_BODIES = 10;
const size_t INIT_NUM_FC = 5;

typedef struct force_aux {
    force_creator_t forcer;
    void *aux;
    free_func_t freer;
    list_t *bodies;
    bool removed;
} force_aux_t;

typedef struct scene {
    list_t *bodies;
    list_t *force_creators;
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
    free(scene);
}

scene_t *scene_init(void) {
    scene_t *scene = malloc(sizeof(scene_t));
    assert(scene != NULL);

    scene->bodies = list_init(INIT_NUM_BODIES, body_free);
    scene->force_creators = list_init(INIT_NUM_FC, force_aux_free); // edit freer

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
        force_func->forcer(force_func->aux);
    }

    for(size_t i = 0; i < list_size(scene->bodies); i++) {
        body_tick(list_get(scene->bodies, i), dt);
    }
}
