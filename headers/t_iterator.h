#ifndef T_ITERATOR_H
#define T_ITERATOR_H

/**
 * Reading head abstraction of Turing machine
 */

typedef struct t_iterator {
    const struct str_hook *current_state;
    unsigned current_position;
} t_iterator;

/**
 * Creates a new Turing machine reading head.
 * @param initial_state - start state (string holder)
 * @return new iterator struct.
 */

struct t_iterator *new_t_iterator(const struct str_hook *initial_state);

/**
 * Deletes a reading head.
 * @param iterator - deleted struct.
 */

void free_t_iterator(struct t_iterator *iterator);

/**
 * Move reading head to the right.
 * @param iterator - reading head.
 * @param tape_ref - Turing machine tape reference.
 *
 * What happens if the iterator reaches end of the tape?
 * By definition the tape of the abstract Turing machine has endless size.
 * Thus, when the iterator reaches end of the tape, the size of this tape will be doubled.
 */

void move_iterator_to_right(struct t_iterator *iterator, struct t_tape *tape_ref);

/**
 * Move reading head to the left.
 * @param iterator - reading head.
 *
 * Once the iterator tried to move to the left from zero position,
 * the program throw error immediately about index out of bound.
 */

void move_iterator_to_left(struct t_iterator *iterator);

#endif //T_ITERATOR_H