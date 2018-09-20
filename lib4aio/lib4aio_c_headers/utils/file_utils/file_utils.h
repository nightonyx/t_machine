#ifndef LIB4AIO_C_FILE_READER_H
#define LIB4AIO_C_FILE_READER_H

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

struct str_builder *read_file_by_str_builder(const char *path);

/**
 * Opens file for writing.
 * @param file_path - path to file.
 * @return file pointer;
 */

struct _IO_FILE *open_file_for_writing(const char *file_path);

/**
 * Writes line to file.
 * @param file - file pointer;
 * @param line - writing line.
 */

void write_line_to_file(struct _IO_FILE *file, const char *line);

#endif //LIB4AIO_C_FILE_READER_H