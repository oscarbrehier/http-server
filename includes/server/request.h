#ifndef SERVER_REQUEST_H
# define SERVER_REQUEST_H

#define POST "POST"
#define GET "GET"

#define DEFAULT_BUFFER_SIZE 2048
#define READ_CHUNK_SIZE 1024

typedef struct s_http_request
{
	char			*method;
	char			*target;
	char			*version;
} t_http_request;

t_http_request	req_parse(char *req_text);
int				read_request(int socket, char **output);

void			free_request(t_http_request *request);

#endif