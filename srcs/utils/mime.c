#include "commons.h"
#include "utils/mime.h"

const char	*get_mime_type(const char *path)
{
	const char	*ext;

	ext = strrchr(path, '.');
	if (!ext) {
		return ("application/octet-stream");
	}
	ext++;
	printf("extension %s\n", ext);
	if (strcmp(ext, "html") == 0) return ("text/html");
	if (strcmp(ext, "css") == 0) return ("text/css");
	if (strcmp(ext, "js") == 0) return ("application/js");
	if (strcmp(ext, "json") == 0) return ("application/json");
	if (strcmp(ext, "png") == 0) return ("image/png");
	if (strcmp(ext, "jpeg") == 0 || strcmp(ext, "jpg") == 0) return ("image/jpeg");
	if (strcmp(ext, "gif") == 0) return ("image/gif");
	if (strcmp(ext, "woff") == 0) return ("font/woff");
	if (strcmp(ext, "ttf") == 0) return ("font/ttf");
	if (strcmp(ext, "mp4") == 0) return ("video/mp4");
	return ("application/octet-stream");
}