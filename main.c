#include "ft_ssl.h"
#include "utils.h"
#include "md5.h"
#include "sha256.h"
// #include "base64.h"
// #include "des.h"
#include "hashing.h"

static uint32_t parse_hash(char **argv, hash_args *param) {
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

static int32_t hash_wrapper(int32_t (*p)(char **, hash_args, int), char ** argv, int argc) {
	hash_args param = {};
	uint32_t i = parse_hash(argv, &param);

	return p(&argv[i], param, argc - i);
}

static int32_t cipher_wrapper(int32_t (*p)(char **, hash_args, int), char ** argv, int argc) {
	hash_args param = {};
	uint32_t i = parse_hash(argv, &param);

	return p(&argv[i], param, argc - i);
}

int32_t base64(char **argv, hash_args param, int argc) {
	(void)argv;
	(void)param;
	return argc;
}

int32_t des(char **argv, hash_args param, int argc) {
	(void)argv;
	(void)param;
	return argc;
}

int32_t des_ecb(char **argv, hash_args param, int argc) {
	(void)argv;
	(void)param;
	return argc;
}

int32_t des_cbc(char **argv, hash_args param, int argc) {
	(void)argv;
	(void)param;
	return argc;
}

int main(int argc, char **argv) {
	if (argc < 2)
		return fprint("usage: ft_ssl command [flags] [file/string]\n"), 0;
	int32_t (*mdc[])(char **, hash_args, int) = {md5, sha256};
	int32_t (*cm[])(char **, hash_args, int) = {base64, des, des_ecb, des_cbc};
	char *name[] = {"md5", "sha256", "base64", "des", "des-ecb", "des-cbc"};
	char *fmd[] = {"-p", "-q", "-r", "-s"};
	int check = -1;
	int nb_mdc = (sizeof(mdc) / sizeof(int32_t *));
	int nb_cm = (sizeof(cm) / sizeof(int32_t *));

	for (int i = 0; i < nb_mdc; ++i) {
		if (!ft_strcmp(argv[1], name[i]))
			check = check && hash_wrapper(mdc[i], &argv[2], argc - 2);
	}

	for (int i = nb_mdc; i < nb_mdc + nb_cm; ++i) {
		if (!ft_strcmp(argv[1], name[i]))
			check = check && cipher_wrapper(cm[i - nb_mdc], &argv[2], argc - 2);
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