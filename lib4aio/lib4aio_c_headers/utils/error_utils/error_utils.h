#ifndef LIB4AIO_C_ERROR_UTILS_H
#define LIB4AIO_C_ERROR_UTILS_H

/**
 * Prints error in console and stops a program.
 * @param tag - point of throwing error (string).
 * @param message - cause (string)
 */

void throw_error_with_tag(
        const char *tag,
        const char *message
) __attribute__ ((noreturn));

/**
 * Prints error in console and stops a program.
 * @param tag - point of throwing error (string).
 * @param message - cause (string)
 * @param hook - value of string holder.
 */

void throw_error_with_str_hook(
        const char *tag,
        const char *message,
        const struct str_hook *hook
) __attribute__ ((__noreturn__));

#endif //LIB4AIO_C_ERROR_UTILS_H