#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

#include "utils.h"

static void ft_putchar(const char c) {
	write(1, &c, 1);
}

static void ft_putstring(const char *str) {
	write(1, str, ft_strlen(str));
}

char *xitoa(char *buff, uint64_t n, size_t len, bool caps) {
	size_t pos = len;
	static char const table[2][16] = {
		{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd',
		 'e', 'f'},
		{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D',
		 'E', 'F'}
	  };
	buff[--pos] = '\0';
	do {
		uint32_t digit = (n % 16);
		n /= 16;
		buff[--pos] = table[caps][digit];
	} while (n);
	return &buff[pos];
}

char *kitoa(char *buff, uint32_t n, size_t len) {
	size_t pos = len;
	bool is_neg = false;
	if ((int32_t)n < 0) {
		n = -n;
		is_neg = true;
	}

	buff[--pos] = '\0';
	while (n >= 10) {
		buff[--pos] = n % 10 + '0';
		n /= 10;
	}
	buff[--pos] = n + '0';
	if (is_neg)
		buff[--pos] = '-';
	return &buff[pos];
}

void fprint(char const *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char c;
	char *s;
	char num_buff[32];
	int32_t padding;

	while (1) {
		while ((c = *fmt++)) {
			if (c == '%')
				break;
			ft_putchar(c);
		}
		if (!c)
			break;

		padding = 0;
		if (*fmt == '0') {
			fmt++;
			while (*fmt >= '0' && *fmt <= '9') {
				padding = padding * 10 + (*fmt - '0');
				fmt++;
			}
		}

		c = *fmt++;
		switch (c) {
			case '\0':
				break;
			case '%':
				ft_putchar('%');
				break;
			case 's':
				s = va_arg(ap, char *);
				if (!s)
					s = "NULL";
				ft_putstring(s);
				break;
			case 'd':
			case 'i': {
				char *num_str =
					kitoa(num_buff, va_arg(ap, int), sizeof(num_buff));
				int32_t len = ft_strlen(num_str);
				while (padding > len) {
					ft_putchar('0');
					padding--;
				}
				ft_putstring(num_str);
				break;
			}
			case 'p':
				ft_putstring("0x");
				/* fallthrough */
			case 'X':
			case 'x': {
				char *num_str = xitoa(num_buff, va_arg(ap, uint32_t),
									   sizeof(num_buff), c == 'X');
				int32_t len = ft_strlen(num_str);
				while (padding > len) {
					ft_putchar('0');
					padding--;
				}
				ft_putstring(num_str);
				break;
			}
			case 'c':
				ft_putchar(va_arg(ap, int));
				break;
			default:
				ft_putchar('%');
				ft_putchar(c);
				break;
		}
	}
	va_end(ap);
}