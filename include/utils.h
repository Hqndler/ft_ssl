#pragma once
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include "ft_ssl.h"

#define BUFFER_LEN 520

void fprint(char const *fmt, ...);

int32_t ft_strlen(const char *str);
int32_t ft_strcmp(char const *s1, char const *s2);
char	*get_next_line(int fd);
char	*ft_strcat(char *s1, char *s2);
void	*ft_realloc(void *ptr, size_t new_size);
void	*ft_calloc(uint32_t size);
void	*ft_memmove(void *destination, void const *source, size_t size);
char	*ft_strdup(char *str);
size_t	ft_strncpy(char *dest, char *src, size_t size);
void	*ft_memcpy(void *destination, void const *source, size_t size);
void	*ft_memset(void *pointer, uint8_t value, size_t count);

/*	 SSL Utils	 */
void	print_hexa(uint8_t *str, uint32_t len);
int		read_file(int fd, uint8_t *input, uint8_t **buffer, uint32_t len);
void	add_buffer(char *str, uint8_t isprint);
char	*get_buffer(void);
void	alg_verbose(ft_ssl_param param, char *input, int file, int stdin);
void 	alg_reverse(char *input, int file);