#include <stdio.h>
#include <stdlib.h>
#include "../source/commands.h"
#include "converter.h"
#include <string.h>

int convert(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: convert <number> <base>\n");
        return 0;
    }

    int num = atoi(argv[1]);
    int base = atoi(argv[2]);

    if (base < 2 || base > 36) {
        printf("Error: base must be between 2 and 36.\n");
        return 0;
    }

    char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char result[64];
    int index = 0;

    if (num == 0) {
        printf("0\n");
        return 0;
    }

    while (num > 0) {
        result[index++] = digits[num % base];
        num /= base;
    }

    // reverse
    for (int i = index - 1; i >= 0; i--) {
        printf("%c", result[i]);
    }

    printf("\n");
    return 0;
}

void init_c() {
    addCommand("convert", convert);
}