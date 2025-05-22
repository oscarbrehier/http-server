#include "commons.h"
#include "thread_pool.h"
#include "client.h"

t_thread_pool	*thread_pool_create(int size)
{
	t_thread_pool	*pool;
	int				i;

	if (size <= 0)
		return (NULL);
	pool = malloc(sizeof(t_thread_pool));
	if (!pool)
	{
		fprintf(stderr, "memory allocation for thread pool failed");
		return (NULL);
	}
	pool->threads = malloc(sizeof(t_thread) * size);
	pool->clients = malloc(sizeof(int) * size);
	if (!pool->threads || !pool->clients)
	{
		fprintf(stderr, "memory allocation for threads failed");
		if (pool->threads)
			free(pool->threads);
		if (pool->clients)
			free(pool->clients);
		free(pool);
		return (NULL);
	}
	pool->size = 0;
	pool->max_size = size;
	pool->shutdown = 0;
	pool->client_count = 0;
	pthread_mutex_init(&pool->lock, NULL);
	pthread_cond_init(&pool->cond, NULL);
	i = 0;
	while (i < size)
	{
		pool->threads[i].is_active = 1;
		if (pthread_create(&pool->threads[i].thread_id, NULL, handle_client, pool) != 0)
		{
			pool->threads[i].is_active = 0;
			perror("pthread_create");
			continue ;
		}
		pool->size++;
		i++;
	}
	return (pool);
}

void	thread_pool_destroy(t_thread_pool *pool)
{
	int	i;

	if (!pool)
		return ;
	pthread_mutex_lock(&pool->lock);
	pool->shutdown = 1;
	pthread_cond_signal(&pool->cond);
	pthread_mutex_unlock(&pool->lock);
	i = 0;
	while (i < pool->size)
	{
		if (pool->threads[i].is_active)
			pthread_join(pool->threads[i].thread_id, NULL);
		i++;
	}
	i = 0;
	while (i < pool->client_count)
	{
		if (pool->clients[i] > 0)
			close(pool->clients[i]);
		i++;
	}
	pthread_mutex_destroy(&pool->lock);
	pthread_cond_destroy(&pool->cond);
	free(pool->threads);
	free(pool->clients);
	free(pool);
}

int	thread_pool_add_client(t_thread_pool *pool, int client_socket)
{
	if (!pool)
		return (-1);
	pthread_mutex_lock(&pool->lock);
	if (pool->client_count < pool->max_size * 2)
	{
		pool->clients[pool->client_count++] = client_socket;
		pthread_cond_signal(&pool->cond);
	}
	pthread_mutex_unlock(&pool->lock);
	return (0);
}