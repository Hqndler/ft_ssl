#pragma once

#include "ft_ssl.h"
#include "hashing.h"

typedef struct {
	uint8_t		digest[32];
	uint8_t		data[67];
	uint32_t	datalen;
	uint64_t	bitlen;
	uint32_t	state[8];
} sha256_context;

int32_t sha256(char **argv, hash_args param, int argc);
// int32_t sha256_compute(uint8_t *input, ft_ssl_param param, int file, int stdin);
// void sha256_finalize(sha256_context *ctx);
// void sha256_update(sha256_context *ctx, uint8_t *buffer, ft_ssl_param param);
// void sha256_init(sha256_context *ctx);