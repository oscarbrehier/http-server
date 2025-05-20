#ifndef SERVER_RESPONSE_H
# define SERVER_RESPONSE_H

typedef enum s_content_type {
	TEXT_HTML,
	APPLICATION_JSON,
} t_content_type;

char	*create_response(int status, t_content_type content_type, char *body);

#endif