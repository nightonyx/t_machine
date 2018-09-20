#ifndef LIB4AIO_C_STRING_HOOK_H
#define LIB4AIO_C_STRING_HOOK_H

/**
 * String hook is abstraction of holding part of source string.
 */

typedef struct str_hook
{
    const char *source_string;
    unsigned start;
    unsigned end;
} str_hook;

/**
 * Creates a new string hook.
 * @param source_ref - source string pointer.
 * @return new str_hook struct.
 */

struct str_hook *new_str_hook(const char *source_ref);

/**
 * Deletes string hook.
 * @param hook - current hook.
 */

void free_str_hook(struct str_hook *hook);

/**
 * Creates a new str_hook by other str_hook.
 * @param other_hook - other str_hook struct;
 * @return new str_hook struct.
 */

struct str_hook *new_str_hook_by_other(const struct str_hook *other_hook);

/**
 * Counts a holding positions in string hook.
 * @param hook - current hook;
 * @return holding size.
 */

unsigned get_str_hook_size(const struct str_hook *hook);

/**
 * Gets a character of source string via str_hook.
 * @param hook - current hook.
 * @param index - absolute position.
 * @return character.
 *
 * Note: if you want relative position you need to use hook->get_str_hook_size(hook->start + index)!
 */

char get_str_hook_char(const struct str_hook *hook, const unsigned index);

/**
 * Matches content of two hooks.
 * @param hook_1 - first;
 * @param hook_2 - second;
 * @return is equals or not (true /false).
 */

_Bool are_equal_hooked_str(const struct str_hook *hook_1, const struct str_hook *hook_2);

/**
 * Matches content of hook with word pattern.
 * @param hook - current hook.
 * @return true / false.
 */

_Bool is_word_hooked(const struct str_hook *hook);

/**
 * Matches content of hook and string.
 * @param hook - current hook;
 * @param str - current string;
 * @return true / false.
 */

_Bool is_hook_equals_str(const struct str_hook *hook, const char *str);

/**
 * Checks holding positions of hook.
 * @param hook - current hook;
 * @return true / false.
 */

_Bool is_not_empty_hooked_str(const struct str_hook *hook);

/**
 * Split string hook by whitespace.
 * @param hook - current hook
 * @return new struct_list struct with str_hook element type.
 */

struct struct_list *split_str_hook_by_space(const struct str_hook *hook);

/**
 * Split string hook by line break.
 * @param hook - current hook
 * @return new struct_list struct with str_hook element type.
 */

struct struct_list *split_str_hook_by_line_break(const struct str_hook *hook);

/**
 * Split string hook by character condition.
 * @param hook - current hook;
 * @param char_condition - character condition function;
 * @return new struct_list struct with str_hook element type.
 */

struct struct_list *split_str_hook_by_char_condition(
        const struct str_hook *hook,
        _Bool (*char_condition)(const char)
);

#endif //LIB4AIO_C_STRING_HOOK_H