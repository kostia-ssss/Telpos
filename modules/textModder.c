#include <stdio.h>
#include "textModder.h"
#include "../source/commands.h"

int reverse(int argc, char *argv[]) {
    if (argc == 2) {
        char *text = argv[1];
        int length = 0;
        while (text[length] != '\0') {
            length++;
        }
        for (int i = length; i--; i > 0) {
            printf("%c", text[i]);
        }
        
        printf("\n");
        return 0;
    }
    else printf("Usage: reverse <text>\n");
}


void init_textSys(void) {
    addCommand("reverse", reverse);
}