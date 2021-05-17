#include "list.h"
#include "assert.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct list {
    void **obj_array;
    size_t size;
    size_t max_size;
    free_func_t freer;
} list_t;

list_t *list_init(size_t initial_size, free_func_t freer) {
    list_t *list = malloc(sizeof(list_t));
    list->obj_array = malloc(initial_size * sizeof(void *));
    list->size = 0;
    list->max_size = initial_size;
    list->freer = freer;

    assert(list != NULL);
    assert(list->obj_array != NULL);
    return list;
}

void list_free(list_t *list) {
    if (list->freer != NULL) {
        for (size_t i = 0; i < list->size; i++) {
            list->freer(list->obj_array[i]);
        }
    }
    free(list->obj_array);
    free(list);
}

size_t list_size(list_t *list) {
    return list->size;
}

size_t list_capacity(list_t *list) {
    return list->max_size;
}

void *list_get(list_t *list, size_t index) {
    assert(index < list->size);
    return list->obj_array[index];
}

void resize(list_t *list) {
    size_t cap = 2 * list->max_size + 1;
    void **new_array = malloc(cap * sizeof(void *));
    for (size_t i = 0; i < list->size; i++) {
        new_array[i] = list->obj_array[i];
    }
    free(list->obj_array);
    list->obj_array = new_array;
    list->max_size = cap;
}

void list_add(list_t *list, void *value) {
    assert(value != NULL);
    if (list->size >= list->max_size) {
        resize(list);
    }
    list->obj_array[list->size] = value;
    list->size++;
}

void *list_remove(list_t *list, size_t index) {
    assert(list->size > 0);
    void *obj = list->obj_array[index];
    for (size_t i = index+1; i < list->size; i++) {
        list->obj_array[i - 1] = list->obj_array[i];
    }
    list->size--;
    return obj;
}
