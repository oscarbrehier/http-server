#include "commons.h"

int	is_space(char c)
{
	if (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

int count_words(char *str)
{
	int i;
	int j;
	int count;

	i = 0;
	j = 0;
	count = 0;
	while (str[i])
	{
		if (is_space(str[i]))
			j = 0;
		else if (j == 0)
		{
			count++;
			j = 1;
		}
		i++;
	}
	return (count);
}

char	*concat_path(const char *dir, const char *filename)
{
	size_t	dir_len;
	size_t	filename_len;
	char	*path;

	dir_len = strlen(dir);
	filename_len = strlen(filename);
	path = (char *)malloc(sizeof(char) * (dir_len + filename_len + 1));
	if (!path)
	{
		perror("malloc");
		return (NULL);
	}
	memcpy(path, dir, dir_len);
	memcpy(path + dir_len, filename, filename_len);
	path[dir_len + filename_len] = '\0';
	return (path);
}