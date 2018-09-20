#ifndef T_CORE_H
#define T_CORE_H

/**
 * The t_core is abstraction of Turing machine system.
 * Although the struct of t_core doesn't match С convention
 * the using of core has pretty style =)
 */

typedef struct t_core {

    /**
     * Logic.
     */

    struct t_iterator *iterator;

    struct t_tape *tape;

    struct struct_list *command_list;

    /**
     * Command file data holder.
     */

    struct str_builder *command_file_builder;

    /**
     * Mode.
     */

    _Bool is_debug_mode;

    /**
     * Output.
     */

    struct _IO_FILE *output_file;

    /*
     * Functions (Is only design pattern =) ).
     */

    /**
     * Extracts command file *.txt, creates Turing machine commands and puts their to core.
     * @param command_file_path path to command file;
     */

    struct t_core *(*init_t_commands)(const char *command_file_path);

    /**
     * Extracts input tape file *.txt, creates Turing machine tape and puts it to core.
     * @param input_file_path - input tape file path.
     */

    struct t_core *(*init_t_tape)(const char *input_file_path);

    /**
     * Opens output file *.txt for writing Turing machine commands in runtime and put file reference in core.
     * @param output_file_path - output file path.
     */

    struct t_core *(*init_t_output_file)(const char *output_file_path);


    /**
     * Creates reading head of Turing machine and put it to core.
     * @param initial_state
     * @return
     */

    struct t_core *(*init_t_iterator)(struct str_hook *initial_state);

    /**
     * Sets Turing machine mode in core.
     * @param is_debug_mode - flag.
     */

    struct t_core *(*init_t_mode)(const _Bool is_debug_mode);

    /**
     * Interprets Turing machine system.
     */

    struct t_core *(*launch)();

    /**
     * Frees Turing machine system.
     */

    void (*finish)();

} t_core;

/**
 * Initializes Turing machine system.
 */

struct t_core *core();

#endif //T_CORE_H