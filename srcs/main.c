#include "commons.h"
#include "dynamic_buffer.h"
#include "server/response.h"
#include "server/request.h"
#include "server/router.h"

#define PORT 8080

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
	char	*raw_request;
	t_http_request req;
	if (read_request(new_socket, &raw_request) == -1)
	{
		close(new_socket);
		close(server_fd);
	}

	parse_http_request(&req, raw_request);
	const char	*response = handle_request(req);
	send(new_socket, response, strlen(response), 0);
	free(raw_request);
	free((void*)response);
	free_request(&req);
	close(new_socket);
	close(server_fd);
	return (0);
}