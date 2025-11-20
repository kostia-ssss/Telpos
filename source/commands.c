#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "../utils/utils.h"

Command *commands = NULL;
int commandsCount = 0;

// --- очищення консолі ---
int clear_(void) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;

    if (hConsole == INVALID_HANDLE_VALUE) return 0;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return 0;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', cellCount, (COORD){0, 0}, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, (COORD){0, 0}, &count);
    SetConsoleCursorPosition(hConsole, (COORD){0, 0});

    return 0;
}

// --- додати команду ---
void addCommand(char *name, int (*func)(int argc, char *argv[])) {
    commands = realloc(commands, sizeof(Command) * (commandsCount + 1));
    commands[commandsCount].name = name;
    commands[commandsCount].func = func;
    commandsCount++;
}

// --- help ---
int help(int argc, char *argv[]) {
    printf("Commands available:\n");
    for (int i = 0; i < commandsCount; i++)
        printf(" - %s\n", commands[i].name);

    writeHistory("help");
    return 0;
}

// --- echo ---
int echo(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++)
        printf("%s ", argv[i]);
    printf("\n");

    writeHistory("echo");
    return 0;
}

// --- logo ---
int logo(int argc, char *argv[]) {
    printf("██████  ██████  ██      ██████  ██████  ██████\n");
    printf("  ██    ██      ██      ██  ██  ██  ██  ██    \n");
    printf("  ██    ██████  ██      ██████  ██  ██  ██████\n");
    printf("  ██    ██      ██      ██      ██  ██      ██\n");
    printf("  ██    ██████  ██████  ██      ██████  ██████\n");

    writeHistory("logo");
    return 0;
}

// --- theme ---
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

        writeHistory("theme");
        return 0;
    }

    // --- ГОТОВІ ПРЕСЕТИ: theme <preset>
    if (argc == 2) {
        char textColorStr[20];
        char bgColorStr[20];

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

        int textColor = colorNameToCode(textColorStr);
        int bgColor   = colorNameToCode(bgColorStr);

        SetConsoleTextAttribute(hConsole, textColor | (bgColor << 4));

        writeConfigValue("../data/theme.cfg", "textColor", textColorStr);
        writeConfigValue("../data/theme.cfg", "bgColor", bgColorStr);

        writeHistory("theme");
        return 0;
    }

    printEvent("ERROR", "Usage: theme <textColor> <bgColor> OR theme <preset>", "red");
    return 0;
}

// --- clear ---
int clear(int argc, char *argv[]) {
    clear_();
    writeHistory("clear");
    return 0;
}

// --- timer ---
int timer(int argc, char *argv[]) {
    int total = 0;

    if (argc == 2) total = atoi(argv[1]);
    else if (argc == 3) total = atoi(argv[2]) * 60 + atoi(argv[1]);
    else if (argc == 4) total = atoi(argv[3]) * 3600 + atoi(argv[2]) * 60 + atoi(argv[1]);
    else {
        printf("Usage:\n  timer <seconds>\n  timer <seconds> <minutes>\n  timer <seconds> <minutes> <hours>\n");
        return 0;
    }

    for (int i = total; i >= 0; i--) {
        printf("\rRemains %02d:%02d:%02d   ", i / 3600, (i / 60) % 60, i % 60);
        Sleep(1000);
    }

    printf("\rDone!               \n");
    writeHistory("timer");
    return 0;
}

// --- time ---
int time_(int argc, char *argv[]) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    printf("Date and time: %02d-%02d-%04d %02d:%02d:%02d\n",
           tm_info->tm_mday,
           tm_info->tm_mon + 1,
           tm_info->tm_year + 1900,
           tm_info->tm_hour,
           tm_info->tm_min,
           tm_info->tm_sec);

    writeHistory("time");
    return 0;
}

// --- history ---
int history(int argc, char *argv[]) {
    char path[MAX_PATH];
    snprintf(path, sizeof(path), "%s/data/history.txt", baseDir);

    FILE *file = fopen(path, "r");
    if (!file) {
        printEvent("Cannot open history file", "ERROR", "red");
        return 0;
    }

    char line[256];
    int lineNum = 1;

    while (fgets(line, sizeof(line), file)) {
        printf("%d: %s", lineNum++, line);
    }

    fclose(file);
    return 1;
}
