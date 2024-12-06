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

void *ft_memmove(void *destination, void const *source, size_t size) {
	size_t i;
	unsigned char *d;
	unsigned char *s;

	d = (unsigned char *)destination;
	s = (unsigned char *)source;
	i = -1;
	if (d > s)
		while (size-- > 0)
			d[size] = s[size];
	else
		while (++i < size)
			d[i] = s[i];
	return (destination);
}


void *ft_calloc(uint32_t size) {
	char *ptr = malloc(size);
	if (!ptr)
		return NULL;
	while (--size)
		ptr[size] = 0;
	ptr[0] = 0;
	return (void *)ptr;
}

void *ft_realloc(void *ptr, size_t new_size) {
	size_t old_size = ft_strlen((char *)ptr);
	if (new_size <= old_size)
		return ptr;
	void *new = ft_calloc(new_size);
	if (!new)
		return NULL;
	ft_memmove(new, ptr, old_size);
	if (old_size)
		free(ptr);
	return new;
}

char	*ft_strcat(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (NULL);

	int	len = ft_strlen(s1);
	int j = -1;
	while (s2[++j])
		s1[len + j] = s2[j];
	s1[len + j] = 0;
	return (s1);
}

char *ft_strdup(char *str) {
	char *dup = malloc(ft_strlen(str) + 1);
	if (!dup)
		return NULL;
	uint32_t i = -1;
	while (str[++i])
		dup[i] = str[i];
	dup[i] = 0;
	return dup;
}

char *ft_strncpy(char *dest, char *src, size_t size) {
	size_t i = 0;
	while (src[i] && i < size) {
		dest[i] = src[i];
		++i;
	}
	return dest;
}

void *ft_memcpy(void *destination, void const *source, size_t size) {
	uint8_t *d;
	uint8_t *s;
	size_t i;

	d = (uint8_t *)destination;
	s = (uint8_t *)source;
	i = -1;
	while (++i < size)
		d[i] = s[i];
	return (destination);
}

void *ft_memset(void *pointer, uint8_t value, size_t count) {
	uint8_t *str;

	str = pointer;
	while (count--) {
		*str = (uint8_t)value;
		str++;
	}
	return (pointer);
}