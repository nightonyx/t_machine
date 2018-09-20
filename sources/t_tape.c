#include <stdlib.h>
#include <t_tape.h>
#include <lib4aio_c_headers/utils/string_utils/common.h>
#include <lib4aio_c_headers/utils/memory_utils/memory_utils.h>

/**
 * Creates new Turing machine tape;
 * @param string - working space for Turing machine simulation;
 * @param capacity - initial size of working space.
 * @return new struct of Turing machine tape.
 */

t_tape *new_t_tape(char *string, const unsigned capacity)
{
    t_tape *tape = new_object(sizeof(t_tape));
    tape->string = string;
    tape->capacity = capacity;
    return tape;
}

/**
 * Deletes Turing machine tape;
 * @param tape - deleted tape;
 */

void free_t_tape(t_tape *tape)
{
    free(tape->string);
    free(tape);
}

/**
 * Doubles size of Turing machine tape.
 * @param tape - Turing machine tape.
 */

void grow_t_tape(t_tape *tape)
{
    tape->capacity *= 2;
    char *old_str = tape->string;
    char *new_str = new_string(old_str);
    tape->string = new_str;
    free(old_str);
}