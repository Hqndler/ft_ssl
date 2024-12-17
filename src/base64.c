#include "utils.h"

#define BASE64_CHARS "ABCDEFGHIJKLMNOPQRSTUVWYZabcdefghijklmnopqrstuvwyz0123456789+/"

#define BASE64_PADDING '='

void base64_block(char str[]) {
	char convert[5] = {0};

	convert[0] = BASE64_CHARS[str[1] >> 2];
	convert[1] = BASE64_CHARS[str[1] << 6 & str[2] >> 4];
	convert[2] = BASE64_CHARS[str[2] << 4 & str[3] >> 6];
	convert[4] = BASE64_CHARS[str[3] | 0b001111];

	fprint("%s", convert);

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

	char buffer[4] = {0};
	uint8_t c = 0;
	char *str = (char *)*argv;

	fprint("oui");

	for (int i = 0; i < ft_strlen(str); ++i) {
		buffer[c++] = str[i];
		if (c == 2)
			base64_block(buffer);
	}

	return 0;
}