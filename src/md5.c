#include "md5.h"
#include "utils.h"
#include "ft_ssl.h"

const uint32_t R[] = {7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
					  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
					  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
					  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21};

const uint32_t K[64] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
						0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
						0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
						0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
						0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
						0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
						0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
						0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
						0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
						0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
						0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
						0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
						0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
						0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
						0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
						0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

static uint8_t PADDING[] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint32_t h0 = 0x67452301;
uint32_t h1 = 0xEFCDAB89;
uint32_t h2 = 0x98BADCFE;
uint32_t h3 = 0x10325476;

#define F(B, C, D) ((B & C) | (~B & D));
#define G(B, C, D) ((B & D) | (C & ~D));
#define H(B, C, D) (B ^ C ^ D);
#define I(B, C, D) (C ^ (B | ~D));


#define ROTATE_LEFT(N, S) ((N << S) | (N >> (32 - S)))

void md5_step(uint32_t *buffer, uint32_t *input){
	uint32_t AA = buffer[0];
	uint32_t BB = buffer[1];
	uint32_t CC = buffer[2];
	uint32_t DD = buffer[3];

	uint32_t E;

	unsigned int j;

	for(unsigned int i = 0; i < 64; ++i){
		switch(i / 16){
			case 0:
				E = F(BB, CC, DD);
				j = i;
				break;
			case 1:
				E = G(BB, CC, DD);
				j = ((i * 5) + 1) % 16;
				break;
			case 2:
				E = H(BB, CC, DD);
				j = ((i * 3) + 5) % 16;
				break;
			default:
				E = I(BB, CC, DD);
				j = (i * 7) % 16;
				break;
		}

		uint32_t temp = DD;
		DD = CC;
		CC = BB;
		BB = BB + ROTATE_LEFT((AA + E + K[i] + input[j]), (R[i]));
		AA = temp;
	}

	buffer[0] += AA;
	buffer[1] += BB;
	buffer[2] += CC;
	buffer[3] += DD;
}

void md5_init(md5_context *ctx){
	ctx->size = (uint64_t)0;

	ctx->buffer[0] = (uint32_t)h0;
	ctx->buffer[1] = (uint32_t)h1;
	ctx->buffer[2] = (uint32_t)h2;
	ctx->buffer[3] = (uint32_t)h3;
}

static int flush_buffer(char *buffer, int index) {
	ft_memset(buffer, 0, index);
	return 0;
}

static void add_buffer(char *str, uint8_t isprint, uint8_t flush) {
	static char buffer[520] = {0};
	static int index = 0;
	static int check = 0;

	if (isprint && index) {
		if (buffer[index - 1] == '\n')
			buffer[index - 1] = 0;
		if (!str)
			fprint("%s\")= ", buffer);
		else
			fprint("%s", buffer);
	}

	if (flush){
		index = flush_buffer(buffer, index);
		check = 0;
		return;
	}
	
	if (str && index + ft_strlen(str) >= 512){
		if (isprint)
			fprint("%s...\")= ", buffer);
		index = flush_buffer(buffer, index);
		check = 1;
	}
	if (!check) {
		ft_strcat(buffer, str);
		index += ft_strlen(str);
	}
}

void md5_block(md5_context *ctx) {
	uint32_t input[16];
	for(unsigned int j = 0; j < 16; ++j){
		input[j] = (uint32_t)(ctx->input[(j * 4) + 3]) << 24 |
					(uint32_t)(ctx->input[(j * 4) + 2]) << 16 |
					(uint32_t)(ctx->input[(j * 4) + 1]) <<  8 |
					(uint32_t)(ctx->input[(j * 4)]);
	}
	md5_step(ctx->buffer, input);
}

int read_file(md5_context *ctx, uint8_t **input_buffer) {
	if (ctx->fd >= 0)
		return read(ctx->fd, ctx->input, 64);
	ft_strncpy((char *)ctx->input, (char *)*input_buffer, 64);
	(*input_buffer) += ft_strlen((char *)ctx->input);
	return ft_strlen((char *)ctx->input);
}

void md5_update(md5_context *ctx, uint8_t *input_buffer, ft_ssl_param param){
	unsigned int offset = ctx->size % 64;

	while (1) {
		int tmp = read_file(ctx, &input_buffer);
		if (tmp <= 0)
			break;
		offset += tmp;
		ctx->size += offset;
		ctx->input[tmp] = 0;
		if (!param.q || !ctx->fd)
			add_buffer((char *)ctx->input, 0, 0);
		if (offset != 64)
			break;

		if(offset % 64 == 0){
			md5_block(ctx);
			offset = 0;
		}
	}
}

void md5_final_update(md5_context *ctx, uint8_t *input_buffer, uint32_t len) {
	uint32_t offset = ctx->size % 64;

	for (uint32_t i = 0; i < len; i++) {
		ctx->input[offset++] = (uint8_t)*(input_buffer + i);
		if (offset % 64 == 0) {
			md5_block(ctx);
			offset = 0;
		}
	}
}

void md5_finalize(md5_context *ctx, ft_ssl_param param){
	(void) param;
	uint32_t input[16];
	unsigned int offset = ctx->size % 64;
	unsigned int padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;

	md5_final_update(ctx, PADDING, padding_length);

	for(unsigned int j = 0; j < 14; ++j){
		input[j] = (uint32_t)(ctx->input[(j * 4) + 3]) << 24 |
				   (uint32_t)(ctx->input[(j * 4) + 2]) << 16 |
				   (uint32_t)(ctx->input[(j * 4) + 1]) <<  8 |
				   (uint32_t)(ctx->input[(j * 4)]);
	}
	input[14] = (uint32_t)(ctx->size * 8);
	input[15] = (uint32_t)((ctx->size * 8) >> 32);

	md5_step(ctx->buffer, input);

	for(unsigned int i = 0; i < 4; ++i){
		ctx->digest[(i * 4) + 0] = (uint8_t)((ctx->buffer[i] & 0x000000FF));
		ctx->digest[(i * 4) + 1] = (uint8_t)((ctx->buffer[i] & 0x0000FF00) >>  8);
		ctx->digest[(i * 4) + 2] = (uint8_t)((ctx->buffer[i] & 0x00FF0000) >> 16);
		ctx->digest[(i * 4) + 3] = (uint8_t)((ctx->buffer[i] & 0xFF000000) >> 24);
	}
}

void print_hexa(uint8_t *str, ft_ssl_param param, uint32_t len) {
	(void)param;
	for (uint32_t i = 0; i < len; ++i)
		fprint("%02x", str[i]);
}

void md5_verbose(ft_ssl_param param, char *input, int file, int stdin) {
	if (!stdin)
		fprint("MD5 ");
	if (!param.p && stdin)
		return fprint("(stdin)= ");
	else if (file)
		return fprint("(%s) = ", input);
	fprint("(\"");
}

void md5_reverse(char *input, int file) {
	if (!file)
		return fprint(" \"%s\"", input);
	fprint(" %s", input);
}

int open_file(md5_context *ctx, char *input) {
	ctx->fd = open(input, O_RDONLY);
	if (ctx->fd < 0)
		return fprint("ft_ssl: md5: %s: %s", input, strerror(errno)), 1;
	return 0;
}

void md5_compute(uint8_t *input, ft_ssl_param param, int file, int stdin) {
	md5_context ctx = {0};

	md5_init(&ctx);
	ctx.fd = !(!file && !stdin) - 1;
	
	if (file && open_file(&ctx, (char *)input))
		return;
	
	md5_update(&ctx, input, param);
	md5_finalize(&ctx, param);
	
	if ((!param.q && !param.r) || (!param.q && stdin))
		md5_verbose(param, (char *)input, file, stdin);
	
	if (stdin && !file && (*(uint32_t *)&param == 16843009)) {
		add_buffer("FUCK", 1, 1);
		fprint("\n");
	}
	else if ((!param.q && param.p && stdin) || (!param.q && !file && !stdin && !param.r)) // a modifier
		add_buffer(NULL, 1, 1);
	else
		add_buffer(NULL, 0, 1);
	print_hexa(ctx.digest, param, 16);
	
	if (!param.q && param.r && !stdin)
		md5_reverse((char *)input, file);

	if (ctx.fd > 0)
		close(ctx.fd);
}

int check_files(char ** argv) {
	uint32_t i = -1;
	uint32_t count = 0;
	
	while (argv[++i]) {
		int fd = open(argv[i], O_RDONLY);
		if (!fd)
			continue;
		close(fd);
		++count;
	}
	return count;
}

int32_t md5(uint8_t **argv, ft_ssl_param param, int argc) {
	int i = -1;

	if (param.p || check_files((char **)argv) == 0){
		md5_compute(NULL, param, 0, 1);
		fprint("\n");
	}

	while (++i < argc) {
		md5_compute(argv[i], param, !(param.s && i == 0), 0);
		fprint("\n");
	}

	return 0;
}
