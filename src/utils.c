#include "utils.h"

int32_t ft_strlen(const char *str) {
	int32_t i = -1;
	while (str[++i])
		;
	return i;
}