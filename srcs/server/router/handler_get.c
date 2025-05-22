#include "commons.h"
#include "server/request.h"
#include "server/response.h"
#include "utils/string.h"
#include "utils/file.h"
#include "utils/mime.h"

const char	*handle_get(t_http_request req)
{
	int 		fd;
	int 		response_size;
	char		*path;
	char 		*target;
	const char 	*res_content;
	const char 	*response;

	fd = -1;
	path = NULL;
	target = NULL;
	res_content = NULL;
	if (!strrchr(req.target, '.'))
	{
		target = concat(req.target, ".html");
		if (!target)
		{
			response = create_response(500, TEXT_HTML, "<h1>Internal Server Error</h1>", -1);
			goto cleanup;
		}
	}
	else
		target = (char *)req.target;
	path = resolve_path(target);
	if (!path)
	{
		fprintf(stderr, "Invalid path\n");
		response = create_response(404, TEXT_HTML, "<h1>404 Not Found</h1>", -1);
		goto cleanup;
	}
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		response = create_response(404, TEXT_HTML, "<h1>404 Not Found</h1>", -1);
		goto cleanup;
	}
	res_content = read_file(fd, &response_size);
	if (!res_content || response_size < 0)
	{
		response = create_response(500, TEXT_HTML, "<h1>Internal Server Error</h1>", -1);
		goto cleanup;
	}
	response = create_response(200, get_mime_type(path), res_content, response_size);
	goto cleanup;
cleanup:
	if (fd >= 0)
		close(fd);
	if (res_content)
		free((void *)res_content);
	if (path)
		free(path);
	if (target && target != req.target)
		free(target);
	return (response);
}