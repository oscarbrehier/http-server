#include "commons.h"
#include "utils/file.h"

char	*read_file(int fd)
{
	char	*buffer;
	char	*temp;
	char	chunk[READ_CHUNK_SIZE];
	size_t	buffer_size;
	ssize_t	bytes_read;
	size_t	capacity;

	capacity = 1024;
	buffer_size = 0;
	buffer = (char *)malloc(sizeof(char) * (capacity + 1));
	if (!buffer)
	{
		perror("malloc");
		return (NULL);
	}
	while ((bytes_read = read(fd, chunk, READ_CHUNK_SIZE)) > 0)
	{
		if (buffer_size + bytes_read > capacity + 1)
		{
			while (buffer_size + bytes_read > capacity)
				capacity *= 2;
			temp = realloc(buffer, capacity);
			if (!temp)
			{
				perror("realloc");
				free(buffer);
				return (NULL);
			}
			buffer = temp;
		}
		memcpy(buffer + buffer_size, chunk, bytes_read);
		buffer_size += bytes_read;
	}
	if (bytes_read < 0)
	{
		perror("read");
		free(buffer);
		return (NULL);
	}
	temp = realloc(buffer, buffer_size + 1);
	if (!temp)
	{
		perror("realloc");
		free(buffer);
		return (NULL);
	}
	buffer = temp;
	buffer[buffer_size] = '\0';
	return (buffer);
}