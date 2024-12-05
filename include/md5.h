#pragma once

#include "ft_ssl.h"

#include <stdlib.h>

typedef struct {
	uint64_t	size;         // Size of input in bytes
	uint32_t	buffer[4];    // Current accumulation of hash
	uint8_t		input[64];    // Input to be used in the next step
	uint8_t		digest[16];   // Result of algorithm
	int			fd;           // fd file to read (-1 if -s)
}	md5_context;

int32_t md5(uint8_t **argv, ft_ssl_param param, int argc);