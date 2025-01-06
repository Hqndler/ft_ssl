#include "utils.h"
#include "base64.h"

#define BASE64_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

#define BASE64_PADDING '='

void base64_init(base64_context *ctx) {
	ctx->fd = 0;
	ctx->len = 0;
}

void base64_block(base64_context *ctx) {
	switch (ft_strlen((char *)ctx->buffer)) {
		case 3:
			ctx->digest[3] = BASE64_CHARS[ctx->buffer[2] & 0b00111111];
		case 2:
			ctx->digest[2] = BASE64_CHARS[((ctx->buffer[1] & 0b00001111) << 2) | \
										  ((ctx->buffer[2] & 0b11000000) >> 6)];
		case 1:
			ctx->digest[1] = BASE64_CHARS[((ctx->buffer[0] & 0b00000011) << 4) | \
										  ((ctx->buffer[1] & 0b11110000) >> 4)];
			ctx->digest[0] = BASE64_CHARS[ctx->buffer[0] >> 2];
			break;
		
		default:
			break;
	}
}

void base64_update(base64_context *ctx, uint8_t *buffer, base64_param param) {
	int tmp;
	while (1) {
		tmp = read_file(ctx->fd, ctx->buffer, &buffer, 3);
		if (tmp != 3)
			break;
		base64_block(ctx);
		ctx->digest[4] = 0;
		write(param.fdo, ctx->digest, 4);
		ft_memset(ctx->digest, 0, 4);
	}
	ctx->len = (uint8_t)tmp;
}

void base64_finalize(base64_context *ctx, base64_param param) {
	if (ctx->len < 1 || ctx->len > 2)
		return;
	
	int i = ctx->len;
	while (i < 3)
		ctx->buffer[i++] = 0;

	base64_block(ctx);

	i = ft_strlen((char *)ctx->digest);
	while (i < 4)
		ctx->digest[i++] = '=';
	
	write(param.fdo, ctx->digest, 4);
}

void base64_encode(uint8_t **argv, base64_param param) {
	base64_context ctx = {0};

	base64_init(&ctx);
	ctx.fd = param.fdi ? param.fdi : 0;
	base64_update(&ctx, argv[0], param);
	base64_finalize(&ctx, param);
}

void base64_decode(uint8_t **argv, base64_param param) {
	(void)argv;
	(void)param;
	fprint("Inside base64 decode nothing here for now\n");
}

int parse_base64(base64_param *param, char **argv) {
	int i = -1;
	int check;
	while (argv[++i]) {
		if (!ft_strcmp(argv[i], "-d"))
			param->d = 1;
		else if (!ft_strcmp(argv[i], "-e"))
			param->e = 1;
		else if (!ft_strcmp(argv[i], "-i")) {
			check = 1;
			if (argv[i + 1]) {
				param->fdi = open_file_flag(argv[++i], O_WRONLY);
				if (param->fdi < 0)
					return -1;
				param->i = 1;
			}
			else
				fprint("ft_ssl : Error : \"-i\" option given without value\n");
			continue;
		}
		else if (!ft_strcmp(argv[i], "-o")) {
			check = 1;
			if (argv[i + 1]) {
				param->fdo = open_file_flag(argv[++i], O_CREAT | O_WRONLY | O_TRUNC);
				if (param->fdo < 0)
					return -1;
				param->o = 1;
			}
			else
				fprint("ft_ssl : Error : \"-o\" option given without value\n");
			continue;
		}
		else
			break;
	}
	if (!param->e || !param->d || !check)
		param->e = 1;
	if (!param->fdo)
		param->fdo = 1;
	return i;
}

int32_t base64(uint8_t **argv, ft_ssl_param p, int argc) {
	(void)p;
	(void)argc;

	base64_param param = {0};
	int i = parse_base64(&param, (char **)argv);
	argv += i;
	argc -= i;

	if (param.e)
		base64_encode(argv, param);
	else
		base64_decode(argv, param);

	fprint("\n");
	return 0;
}