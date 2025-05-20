#include "commons.h"
#include "server/request.h"
#include "utils/string.h"
#include "utils/file.h"

const char	*serve_request(t_request req)
{
	int			fd;
	char		*path;
	const char	*response;
	
	if (strcmp(req.method, GET) == 0)
	{
		path = concat_path("www/", req.target);
		if (!path)
		{
			fprintf(stderr, "invalid path\n");
			free(path);
			return (NULL);
		}
		fd = open(path, O_RDONLY);
		if (fd < 0)
		{
			// throw 404 here
			perror("open");
			free(path);
			return (NULL);
		}
		response = read_file(fd);
		if (!response)
		{
			close(fd);
			free(path);
			return (NULL);
		}
	}
	free(path);
	close(fd);
	return (response);
}