#pragma once
#include <stdint.h>
#include <errno.h>
#include <string.h>

typedef struct __attribute__((packed)) {
	uint8_t p;
	uint8_t	q;
	uint8_t	r;
	uint8_t	s;
} ft_ssl_param;

int32_t base64(uint8_t **argv, ft_ssl_param param, int argc); // will be removed in the future