#include "commons.h"
#include "server/request.h"

void	free_request(t_http_request *request)
{
	int	i;

	free(request->method);
	free(request->target);
	free(request->version);
	free(request->body);
	i = 0;
	while (i < request->header_count)
	{
		free(request->headers[i].key);
		free(request->headers[i].value);
		i++;
	}
	free(request->headers);
}