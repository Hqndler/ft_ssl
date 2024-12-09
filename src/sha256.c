#include "sha256.h"
#include "utils.h"

#define ROTATE_LEFT(a, b) (((a) << (b)) | ((a) >> (32 - (b))))
#define ROTATE_RIGHT(a,b) (((a) >> (b)) | ((a) << (32 - (b))))

#define CH(x, y, z)		(((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z)	(((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x)	(ROTATE_RIGHT(x, 2) ^ ROTATE_RIGHT(x, 13) ^ ROTATE_RIGHT(x, 22))
#define EP1(x)	(ROTATE_RIGHT(x, 6) ^ ROTATE_RIGHT(x, 11) ^ ROTATE_RIGHT(x, 25))
#define SIG0(x)	(ROTATE_RIGHT(x, 7) ^ ROTATE_RIGHT(x, 18) ^ ((x) >> 3))
#define SIG1(x)	(ROTATE_RIGHT(x,17) ^ ROTATE_RIGHT(x, 19) ^ ((x) >> 10))

static const uint32_t K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void sha256_transform(sha256_context *ctx, const uint8_t data[]) {
	uint32_t a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | \
			   (data[j + 1] << 16) | \
			   (data[j + 2] << 8) | \
			   (data[j + 3]);
	
	for ( ; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e,f,g) + K[i] + m[i];
		t2 = EP0(a) + MAJ(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

void sha256_init(sha256_context *ctx) {
	ctx->datalen = 0;
	ctx->bitlen = 0;
	
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

void sha256_update(sha256_context *ctx, uint8_t *buffer, ft_ssl_param param) {
	while (1) {
		int tmp = read_file(ctx->fd, ctx->data, &buffer, 64);
		if (tmp <= 0)
			break;
		ctx->datalen += tmp;
		if (!param.q || !ctx->fd)
			add_buffer((char *)ctx->data, 0, 0);
		
		if (ctx->datalen == 64) {
			sha256_transform(ctx, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

void sha256_finalize(sha256_context *ctx) {
	uint32_t i;

	i = ctx->datalen;

	// Pad whatever data is left in the buffer.
	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;
		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else {
		ctx->data[i++] = 0x80;
		while (i < 64)
			ctx->data[i++] = 0x00;
		sha256_transform(ctx, ctx->data);
		memset(ctx->data, 0, 56);
	}

	// Append to the padding the total message's length in bits and transform.
	ctx->bitlen  += ctx->datalen * 8;
	ctx->data[63] = ctx->bitlen;
	ctx->data[62] = ctx->bitlen >> 8;
	ctx->data[61] = ctx->bitlen >> 16;
	ctx->data[60] = ctx->bitlen >> 24;
	ctx->data[59] = ctx->bitlen >> 32;
	ctx->data[58] = ctx->bitlen >> 40;
	ctx->data[57] = ctx->bitlen >> 48;
	ctx->data[56] = ctx->bitlen >> 56;
	sha256_transform(ctx, ctx->data);

	// Since this implementation uses little endian byte ordering and SHA uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	for (i = 0; i < 4; ++i) {
		ctx->digest[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		ctx->digest[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

static int open_file(sha256_context *ctx, char *input) {
	ctx->fd = open(input, O_RDONLY);
	if (ctx->fd < 0)
		return fprint("ft_ssl: sha256: %s: %s", input, strerror(errno)), 1;
	return 0;
}

void sha256_compute(uint8_t *input, ft_ssl_param param, int file, int stdin) {
	sha256_context ctx = {0};

	sha256_init(&ctx);
	ctx.fd = !(!file && !stdin) - 1;
	
	if (file && open_file(&ctx, (char *)input))
		return;
	
	sha256_update(&ctx, input, param);
	sha256_finalize(&ctx);
	
	if ((!param.q && !param.r) || (!param.q && stdin)) {
		if (!stdin)
			fprint("SHA256 ");
		alg_verbose(param, (char *)input, file, stdin);
	}
	
	if (stdin && !file && (*(uint32_t *)&param == 16843009)) {
		add_buffer("FUCK", 1, 1);
		fprint("\n");
	}
	else if ((!param.q && param.p && stdin) || (!param.q && !file && !stdin && !param.r))
		add_buffer(NULL, 1, 1);
	else
		add_buffer(NULL, 0, 1);
	print_hexa(ctx.digest, 32);
	
	if (!param.q && param.r && !stdin)
		alg_reverse((char *)input, file);

	if (ctx.fd > 0)
		close(ctx.fd);
}

int32_t sha256(uint8_t **argv, ft_ssl_param param, int argc) {
	int i = -1;

	if (param.p || check_files((char **)argv) == 0 - param.s){
		sha256_compute(NULL, param, 0, 1);
		fprint("\n");
	}

	while (++i < argc) {
		sha256_compute(argv[i], param, !(param.s && i == 0), 0);
		fprint("\n");
	}

	return 0;
}