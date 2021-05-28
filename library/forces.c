#include "scene.h"
#include "math.h"
#include "forces.h"
#include <stdlib.h>
#include "collision.h"
#include <stdio.h>

// can add these as function parameters and set them in the demo
const double MIN_DIST = 5;
const double RECOIL_DIST = 0.25;
const double RECOIL_ENEMY = 15;

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

void newtonian_gravity(void *aux) {
    aux_t *values = (aux_t *) aux;
    body_t *body1 = values->body1;
    body_t *body2 = values->body2;
    double G = values->param;
    vector_t center1 = body_get_centroid(body1);
    vector_t center2 = body_get_centroid(body2);
    double mass1 = body_get_mass(body1);
    double mass2 = body_get_mass(body2);
    vector_t sep = {.x = center2.x - center1.x, .y = center2.y - center1.y};
    double dist = sqrt(pow(sep.x, 2) + pow(sep.y, 2));
    if (dist > MIN_DIST) {
        double mag = -G * mass1 * mass2 / pow(dist, 2);
        vector_t F2 = {.x = mag * sep.x / dist, .y = mag * sep.y / dist};
        vector_t F1 = {.x = -mag * sep.x / dist, .y = -mag * sep.y / dist};
        body_add_force(body2, F2);
        body_add_force(body1, F1);
    }
}

void create_newtonian_gravity(scene_t *scene, double G, body_t *body1, body_t *body2){
    aux_t *aux = malloc(sizeof(aux_t));
    aux->param = G;
    aux->body1 = body1;
    aux->body2 = body2;
    list_t *bodies = list_init(2, NULL);
    list_add(bodies, body1);
    list_add(bodies, body2);
    scene_add_bodies_force_creator(scene, newtonian_gravity, aux, bodies, free);
}

void spring(void *aux) {
    aux_t *values = (aux_t *) aux;
    body_t *body1 = values->body1;
    body_t *body2 = values->body2;
    double k = values->param;
    vector_t center1 = body_get_centroid(body1);
    vector_t center2 = body_get_centroid(body2);
    vector_t sep = {.x = center2.x - center1.x, .y = center2.y - center1.y};
    double dist = sqrt(pow(sep.x, 2) + pow(sep.y, 2));
    double mag = k * dist;
    /*
    mag is (-) if dist is larger than eq, so forces will PULL bodies together
    mag is (+) if dist is smaller than eq, so forces will PUSH bodies apart
    */
    vector_t F1 = {.x = mag * sep.x / dist, .y = mag * sep.y / dist};
    vector_t F2 = {.x = -mag * sep.x / dist, .y = -mag * sep.y / dist};
    body_add_force(body2, F2);
    body_add_force(body1, F1);
}

void create_spring(scene_t *scene, double k, body_t *body1, body_t *body2) {
    aux_t *aux = malloc(sizeof(aux_t));
    aux->param = k;
    aux->body1 = body1;
    aux->body2 = body2;
    list_t *bodies = list_init(2, NULL);
    list_add(bodies, body1);
    list_add(bodies, body2);
    scene_add_bodies_force_creator(scene, spring, aux, bodies, free);
}


void drag(void *aux) {
    aux_t *values = (aux_t *) aux;
    body_t *body = values->body1;
    double gamma = values->param;
    vector_t velocity = body_get_velocity(body);
    vector_t drag = {.x = -velocity.x * gamma, .y = -velocity.y * gamma};
    body_add_force(body, drag);
}

void create_drag(scene_t *scene, double gamma, body_t *body) {
    aux_t *aux = malloc(sizeof(aux_t));
    aux->param = gamma;
    aux->body1 = body;
    aux->body2 = NULL;
    list_t *bodies = list_init(1, NULL);
    list_add(bodies, body);
    scene_add_bodies_force_creator(scene, drag, aux, bodies, free);
}

void collision(void *aux) {
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
    create_collision(scene, body1, body2, physics_collision, aux, free);
}

// TO DO: update to cause damage and destroy the second body if health drops below zero
void semi_destructive_collision(body_t *body1, body_t *body2, vector_t axis, void *aux) {
    stats_info_t body1_info = body_get_stats_info(body1);
    stats_info_t body2_info = body_get_stats_info(body2);
    /*
        create_x_collision(player, bullet, etc.)
        create_x_collison(player, enemy)
    */
    // types: PLAYER ENEMY ENEMY_BULLET PLAYER_BULLET(?)

    if (strcmp(body_get_type(body2), "PLAYER") == 0 && strcmp(body_get_type(body1), "ENEMY") == 0) {
        if(body_get_invulnerability_timer(body2) <= 0) {
            body2_info.health -= body1_info.attack;
            body_set_stats_info(body2, body2_info);
            body_set_invulnerability_timer(body2, body_get_stats_info(body2).invulnerability_timer); // TODO: Invulnerability timer magic number
            printf("Health: %f\n", body2_info.health);
            if (body2_info.health <= 0) body_remove(body2);
        }
    } else if ((strcmp(body_get_type(body1), "PLAYER") == 0 && (strcmp(body_get_type(body2), "ENEMY_BULLET") == 0) ||
    (strcmp(body_get_type(body1), "ENEMY") == 0 && (strcmp(body_get_type(body2), "PLAYER_BULLET") == 0)))) {
        if(body_get_invulnerability_timer(body1) <= 0 || strcmp(body_get_type(body1), "PLAYER") != 0) {
            body1_info.health -= body2_info.attack;
            body_set_stats_info(body1, body1_info);
            body_set_invulnerability_timer(body1, body_get_stats_info(body1).invulnerability_timer); // TODO: Invulnerability timer magic number
            if (body1_info.health <= 0) body_remove(body1);
            body_remove(body2);
        }
        if(strcmp(body_get_type(body1), "ENEMY") == 0) {
            body_set_hit_timer(body1, 0.3);
        }
    }

}

void create_semi_destructive_collision(scene_t *scene, body_t *body1, body_t *body2) {
    aux_t *aux = malloc(sizeof(aux_t));
    aux->param = 1;
    aux->body1 = body1;
    aux->body2 = body2;
    create_collision(scene, body1, body2, semi_destructive_collision, aux, free);
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
        if (strcmp(body_get_type(body), "PLAYER_BULLET") == 0 || strcmp(body_get_type(body), "ENEMY_BULLET") == 0) {
            body_remove(body);
        } else if (strcmp(body_get_type(body), "PLAYER") == 0 || strcmp(body_get_type(body), "ENEMY") == 0) {
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
        // still need to deal with enemy collisions
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
        create_physics_collision(scene, 0.5, body, enemy);
        create_semi_destructive_collision(scene, enemy, player);
    }
}

