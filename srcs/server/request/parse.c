#include "commons.h"
#include "server/request.h"
#include "utils/string.h"

char	*extract_line(char *text)
{
	const char	*crlf;
	size_t		line_length;
	char		*line;

	crlf = strstr(text, "\r\n");
	if (!crlf)
	{
		fprintf(stderr, "Invalid HTTP request: end of line missing\n");
		return (NULL);
	}
	line_length = crlf - text;
	line = (char *)malloc(sizeof(char) * (line_length + 1));
	if (!line)
	{
		perror("malloc");
		return (NULL);
	}
	memcpy(line, text, line_length);
	line[line_length] = '\0';
	return (line);
}

void	parse_start_line(char *start_line, t_http_request *req)
{
	char	*buffer;
	char	*method;
	char	*target;
	char	*version;
	
	buffer = strdup(start_line);
	if (!buffer)
	{
		perror("strdup");
		return ;
	}
	method = strtok(buffer, " ");
	target = strtok(NULL, " ");
	version = strtok(NULL, "\r\n");
	if (!method || !target || !version)
	{
		fprintf(stderr, "Malformed HTTP start line\n");
		free(buffer);
		return ;
	}
	req->method = strdup(method);
	req->target = strdup(target);
	req->version = strdup(version);
	if (!req->method || !req->target || !req->version)
	{
		perror("strdup for request fields");
	}
	free(buffer);
}

t_http_request	req_parse(char *req_text)
{
	t_http_request	req;
	char		*start_line_text;

	start_line_text = extract_line(req_text);
	parse_start_line(start_line_text, &req);
	free(start_line_text);
	return (req);
}