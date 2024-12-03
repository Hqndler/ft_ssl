#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	uint64_t	size;         // Size of input in bytes
	uint32_t	buffer[4];    // Current accumulation of hash
	uint8_t		input[64];    // Input to be used in the next step
	uint8_t		digest[16];   // Result of algorithm
}	md5_context;

typedef struct {
	bool	p;
	bool	q;
	bool	r;
	bool	s;
} md5_param;

int32_t md5(uint8_t **argv);