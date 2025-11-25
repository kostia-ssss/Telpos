#include <stdio.h>
#include <string.h>
#include "textModder.h"
#include "../source/commands.h"

// -----------------------------------------------------------
// Повертає індекс символу в рядку (або -1 якщо нема)
// -----------------------------------------------------------
int isIn(char letter, const char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (letter == text[i]) return i;
    }
    return -1;
}

// -----------------------------------------------------------
// Команда: reverse <text>
// -----------------------------------------------------------
int reverse(int argc, char *argv[]) {
    if (argc == 2) {
        char *text = argv[1];

        int length = 0;
        while (text[length] != '\0') length++;

        for (int i = length - 1; i >= 0; i--) {
            printf("%c", text[i]);
        }

        printf("\n");
        return 0;
    }

    printf("Usage: reverse <text>\n");
    return 1;
}

// -----------------------------------------------------------
// Команда: encode <text>
// Caesar cipher (shift 3)
// -----------------------------------------------------------
int encode(int argc, char *argv[]) {
    if (argc == 2) {
        const char *letters = "abcdefghijklmnopqrstuvwxyz";
        const char *capsLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        char *text = argv[1];
        int n = 0;

        while (text[n] != '\0') {
            char c = text[n];

            int id = isIn(c, letters);
            int idCaps = isIn(c, capsLetters);

            if (id != -1) {
                printf("%c", letters[(id + 3) % 26]);
            }
            else if (idCaps != -1) {
                printf("%c", capsLetters[(idCaps + 3) % 26]);
            }
            else {
                printf("%c", c);
            }

            n++;
        }

        printf("\n");
        return 0;
    }

    printf("Usage: encode <text>\n");
    return 1;
}

// -----------------------------------------------------------
// Реєстрація команд
// -----------------------------------------------------------
void init_textSys(void) {
    addCommand("reverse", reverse);
    addCommand("encode", encode);
}
