#ifndef UTILS_FILE_H
# define UTILS_FILE_H

#define READ_CHUNK_SIZE 1024

char	*read_file(int fd, int *out_size);
char	*resolve_path(const char *filename);

#endif