#pragma once

#include <stdint.h>

typedef struct __attribute__((packed)) {
	uint8_t p;
	uint8_t	q;
	uint8_t	r;
	uint8_t	s;
} hash_args;

typedef struct {
	int		fd;
	void	(*update)(void **, hash_args, int, char*);
	void	(*finalize)(void **);
	void	*ctx;
	char	name[8];
	int		size;
}	hash_ctx;

int32_t hash_compute(char *input, hash_args param, hash_ctx *ctx, int file, int stdin);