#ifndef LIB4AIO_C_LOG_UTILS_H
#define LIB4AIO_C_LOG_UTILS_H

/**
 * Prints a message.
 * @param tag - point of messaging (string);
 * @param message - string.
 */

void log_info(const char *tag, const char *message);

/**
 * Prints a character info.
 * @param tag - point of messaging (string);
 * @param message - info about letter.
 * @param value - char value.
 */

void log_info_char(const char *tag, const char *message, const char value);

/**
 * Prints a string holder info.
 * @param tag - point of messaging (string);
 * @param message - info about string holder.
 * @param hook - value of str_hook struct;
 */

void log_info_str_hook(const char *tag, const char *message, const struct str_hook *hook);

#endif //LIB4AIO_C_LOG_UTILS_H