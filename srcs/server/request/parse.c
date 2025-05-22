#include "commons.h"
#include "server/request.h"
#include "utils/string.h"

char	*extract_line(char *text, size_t *length)
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
	if (length)
		*length = line_length;
	return (line);
}

t_http_method	parse_method(const char *method_str)
{
	if (strcmp(method_str, "GET") == 0)
		return (HTTP_METHOD_GET);
	else if (strcmp(method_str, "POST") == 0)
		return (HTTP_METHOD_POST);
	return (HTTP_METHOD_UNKNOWN);
}

int	parse_start_line(t_http_request *req, char *raw_request)
{
	char	*line;
	char	*method;
	char	*target;
	char	*version;
	size_t	line_length;
	
	line = extract_line(raw_request, &line_length);
	if (!line)
	{
		perror("strdup");
		return (-1);
	}
	method = strtok(line, " ");
	target = strtok(NULL, " ");
	version = strtok(NULL, "\r\n");
	if (!method || !target || !version)
	{
		fprintf(stderr, "Malformed HTTP start line\n");
		free(line);
		return (-1);
	}
	req->method = parse_method(method);
	req->target = strdup(target);
	req->version = strdup(version);
	if (!req->target || !req->version)
	{
		perror("strdup for request fields");
		free(line);
		return (-1);
	}
	free(line);
	return (line_length);
}

int process_header(t_http_request *request, int header_idx, char *line)
{
	char		*colon;
	t_header	header;
	char		*raw_value;
	char		*trimmed_value;

	colon = strchr(line, ':');
	if (!colon)
	{
		fprintf(stderr, "Malformed header (missing colon)\n");
		return (-1);
	}
	*colon = '\0';
	header.key = strdup(line);
	raw_value = strdup(colon + 1);
	if (!header.key || !raw_value)
	{
		perror("strdup for header");
		return (-1);
	}
	trimmed_value = raw_value;
	while (*trimmed_value == ' ')
		trimmed_value++;
	header.value = strdup(trimmed_value);
	if (!header.value)
	{
		free(raw_value);
		perror("strdup for header");
		return (-1);
	}
	request->headers[header_idx] = header;
	free(raw_value);
	return (0);
}

int	parse_headers(t_http_request *request, char *raw_request, int start_idx)
{
	int			header_count;	
	size_t		line_len;
	char		*line;
	char		*current;
	const char	*headers_end;
	int			headers_capacity;
	t_header	*temp;

	headers_capacity = 8;
	request->headers = malloc(sizeof(t_header) * headers_capacity);
	if (!request->headers)
	{
		perror("malloc headers");
		return (-1);
	}
	headers_end = strstr(raw_request, "\r\n\r\n");
	if (!headers_end)
	{
		fprintf(stderr, "Invalid HTTP request: end of headers missing\n");
		return (-1);
	}
	header_count = 0;
	current = raw_request + start_idx + 2;
	while (current < headers_end)
	{
		if (header_count >= headers_capacity)
		{
			headers_capacity *= 2;
			temp = realloc(request->headers, sizeof(t_header) * headers_capacity);
			if (!temp)
			{
				perror("realloc headers");
				return (-1);
			}
			request->headers = temp;
		}
		line = extract_line(current, &line_len);
		if (!line)
			return (-1);
		if (process_header(request, header_count, line) == -1)
			return (-1);
		free(line);
		current += line_len + 2;
		header_count++;
	}
	request->header_count = header_count;
	return (0);
}

int	copy_body(t_http_request *request, char *raw_request)
{
	const char	*body_start;
	size_t		body_length;
	char		*cl_header;
	
	body_start = strstr(raw_request, "\r\n\r\n");
	if (!body_start)
	{
		request->body = NULL;
		return (0);
	}
	body_start += 4;
	cl_header = get_header(request, "Content-Length");
	body_length = cl_header ? (size_t)atoi(cl_header) : strlen(body_start);
	if (body_length == 0)
	{
		request->body = NULL;
		return (0);
	}
	request->body = (char *)malloc(sizeof(char) * (body_length + 1));
	if (!request->body)
	{
		perror("malloc request body");
		return (-1);
	}
	memcpy(request->body, body_start, body_length);
	request->body[body_length] = '\0';
	return (0);
}

int	parse_http_request(t_http_request *request, char *raw_request)
{
	int				start_line_len;

	if (!request)
		return (-1);
	memset(request, 0, sizeof(t_http_request));
	start_line_len = parse_start_line(request, raw_request);
	if (start_line_len < 0)
		return (-1);
	if (parse_headers(request, raw_request, start_line_len) < 0 ||
		copy_body(request, raw_request)
	)
	{
		return (-1);
	}
	return (0);
}