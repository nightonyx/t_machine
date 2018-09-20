#include <string.h>
#include <stdlib.h>
#include <lib4aio_c_headers/utils/string_utils/common.h>
#include <lib4aio_c_headers/utils/str_builder/str_builder.h>
#include <lib4aio_c_headers/utils/memory_utils/memory_utils.h>

#define INIT_CAPACITY 2

/**
 * Creates a new builder.
 * @return new str_builder struct.
 */

str_builder *new_str_builder()
{
    str_builder *builder = new_object(sizeof(str_builder));
    reset_str_builder(builder);
    return builder;
}

/**
 * Doubles size of string builder.
 * @param builder - current builder
 * @param applied_length - range of applied size.
 */

static void grow_by(str_builder *builder, const unsigned applied_length)
{
    const unsigned new_length = builder->length + applied_length + 1;
    if (new_length >= builder->capacity) {
        while (new_length >= builder->capacity) {
            builder->capacity *= 2;
        }
        char *new_string = new_object_array(builder->capacity, sizeof(char));
        strcpy(new_string, builder->string_value);
        free(builder->string_value);
        builder->string_value = new_string;
    }
}

/**
 * Adds a character in string builder.
 * @param builder - current builder.
 * @param c - character;
 */

void append_char_to_str_builder(str_builder *builder, const char c)
{
    grow_by(builder, 1);
    builder->string_value[builder->length++] = c;
}

/**
 * Extracts a collected string in string builder accumulator and resets it.
 * @param builder - current builder;
 * @return collected string.
 */

char *pop_string_from_str_builder(str_builder *builder)
{
    char *old_str = builder->string_value;
    char *new_str = new_string(old_str);
    reset_str_builder(builder);
    //------------------------------------------------------------------------------------------------------------------
    //GC:
    free(old_str);
    //------------------------------------------------------------------------------------------------------------------
    return new_str;
}

/**
 * Resets accumulator of string builder.
 * @param builder - current builder.
 */

void reset_str_builder(str_builder *builder)
{
    builder->string_value = new_object_array(INIT_CAPACITY, sizeof(char));
    builder->capacity = INIT_CAPACITY;
    builder->length = 0;
}

/**
 * Deletes a string builder.
 * @param builder - current buider.
 */

void free_str_builder(str_builder *builder)
{
    free(builder->string_value);
    free(builder);
}