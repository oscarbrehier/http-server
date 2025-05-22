#include "commons.h"
#include "server/request.h"
#include "server/response.h"
#include "server/router.h"
#include "utils/mime.h"

const char	*handle_request(t_http_request req)
{
	switch (req.method)
	{
		case HTTP_METHOD_GET:
			return (handle_get(req));
		case HTTP_METHOD_POST:
			return (handle_post(req));
		default:
			return create_response(405, TEXT_HTML, "<h1>Method Not Allowed</h1>", -1);
	}
}