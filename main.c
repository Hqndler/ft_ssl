#include "md5.h"
#include "utils.h"

#include <stdio.h>

int32_t sha256(uint8_t **argv) {
    (void)argv;
    return printf("inside sha256 %s\n", argv[0]);
}

int main(int argc, char **argv) {
    if (argc < 2)
        return 0;
    int32_t (*p[2]) (uint8_t **argv) = {md5, sha256};
    char *f[] = {"md5", "sha256"};

    for (int i = 0; i < 2; ++i) {
        if (!ft_strcmp(argv[1], f[i]))
            p[i]((uint8_t **)&argv[2]);
    }
    return 0;
}