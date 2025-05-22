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

char	*concat(char *s1, char *s2)
{
	char		*str;
	int			s1_len;
	int			s2_len;

	s1_len = strlen(s1);
	s2_len = strlen(s2);
	str = malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!str)
		return (NULL);
	memcpy(str, s1, s1_len);
	memcpy(str + s1_len, s2, s2_len);
	str[s1_len + s2_len] = '\0';
	return (str);
}
