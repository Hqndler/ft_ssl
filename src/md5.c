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

uint32_t rotate_left(uint32_t n, uint32_t s) {
	return (n << s) | (n >> (32 - s));
}

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
		BB = BB + rotate_left(AA + E + K[i] + input[j], R[i]);
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

void md5_update(md5_context *ctx, uint8_t *input_buffer, size_t input_len){
	uint32_t input[16];
	unsigned int offset = ctx->size % 64;
	ctx->size += (uint64_t)input_len;

	for(unsigned int i = 0; i < input_len; ++i){
		ctx->input[offset++] = (uint8_t)*(input_buffer + i);

		if(offset % 64 == 0){
			for(unsigned int j = 0; j < 16; ++j){
				input[j] = (uint32_t)(ctx->input[(j * 4) + 3]) << 24 |
						   (uint32_t)(ctx->input[(j * 4) + 2]) << 16 |
						   (uint32_t)(ctx->input[(j * 4) + 1]) <<  8 |
						   (uint32_t)(ctx->input[(j * 4)]);
			}
			md5_step(ctx->buffer, input);
			offset = 0;
		}
	}
}

void md5_finalize(md5_context *ctx){
	uint32_t input[16];
	unsigned int offset = ctx->size % 64;
	unsigned int padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;

	md5_update(ctx, PADDING, padding_length);
	ctx->size -= (uint64_t)padding_length;

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

static uint8_t *get_file(char *file) {
	int fd = 0;
	char * str;
	
	if (file) {
		fd = open(file, O_RDONLY);
		if (fd < 0)
			return NULL;
	}
	
	char *content = malloc(1);
	if (!content)
		return NULL;
	content[0] = 0;
	
	while (1) {
		if (!(str = get_next_line(fd)))
			break;
		content = realloc(content, ft_strlen(content) + ft_strlen(str) + 1);
		ft_strcat(content, str);
		free(str);
	}
	return (uint8_t *)content;
}

void print_hexa(uint8_t *str, ft_ssl_param param, uint32_t len) {
	(void)param;
	for (uint32_t i = 0; i < len; ++i)
		fprint("%02x", str[i]);
}

void md5_compute(uint8_t *input, ft_ssl_param param) {
	md5_context ctx = {0};
	md5_init(&ctx);
	md5_update(&ctx, input, ft_strlen((char *)input));
	md5_finalize(&ctx);
	print_hexa(ctx.digest, param, 16);
}

void md5_reverse(char *input, int file) {
	if (!file)
		return fprint(" \"%s\"", input);
	fprint(" %s", input);
}

void md5_verbose(ft_ssl_param param, char *input, int file, int stdin) {
	(void)file;
	if (!stdin)
		fprint("MD5 ");
	if (!param.p && stdin)
		return fprint("(stdin)= ");
	else if (file)
		return fprint("(%s) = ", input);
	int t = ft_strlen(input);
	if (input[t - 1] != '\n')
		return fprint("(\"%s\") = ", input);
	input[t - 1] = 0;
	fprint("(\"%s\")= ", input);
	input[t - 1] = '\n';
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
	// fprint("check files %d\n", count);
	return count;
}

int32_t md5(uint8_t **argv, ft_ssl_param param, int argc) {
	int i = -1;
	uint8_t *ptr = NULL;

	if (param.p || check_files((char **)argv) == 0) {
		// fprint("allo ?");
		ptr = get_file(NULL);
		if (!param.q)
			md5_verbose(param, (char *)ptr, 0, 1);

		if (*(uint32_t *)&param == 16843009)
			fprint("%s", (char *)ptr);

		md5_compute(ptr, param);

		fprint("\n");
	}

	while (++i < argc) {

		// fprint("%d %d %s\n", i, argc - 1, argv[i]);

		if (param.s && i == 0) {
			ptr = (uint8_t *)ft_strdup((char *)argv[i]);
			if (!ptr)
				return fprint("Malloc error\n"), 1;
		}
		else
			ptr = get_file((char *)argv[i]);

		// fprint("%s", (char *)ptr);
		if (!ptr) {
			fprint("%s: no such file or directory\n", argv[i]);
			continue;
		}

		if (!param.r && !param.q)
			md5_verbose(param, (param.s && i == 0) ? (char *)ptr : (char *)argv[i], \
							   (param.s && i == 0) ? 0 : 1, 0);

		md5_compute(ptr, param);

		if (param.r && !param.q)
			md5_reverse((param.s && i == 0) ? (char *)ptr : (char *)argv[i], \
			 			(param.s && i == 0) ? 0 : 1);
		free(ptr);
		fprint("\n");
	}

	return 0;
}
