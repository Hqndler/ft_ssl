#include <stdio.h>
#include <string.h>

int md5(char **argv) {
    (void)argv;
    return printf("inside md5 %s\n", argv[0]);
}

int sha256(char **argv) {
    (void)argv;
    return printf("inside sha256 %s\n", argv[0]);
}

int main(int argc, char **argv) {
    if (argc < 2)
        return 0;
    int (*p[2]) (char **argv) = {md5, sha256};
    char *f[] = {"md5", "sha256"};

    for (int i = 0; i < 2; ++i) {
        if (!strcmp(argv[1], f[i]))
            p[i](&argv[2]);
    }
    return 0;
}