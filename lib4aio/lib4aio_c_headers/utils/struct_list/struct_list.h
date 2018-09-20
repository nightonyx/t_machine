#ifndef LIB4AIO_C_STRUCT_LIST_H
#define LIB4AIO_C_STRUCT_LIST_H

/**
 * Convenient abstract of struct list.
 */

typedef struct struct_list {
    unsigned type;
    unsigned capacity;
    unsigned size;
    void **elements;
} struct_list;

/**
 * Creates a new list.
 * @param type - type of element.
 * @return new struct_list struct.
 */

struct struct_list *new_struct_list(const unsigned type);

/**
 * Adds an element in list.
 * @param list - current list;
 * @param element - added element.
 */

void add_struct_in_list(struct_list *list, void *element);

/**
 * Frees elements in list.
 * @param list - current list.
 * @param free_element - function of each element deleting.
 */

void free_structs_in_list(struct_list *list, void (*free_element)(void *));

/**
 * Deletes a struct list.
 * @param list - deleted list.
 *
 * Note: if your list contains references that only referred by list
 * then previously use free_structs_in_list and after that use free_struct_list!
 */

void free_struct_list(struct_list *list);

/**
 * Creates a new filtered list.
 * @param list - source list;
 * @param filter_condition - filter function
 * @param copy_constructor copy constructor function.
 * @return new struct_list struct of element type.
 */

struct struct_list *filter_struct_list(
        const struct struct_list *list,
        _Bool (*filter_condition)(const void *),
        void *(*copy_constructor)(const void *)
);

#endif //LIB4AIO_C_STRUCT_LIST_H