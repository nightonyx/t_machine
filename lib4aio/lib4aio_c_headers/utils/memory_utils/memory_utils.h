#ifndef LIB4AIO_C_MEMORY_UTILS_H
#define LIB4AIO_C_MEMORY_UTILS_H

/**
 * Safely allocates new memory.
 * @param size_of_object - size of element.
 * @return memory;
 */

void *new_object(const unsigned size_of_object);

/**
 * Safely allocates new array abstraction memory.
 * @param number_of_elements - element count;
 * @param size_of_object - size of each element.
 * @return memory.
 */

void *new_object_array(const unsigned number_of_elements, const unsigned size_of_object);

/**
 * Safely reallocates array abstraction memory.
 * @param object_array - current pointer of memory;
 * @param new_number_of_elements - new element count;
 * @param size_of_object - size of each element.
 * @return memory.
 */

void *reallocate_object_array(
        void *object_array,
        const unsigned new_number_of_elements,
        const unsigned size_of_object
);

#endif //LIB4AIO_C_MEMORY_UTILS_H