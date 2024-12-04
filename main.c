#include "md5.h"
#include "utils.h"
#include "ft_ssl.h"

#include <stdio.h>

int32_t sha256(uint8_t **argv, ft_ssl_param param, int argc) {
	(void)argv;
	(void)argc;
	(void)param;
	return printf("inside sha256 %s\n", argv[0]);
}

static uint32_t parse_arg(char **argv, ft_ssl_param *param) {
	uint32_t i = -1;
	while (argv[++i]) {
		if (!ft_strcmp(argv[i], "-r"))
			param->r = 1;
		else if (!ft_strcmp(argv[i], "-p"))
			param->p = 1;
		else if (!ft_strcmp(argv[i], "-q"))
			param->q = 1;
		else if (!ft_strcmp(argv[i], "-s"))
			param->s = 1;
		else
			break;
	}
	return i;
}

static int32_t wrapper(int32_t (*p)(uint8_t **, ft_ssl_param, int), uint8_t ** argv, int argc) {
	ft_ssl_param param = {};
	uint32_t i = parse_arg((char **)argv, &param);

	// fprint("p = %d, q = %d, r = %d, s = %d\n", param.p, param.q, param.r, param.s);

	p(&argv[i], param, argc - i);
	return 0;
}

int main(int argc, char **argv) {
	if (argc < 2)
		return 0;
	int32_t (*p[2])(uint8_t **, ft_ssl_param, int) = {md5, sha256};
	char *f[] = {"md5", "sha256"};

	for (int i = 0; i < 2; ++i) {
		if (!ft_strcmp(argv[1], f[i]))
			wrapper(p[i], (uint8_t **)&argv[2], argc - 2);
	}
	return 0;
}