#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

Command *commands = NULL;
int commandsCount = 0;

void addCommand(char *name, int (*func)(int argc, char *argv[])) {
    commands = realloc(commands, sizeof(Command) * (commandsCount + 1));
    commands[commandsCount].name = name;
    commands[commandsCount].func = func;
    commandsCount++;
}

int help(int argc, char *argv[]) {
    printf("Commands available:\n");
    printf("help - show this message\n");
    printf("echo - repeat what you type\n");
    printf("logo - show Telpos logo\n");
    printf("theme - change text and background colors\n");
    printf("exit - exit the terminal\n");
    return 0;
}

int echo(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}

int logo(int argc, char *argv[]) {
    printf("██████  ██████  ██      ██████  ██████  ██████\n");
    printf("  ██    ██      ██      ██  ██  ██  ██  ██    \n");
    printf("  ██    ██████  ██      ██████  ██  ██  ██████\n");
    printf("  ██    ██      ██      ██      ██  ██      ██\n");
    printf("  ██    ██████  ██████  ██      ██████  ██████\n");
    return 0;
}

int colorNameToCode(const char *name) {
    if (strcmp(name, "black") == 0) return 0;
    if (strcmp(name, "red") == 0) return 4;
    if (strcmp(name, "green") == 0) return 2;
    if (strcmp(name, "yellow") == 0) return 6;
    if (strcmp(name, "blue") == 0) return 1;
    if (strcmp(name, "magenta") == 0) return 5;
    if (strcmp(name, "cyan") == 0) return 3;
    if (strcmp(name, "white") == 0) return 7;
    return 7; // стандартний
}

int theme(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: theme <textColor> <bgColor>\n");
        return 0;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int textColor = colorNameToCode(argv[1]);
    int bgColor = colorNameToCode(argv[2]);

    SetConsoleTextAttribute(hConsole, textColor | (bgColor << 4));
    return 0;
}
