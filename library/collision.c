#include "collision.h"
#include "vector.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

void add_edges(list_t *edges, list_t *shape) {
    for (int i = 0; i < list_size(shape); i++) {
        vector_t *edge = malloc(sizeof(vector_t));
        *edge = vec_subtract(*(vector_t*) list_get(shape, (i+1) % list_size(shape)), *(vector_t*) list_get(shape, i));
        list_add(edges, edge);
    }
}

double is_separating_axis(vector_t axis, list_t *shape1, list_t* shape2) {
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


collision_info_t find_collision(list_t *shape1, list_t *shape2) {
    list_t *edges = list_init(list_size(shape1) + list_size(shape2), free);
    add_edges(edges, shape1);
    add_edges(edges, shape2);

    collision_info_t info = {
        .collided = false,
        .axis = VEC_ZERO
    };
    double current_min = INFINITY;

    // Bounding Box Check
    double x_min = INFINITY;
    double x_max = -INFINITY;
    double y_min = INFINITY;
    double y_max = -INFINITY;
    for (size_t i = 0; i < list_size(shape1); i++) {
        vector_t *curr = list_get(shape1, i);
        if (curr->x < x_min) x_min = curr->x;
        if (curr->x > x_max) x_max = curr->x;
        if (curr->y < y_min) y_min = curr->y;
        if (curr->y > y_max) y_max = curr->y;
    }
    bool in_box = false;
    for (size_t i = 0; i < list_size(shape2); i++) {
        vector_t *curr = list_get(shape2, i);
        if ((curr->x >= x_min && curr->x <= x_max) && (curr->y >= y_min && curr->y <= y_max)) {
            in_box = true;
        }
    }
    if (!in_box) return info;

    // Separating Axis Check
    for (int i = 0; i < list_size(edges); i++) {
        vector_t edge = *(vector_t*) list_get(edges, i);

        // Generates orthogonal axis by rotating vector by 90 degrees
        vector_t ortho_axis = vec_rotate(edge, M_PI_2);
        double separating_axis = is_separating_axis(ortho_axis, shape1, shape2);
        if (separating_axis < 0) {
            return info;
        } else {
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
