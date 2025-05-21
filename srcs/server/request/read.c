#include "commons.h"
#include "server/request.h"
#include "dynamic_buffer.h"

int	read_request(int socket, char **output)
{
	t_dynamic_buffer	*recv_buffer;
	ssize_t				bytes_read;

	recv_buffer = dynamic_buffer_create(DEFAULT_BUFFER_SIZE);
	if (!recv_buffer)
	{
		fprintf(stderr, "buffer memory allocation failured\n");
		return (-1);
	}
	char	recv_chunk[READ_CHUNK_SIZE];
	while ((bytes_read = recv(socket, recv_chunk, sizeof(recv_chunk) - 1, 0)) > 0)
	{
		recv_chunk[bytes_read] = '\0';
		dynamic_buffer_append(recv_buffer, recv_chunk, bytes_read);
		if (strchr(recv_chunk, '\n') != NULL)
			break ;
	}
	if (bytes_read <= 0)
	{
		printf("Request does not contain any content\n");
		dynamic_buffer_destroy(recv_buffer);
		return (-1);
	}
	*output = strdup(recv_buffer->buffer);
	if (!*output)
	{
		perror("strdup");
		dynamic_buffer_destroy(recv_buffer);
		return (-1);
	}
	printf("Request content: \n%s\n", recv_buffer->buffer);
	dynamic_buffer_destroy(recv_buffer);
	return (0);
}