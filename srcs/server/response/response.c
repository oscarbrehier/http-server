#include "commons.h"
#include "server/response.h"
#include "utils/mime.h"

const char	*get_reason_phrase(int status_code)
{
	switch (status_code) {
		case 200: return "OK";
		case 201: return "Created";
		case 204: return "No Content";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 500: return "Internal Server Error";
		case 503: return "Service Unavailable";
		default:  return "Unknown Status";
	}
}

char	*create_status_line(const char *version, int status_code)
{
	const char	*reason;
	char		*status_line;
	size_t		total_length;
	int			written;

	reason = get_reason_phrase(status_code);
	total_length = snprintf(NULL, 0, "HTTP/%s %d %s\r\n", version, status_code, reason);
	if (total_length <= 0)
	{
		fprintf(stderr, "status line invalid length\n");
		return (NULL);
	}
	status_line = (char *)malloc(sizeof(char) * (total_length + 1));
	written = snprintf(status_line, total_length + 1, "HTTP/%s %d %s\r\n", version, status_code, reason);
	if (written < 0 || (size_t)written != total_length)
	{
		perror("snprintf formatting failed");
		free((void *)status_line);
		return (NULL);
	}
	return (status_line);
}

char	*create_response(int status, const char *content_type, const char *body, size_t body_length)
{
	char		*status_line;
	char		*response;
	size_t		response_length;

	if (body_length == (size_t)-1)
	{
		body_length = strlen(body);
	}
	status_line = create_status_line("1.1", status);
	if (!status_line)
		return (NULL);
	response_length = snprintf(NULL, 0,
		"%sContent-Type: %s\r\nContent-Length: %ld\r\n\r\n%s", status_line, content_type, body_length, body);
	if (response_length <= 0)
	{
		fprintf(stderr, "response length invalid\n");
		free(status_line);
		return (NULL);
	}

	response = (char *)malloc(sizeof(char) * (response_length + 1));
	if (!response)
	{
		perror("malloc");
		free(status_line);
		return (NULL);
	}
	snprintf(response, response_length + 1,
		"%sContent-Type: %s\r\nContent-Length: %ld\r\n\r\n%s", status_line, content_type, body_length, body);
	free(status_line);
	return (response);
}