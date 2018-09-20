#include <stdio.h>
#include <stdbool.h>
#include <lib4aio_c_headers/utils/str_hook/str_hook.h>
#include <lib4aio_c_headers/utils/error_utils/error_utils.h>
#include <lib4aio_c_headers/utils/str_builder/str_builder.h>

#define LIB4AIO_C_FILE_READER_INFO_DEBUG

#define LIB4AIO_C_FILE_READER_INFO_TAG "LIB4AIO_C_FILE_READER"

#define LIB4AIO_C_FILE_READER_ERROR_TAG "LIB4AIO_C_FILE_READER_ERROR"

#ifdef LIB4AIO_C_FILE_READER_INFO_DEBUG

#include <lib4aio_c_headers/utils/log_utils/log_utils.h>

#endif

/**
 * Business logic.
 */

#define READ "r"

#define WRITE "w"

/**
 * Reads file by path and collects file data to string builder.
 * @param path - file path.
 * @return string builder.
 *
 * To return a string builder is more convenient way because
 * a programmer knows what is a string content by using
 * ->string_value and it length by using ->length.
 * Anyway you always can extract string by function
 * pop_string_from_str_builder and delete your builder by free_str_builder.
 */

str_builder *read_file_by_str_builder(const char *path)
{
    str_builder *string_builder = new_str_builder();
    FILE *file = fopen(path, READ);
    if (file) {
        while (true) {
            const char current_char = (char) fgetc(file);
            if (current_char != EOF) {
                append_char_to_str_builder(string_builder, current_char);
            } else {
                break;
            }
        }
        fclose(file);
#ifdef LIB4AIO_C_FILE_READER_INFO_DEBUG
        str_hook *message = new_str_hook(string_builder->string_value);
        message->start = 0;
        message->end = string_builder->length;
        log_info_str_hook(LIB4AIO_C_FILE_READER_INFO_TAG, "File content:\n\n", message);
        free_str_hook(message);
#endif
        return string_builder;
    } else {
        throw_error_with_tag(LIB4AIO_C_FILE_READER_ERROR_TAG, "Cannot open source file.");
    }
}

/**
 * Opens file for writing.
 * @param file_path - path to file.
 * @return file pointer;
 */

FILE *open_file_for_writing(const char *file_path)
{
    return fopen(file_path, WRITE);
}

/**
 * Writes line to file.
 * @param file - file pointer;
 * @param line - writing line.
 */

void write_line_to_file(struct _IO_FILE *file, const char *line)
{
    fputs(line, file);
}