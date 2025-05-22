#ifndef UTILS_MIME_H
# define UTILS_MIME_H

typedef enum s_content_type {
	TEXT_HTML,
	TEXT_CSS,
	APPLICATION_JS,
	APPLICATION_JSON,
	APPLICATION_OCTET_STREAM,
	IMAGE_PNG,
	IMAGE_JPEG,
	IMAGE_GIF,
	FONT_WOFF,
	FONT_TTF,
	VIDEO_MP4
} t_content_type;

const char	*get_mime_type(const char *path);

#endif