#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../utils/utils.h"

Command *commands = NULL;
int commandsCount = 0;

int clear_(void) {
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

void addCommand(char *name, int (*func)(int argc, char *argv[])) {
    commands = realloc(commands, sizeof(Command) * (commandsCount + 1));
    commands[commandsCount].name = name;
    commands[commandsCount].func = func;
    commandsCount++;
}

int help(int argc, char *argv[]) {
    printf("Commands available:\n");
    for (int i = 0; i < commandsCount; i++) printf(" - %s\n", commands[i].name);
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

int theme(int argc, char *argv[]) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // --- ЯВНА ТЕМА: theme <text> <bg>
    if (argc == 3) {
        int textColor = colorNameToCode(argv[1]);
        int bgColor   = colorNameToCode(argv[2]);

        if (textColor == -1 || bgColor == -1) {
            printEvent("ERROR", "Unknown color", "red");
            return 0;
        }

        SetConsoleTextAttribute(hConsole, textColor | (bgColor << 4));

        writeConfigValue("../data/theme.cfg", "textColor", argv[1]);
        writeConfigValue("../data/theme.cfg", "bgColor", argv[2]);
        return 0;
    }

    // --- ГОТОВІ ПРЕСЕТИ: theme <preset>
    if (argc == 2) {
        char textColorStr[20];
        char bgColorStr[20];
        int textColor, bgColor;

        if (strcmp(argv[1], "classic") == 0) {
            strcpy(textColorStr, "green");
            strcpy(bgColorStr, "black");
        }
        else if (strcmp(argv[1], "dark") == 0) {
            strcpy(textColorStr, "white");
            strcpy(bgColorStr, "black");
        }
        else if (strcmp(argv[1], "light") == 0) {
            strcpy(textColorStr, "black");
            strcpy(bgColorStr, "white");
        }
        else {
            printEvent("ERROR", "Unknown theme preset", "red");
            return 0;
        }

        textColor = colorNameToCode(textColorStr);
        bgColor   = colorNameToCode(bgColorStr);

        SetConsoleTextAttribute(hConsole, textColor | (bgColor << 4));

        writeConfigValue("../data/theme.cfg", "textColor", textColorStr);
        writeConfigValue("../data/theme.cfg", "bgColor", bgColorStr);
        return 0;
    }

    printEvent("ERROR", "Usage: theme <textColor> <bgColor> OR theme <preset>", "red");
    return 0;
}


int clear(int argc, char *argv[]) {
    clear_();
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