#include "md5.h"
#include "sha256.h"
#include "utils.h"
#include "ft_ssl.h"

#include <stdio.h>

#define NBCOMMANDS 2
#define NBFLAGS 4

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

	return p(&argv[i], param, argc - i);
}

int main(int argc, char **argv) {
	if (argc < 2)
		return fprint("usage: ft_ssl command [flags] [file/string]\n"), 0;
	int32_t (*p[2])(uint8_t **, ft_ssl_param, int) = {md5, sha256};
	char *f[] = {"md5", "sha256"};
	char *o[] = {"-p", "-q", "-r", "-s"};
	int check = -1;

	for (int i = 0; i < NBCOMMANDS; ++i) {
		if (!ft_strcmp(argv[1], f[i]))
			check = check && wrapper(p[i], (uint8_t **)&argv[2], argc - 2);
	}
	if (check == -1) {
		fprint("ft_ssl: Error: '%s' is an invalid command.\n", argv[1]);
		fprint("\nCommands:\n");
		for (int i = 0; i < NBCOMMANDS; ++i)
			fprint("%s\n", f[i]);
		fprint("\nFlags:\n");
		for (int i = 0; i < NBFLAGS; ++i)
			fprint("%s ", o[i]);
		fprint("\n");
		check = -check;
	}

	return check;
}