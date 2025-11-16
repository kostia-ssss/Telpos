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

int clear(int argc, char *argv[]) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;

    if (hConsole == INVALID_HANDLE_VALUE) return 0;

    // Отримати інформацію про консоль
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return 0;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    // Заповнити пробілами
    FillConsoleOutputCharacter(hConsole, ' ', cellCount, (COORD){0, 0}, &count);

    // Відновити атрибути кольору
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, (COORD){0, 0}, &count);

    // Перемістити курсор в початок
    SetConsoleCursorPosition(hConsole, (COORD){0, 0});

    return 0;
}

int timer(int argc, char *argv[]) {
    if (argc == 2) {
        int seconds = atoi(argv[1]);

        for (int i = seconds; i >= 0; i--) {
            printf("\rRemains %d seconds   ", i);
            Sleep(1000);
        }
    }

    else if (argc == 3) {
        int seconds = atoi(argv[1]);
        int minutes = atoi(argv[2]);
        int total = minutes * 60 + seconds;

        for (int i = total; i >= 0; i--) {
            printf("\rRemains %d minutes %d seconds   ", i / 60, i % 60);
            Sleep(1000);
        }
    }

    else if (argc == 4) {
        int seconds = atoi(argv[1]);
        int minutes = atoi(argv[2]);
        int hours = atoi(argv[3]);

        int total = hours * 3600 + minutes * 60 + seconds;

        for (int i = total; i >= 0; i--) {
            printf("\rRemains %d hours %d minutes %d seconds   ",
                   i / 3600, (i / 60) % 60, i % 60);
            Sleep(1000);
        }
    }

    else {
        printf("Usage:\n");
        printf("  timer <seconds>\n");
        printf("  timer <seconds> <minutes>\n");
        printf("  timer <seconds> <minutes> <hours>\n");
    }

    printf("\rDone!\n");
    return 0;
}
