#ifndef SERVER_RESPONSE_H
# define SERVER_RESPONSE_H

typedef enum s_content_type t_content_type;

char	*create_response(int status, const char *content_type, const char *body, size_t body_length);

#endif