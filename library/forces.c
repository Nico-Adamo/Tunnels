#include "scene.h"
#include "math.h"
#include "forces.h"
#include <stdlib.h>
#include "collision.h"
#include "sound.h"
#include <stdio.h>

const double RECOIL_DIST = 0.25;

const char* SOUNDS[] = {
    "assets/sounds/player_hit_1.wav",
    "assets/sounds/player_hit_2.wav",
    "assets/sounds/player_hit_3.wav",
    "assets/sounds/mob_hit_1.wav",
    "assets/sounds/mob_hit_2.wav",
    "assets/sounds/mob_hit_3.wav"
};

typedef struct aux {
    double param;
    body_t *body1;
    body_t *body2;
} aux_t;

typedef struct collision_aux {
    void *aux;
    body_t *body1;
    body_t *body2;
    collision_handler_t handler;
    bool collided;
} collision_aux_t;

typedef struct tile_aux {
    tile_t *tile;
    body_t *body;
} tile_aux_t;

int sign(double x) {
  if (x > 0.0) return 0.01;
  if (x < 0.0) return -0.01;
  return 0;
}

void collision_aux_free(void *collision_aux) {
    collision_aux_t *coll = (collision_aux_t *) collision_aux;
    free(coll->aux);
    free(coll);
}

void collision(void *aux, double dt) {
    collision_aux_t *values = (collision_aux_t *) aux;
    body_t *body1 = values->body1;
    body_t *body2 = values->body2;
    collision_info_t col_info = find_collision(body_get_hitbox(body1), body_get_hitbox(body2));
    if (values->collided && !col_info.collided) {
        values->collided = false;
    }
    else if (col_info.collided && !values->collided) {
        values->handler(body1, body2, col_info.axis, values->aux);
        values->collided = true;
    }
}

void create_collision(scene_t *scene, body_t *body1, body_t *body2, collision_handler_t handler, void *aux, free_func_t freer) {
    list_t *bodies = list_init(2, NULL);
    list_add(bodies, body1);
    list_add(bodies, body2);
    collision_aux_t *a = malloc(sizeof(collision_aux_t));
    a->aux = aux;
    a->body1 = body1;
    a->body2 = body2;
    a->collided = false;
    a->handler = handler;
    scene_add_bodies_force_creator(scene, collision, a, bodies, freer);
}


void destructive_collision(body_t *body1, body_t *body2, vector_t axis, void *aux) {
    body_remove(body1);
    body_remove(body2);
}


void create_destructive_collision(scene_t *scene, body_t *body1, body_t *body2) {
    aux_t *aux = malloc(sizeof(aux_t));
    aux->param = 0;
    aux->body1 = body1;
    aux->body2 = body2;
    create_collision(scene, body1, body2, destructive_collision, aux, free);
}

void physics_collision(body_t *body1, body_t *body2, vector_t axis, void *aux) {
    aux_t *values = (aux_t *) aux;
    double C_r = values->param;
    double u_a = vec_dot(body_get_velocity(body1), axis);
    double u_b = vec_dot(body_get_velocity(body2), axis);
    double m_a = body_get_mass(body1);
    double m_b = body_get_mass(body2);
    double imp_1;
    if (m_a != INFINITY && m_b != INFINITY) {
        imp_1 = ((m_a * m_b) / (m_a + m_b)) * (1 + C_r) * (u_b - u_a);
    }
    else if (m_a == INFINITY) {
        imp_1 = m_b * (1 + C_r) * (u_b - u_a);
    }
    else if (m_b == INFINITY) {
        imp_1 = m_a * (1 + C_r) * (u_b - u_a);
    }

    vector_t J_1 = {.x = imp_1 * axis.x, .y = imp_1 * axis.y};
    vector_t J_2 = vec_negate(J_1);
    body_add_impulse(body1, J_1);
    body_add_impulse(body2, J_2);
}

void create_physics_collision(scene_t *scene, double elasticity, body_t *body1, body_t *body2) {
    aux_t *aux = malloc(sizeof(aux_t));
    aux->param = elasticity;
    aux->body1 = body1;
    aux->body2 = body2;
    create_collision(scene, body1, body2, physics_collision, aux, collision_aux_free);
}

void semi_destructive_collision(body_t *body1, body_t *body2, vector_t axis, void *aux) {
    stats_info_t body1_info = body_get_stats_info(body1);
    stats_info_t body2_info = body_get_stats_info(body2);

    if (body_get_type(body2) == PLAYER && body_get_type(body1) >= ENEMY) {
        if(body_get_invulnerability_timer(body2) <= 0) {
            body2_info.health -= body1_info.attack;
            body_set_stats_info(body2, body2_info);
            body_set_invulnerability_timer(body2, body_get_stats_info(body2).invulnerability_timer);
            if (body2_info.health <= 0) body_remove(body2);
        }
    } else if ((body_get_type(body1) == PLAYER && body_get_type(body2) == ENEMY_BULLET) ||
    (body_get_type(body1) >= ENEMY && body_get_type(body2) == PLAYER_BULLET)) {
        if(body_get_invulnerability_timer(body1) <= 0) {
            body1_info.health -= body2_info.attack;
            body_set_stats_info(body1, body1_info);
            body_set_invulnerability_timer(body1, body_get_stats_info(body1).invulnerability_timer);
            if (body1_info.health <= 0) body_remove(body1);
            body_remove(body2);
            if(body_get_type(body1) == PLAYER) {
                int sound = rand() % 3;
                if(sound == 0) sound_play(SOUNDS[0]);
                if(sound == 1) sound_play(SOUNDS[1]);
                if(sound == 2) sound_play(SOUNDS[2]);
            }
        }
        if(body_get_type(body1) >= ENEMY) {
            int sound = rand() % 3;
            if(sound == 0) sound_play(SOUNDS[3]);
            if(sound == 1) sound_play(SOUNDS[4]);
            if(sound == 2) sound_play(SOUNDS[5]);

            body_set_hit_timer(body1, 0.3);
            if (body_get_type(body1) == ENEMY) {
                body_add_impulse(body1, vec_multiply(body_get_mass(body1) / 2, body_get_velocity(body2)));
            }
        }
    }

}


void create_semi_destructive_collision(scene_t *scene, body_t *body1, body_t *body2) {
    aux_t *aux = malloc(sizeof(aux_t));
    aux->param = 1;
    aux->body1 = body1;
    aux->body2 = body2;
    create_collision(scene, body1, body2, semi_destructive_collision, aux, collision_aux_free);
}

void tile_collision(void *aux, double dt) {
    tile_aux_t *tile_aux = (tile_aux_t *) aux;
    body_t *body = tile_aux->body;
    tile_t *tile = tile_aux->tile;
    rect_t hitbox = body_get_collision_hitbox(body);
    vector_t final_velocity = body_calculate_velocity(body, dt);
    vector_t average_velocity = {
        .x = (final_velocity.x + body_get_velocity(body).x) / 2,
        .y = (final_velocity.y + body_get_velocity(body).y) / 2
    };
    vector_t displacement = vec_multiply(dt, average_velocity);
    collision_info_t col_info = find_collision((rect_t) {hitbox.x + displacement.x, hitbox.y + displacement.y, hitbox.w, hitbox.h}, tile_get_hitbox(tile));
    if (col_info.collided) {
        if (body_get_type(body) == PLAYER_BULLET || body_get_type(body) == ENEMY_BULLET) {
            body_remove(body);
        } else if (body_get_type(body) == PLAYER || body_get_type(body) >= ENEMY) {
            vector_t recoil = {
                .x = col_info.axis.x * RECOIL_DIST,
                .y = col_info.axis.y * RECOIL_DIST
            };
            vector_t new_velocity = body_get_velocity(body);
            if (col_info.axis.x == 1) {
                new_velocity.x = 0;
                if (average_velocity.x == 0) {
                    if (body_get_hitbox(body).x > tile_get_hitbox(tile).x) {
                        body_set_centroid(body, vec_add(body_get_centroid(body), recoil));
                    } else {
                        body_set_centroid(body, vec_add(body_get_centroid(body), vec_negate(recoil)));
                    }
                }
            } else if (col_info.axis.y == 1) {
                new_velocity.y = 0;
                if (average_velocity.y == 0) {
                    if (body_get_hitbox(body).y > tile_get_hitbox(tile).y) {
                        body_set_centroid(body, vec_add(body_get_centroid(body), recoil));
                    } else {
                        body_set_centroid(body, vec_add(body_get_centroid(body), vec_negate(recoil)));
                    }
                }
            }
            body_set_velocity(body, new_velocity);
        }
    }

}

void create_tile_collision(scene_t *scene, body_t *body) {
    list_t *tiles = scene_get_collider_tiles(scene);
    for (size_t i = 0; i < list_size(tiles); i++) {
        tile_aux_t *aux = malloc(sizeof(tile_aux_t));
        list_t *body_list = list_init(1, NULL);
        list_add(body_list, body);
        aux->tile = list_get(tiles, i);
        aux->body = body;
        scene_add_bodies_force_creator(scene, tile_collision, aux, body_list, free);
    }
}

void create_enemy_collision(scene_t *scene, body_t *body, body_t *player) {
    list_t *enemies = scene_get_enemies(scene);
    for (size_t i = 0; i < list_size(enemies); i++) {
        body_t *enemy = list_get(enemies, i);
        if (body_get_type(enemy) == ENEMY) {
           create_physics_collision(scene, 0.5, body, enemy);
        }
        create_semi_destructive_collision(scene, enemy, player);

    }
}

