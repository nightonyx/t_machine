#include <stdlib.h>
#include <t_command.h>
#include <lib4aio_c_headers/utils/str_hook/str_hook.h>
#include <lib4aio_c_headers/utils/memory_utils/memory_utils.h>

/**
 * Creates a new Turing machine command.
 * @param source_char - old letter;
 * @param source_state - old state;
 * @param destination_char - new letter;
 * @param destination_state - new state;
 * @param iterator_command_type - reading head movement command.
 * @return new struct of Turing machine command.
 */

t_command *new_t_command(
        char source_char,
        const str_hook *source_state,
        char destination_char,
        const str_hook *destination_state,
        t_iterator_command_type iterator_command_type
)
{
    t_command *command = new_object(sizeof(t_command));
    command->source_char = source_char;
    command->source_state = source_state;
    command->destination_char = destination_char;
    command->destination_state = destination_state;
    command->iterator_command_type = iterator_command_type;
    return command;
}

/**
 * Delete Turing machine command.
 * @param command deleted command.
 */

void free_t_command(t_command *command)
{
    free_str_hook((str_hook *) command->source_state);
    free_str_hook((str_hook *) command->destination_state);
    free(command);
}