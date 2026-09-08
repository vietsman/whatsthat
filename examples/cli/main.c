/* SPDX-License-Identifier: MIT */
#include <stdio.h>
#include <string.h>

#include "whatsthat/whatsthat.h"

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--version") == 0) {
        printf("whatsthat %s\n", whatsthat_version());
        return 0;
    }

    printf("whatsthat CLI scaffold (%s)\n", whatsthat_version());
    return 0;
}
