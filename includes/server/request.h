#ifndef SERVER_REQUEST_H
# define SERVER_REQUEST_H

typedef enum {
	HTTP_METHOD_GET,
	HTTP_METHOD_POST,
	HTTP_METHOD_UNKNOWN,
} t_http_method;

#define DEFAULT_BUFFER_SIZE 2048
#define READ_CHUNK_SIZE 1024

typedef struct s_header
{
	char	*key;
	char	*value;
} t_header;

typedef struct s_http_request
{
	t_http_method	method;
	char			*target;
	char			*version;
	t_header		*headers;
	int				header_count;
	char			*body;
} t_http_request;

int				parse_http_request(t_http_request *request, char *raw_request);
int				read_request(int socket, char **output);

char			*get_header(t_http_request *request, const char *key);

void			free_request(t_http_request *request);

#endif