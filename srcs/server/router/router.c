#include "commons.h"
#include "server/request.h"
#include "server/response.h"
#include "utils/string.h"
#include "utils/file.h"

const char	*handle_request(t_http_request req)
{
	int			fd;
	char		*path;
	char		*target;
	char		*res;
	const char	*res_content;
	int			response_size;
	
	target = strdup(req.target);
	if (!target)
		return create_response(500, TEXT_HTML, "<h1>Internal Server Error</h1>", -1);
	if (strcmp(req.method, GET) == 0)
	{
		if (!strstr(target, ".html"))
		{
			target = concat(target, ".html");
			if (!target)
				return create_response(500, TEXT_HTML, "<h1>Internal Server Error</h1>", -1);
		}
		path = resolve_path(target);
		free(target);
		if (!path)
		{
			fprintf(stderr, "invalid path\n");
			return create_response(404, TEXT_HTML, "<h1>404 Not Found</h1>", -1);
		}
		fd = open(path, O_RDONLY);
		if (fd < 0)
		{
			perror("open");
			free(path);
			return create_response(404, TEXT_HTML, "<h1>404 Not Found</h1>", -1);
		}
		res_content = read_file(fd, &response_size);
		if (!res_content || response_size < 0)
		{
			close(fd);
			free(path);
			return create_response(500, TEXT_HTML, "<h1>Internal Server Error</h1>", -1);
		}
	}
	else
	{
		return create_response(405, TEXT_HTML, "<h1>Method Not Allowed</h1>", -1);
	}
	free(path);
	close(fd);
	res = create_response(200, TEXT_HTML, res_content, response_size);
	free((void *)res_content);
	return (res);
}