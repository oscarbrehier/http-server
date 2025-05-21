#include "commons.h"
#include "server/request.h"

char	*get_header(t_http_request *request, const char *key)
{
	int	i;

	i = 0;
	while (i < request->header_count)
	{
		if (strcasecmp(request->headers[i].key, key) == 0)
			return (request->headers[i].value);
		i++;
	}
	return (NULL);
}