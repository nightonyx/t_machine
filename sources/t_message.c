#include <stdio.h>
#include <t_tape.h>
#include <t_command.h>
#include <t_iterator.h>
#include <lib4aio_c_headers/utils/str_hook/str_hook.h>
#include <lib4aio_c_headers/utils/str_builder/str_builder.h>

#define T_LINE_BREAK '\n'

#define T_POINTER 'V'

#define T_UNDERSCORE '_'

#define T_WHITESPACE ' '

#define T_HYPHEN '-'

/**
 * Collects holding characters of str_hook holder to string builder.
 * @param hook - current hook.
 * @param string_builder - current builder.
 */

static void put_str_hook_to_str_builder(const str_hook *hook, str_builder *string_builder)
{
    const char *string_ref = hook->source_string;
    const unsigned start = hook->start;
    const unsigned end = hook->end;
    for (unsigned i = start; i < end; ++i) {
        const char current_symbol = string_ref[i];
        append_char_to_str_builder(string_builder, current_symbol);
    }
}

/**
 * Collect a pretty message about Turing machine system.
 * @param tape - Turing machine tape;
 * @param iterator - Turing Machine reading head;
 * @param command - current Turing machine command;
 * @return string.
 */

const char *collect_t_output_message(const t_tape *tape, const t_iterator *iterator, const t_command *command)
{
    str_builder *string_builder = new_str_builder();
    //Start to build message:
    //------------------------------------------------------------------------------------------------------------------
    append_char_to_str_builder(string_builder, T_LINE_BREAK);
    const unsigned tape_length = tape->capacity;
    //Collect iterator position line:
    const unsigned iterator_position = iterator->current_position;
    for (unsigned i = 0; i < tape_length; ++i) {
        char c;
        if (i == iterator_position) {
            c = T_POINTER;
        } else {
            c = T_UNDERSCORE;
        }
        append_char_to_str_builder(string_builder, c);
    }
    append_char_to_str_builder(string_builder, T_LINE_BREAK);
    //Collect tape:
    const char *string = tape->string;
    for (unsigned j = 0; j < tape_length; ++j) {
        append_char_to_str_builder(string_builder, string[j]);
    }
    append_char_to_str_builder(string_builder, T_LINE_BREAK);
    //Collect current command:
    //Put source letter:
    append_char_to_str_builder(string_builder, command->source_char);
    append_char_to_str_builder(string_builder, T_WHITESPACE);
    //Put source command:
    put_str_hook_to_str_builder(command->source_state, string_builder);
    //Put hyphen:
    append_char_to_str_builder(string_builder, T_WHITESPACE);
    append_char_to_str_builder(string_builder, T_HYPHEN);
    append_char_to_str_builder(string_builder, T_WHITESPACE);
    //Put destination letter:
    append_char_to_str_builder(string_builder, command->destination_char);
    append_char_to_str_builder(string_builder, T_WHITESPACE);
    //Put destination command:
    put_str_hook_to_str_builder(command->destination_state, string_builder);
    append_char_to_str_builder(string_builder, T_LINE_BREAK);
    //------------------------------------------------------------------------------------------------------------------
    //Building is complete:
    const char *message = pop_string_from_str_builder(string_builder);
    //------------------------------------------------------------------------------------------------------------------
    //GC:
    free_str_builder(string_builder);
    //------------------------------------------------------------------------------------------------------------------
    return message;
}