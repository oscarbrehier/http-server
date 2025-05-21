#ifndef SERVER_REQUEST_H
# define SERVER_REQUEST_H

#define POST "POST"
#define GET "GET"

#define DEFAULT_BUFFER_SIZE 2048
#define READ_CHUNK_SIZE 1024

typedef struct s_header
{
	char	*key;
	char	*value;
} t_header;

typedef struct s_http_request
{
	char			*method;
	char			*target;
	char			*version;
	t_header		*headers;
	int				header_count;
	char			*body;
} t_http_request;

int				req_parse(t_http_request *request, char *req_text);
int				read_request(int socket, char **output);

char			*get_header(t_http_request *request, const char *key);

void			free_request(t_http_request *request);

#endif