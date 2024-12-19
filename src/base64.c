#include "utils.h"

#define BASE64_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

#define BASE64_PADDING '='

void base64_block(uint8_t str[]) {
	char convert[5] = {0};
	fprint("%s\n", str);

	convert[0] = BASE64_CHARS[str[0] >> 2];
	convert[1] = BASE64_CHARS[((str[0] & 0b00000011) << 4) | ((str[1] & 0b11110000) >> 4)];
	convert[2] = BASE64_CHARS[((str[1] & 0b00001111) << 2) | ((str[2] & 0b11000000) >> 6)];
	convert[3] = BASE64_CHARS[str[2] & 0b00111111];

	fprint("(%s)\n", convert);
}

int32_t base64(uint8_t **argv, ft_ssl_param param, int argc) {
	// int i = -1;
	// int ret = 0;

	// if (param.p || (!argc && !param.s)) {
	// 	ret = (ret | md5_compute(NULL, param, 0, 1));
	// 	fprint("\n");
	// }

	// while (++i < argc) {
	// 	ret = (ret | md5_compute(argv[i], param, !(param.s && i == 0), 0));
	// 	fprint("\n");
	// }
	(void)param;
	(void)argc;

	uint8_t buffer[4] = {0};
	uint8_t c = 0;
	char *str = (char *)*argv;

	for (int i = 0; i < ft_strlen(str); ++i) {
		buffer[c++] = str[i];
		if (c == 3)
			base64_block(buffer);
	}

	return 0;
}