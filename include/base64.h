#pragma once

#include "ft_ssl.h"

typedef struct {
	uint8_t		buffer[4];
	uint8_t		digest[5];
	uint8_t		len;
	int			fd;
} base64_context;

typedef struct __attribute__((packed)) {
	uint8_t	d;
	uint8_t	e;
	uint8_t	i;
	uint8_t	o;
	int		fdi;
	int		fdo;
} base64_param;