#ifndef T_TAPE_H
#define T_TAPE_H

/**
 * Abstraction of Turing machine tape.
 */

typedef struct t_tape {
    unsigned capacity;
    char *string;
} t_tape;

/**
 * Creates new Turing machine tape;
 * @param string - working space for Turing machine simulation;
 * @param capacity - initial size of working space.
 * @return new struct of Turing machine tape.
 */

struct t_tape *new_t_tape(char *string, const unsigned capacity);

/**
 * Deletes Turing machine tape;
 * @param tape - deleted tape;
 */

void free_t_tape(struct t_tape *tape);

/**
 * Doubles size of Turing machine tape.
 * @param tape - Turing machine tape.
 */

void grow_t_tape(struct t_tape *tape);

#endif //T_TAPE_H