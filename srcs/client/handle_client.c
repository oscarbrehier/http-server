#include "commons.h"
#include "server/request.h"
#include "server/response.h"
#include "server/router.h"
#include "thread_pool.h"

void	*handle_client(void *arg)
{
	int				i;
	char			*raw_request;
	t_http_request	request;
	const char		*response;
	int				client_socket;
	t_thread_pool	*pool;

	pool = (t_thread_pool *)arg;
	while (!pool->shutdown)
	{
		pthread_mutex_lock(&pool->lock);
		while (pool->client_count == 0 && !pool->shutdown)
		{
			pthread_cond_wait(&pool->cond, &pool->lock);
		}
		if (pool->shutdown)
		{
			pthread_mutex_unlock(&pool->lock);
			break ;
		}
		client_socket = pool->clients[0];
		i = 0;
		while (i < pool->client_count - 1)
		{
			pool->clients[i] = pool->clients[i + 1];
			i++;
		}
		pool->client_count--;
		pthread_mutex_unlock(&pool->lock);
		if (client_socket > 0)
		{
			// serving client
			if (read_request(client_socket, &raw_request) == -1)
			{
				close(client_socket);
				if (raw_request)
					free(raw_request);
				return (NULL);
			}
			parse_http_request(&request, raw_request);
			response = handle_request(request);
			send(client_socket, response, strlen(response), 0);
			free(raw_request);
			free((void*)response);
			free_request(&request);
		}
		close(client_socket);
	}
	return (NULL);
}