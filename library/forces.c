#include "scene.h"
#include "math.h"
#include "forces.h"
#include <stdlib.h>
#include "collision.h"
#include <stdio.h>

// can add these as function parameters and set them in the demo
const double MIN_DIST = 5;

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
    collision_info_t col_info = find_collision(body_get_shape(body1), body_get_shape(body2));
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

void semi_destructive_collision(body_t *body1, body_t *body2, vector_t axis, void *aux) {
    body_remove(body1);
}


void create_destructive_collision(scene_t *scene, body_t *body1, body_t *body2) {
    aux_t *aux = malloc(sizeof(aux_t));
    aux->param = 0;
    aux->body1 = body1;
    aux->body2 = body2;
    create_collision(scene, body1, body2, destructive_collision, aux, free);
}

void create_semi_destructive_collision(scene_t *scene, body_t *body1, body_t *body2) {
    aux_t *aux = malloc(sizeof(aux_t));
    aux->param = 0;
    aux->body1 = body1;
    aux->body2 = body2;
    create_collision(scene, body1, body2, semi_destructive_collision, aux, free);
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