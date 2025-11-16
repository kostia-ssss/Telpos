#include <stdio.h>
#include <string.h>
#include "commands.h"
#include <windows.h>

int main(void) {
    printf("Welcome to Telpos!\n");

    // UTF-8 для консолі
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    addCommand("help", help);
    addCommand("echo", echo);
    addCommand("logo", logo);
    addCommand("theme", theme);
    addCommand("clear", clear);
    addCommand("timer", timer);

    char input[256];
    char *argv[10];
    int argc;

    while (1) {
        printf(">>> ");
        if (!fgets(input, 256, stdin)) break;

        argc = 0;
        char *token = strtok(input, " \n");
        while (token != NULL && argc < 10) {
            argv[argc++] = token;
            token = strtok(NULL, " \n");
        }

        if (argc == 0) continue;

        int found = 0;
        for (int i = 0; i < commandsCount; i++) {
            if (strcmp(commands[i].name, argv[0]) == 0) {
                commands[i].func(argc, argv);
                found = 1;
                break;
            }
        }

        if (!found) {
            if (strcmp("exit", argv[0]) == 0) break;
            else printf("Unknown command: %s\n", argv[0]);
        }
    }

    return 0;
}
