#include <stdio.h>
#include <t_core.h>
#include <stdlib.h>
#include <t_tape.h>
#include <stdbool.h>
#include <t_message.h>
#include <t_command.h>
#include <t_iterator.h>
#include <lib4aio_c_headers/utils/str_hook/str_hook.h>
#include <lib4aio_c_headers/utils/log_utils/log_utils.h>
#include <lib4aio_c_headers/utils/string_utils/common.h>
#include <lib4aio_c_headers/utils/file_utils/file_utils.h>
#include <lib4aio_c_headers/utils/error_utils/error_utils.h>
#include <lib4aio_c_headers/utils/struct_list/struct_list.h>
#include <lib4aio_c_headers/utils/str_builder/str_builder.h>
#include <lib4aio_c_headers/utils/memory_utils/memory_utils.h>

#define T_CORE_DEBUG

#define T_CORE_INFO_TAG "T_CORE"

#define T_CORE_ERROR_TAG "T_CORE_ERROR"

/**
 * Business logic.
 */

#define T_COMMAND_PARTS 6

#define HYPHEN "-"

#define MOVE_TO_LEFT "L"

#define MOVE_TO_RIGHT "R"

#define HOLD_POSITION "H"

#define STOP_PROGRAM "S"

/**
 * Global core in file scope.
 */

t_core *s_core;

/**
 * Create a new struct of Turing machine command.
 * @param chunk - string holder (str_hook);
 * @return command.
 */

static t_command *create_t_command(const str_hook *chunk)
{
    struct_list *dirty_command_parts = split_str_hook_by_space(chunk);
    //Prepare to filter:
    //Create function of condition:
    _Bool (*filter_condition)(const void *) = (_Bool (*)(const void *)) is_not_empty_hooked_str;
    //Create function of copying:
    void *(*copy_constructor)(const void *) = (void *(*)(const void *)) new_str_hook_by_other;
    struct_list *clean_command_parts = filter_struct_list(
            dirty_command_parts,
            filter_condition,
            copy_constructor
    );
    const _Bool is_valid_t_command_parts = clean_command_parts->size == T_COMMAND_PARTS;
    if (is_valid_t_command_parts) {
        str_hook **parts = (str_hook **) clean_command_parts->elements;
        //Extract command holders:
        const str_hook *source_char_holder = parts[0];
        const str_hook *source_state_holder = parts[1];
        const str_hook *hyphen_holder = parts[2];
        const str_hook *destination_char_holder = parts[3];
        const str_hook *destination_state_holder = parts[4];
        const str_hook *iterator_flag_holder = parts[5];
        //Check hypotheses:
        const _Bool is_first_symbol = get_str_hook_size(source_char_holder) == 1;
        const _Bool is_second_state = is_word_hooked(source_state_holder);
        const _Bool is_third_hyphen = is_hook_equals_str(hyphen_holder, HYPHEN);
        const _Bool is_fourth_symbol = get_str_hook_size(destination_char_holder) == 1;
        const _Bool is_fifth_state = is_word_hooked(destination_state_holder);
        //Check iterator flag:
        const _Bool is_sixth_iterator_flag_move_to_left = is_hook_equals_str(iterator_flag_holder, MOVE_TO_LEFT);
        const _Bool is_sixth_iterator_flag_move_to_right = is_hook_equals_str(iterator_flag_holder, MOVE_TO_RIGHT);
        const _Bool is_sixth_iterator_flag_move_to_hold = is_hook_equals_str(iterator_flag_holder, HOLD_POSITION);
        const _Bool is_sixth_iterator_flag_move_to_stop = is_hook_equals_str(iterator_flag_holder, STOP_PROGRAM);
        const _Bool is_sixth_iterator_flag = is_sixth_iterator_flag_move_to_left
                                             || is_sixth_iterator_flag_move_to_right
                                             || is_sixth_iterator_flag_move_to_hold
                                             || is_sixth_iterator_flag_move_to_stop;
        if (is_first_symbol
            && is_second_state
            && is_third_hyphen
            && is_fourth_symbol
            && is_fifth_state
            && is_sixth_iterator_flag) {
            char source_char = get_str_hook_char(source_char_holder, source_char_holder->start);
            const struct str_hook *source_state = new_str_hook_by_other(source_state_holder);
            char destination_char = get_str_hook_char(destination_char_holder, destination_char_holder->start);
            const struct str_hook *destination_state = new_str_hook_by_other(destination_state_holder);
            //Define iterator command type:
            t_iterator_command_type iterator_command_type = 0;
            if (is_sixth_iterator_flag_move_to_left) {
                iterator_command_type = T_MOVE_LEFT;
            } else if (is_sixth_iterator_flag_move_to_right) {
                iterator_command_type = T_MOVE_RIGHT;
            } else if (is_sixth_iterator_flag_move_to_hold) {
                iterator_command_type = T_HOLD_POSITION;
            } else if (is_sixth_iterator_flag_move_to_stop) {
                iterator_command_type = T_STOP;
            }
            t_command *command = new_t_command(
                    source_char,
                    source_state,
                    destination_char,
                    destination_state,
                    iterator_command_type
            );
            //----------------------------------------------------------------------------------------------------------
            //Define free function:
            void (*free_function) = (void *) (void (*)(void *)) free_str_hook;
            //GC:
            free_structs_in_list(dirty_command_parts, free_function);
            free_structs_in_list(clean_command_parts, free_function);
            free_struct_list(dirty_command_parts);
            free_struct_list(clean_command_parts);
            //----------------------------------------------------------------------------------------------------------
            return command;
        } else {
            throw_error_with_str_hook(
                    T_CORE_ERROR_TAG,
                    "Invalid turing command. Expected '<ASCII symbol> <state> - <ASCII symbol> <state>'",
                    chunk
            );
        }
    } else {
        throw_error_with_str_hook(
                T_CORE_ERROR_TAG,
                "Wrong command parts. Expected '<ASCII symbol> <state> - <ASCII symbol> <state>'",
                chunk
        );
    }
}

/**
 * Extracts command file *.txt, creates Turing machine commands and puts their to core.
 * @param command_file_path path to command file;
 */

static t_core *inflate_t_commands(const char *command_file_path)
{
    //Read file:
    str_builder *file_builder = read_file_by_str_builder(command_file_path);
    //Init string scope:
    str_hook *file_content = new_str_hook(file_builder->string_value);
    file_content->start = 0;
    file_content->end = file_builder->length;
    //Prepare to parse content:
    struct_list *chunk_list = split_str_hook_by_line_break(file_content);
#ifdef T_CORE_DEBUG
    for (unsigned j = 0; j < chunk_list->size; ++j) {
        log_info_str_hook(T_CORE_INFO_TAG, "<HOOK>", chunk_list->elements[j]);
    }
#endif
    //Prepare to filter:
    //Create function of condition:
    _Bool (*filter_condition)(const void *) = (_Bool (*)(const void *)) is_not_empty_hooked_str;
    //Create function of copying:
    void *(*copy_constructor)(const void *) = (void *(*)(const void *)) new_str_hook_by_other;
    //Filter elements:
    struct_list *command_hook_list = filter_struct_list(
            chunk_list,
            filter_condition,
            copy_constructor
    );
#ifdef T_CORE_DEBUG
    log_info(T_CORE_INFO_TAG, "-------------------------------");
    for (unsigned j = 0; j < command_hook_list->size; ++j) {
        log_info_str_hook(T_CORE_INFO_TAG, "<COMMAND HOOK>", command_hook_list->elements[j]);
    }
#endif
    const unsigned command_hook_count = command_hook_list->size;
    //Check command count:
    const _Bool has_commands = command_hook_count > 0;
    if (has_commands) {
        str_hook **command_hooks = (str_hook **) command_hook_list->elements;
        //Prepare to create commands for command list of t_core:
        struct_list *command_list = new_struct_list(sizeof(t_command));
        for (unsigned i = 0; i < command_hook_count; ++i) {
            const str_hook *command_hook = command_hooks[i];
            t_command *command = create_t_command(command_hook);
            add_struct_in_list(command_list, command);
#ifdef T_CORE_DEBUG
            log_info(T_CORE_INFO_TAG, "<T_COMMAND> ----------");
            log_info_char(T_CORE_INFO_TAG, "SRC CHAR:", command->source_char);
            log_info_str_hook(T_CORE_INFO_TAG, "SRC STATE:", command->source_state);
            log_info_char(T_CORE_INFO_TAG, "DST CHAR:", command->destination_char);
            log_info_str_hook(T_CORE_INFO_TAG, "DST STATE:", command->destination_state);
            switch (command->iterator_command_type) {
                case T_MOVE_RIGHT:
                    log_info(T_CORE_INFO_TAG, "TYPE: RIGHT");
                    break;
                case T_MOVE_LEFT:
                    log_info(T_CORE_INFO_TAG, "TYPE: LEFT");
                    break;
                case T_HOLD_POSITION:
                    log_info(T_CORE_INFO_TAG, "TYPE: HOLD");
                    break;
                case T_STOP:
                    log_info(T_CORE_INFO_TAG, "TYPE: STOP");
                    break;
            }
#endif
        }
        //Set input data to core:
        s_core->command_file_builder = file_builder;
        //Set commands to core:
        s_core->command_list = command_list;
        //--------------------------------------------------------------------------------------------------------------
        //Define free function:
        void (*free_function)(void *) = (void (*)(void *)) free_str_hook;
        //GC:
        free_structs_in_list(command_hook_list, free_function);
        free_structs_in_list(chunk_list, free_function);
        free_struct_list(command_hook_list);
        free_struct_list(chunk_list);
        //--------------------------------------------------------------------------------------------------------------
        return s_core;
    } else {
        throw_error_with_tag(T_CORE_ERROR_TAG, "The command file doesn't contain commands!");
    }
}

/**
 * Extracts input tape file *.txt, creates Turing machine tape and puts it to core.
 * @param input_file_path - input tape file path.
 */

static t_core *inflate_t_tape(const char *input_file_path)
{
#ifdef T_CORE_DEBUG
    log_info(T_CORE_INFO_TAG, "Inflate tape...");
#endif
    //Read file:
    str_builder *file_builder = read_file_by_str_builder(input_file_path);
    char *input_string = file_builder->string_value;
    const unsigned input_string_length = file_builder->length;
    const _Bool has_content = input_string_length > 0;
    if (has_content) {
        t_tape *tape = new_t_tape(input_string, input_string_length);
        //Set tape to core:
        s_core->tape = tape;
        //--------------------------------------------------------------------------------------------------------------
        //GC:
        reset_str_builder(file_builder);
        free_str_builder(file_builder);
        //--------------------------------------------------------------------------------------------------------------
#ifdef T_CORE_DEBUG
        str_hook *tape_holder = new_str_hook(tape->string);
        tape_holder->start = 0;
        tape_holder->end = tape->capacity;
        log_info_str_hook(T_CORE_INFO_TAG, "TAPE:", tape_holder);
        free_str_hook(tape_holder);
#endif
        return s_core;
    } else {
        throw_error_with_tag(T_CORE_ERROR_TAG, "Input file is empty!");
    }
}

/**
 * Opens output file *.txt for writing Turing machine commands in runtime and put file reference in core.
 * @param output_file_path - output file path.
 */

static t_core *inflate_output_file(const char *output_file_path)
{
    FILE *output_file = open_file_for_writing(output_file_path);
    //Set output file to core for writing:
    s_core->output_file = output_file;
#ifdef T_CORE_DEBUG
    log_info(T_CORE_INFO_TAG, "Output file is opened");
#endif
    return s_core;
}

/**
 * Sets Turing machine mode in core.
 * @param is_debug_mode - flag.
 */

static t_core *set_core_mode(const _Bool is_debug_mode)
{
    s_core->is_debug_mode = is_debug_mode;
    return s_core;
}

/**
 * Creates reading head of Turing machine and put it to core.
 * @param initial_state
 * @return
 */

static t_core *inflate_t_iterator(str_hook *initial_state)
{
    t_iterator *iterator = new_t_iterator(initial_state);
    //Put iterator to core:
    s_core->iterator = iterator;
    return s_core;
}

/**
 * Tries to find next command by letter and state.
 * If command is absent then throws error.
 * @param tape - Turing machine tape;
 * @param iterator - Turing machine reading head;
 * @return struct of Turing machine command.
 */

static const t_command *find_command_by_state(t_tape *tape, t_iterator *iterator)
{
    const char *string = tape->string;
    const unsigned current_position = iterator->current_position;
    const char current_char = string[current_position];
    const str_hook *current_state = iterator->current_state;
    //Find command by current letter & current state:
    const struct_list *command_list = s_core->command_list;
    const t_command **commands = (const t_command **) command_list->elements;
    const unsigned command_count = command_list->size;
    for (unsigned i = 0; i < command_count; ++i) {
        const t_command *command = commands[i];
        const _Bool matches_letter = command->source_char == current_char;
        const _Bool matches_state = are_equal_hooked_str(command->source_state, current_state);
        if (matches_letter && matches_state) {
            return command;
        }
    }
    log_info(T_CORE_ERROR_TAG, "Detected invalid combination:");
    log_info_char(T_CORE_ERROR_TAG, "Letter:", current_char);
    log_info_str_hook(T_CORE_ERROR_TAG, "State:", current_state);
    throw_error_with_tag(T_CORE_ERROR_TAG, "Invalid state...");
}

/**
 * Define user interface commands for debug mode.
 */

#define T_INPUT_MESSAGE_SIZE 5

#define T_INPUT_MESSAGE_NEXT ">"

#define T_INPUT_MESSAGE_RUN ">>"

#define T_INPUT_MESSAGE_STOP "||"

/**
 * Interprets Turing machine system.
 */

static t_core *launch_program()
{
#ifdef T_CORE_DEBUG
    log_info(T_CORE_INFO_TAG, "launch a program");
#endif
    //Get tape:
    t_tape *tape = s_core->tape;
    char *string = tape->string;
    //Get iterator:
    t_iterator *iterator = s_core->iterator;
    const str_hook *initial_state = iterator->current_state;
    _Bool is_running = true;
    while (is_running) {
        const t_command *command = find_command_by_state(tape, iterator);

        const str_hook *new_state = command->destination_state;
        const unsigned current_position = iterator->current_position;
        //Change iterator state:
        iterator->current_state = new_state;
        //Change letter on current position of tape:
        string[current_position] = command->destination_char;

        //Printing a output_message:
        //--------------------------------------------------------------------------------------------------------------
        const char *output_message = collect_t_output_message(tape, iterator, command);
        write_line_to_file(s_core->output_file, output_message);
        //--------------------------------------------------------------------------------------------------------------
        if (s_core->is_debug_mode) {
            char input_message[T_INPUT_MESSAGE_SIZE];
            log_info(T_CORE_INFO_TAG, "Enter a message:");
            scanf("%4s", input_message);
            if (are_equal_strings(input_message, T_INPUT_MESSAGE_NEXT)) {
                printf("%s", output_message);
            } else if (are_equal_strings(input_message, T_INPUT_MESSAGE_RUN)) {
                s_core->is_debug_mode = false;
            } else if (are_equal_strings(input_message, T_INPUT_MESSAGE_STOP)) {
                is_running = false;
            } else {
                throw_error_with_tag(T_CORE_ERROR_TAG, "Invalid input command. Expected: ENTER or '>>' or '||'");
            }
        }
        //--------------------------------------------------------------------------------------------------------------
        //GC:
        free_string((char *) output_message);
        //--------------------------------------------------------------------------------------------------------------
        switch (command->iterator_command_type) {
            case T_MOVE_RIGHT:
                move_iterator_to_right(iterator, tape);
                break;
            case T_MOVE_LEFT:
                move_iterator_to_left(iterator);
                break;
            case T_HOLD_POSITION:
                break;
            case T_STOP:
                //Finish a program:
                is_running = false;
                iterator->current_state = initial_state;
        }
    }
#ifdef T_CORE_DEBUG
    log_info(T_CORE_INFO_TAG, "Simulation is complete");
#endif
    return s_core;
}

/**
 * Frees Turing machine system.
 */

static void free_core()
{
    fclose(s_core->output_file);
    //------------------------------------------------------------------------------------------------------------------
    void (*free_function)(void *) = (void (*)(void *)) free_t_command;
    //GC:
    free_t_iterator(s_core->iterator);
    free_t_tape(s_core->tape);
    free_structs_in_list(s_core->command_list, free_function);
    free_struct_list(s_core->command_list);
    free_str_builder(s_core->command_file_builder);
    free(s_core);
}

/**
 * Initializes Turing machine system.
 */

t_core *core()
{
    s_core = new_object(sizeof(t_core));
    s_core->init_t_commands = inflate_t_commands;
    s_core->init_t_tape = inflate_t_tape;
    s_core->init_t_output_file = inflate_output_file;
    s_core->init_t_mode = set_core_mode;
    s_core->init_t_iterator = inflate_t_iterator;
    s_core->launch = launch_program;
    s_core->finish = free_core;
#ifdef T_CORE_DEBUG
    log_info(T_CORE_INFO_TAG, "Core initializing is complete");
#endif
    return s_core;
}