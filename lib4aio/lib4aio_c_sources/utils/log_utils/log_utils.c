#include <stdio.h>
#include <lib4aio_c_headers/utils/str_hook/str_hook.h>

/**
 * Prints a message.
 * @param tag - point of messaging (string);
 * @param message - string.
 */

void log_info(const char *tag, const char *message)
{
    printf("\n%s: %s\n", tag, message);
}

/**
 * Prints a character info.
 * @param tag - point of messaging (string);
 * @param message - info about letter.
 * @param value - char value.
 */

void log_info_char(const char *tag, const char *message, const char value)
{
    printf("\n%s: %s -%c-\n", tag, message, value);
}

/**
 * Prints a string holder info.
 * @param tag - point of messaging (string);
 * @param message - info about string holder.
 * @param hook - value of str_hook struct;
 */

void log_info_str_hook(const char *tag, const char *message, const str_hook *hook)
{
    printf("\n%s: %s -", tag, message);
    for (unsigned i = hook->start; i < hook->end; ++i) {
        printf("%c", hook->source_string[i]);
    }
    printf("-\n");
}