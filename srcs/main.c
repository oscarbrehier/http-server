#include "globals.h"
#include "commons.h"
#include "dynamic_buffer.h"
#include "server/response.h"
#include "server/request.h"
#include "server/router.h"
#include "client.h"
#include "thread_pool.h"
#include "utils/signal.h"

#define PORT 8080

t_thread_pool	*thread_pool;
volatile int	running = 1;

void	server_stop(void)
{
	struct sockaddr_in	addr;
	int					sock;

	running = 0;
	if (thread_pool)
	{
		pthread_mutex_lock(&thread_pool->lock);
		thread_pool->shutdown = 1;
		pthread_cond_broadcast(&thread_pool->cond);
		pthread_mutex_unlock(&thread_pool->lock);
	}
	if (thread_pool)
	{
		for (int i = 0; i < thread_pool->client_count; i++)
		{
			shutdown(thread_pool->clients[i], SHUT_RDWR);
			close(thread_pool->clients[i]);
		}
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock >= 0)
	{
		connect(sock, (struct sockaddr *)&addr, sizeof(addr));
		close(sock);
	}
}

int	main(void)
{
	int					server_fd;
	int					opt;
	struct sockaddr_in	address;
	socklen_t			addrlen;
	int					client_socket;
	
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
	setup_signal();
	if ((thread_pool = thread_pool_create(DEFAULT_THREAD_POOL_SIZE)) == NULL)
	{
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %d\n", PORT);
	printf("Waiting for connections\n");

	while (1)
	{
		client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
		if (client_socket < 0)
		{
			perror("accept");
			close(server_fd);
			exit(EXIT_FAILURE);
		}
		printf("Connection accepted\n");
		thread_pool_add_client(thread_pool, client_socket);
	}
	
	//close server_fd on client_handler error
	if (thread_pool)
		thread_pool_destroy(thread_pool);
	close(server_fd);
	return (0);
}