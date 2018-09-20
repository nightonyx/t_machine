#ifndef T_MESSAGE_H
#define T_MESSAGE_H

/**
 * Collect a pretty message about Turing machine system.
 * @param tape - Turing machine tape;
 * @param iterator - Turing Machine reading head;
 * @param command - current Turing machine command;
 * @return string.
 */

const char *collect_t_output_message(
        const struct t_tape *tape,
        const struct t_iterator *iterator,
        const struct t_command *command
);

#endif //T_MESSAGE_H