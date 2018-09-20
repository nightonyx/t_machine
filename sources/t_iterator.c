#include <stdlib.h>
#include <t_tape.h>
#include <t_iterator.h>
#include <lib4aio_c_headers/utils/str_hook/str_hook.h>
#include <lib4aio_c_headers/utils/memory_utils/memory_utils.h>
#include <lib4aio_c_headers/utils/error_utils/error_utils.h>

#define T_ITERATOR_ERROR_TAG "T_ITERATOR_ERROR"

/**
 * Creates a new Turing machine reading head.
 * @param initial_state - start state (string holder)
 * @return new iterator struct.
 */

t_iterator *new_t_iterator(const str_hook *initial_state)
{
    t_iterator *iterator = new_object(sizeof(t_iterator));
    iterator->current_position = 0;
    iterator->current_state = initial_state;
    return iterator;
}

/**
 * Deletes a reading head.
 * @param iterator - deleted struct.
 */

void free_t_iterator(t_iterator *iterator)
{
    free_str_hook((str_hook *) iterator->current_state);
    free(iterator);
}

/**
 * Move reading head to the right.
 * @param iterator - reading head.
 * @param tape_ref - Turing machine tape reference.
 *
 * What happens if the iterator reaches end of the tape?
 * By definition the tape of the abstract Turing machine has endless size.
 * Thus, when the iterator reaches end of the tape, the size of this tape will be doubled.
 */

void move_iterator_to_right(t_iterator *iterator, t_tape *tape_ref)
{
    if (iterator->current_position == tape_ref->capacity) {
        grow_t_tape(tape_ref);
    }
    iterator->current_position++;
}

/**
 * Move reading head to the left.
 * @param iterator - reading head.
 *
 * Once the iterator tried to move to the left from zero position,
 * the program throw error immediately about index out of bound.
 */

void move_iterator_to_left(t_iterator *iterator)
{
    if (iterator->current_position == 0) {
        throw_error_with_tag(T_ITERATOR_ERROR_TAG, "Index out of bound!");
    } else {
        iterator->current_position--;
    }
}