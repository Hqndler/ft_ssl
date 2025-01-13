#include "utils.h"
#include "ft_ssl.h"

void print_hexa(uint8_t *str, uint32_t len) {
	for (uint32_t i = 0; i < len; ++i)
		fprint("%02x", str[i]);
}

int read_file(int fd, uint8_t *input, uint8_t **buffer, uint32_t len) {
	uint8_t *ptr = *buffer;
	if (fd >= 0)
		return read(fd, input, len);
	(*buffer) += ft_strncpy((char *)input, (char *)*buffer, len);
	return *buffer - ptr;
}

char *get_buffer(void) {
	static char buffer[BUFFER_LEN] = {0};
	return &*(char *)buffer;
}

void add_buffer(char *str, uint8_t isprint) {
	char *buffer = get_buffer();
	static int index = 0;
	static int check = 0;

	if (isprint && (index || !buffer[0])) {
		if (buffer[index - 1] == '\n')
			buffer[index - 1] = 0;
		if (!str)
			fprint("%s\")= ", buffer);
		else
			fprint("%s", buffer);
		return;
	}

	
	if (str && index + ft_strlen(str) >= 512){
		if (isprint)
			fprint("%s...\")= ", buffer);
		index = 0;
		check = 1;
	}
	if (!check) {
		ft_strcat(buffer, str);
		index += ft_strlen(str);
	}
}

void alg_verbose(ft_ssl_param param, char *input, int file, int stdin) {
	if (!param.p && stdin)
		return fprint("(stdin)= ");
	else if (file)
		return fprint("(%s) = ", input);
	fprint("(\"");
}

void alg_reverse(char *input, int file) {
	if (!file)
		return fprint(" \"%s\"", input);
	fprint(" %s", input);
}

int open_file_flag(char *file, int mask, int mode) {
	int fd = open(file, mask, mode);
	if (fd < 0)
		fprint("ft_ssl : Error : \"%s\" %s\n", file, strerror(errno));
	return fd;
}