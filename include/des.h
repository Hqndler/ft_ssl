#pragma once

#include "ft_ssl.h"

typedef struct {
	int		a;
	int		d;
	int		e;
	int		fdi;
	int		fdo;
	char	*key;
	char	*pass;
	char	*salt;
	char	*vector;
} des_param;

int32_t des(uint8_t **argv, ft_ssl_param param, int argc);