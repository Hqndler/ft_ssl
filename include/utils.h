#pragma once
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

void fprint(char const *fmt, ...);

int32_t ft_strlen(const char *str);
int32_t ft_strcmp(char const *s1, char const *s2);
char	*get_next_line(int fd);
char	*ft_strcat(char *s1, char *s2);
void *ft_realloc(void *ptr, size_t new_size);
void *ft_calloc(uint32_t size);
void *ft_memmove(void *destination, void const *source, size_t size);
char *ft_strdup(char *str);
char *ft_strncpy(char *dest, char *src, size_t size) ;