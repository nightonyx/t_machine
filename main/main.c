#include <string.h>
#include <t_core.h>
#include <stdbool.h>
#include <lib4aio_c_headers/utils/str_hook/str_hook.h>
#include <lib4aio_c_headers/utils/string_utils/common.h>
#include <lib4aio_c_headers/utils/error_utils/error_utils.h>

#define T_MACHINE_SIMULATOR_DEBUG

#define T_MACHINE_SIMULATOR_INFO_TAG "T_MACHINE_SIMULATOR"

#define T_MACHINE_SIMULATOR_ERROR_TAG "T_MACHINE_SIMULATOR_ERROR"

#define DEBUG_FLAG "-d"

#define MAIN_ARG_COUNT 5

#define MAX_ARG_COUNT 6

#define DEBUG_FLAG_INDEX 6

#ifdef T_MACHINE_SIMULATOR_DEBUG

#include <lib4aio_c_headers/utils/log_utils/log_utils.h>

#endif

/**
 * Initializes and launches the t_core of Turing Machine.
 *
 * @param command_file_path - file with Turing machine commands;
 * @param input_file_path - input tape (pure string);
 * @param output_file_path - output tape (tape result);
 * @param is_debug_mode - is debug mode (true / false).
 */

static void make_t_machine(
        const char *command_file_path,
        const char *input_file_path,
        const char *output_file_path,
        str_hook *initial_state,
        const _Bool is_debug_mode
)
{
#ifdef T_MACHINE_SIMULATOR_DEBUG
    log_info(T_MACHINE_SIMULATOR_INFO_TAG, "Launch simulator");
#endif
    //Is only design pattern, no more =)
    core()
            ->init_t_commands(command_file_path)
            ->init_t_tape(input_file_path)
            ->init_t_output_file(output_file_path)
            ->init_t_mode(is_debug_mode)
            ->init_t_iterator(initial_state)
            ->launch()
            ->finish();
#ifdef T_MACHINE_SIMULATOR_DEBUG
    log_info(T_MACHINE_SIMULATOR_INFO_TAG, "Program is finished");
#endif
}

/**
 * Simulates the Turing machine.
 *
 * @param argc - arg count;
 * @param argv - strings:
 *      1.) commands;
 *      2.) input file (string);
 *      3.) output file (string);
 *      4.) initial state (string)
 *      5.) debug flag ('-d')
 * @return - program result.
 */

int main(int argc, char *argv[])
{
    const _Bool is_valid_arg_count = argc >= MAIN_ARG_COUNT && argc <= MAX_ARG_COUNT;
    if (is_valid_arg_count) {
        //Define program args:
        const char *command_file_path = argv[1];
        const char *input_file_path = argv[2];
        const char *output_file_path = argv[3];
        const char *initial_state = argv[4];
        _Bool is_debug = false;
        //Check debug flag:
        const _Bool has_debug_flag = argc == DEBUG_FLAG_INDEX;
        if (has_debug_flag) {
            const char *debug_flag = argv[5];
            const _Bool is_valid_debug_flag = are_equal_strings(debug_flag, DEBUG_FLAG);
            if (is_valid_debug_flag) {
                is_debug = true;
            } else {
                throw_error_with_tag(T_MACHINE_SIMULATOR_ERROR_TAG, "Expected debug flag '[-d]'");
            }
        }
        //Define initial state:
        str_hook *initial_state_holder = new_str_hook(initial_state);
        initial_state_holder->start = 0;
        initial_state_holder->end = (unsigned int) strlen(initial_state);
#ifdef T_MACHINE_SIMULATOR_DEBUG
        log_info_str_hook(T_MACHINE_SIMULATOR_INFO_TAG, "Initial state:", initial_state_holder);
#endif
        //Check initial state name:
        const _Bool is_valid_name_of_state = is_word_hooked(initial_state_holder);
        if (is_valid_name_of_state) {
#ifdef T_MACHINE_SIMULATOR_DEBUG
            log_info(T_MACHINE_SIMULATOR_INFO_TAG, "Initial state is valid!");
#endif
            //Initialize & launch simulator:
            make_t_machine(
                    command_file_path,
                    input_file_path,
                    output_file_path,
                    initial_state_holder,
                    is_debug
            );
            return 0;
        } else {
            throw_error_with_str_hook(
                    T_MACHINE_SIMULATOR_ERROR_TAG,
                    "Invalid name of initial state!",
                    initial_state_holder
            );
        }
    } else {
        throw_error_with_tag(
                T_MACHINE_SIMULATOR_ERROR_TAG,
                "Expected commands '<command file> <input tape file> <output tape file> <initial state> [-d]'");
    }
}