#include "utils.h"

int32_t ft_strlen(const char *str) {
	int32_t i = -1;
	while (str[++i])
		;
	return i;
}

int32_t ft_strcmp(char const *s1, char const *s2) {
	uint32_t i;

	if (!s1 || !s2)
		return (0);
	i = 0;
	while (s1[i] == s2[i]) {
		if (!s1[i] && !s2[i])
			return (0);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}