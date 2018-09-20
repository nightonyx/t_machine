#ifndef LIB4AIO_C_STRING_UTILS_COMMON_H
#define LIB4AIO_C_STRING_UTILS_COMMON_H

/**
 * Creates a new string by other string.
 * @param src - source string;
 * @return new string.
 */

char *new_string(const char *src);

/**
 * Matches content of two strings.
 * @param first - first string;
 * @param second - second string;
 * @return true / false.
 */

_Bool are_equal_strings(const char *first, const char *second);

/**
 * Deletes a string.
 * @param src - deleted string.
 */

void free_string(char *src);

#endif //LIB4AIO_C_STRING_UTILS_COMMON_H