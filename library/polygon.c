#include "list.h"
#include "vector.h"
#include <math.h>
#include <stdio.h>

double polygon_area(list_t *polygon) {
    double sum = 0;
    for(size_t i = 0; i < list_size(polygon); i++) {
        vector_t v = *(vector_t *) list_get(polygon, i);
        vector_t v_p = *(vector_t *) list_get(polygon, (i+1) % list_size(polygon));
        sum += vec_cross(v, v_p);
    }

    return 0.5 * sum;
}

double polygon_centroid_x(list_t *polygon, double area) {
    double cent_sum_x = 0;
    for(size_t i = 0; i < list_size(polygon); i++) {
        vector_t v = *(vector_t *) list_get(polygon, i);
        vector_t v_p = *(vector_t *) list_get(polygon, (i+1) % list_size(polygon));
        cent_sum_x += (v.x + v_p.x) * vec_cross(v, v_p);
    }

    return cent_sum_x / (6.0 * area);
}

double polygon_centroid_y(list_t *polygon, double area) {
    double cent_sum_y = 0;
    for(size_t i = 0; i < list_size(polygon); i++) {
        vector_t v = *(vector_t *) list_get(polygon, i);
        vector_t v_p = *(vector_t *) list_get(polygon, (i+1) % list_size(polygon));
        cent_sum_y += (v.y + v_p.y) * vec_cross(v, v_p);
    }

    return cent_sum_y / (6.0 * area);
}

vector_t polygon_centroid(list_t *polygon) {
    double area = polygon_area(polygon);
    vector_t centroid = {
        .x = polygon_centroid_x(polygon, area),
        .y = polygon_centroid_y(polygon, area)
    };

    return centroid;
}

void polygon_translate(list_t *polygon, vector_t translation) {
    for(size_t i = 0; i < list_size(polygon); i++) {
        vector_t *curr_vector = list_get(polygon, i);
        *curr_vector = vec_add(*curr_vector, translation);
    }
}

void polygon_rotate(list_t *polygon, double angle, vector_t point) {
    for(size_t i = 0; i < list_size(polygon); i++) {
        vector_t *curr_vector = list_get(polygon, i);
        *curr_vector = vec_subtract(*curr_vector, point);   // Translate to center around origin
        *curr_vector = vec_rotate(*curr_vector, angle);     // Rotate
        *curr_vector = vec_add(*curr_vector, point);        // Translate back
    }
}




