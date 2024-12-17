#include "md5.h"
#include "sha256.h"
#include "utils.h"
#include "ft_ssl.h"

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

static int32_t cipher_wrapper(int32_t (*p)(uint8_t **, ft_ssl_param, int), uint8_t ** argv, int argc) {
	ft_ssl_param param = {};
	uint32_t i = parse_arg((char **)argv, &param);

	

	return p(&argv[i], param, argc - i);
}

// int32_t base64(uint8_t **argv, ft_ssl_param param, int argc) {
// 	(void)argv;
// 	(void)param;
// 	return argc;
// }

int32_t des(uint8_t **argv, ft_ssl_param param, int argc) {
	(void)argv;
	(void)param;
	return argc;
}

int32_t des_ecb(uint8_t **argv, ft_ssl_param param, int argc) {
	(void)argv;
	(void)param;
	return argc;
}

int32_t des_cbc(uint8_t **argv, ft_ssl_param param, int argc) {
	(void)argv;
	(void)param;
	return argc;
}

int main(int argc, char **argv) {
	if (argc < 2)
		return fprint("usage: ft_ssl command [flags] [file/string]\n"), 0;
	int32_t (*mdc[])(uint8_t **, ft_ssl_param, int) = {md5, sha256};
	int32_t (*cm[])(uint8_t **, ft_ssl_param, int) = {base64, des, des_ecb, des_cbc};
	char *name[] = {"md5", "sha256", "base64", "des", "des-ecb", "des-cbc"};
	char *fmd[] = {"-p", "-q", "-r", "-s"};
	int check = -1;
	int nb_mdc = (sizeof(mdc) / sizeof(int32_t *));
	int nb_cm = (sizeof(cm) / sizeof(int32_t *));

	for (int i = 0; i < nb_mdc; ++i) {
		if (!ft_strcmp(argv[1], name[i]))
			check = check && wrapper(mdc[i], (uint8_t **)&argv[2], argc - 2);
	}

	for (int i = nb_mdc; i < nb_mdc + nb_cm; ++i) {
		if (!ft_strcmp(argv[1], name[i]))
			check = check && cipher_wrapper(cm[i], (uint8_t **)&argv[2], argc - 2);
	}

	if (check == -1) {
		fprint("ft_ssl: Error: '%s' is an invalid command.\n", argv[1]);
		fprint("\nStandard commands:\n");
		fprint("\nMessage Digest commands:\n");
		for (int i = 0; i < nb_mdc; ++i)
			fprint("%s\n", name[i]);
		fprint("\nMessage Digest Flags:\n");
		for (int i = 0; i < (int)(sizeof(fmd) / sizeof(char *)); ++i)
			fprint("%s ", fmd[i]);
		fprint("\n");
		fprint("\nCipher commands:\n");
		for (int i = nb_mdc; i < nb_cm + nb_mdc; ++i)
			fprint("%s\n", name[i]);
		check = -check;
	}

	return check;
}