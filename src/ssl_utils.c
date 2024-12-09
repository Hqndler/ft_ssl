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

static int flush_buffer(char *buffer, int index) {
	ft_memset(buffer, 0, index);
	return 0;
}

void add_buffer(char *str, uint8_t isprint, uint8_t flush) {
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

uint32_t check_files(char **argv) {
	uint32_t i = -1;
	uint32_t count = 0;
	
	while (argv[++i]) {
		int fd = access(argv[i], F_OK);
		if (fd == -1)
			continue;
		++count;
	}
	return count;
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