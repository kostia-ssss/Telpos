#include <stdio.h>
#include <stdlib.h>
#include "../source/commands.h"
#include "converter.h"
#include <string.h>
#include "../utils/utils.h"

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

int calc(int argc, char *argv[]) {
    if (argc == 4) {
        int num1 = atoi(argv[1]);
        int num2 = atoi(argv[3]);
        char op = argv[2][0];  // беремо символ оператора
        int result;

        switch (op) {
            case '+': result = num1 + num2; break;
            case '-': result = num1 - num2; break;
            case '*': result = num1 * num2; break;
            case '/':
                if (num2 == 0) {
                    printf("Error: division by zero\n");
                    return 1;
                }
                result = num1 / num2;
                break;
            case '%':
                if (num2 == 0) {
                    printf("Error: modulo by zero\n");
                    return 1;
                }
                result = num1 % num2;
                break;
            default:
                printf("Invalid operator\n");
                return 1;
        }

        printf("%d\n", result);
        writeHistory("calc");
    }
    else {
        printf("Usage: calc <num1> <+|-|*|/|%> <num2>\n");
    }

    return 0;
}

void init_c(void) {
    addCommand("convert", convert);
    addCommand("calc", calc);
}