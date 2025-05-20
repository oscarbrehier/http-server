#include "commons.h"
#include "dynamic_buffer.h"
#include "server/response.h"

#define PORT 8080
#define DEFAULT_BUFFER_SIZE 2048
#define READ_CHUNK_SIZE 1024

int	read_request(int socket)
{
	t_dynamic_buffer	*recv_buffer;

	recv_buffer = dynamic_buffer_create(DEFAULT_BUFFER_SIZE);
	if (!recv_buffer)
	{
		dynamic_buffer_destroy(recv_buffer);
		fprintf(stderr, "buffer memory allocation failured\n");
		return (-1);
	}
	char	recv_chunk[READ_CHUNK_SIZE];
	ssize_t	bytes_read;
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
		return (-1);
	}
	printf("Request content: \n%s\n", recv_buffer->buffer);
	dynamic_buffer_destroy(recv_buffer);
	return (0);
}

int	main(void)
{
	int					server_fd;
	int					new_socket;
	int					opt;
	struct sockaddr_in	address;
	socklen_t			addrlen;
	
	opt = 1;
	addrlen = sizeof(address);
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %d\n", PORT);
	printf("Waiting for connections\n");
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0)
	{
		perror("accept");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	printf("Connection accepted\n");
	if (read_request(new_socket) == -1)
	{
		close(new_socket);
		close(server_fd);
	}
	char *response = create_response(200, TEXT_HTML, "<h1>Hello world</h1>");
	send(new_socket, response, strlen(response), 0);
	free(response);
	close(new_socket);
	close(server_fd);
	return (0);
}