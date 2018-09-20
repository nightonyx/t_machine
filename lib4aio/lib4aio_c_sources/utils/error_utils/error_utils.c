#include <stdio.h>
#include <stdlib.h>
#include <lib4aio_c_headers/utils/str_hook/str_hook.h>

/**
 * Prints error in console and stops a program.
 * @param tag - point of throwing error (string).
 * @param message - cause (string)
 */

void throw_error_with_tag(const char *tag, const char *message)
{
    printf("\n%s: %s\n", tag, message);
    exit(1);
}

/**
 * Prints error in console and stops a program.
 * @param tag - point of throwing error (string).
 * @param message - cause (string)
 * @param hook - value of string holder.
 */

void throw_error_with_str_hook(const char *tag, const char *message, const str_hook *hook)
{
    const unsigned end = hook->end;
    const char *source_string = hook->source_string;
    printf("\n%s: %s -", tag, message);
    for (unsigned i = hook->start; i < end; ++i) {
        printf("%c", source_string[i]);
    }
    printf("-\n");
    exit(1);
}