#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <lib4aio_c_headers/utils/struct_list/struct_list.h>
#include <lib4aio_c_headers/utils/memory_utils/memory_utils.h>

/**
 * Creates a new string by other string.
 * @param src - source string;
 * @return new string.
 */

char *new_string(const char *src)
{
    if (src) {
        char *new_string = new_object_array((const unsigned int) (strlen(src) + 1), sizeof(char));
        strcpy(new_string, src);
        return new_string;
    } else {
        return NULL;
    }
}

/**
 * Matches content of two strings.
 * @param first - first string;
 * @param second - second string;
 * @return true / false.
 */

_Bool are_equal_strings(const char *first, const char *second)
{
    return strcmp(first, second) == 0;
}

/**
 * Deletes a string.
 * @param src - deleted string.
 */

void free_string(char *src)
{
    if (src) {
        free(src);
    }
}