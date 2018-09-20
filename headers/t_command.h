#ifndef T_COMMAND_H
#define T_COMMAND_H

/**
 * Defines reading head command of Turing machine.
 */

typedef enum t_iterator_command_type {
    T_MOVE_RIGHT, T_MOVE_LEFT, T_HOLD_POSITION, T_STOP
} t_iterator_command_type;

/**
 * Abstraction of Turing machine command.
 */

typedef struct t_command {
    char source_char;
    const struct str_hook *source_state;
    char destination_char;
    const struct str_hook *destination_state;
    enum t_iterator_command_type iterator_command_type;
} t_command;

/**
 * Creates a new Turing machine command.
 * @param source_char - old letter;
 * @param source_state - old state;
 * @param destination_char - new letter;
 * @param destination_state - new state;
 * @param iterator_command_type - reading head movement command.
 * @return new struct of Turing machine command.
 */

struct t_command *new_t_command(
        const char source_char,
        const struct str_hook *source_state,
        const char destination_char,
        const struct str_hook *destination_state,
        enum t_iterator_command_type iterator_command_type
);

/**
 * Delete Turing machine command.
 * @param command deleted command.
 */

void free_t_command(struct t_command *command);

#endif //T_COMMAND_H