#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "commands.h"
#include "../utils/utils.h"
#include "../modules/fileSystem.h"

char baseDir[MAX_PATH];

int main(void) {
    GetCurrentDirectoryA(MAX_PATH, baseDir);

    // якщо запускаємо з build, піднімаємося в корінь
    char *lastSlash = strrchr(baseDir, '\\');
    if (lastSlash) *lastSlash = '\0';  // видаляємо "\build"

    char *textC = readConfigValue("../data/theme.cfg", "textColor");
    char *bgC = readConfigValue("../data/theme.cfg", "bgColor");

    if (textC && bgC) {
        char *arr[3] = {"theme", textC, bgC};
        theme(3, arr);

        free(textC);
        free(bgC);
    }
    
    clear_();
    printf("Welcome to Telpos!\n");

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    addCommand("help", help);
    addCommand("echo", echo);
    addCommand("logo", logo);
    addCommand("theme", theme);
    addCommand("clear", clear);
    addCommand("timer", timer);
    addCommand("time", time_);
    addCommand("history", history);

    init_fs();
    

    char input[256];
    char *argv[10];
    int argc;

    while (1) {
        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        printf("%s >>> ", buffer);
        if (!fgets(input, sizeof(input), stdin)) break;

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
