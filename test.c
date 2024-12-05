#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void pointer_mess(char *str) {
    printf("%p, %s\n", str, str);

    while (*str != 'O')
        ++str;
    printf("%p, %s\n", str, str);
}

int main(void) {
    char *str = strdup("TEST SUR UNE GROSSE STRING");
    printf("%p, %s\n", str, str);
    pointer_mess(str);
    printf("%p, %s\n", str, str);
    free(str);
    return 0;
}

// cabe45dcc9ae5b66ba86600cca6b8ba8 md5 a * 1000