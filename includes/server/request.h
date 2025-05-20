#ifndef SERVER_REQUEST_H
# define SERVER_REQUEST_H

#define POST "POST"
#define GET "GET"

typedef struct s_request
{
	char			*method;
	char			*target;
	char			*version;
} t_request;

t_request	req_parse(char *req_text);
const char	*serve_request(t_request req);

void	free_request(t_request *request);

#endif