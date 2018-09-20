#include <string.h>
#include <lib4aio_c_headers/utils/str_hook/str_hook.h>
#include <lib4aio_c_headers/utils/struct_list/struct_list.h>
#include <lib4aio_c_headers/utils/memory_utils/memory_utils.h>

#define WHITESPACE ' '

#define LINE_BREAK '\n'

/**
 * Compares character with whitespace.
 * @param c - current letter.
 * @return true / false.
 */

static inline _Bool is_space(const char c)
{
    return c == WHITESPACE;
}

/**
 * Compares character with line break.
 * @param c -current letter
 * @return true / false.
 */

static inline _Bool is_line_break(const char c)
{
    return c == LINE_BREAK;
}

/**
 * Split string hook by whitespace.
 * @param hook - current hook
 * @return new struct_list struct with str_hook element type.
 */

struct_list *split_str_hook_by_space(const str_hook *hook)
{
    return split_str_hook_by_char_condition(hook, is_space);
}

/**
 * Split string hook by line break.
 * @param hook - current hook
 * @return new struct_list struct with str_hook element type.
 */

struct_list *split_str_hook_by_line_break(const str_hook *hook)
{
    return split_str_hook_by_char_condition(hook, is_line_break);
}

/**
 * Split string hook by character condition.
 * @param hook - current hook;
 * @param char_condition - character condition function;
 * @return new struct_list struct with str_hook element type.
 */

struct_list *split_str_hook_by_char_condition(const str_hook *hook, _Bool (*char_condition)(const char))
{
    const char *src = hook->source_string;
    const unsigned start = hook->start;
    const unsigned end = hook->end;
    struct_list *hooks = new_struct_list(sizeof(str_hook));
    str_hook *watcher = new_str_hook(NULL);
    watcher->start = start;
    for (unsigned i = start; i < end; ++i) {
        const char symbol = src[i];
        const _Bool is_split_point = char_condition(symbol);
        if (is_split_point) {
            str_hook *new_hook = new_str_hook(src);
            new_hook->start = watcher->start;
            new_hook->end = i;
            add_struct_in_list(hooks, new_hook);
            watcher->start = i + 1;
        }
    }
    str_hook *last_hook = new_str_hook(src);
    last_hook->start = watcher->start;
    last_hook->end = end;
    add_struct_in_list(hooks, last_hook);
    return hooks;
}