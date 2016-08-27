//
// Created by Richard Chien on 8/27/16.
//

#ifndef CTREEMAP_H
#define CTREEMAP_H

#include <string.h>
#include <stdlib.h>

/*
 * An example:
 *
 * `GENERATE_MAP_TYPE(MyMap, char *)` will generate the following:
 *
 * - type `MyMap_Entry` representing struct `_MyMap_Entry`, containing members:
 *   - char *key
 *   - char *value (`char *` is the second parameter of the macro)
 *
 * - type `MyMap` representing struct `_MyMap`, containing member methods:
 *   - void put(MyMap *self, const char *key, char *value)
 *   - char *get(const MyMap *self, const char *key)
 *   - void remove(MyMap *self, const char *key)
 *   - void clear(MyMap *self)
 *   - void foreach(MyMap *self, void (*foreach_callback)(MyMap_Entry *entry))
 *
 * - MyMap *new_MyMap(void (*release_value_callback)(MyMap_Entry *)),
 *   the `release_value_callback` is a callback to release memory of value when needed,
 *   and can be NULL
 *
 * - void delete_MyMap(MyMap **self)
 */
#define GENERATE_MAP_TYPE(type_name, value_type) \
\
typedef struct _##type_name##_Entry { \
    char *key; \
    value_type value; \
} type_name##_Entry; \
\
typedef struct _##type_name type_name; \
\
struct _##type_name { \
    void *__root; \
    void (*__release_value_callback)(type_name##_Entry *); \
    void (*put)(type_name *self, const char *key, value_type value);\
    value_type (*get)(const type_name *self, const char *key); \
    void (*remove)(type_name *self, const char *key); \
    void (*clear)(type_name *self); \
    void (*foreach)(type_name *self, void (*foreach_callback)(const type_name##_Entry *entry)); \
}; \
\
static void map_##type_name##_put(type_name *self, const char *key, value_type value) { \
    type_name##_Entry *e = malloc(sizeof(type_name##_Entry)); \
    e->key = strdup(key); \
    e->value = value; \
    __map_insert_entry(self, e); \
} \
\
static value_type map_##type_name##_get(const type_name *self, const char *key) { \
    type_name##_Entry *e = __map_find_entry(self, key); \
    return e ? e->value : NULL; \
} \
\
static void map_##type_name##_remove(type_name *self, const char *key) { \
    __map_remove(self, key); \
} \
\
static void map_##type_name##_clear(type_name *self) { \
    __map_clear(self); \
} \
\
static void map_##type_name##_foreach(type_name *self, void (*foreach_callback)(const type_name##_Entry *entry)) { \
    __map_traverse(self, foreach_callback); \
} \
\
static type_name *new_##type_name(void (*release_value_callback)(type_name##_Entry *)) { \
    type_name *m = malloc(sizeof(type_name)); \
    m->__root = NULL; \
    m->__release_value_callback = release_value_callback; \
    m->put = map_##type_name##_put; \
    m->get = map_##type_name##_get; \
    m->remove = map_##type_name##_remove; \
    m->clear = map_##type_name##_clear; \
    m->foreach = map_##type_name##_foreach; \
    return m; \
} \
\
static void delete_##type_name(type_name **self) { \
    __map_destroy(self); \
}

void __map_insert_entry(void *self, void *entry);

void *__map_find_entry(void *self, const char *key);

void __map_remove(void *self, const char *key);

void __map_clear(void *self);

void __map_traverse(void *self, void (*_traverse_callback)(const void *entry));

void __map_destroy(void **self);

#endif //CTREEMAP_H
