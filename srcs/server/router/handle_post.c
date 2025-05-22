#include "commons.h"
#include "server/request.h"
#include "server/response.h"
#include "utils/string.h"
#include "utils/file.h"
#include "utils/mime.h"

const char	*handle_post(t_http_request req)
{
	const char	*response;
	
	if (strcmp(req.target, "/submit") == 0)
	{
		printf("Received POST data: \n%s\n", req.body);
		response = create_response(200, TEXT_HTML, "<h1>POST received</h1>", -1);
	}
	else
	{
		response = create_response(404, TEXT_HTML, "<h1>404 Not Found</h1>", -1);
	}
	return (response);
}