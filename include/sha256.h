#pragma once

#include "ft_ssl.h"

typedef struct {
	uint8_t		data[67];
	uint32_t	datalen;
	uint64_t	bitlen;
	uint32_t	state[8];
	uint8_t		digest[32];
	int			fd;
} sha256_context;

int32_t sha256(uint8_t **argv, ft_ssl_param param, int argc);