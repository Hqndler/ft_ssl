#pragma once

#include "ft_ssl.h"
#include "hashing.h"

typedef struct {
	uint8_t		digest[16];   // Result of algorithm
	uint64_t	size;         // Size of input in bytes
	uint32_t	buffer[4];    // Current accumulation of hash
	uint8_t		input[67];    // Input to be used in the next step
}	md5_context;

int32_t md5(char **argv, hash_args param, int argc);
// void md5_init(md5_context *ctx);
// void md5_update(md5_context *ctx, uint8_t *buffer, ft_ssl_param param);
// void md5_finalize(md5_context *ctx);
// int32_t md5_compute(uint8_t *input, ft_ssl_param param, int file, int stdin) ;