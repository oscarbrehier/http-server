#ifndef SERVER_ROUTER_H
# define SERVER_ROUTER_H

typedef struct s_http_request t_http_request;

const char	*handle_request(t_http_request req);
const char	*handle_get(t_http_request req);
const char	*handle_post(t_http_request req);

#endif