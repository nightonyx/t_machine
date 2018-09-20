#include <stdlib.h>
#include <stdbool.h>
#include <lib4aio_c_headers/utils/struct_list/struct_list.h>
#include <lib4aio_c_headers/utils/memory_utils/memory_utils.h>

#define INIT_CAPACITY 2

/**
 * Creates a new list.
 * @param type - type of element.
 * @return new struct_list struct.
 */

struct_list *new_struct_list(const unsigned type)
{
    struct_list *list = new_object(sizeof(struct_list));
    list->type = type;
    list->capacity = INIT_CAPACITY;
    list->size = 0;
    list->elements = new_object_array(INIT_CAPACITY, sizeof(type));
    return list;
}

/**
 * Doubles size of list.
 * @param list - current list.
 */

static void grow(struct_list *list)
{
    if (list->size == list->capacity) {
        list->capacity = list->capacity * 2;
        list->elements = reallocate_object_array(
                list->elements,
                list->capacity,
                list->type
        );
    }
}

/**
 * Adds an element in list.
 * @param list - current list;
 * @param element - added element.
 */

void add_struct_in_list(struct_list *list, void *element)
{
    grow(list);
    list->elements[list->size++] = element;
}

/**
 * Frees elements in list.
 * @param list - current list.
 * @param free_element - function of each element deleting.
 */

void free_structs_in_list(struct_list *list, void (*free_element)(void *))
{
    void **elements = list->elements;
    for (unsigned i = 0; i < list->size; ++i) {
        void *old_element = elements[i];
        list->elements[i] = NULL;
        free_element(old_element);
    }
}

/**
 * Deletes a struct list.
 * @param list - deleted list.
 *
 * Note: if your list contains references that only referred by list
 * then previously use free_structs_in_list and after that use free_struct_list!
 */

void free_struct_list(struct_list *list)
{
    const unsigned size = list->size;
    for (unsigned i = 0; i < size; ++i) {
        list->elements[i] = NULL;
    }
    free(list->elements);
    free(list);
}

/**
 * Creates a new filtered list.
 * @param list - source list;
 * @param filter_condition - filter function
 * @param copy_constructor copy constructor function.
 * @return new struct_list struct of element type.
 */

struct_list *filter_struct_list(
        const struct_list *list,
        _Bool (*filter_condition)(const void *),
        void *(*copy_constructor)(const void *)
)
{
    const unsigned list_size = list->size;
    void **elements = list->elements;
    struct_list *new_list = new_struct_list(list->type);
    for (unsigned k = 0; k < list_size; ++k) {
        const void *element = elements[k];
        if (filter_condition(element)) {
            void *new_hook = copy_constructor(element);
            add_struct_in_list(new_list, new_hook);
        }
    }
    return new_list;
}