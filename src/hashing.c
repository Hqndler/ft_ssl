#include "hashing.h"
#include "utils.h"

static int open_file(hash_ctx *ctx, char *input) {
	ctx->fd = open(input, O_RDONLY);
	if (ctx->fd < 0)
		return fprint("ft_ssl: %s: %s: %s", ctx->name, input, strerror(errno)), 1;
	return 0;
}

int32_t hash_compute(char *input, hash_args param, hash_ctx *ctx, int file, int stdin) {
	ctx->fd = !(!file && !stdin) - 1;

	if (file && open_file(ctx, input))
		return 1;
	
	ctx->update(&ctx->ctx, param, ctx->fd, input);
	ctx->finalize(&ctx->ctx);
	
	if ((!param.q && !param.r) || (!param.q && stdin)) {
		if (!stdin)
			fprint("%s ", ctx->name);
		hash_verbose(param, stdin, file, input);
	}
	
	if (stdin && !file && (*(uint32_t *)&param == 16843009)) {
		add_buffer(ctx->name, 1);
		fprint("\n");
	}
	else if ((!param.q && param.p && stdin) || \
			 (!param.q && !param.r && !stdin && !file))
		add_buffer(NULL, 1);
	ft_memset(get_buffer(), 0, BUFFER_LEN);
	print_hexa((uint8_t *)ctx->ctx, ctx->size);
	
	if (!param.q && param.r && !stdin)
		hash_reverse(file, input);

	if (ctx->fd > 0)
		close(ctx->fd);
	return 0;
}