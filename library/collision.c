#include "collision.h"
#include "vector.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "sdl_wrapper.h"
#include "body.h"

double x_separation( rect_t hitbox1, rect_t hitbox2) {
    double min1 = hitbox1.x;
    double max1 = hitbox1.x + hitbox1.w;
    double min2 = hitbox2.x;
    double max2 = hitbox2.x + + hitbox2.w;

    if ((hitbox1.x < hitbox2.x + hitbox2.w &&
    hitbox1.x + hitbox1.w > hitbox2.x) ||
    (hitbox2.x < hitbox1.x + hitbox1.w &&
    hitbox2.x + hitbox2.w > hitbox1.x)) {
        return fmin(max1 - min2, max2 - min1);
    }

    return -1;
}

double y_separation(rect_t hitbox1, rect_t hitbox2) {
    double min1 = hitbox1.y;
    double max1 = hitbox1.y + hitbox1.h;
    double min2 = hitbox2.y;
    double max2 = hitbox2.y + hitbox2.h;

    if ((hitbox1.y < hitbox2.y + hitbox2.h &&
    hitbox1.y + hitbox2.h > hitbox2.y) ||
    (hitbox2.y < hitbox1.y + hitbox1.h &&
    hitbox2.y + hitbox1.h > hitbox1.y)) {
        return fmin(max1 - min2, max2 - min1);
    }

    return -1;
}
collision_info_t find_collision(rect_t hitbox1, rect_t hitbox2) {
    collision_info_t info = {
        .collided = false,
        .axis = VEC_ZERO
    };

    vector_t horizontal_axis = {
        .x = 1,
        .y = 0
    };

    vector_t vertical_axis = {
        .x = 0,
        .y = 1
    };

    double separating_axis1 = x_separation(hitbox1, hitbox2);
    double separating_axis2 = y_separation(hitbox1, hitbox2);
    if (separating_axis1 < 0 || separating_axis2 < 0) {
        return info;
    }
    else {
        info.collided = true;
        if (separating_axis1 < separating_axis2) {
            info.axis = horizontal_axis;
        }
        else {
            info.axis = vertical_axis;
        }
    }
    return info;
}

// For lists
void add_edges(list_t *edges, list_t *shape) {
    for (int i = 0; i < list_size(shape); i++) {
        vector_t *edge = malloc(sizeof(vector_t));
        *edge = vec_subtract(*(vector_t*) list_get(shape, (i+1) % list_size(shape)), *(vector_t*) list_get(shape, i));
        list_add(edges, edge);
    }
}



double is_separating_axis_list(vector_t axis, list_t *shape1, list_t* shape2) {
    double min1 = INFINITY;
    double max1 = -INFINITY;
    double min2 = INFINITY;
    double max2 = -INFINITY;

    for (int i = 0; i < list_size(shape1); i++) {
        double projected_point = vec_dot(*(vector_t*) list_get(shape1, i), axis);
        max1 = fmax(max1, projected_point);
        min1 = fmin(min1, projected_point);
    }

    for (int i = 0; i < list_size(shape2); i++) {
        double projected_point = vec_dot(*(vector_t*) list_get(shape2, i), axis);
        max2 = fmax(max2, projected_point);
        min2 = fmin(min2, projected_point);
    }

    if (max1 >= min2 && max2 >= min1) {
        return fmin(max1 - min2, max2 - min1);
    }

    return -1;
}


collision_info_t find_collision_list(list_t *shape1, list_t *shape2) {
    list_t *edges = list_init(list_size(shape1) + list_size(shape2), free);
    add_edges(edges, shape1);
    add_edges(edges, shape2);

    collision_info_t info = {
        .collided = false,
        .axis = VEC_ZERO
    };
    double current_min = INFINITY;
    // Separating Axis Check
    for (int i = 0; i < list_size(edges); i++) {
        vector_t edge = *(vector_t*) list_get(edges, i);

        // Generates orthogonal axis by rotating vector by 90 degrees
        vector_t ortho_axis = vec_rotate(edge, M_PI_2);
        double separating_axis = is_separating_axis_list(ortho_axis, shape1, shape2);
        if (separating_axis < 0) {
            return info;
        }
        else {
            if (separating_axis < current_min) {
                current_min = separating_axis;
                info.axis = ortho_axis; // come back here if broken
                double len = sqrt(pow(info.axis.x, 2) + pow(info.axis.y, 2));
                info.axis.y /= len;
                info.axis.x /= len;
            }
        }

    }

    info.collided = true;
    return info;
}
