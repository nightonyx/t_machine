cmake_minimum_required(VERSION 3.12)

project(t_machine C)

set(CMAKE_C_STANDARD 11)

include_directories(headers lib4aio)

add_executable(
        t_machine
        #---------------------------------------------------------------------------------------------------------------
        #HEADERS:
        #lib4aio:
        lib4aio/lib4aio_c_headers/utils/struct_list/struct_list.h
        lib4aio/lib4aio_c_headers/utils/memory_utils/memory_utils.h
        lib4aio/lib4aio_c_headers/utils/error_utils/error_utils.h
        lib4aio/lib4aio_c_headers/utils/file_utils/file_utils.h
        lib4aio/lib4aio_c_headers/utils/str_hook/str_hook.h
        lib4aio/lib4aio_c_headers/utils/log_utils/log_utils.h
        lib4aio/lib4aio_c_headers/utils/string_utils/common.h
        lib4aio/lib4aio_c_headers/utils/str_builder/str_builder.h
        #t_machine:
        headers/t_tape.h
        headers/t_iterator.h
        headers/t_command.h
        headers/t_core.h
        headers/t_message.h
        #---------------------------------------------------------------------------------------------------------------
        #SOURCES:
        #lib4aio:
        lib4aio/lib4aio_c_sources/utils/struct_list/struct_list.c
        lib4aio/lib4aio_c_sources/utils/memory_utis/memory_utils.c
        lib4aio/lib4aio_c_sources/utils/str_hook/str_hook.c
        lib4aio/lib4aio_c_sources/utils/error_utils/error_utils.c
        lib4aio/lib4aio_c_sources/utils/file_utils/file_utils.c
        lib4aio/lib4aio_c_sources/utils/str_builder/str_builder.c
        lib4aio/lib4aio_c_sources/utils/log_utils/log_utils.c
        lib4aio/lib4aio_c_sources/utils/string_utils/common.c
        lib4aio/lib4aio_c_sources/utils/str_hook/str_hook_split.c
        #t_machine:
        sources/t_command.c
        sources/t_iterator.c
        sources/t_tape.c
        sources/t_core.c
        sources/t_message.c
        #---------------------------------------------------------------------------------------------------------------
        #MAIN:
        main/main.c
        #---------------------------------------------------------------------------------------------------------------
)