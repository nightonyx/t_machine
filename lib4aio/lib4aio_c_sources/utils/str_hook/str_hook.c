#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <lib4aio_c_headers/utils/str_hook/str_hook.h>
#include <lib4aio_c_headers/utils/memory_utils/memory_utils.h>
#include <lib4aio_c_headers/utils/struct_list/struct_list.h>

/**
 * Creates a new string hook.
 * @param source_ref - source string pointer.
 * @return new str_hook struct.
 */

str_hook *new_str_hook(const char *source_ref)
{
    str_hook *hook = new_object(sizeof(str_hook));
    hook->source_string = source_ref;
    hook->start = 0;
    hook->end = 0;
    return hook;
}

/**
 * Deletes string hook.
 * @param hook - current hook.
 */

void free_str_hook(str_hook *hook)
{
    if (hook) {
        free(hook);
    }
}

//#define STRING_HOOK_DEBUG

#ifdef STRING_HOOK_DEBUG

#include <lib4aio_c_headers/utils/log_utils/log_utils.h>

#endif

#define STRING_HOOK_TAG "STRING_HOOK"

/**
 * Creates a new str_hook by other str_hook.
 * @param other_hook - other str_hook struct;
 * @return new str_hook struct.
 */

str_hook *new_str_hook_by_other(const str_hook *other_hook)
{
    str_hook *hook = new_object(sizeof(str_hook));
    hook->source_string = other_hook->source_string;
    hook->start = other_hook->start;
    hook->end = other_hook->end;
    return hook;
}

/**
 * Counts a holding positions in string hook.
 * @param hook - current hook;
 * @return holding size.
 */

unsigned get_str_hook_size(const str_hook *hook)
{
    return hook->end - hook->start;
}

/**
 * Gets a character of source string via str_hook.
 * @param hook - current hook.
 * @param index - absolute position.
 * @return character.
 *
 * Note: if you want relative position you need to use hook->get_str_hook_size(hook->start + index)!
 */

char get_str_hook_char(const str_hook *hook, const unsigned index)
{
    return hook->source_string[index];
}

/**
 * Matches content of two hooks.
 * @param hook_1 - first;
 * @param hook_2 - second;
 * @return is equals or not (true /false).
 */

_Bool are_equal_hooked_str(const str_hook *hook_1, const str_hook *hook_2)
{
    const int size_1 = get_str_hook_size(hook_1);
    const int size_2 = get_str_hook_size(hook_2);
    if (size_1 != size_2) {
        return false;
    }
    const char *str_1 = hook_1->source_string;
    const char *str_2 = hook_2->source_string;
    const unsigned start_1 = hook_1->start;
    const unsigned start_2 = hook_2->start;
    for (unsigned i = 0; i < size_1; ++i) {
        if (str_1[start_1 + i] != str_2[start_2 + i]) {
            return false;
        }
    }
    return true;
}

/**
 * Matches content of hook with word pattern.
 * @param hook - current hook.
 * @return true / false.
 */

_Bool is_word_hooked(const str_hook *hook)
{
    const unsigned length = get_str_hook_size(hook);
    if (length < 1) {
        return false;
    }
    const char first_symbol = get_str_hook_char(hook, hook->start);
    if (isalpha(first_symbol)) {
        if (length > 1) {
            for (unsigned i = hook->start + 1; i < length; ++i) {
                const char symbol = get_str_hook_char(hook, i);
                if (!isalnum(symbol)) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

/**
 * Matches content of hook and string.
 * @param hook - current hook;
 * @param str - current string;
 * @return true / false.
 */

_Bool is_hook_equals_str(const str_hook *hook, const char *str)
{
    const unsigned hook_size = get_str_hook_size(hook);
    const unsigned str_length = (const unsigned int) strlen(str);
    if (hook_size != str_length) {
        return false;
    }
    const char *hooked_str = hook->source_string;
    const unsigned start = hook->start;
    for (unsigned i = 0; i < hook_size; ++i) {
        if (hooked_str[start + i] != str[i]) {
            return false;
        }
    }
    return true;
}

/**
 * Checks holding positions of hook.
 * @param hook - current hook;
 * @return true / false.
 */

_Bool is_not_empty_hooked_str(const str_hook *hook)
{
    return hook->end - hook->start > 0;
}

