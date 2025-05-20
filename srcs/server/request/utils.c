#include "commons.h"
#include "server/request.h"

void	free_request(t_request *request)
{
	free(request->method);
	free(request->target);
	free(request->version);
}